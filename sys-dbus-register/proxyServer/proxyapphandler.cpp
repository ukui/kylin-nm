/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "proxyapphandler.h"
#include <QDebug>

const QStringList systemCmdList = {"cat",
                                "grep",
                                "/bin/sh",
                                "sh",
                                "wc",
                                "ps",
                                "dpkg",
                                "systemctl",
                                "awk",
                                "head",
                                "runc",
                                "net",
                                "/usr/bin/bash",
                                "dpkg-query",
                                "top",
                                "gpg",
                                "/usr/bin/cat",
                                "/usr/bin/dpkg",
                                "/usr/bin/gpg",
                                "/bin/gpgconf",
                                "cut",
                                "mkdir",
                                "lsblk",
                                "lspci",
                                "rm",
                                "cd",
                                "vim",
                                "bash",
                                "/lib/systemd/systemd",
                                "/usr/bin/ukui-menu",
                                "ls",
                                "systemd"
                              };

/*
 * connect to netlink
 * returns netlink socket, or -1 on error
 */
bool ProcAddServer::netlinkConnect()
{
    struct sockaddr_nl sa_nl;

    m_sockFd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if (m_sockFd < 0) {
        perror("socket");
        return false;
    }

    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_groups = CN_IDX_PROC;
    sa_nl.nl_pid = getpid();

    if (bind(m_sockFd, (struct sockaddr *)&sa_nl, sizeof(sa_nl)) < 0) {
        perror("bind");
        close(m_sockFd);
        m_sockFd = -1;
        return false;
    }

    return true;
}

/*
 * subscribe on proc events (process notifications)
 */
bool ProcAddServer::setProcEvListen(int nl_sock, bool enable)
{
    struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
        struct nlmsghdr nl_hdr;
        struct __attribute__ ((__packed__)) {
            enum proc_cn_mcast_op cn_mcast;
            struct cn_msg cn_msg;
        };
    } nlcn_msg;

    memset(&nlcn_msg, 0, sizeof(nlcn_msg));
    nlcn_msg.nl_hdr.nlmsg_len = sizeof(nlcn_msg);
    nlcn_msg.nl_hdr.nlmsg_pid = getpid();
    nlcn_msg.nl_hdr.nlmsg_type = NLMSG_DONE;

    nlcn_msg.cn_msg.id.idx = CN_IDX_PROC;
    nlcn_msg.cn_msg.id.val = CN_VAL_PROC;
    nlcn_msg.cn_msg.len = sizeof(enum proc_cn_mcast_op);

    nlcn_msg.cn_mcast = enable ? PROC_CN_MCAST_LISTEN : PROC_CN_MCAST_IGNORE;

    if (send(nl_sock, &nlcn_msg, sizeof(nlcn_msg), 0) < 0) {
        perror("netlink send");
        return false;
    }

    return true;
}

/*
 * handle a single process event
 */
int ListenThObject::handleProcEv()
{
    int rc;
    struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
        struct nlmsghdr nl_hdr;
        struct __attribute__ ((__packed__)) {
            struct proc_event proc_ev;
            struct cn_msg cn_msg;
        };
    } nlcn_msg;

    while (!m_exitStat) {
        rc = recv(m_sockFd, &nlcn_msg, sizeof(nlcn_msg), 0);
        if (rc == 0) {
            /* shutdown? */
            return 0;
        } else if (rc == -1) {
            if (errno == EINTR) continue;
            perror("netlink recv");
            return -1;
        }
        switch (nlcn_msg.proc_ev.what) {
            case PROC_EVENT_FORK:
                if (nlcn_msg.proc_ev.event_data.fork.parent_pid > 1) {
                    Q_EMIT this->procAdd(nlcn_msg.proc_ev.event_data.fork.parent_pid);
                }
                break;

            case PROC_EVENT_EXEC:
                if (nlcn_msg.proc_ev.event_data.exec.process_pid > 1) {
                    Q_EMIT this->procAdd(nlcn_msg.proc_ev.event_data.exec.process_pid);
                }
                break;

            case PROC_EVENT_NONE:
            case PROC_EVENT_UID:
            case PROC_EVENT_GID:
                break;

            case PROC_EVENT_EXIT:
                break;

            default:
                break;
        }
    }
    return 0;
}

ListenThObject::ListenThObject(QObject *parent) : QObject(parent)
{
}

void ProcAddServer::startListen(int pid)
{
    if (pid <= 1) {
        return;
    }
//    PEEK_LIMIT();
    if (m_usedModelList.isEmpty()) {
        m_usedModelList.append(pid);
    } else if (m_usedModelList.contains(pid)) {
        return;
    } else {
        m_usedModelList.append(pid);
        return;
    }

    if (m_sockFd < 0) {
        if (!netlinkConnect()) {
            qWarning()<<"Init netlink connector Error!";
            return;
        } else {
            if (!setProcEvListen(m_sockFd, true)) {
                close(m_sockFd);
                m_sockFd = -1;
                qWarning()<<"Init netlink connector Error!";
                return;
            }
        }
    }

    if (m_listenTh != nullptr && m_listenThObj != nullptr) {
        m_listenThObj->setExitStat(false);
        return;
    }

    m_listenTh = new QThread();
    m_listenThObj = new ListenThObject();
    m_listenThObj->setSockFd(m_sockFd);
    m_listenThObj->moveToThread(m_listenTh);
    connect(m_listenTh, &QThread::finished, m_listenThObj, &ListenThObject::deleteLater);
    connect(m_listenTh, &QThread::finished, m_listenTh, &QObject::deleteLater);
    connect(m_listenTh, &QThread::started, m_listenThObj, &ListenThObject::handleProcEv);

    connect(m_listenThObj, &ListenThObject::procAdd, this, [&](int pid) {
        DealData *dealData = new DealData(pid, this);
        dealData->setAutoDelete(true);
        m_queDutyPool->start(dealData);
    }, Qt::QueuedConnection);

    m_listenTh->start();
}

void ProcAddServer::stopListen(int pid)
{
//    PEEK_LIMIT();
    if (m_usedModelList.contains(pid)) {
        m_usedModelList.removeOne(pid);
    }

    if (!m_usedModelList.isEmpty()) {
        return;
    }
    if (m_listenThObj != nullptr) {
        m_listenThObj->setExitStat(true);
    }

    if (m_listenTh != nullptr && m_listenTh->isRunning()) {
        disconnect(m_listenThObj);
        m_listenTh->quit();
        m_listenTh->wait();
        m_listenTh = nullptr;
        m_listenThObj = nullptr;

        if (m_sockFd >= 0) {
            setProcEvListen(m_sockFd, false);
            close(m_sockFd);
            m_sockFd = -1;
        }
    }
}

DealData::DealData(int pid, QObject *obj)
{
    qRegisterMetaType<MyMap>("MyMap");
    m_pid = pid;
    m_obj = obj;
}

bool DealData::isSystemCmd(QString cmdline)
{
    if (cmdline.isEmpty()) {
        return true;
    }
    QStringList fileLine = cmdline.split(" ");
    QSet<QString> intersection = systemCmdList.toSet().intersect(fileLine.toSet());
    if (intersection.isEmpty()) {
        return false;
    }
    return true;
}

void DealData::run()
{
    QMap<QString, QString> procInfo;
    QString procDir = QString("/proc/%1/").arg(m_pid);
    if (QDir(procDir).exists()) {
        QString cmdLine = getFileMsg(procDir + "cmdline");
        if (isSystemCmd(cmdLine)) {
            return;
        }
        QString environ = getFileMsg(procDir + "environ");
        QString status = getFileMsg(procDir + "status");
        procInfo.insert(PROCINFOKEY_TYPE, "app");
        procInfo.insert(PROCINFOKEY_PID, QString::number(m_pid));
        procInfo.insert(PROCINFOKEY_CMDLINE, cmdLine);
        procInfo.insert(PROCINFOKEY_DESKTOP, getEnvironMsg(environ));
        getStatusMsg(status, &procInfo);
        QMetaObject::invokeMethod(m_obj, "emitSignal", Q_ARG(MyMap, procInfo));
    }
}

QString DealData::getFileMsg(QString filePath)
{
    QFile file(filePath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QByteArray fileMsg = file.readAll();
        //去掉fileMsg中的乱码，用空格替代
        fileMsg.replace(0, 32);
        file.close();
        return fileMsg;
    }
    return QString();
}

QString DealData::getEnvironMsg(QString environ)
{
    QStringList msgList = environ.split(" ");
    for (QString msgStr : msgList) {
        if (msgStr.startsWith(FILEKEY_DESKTOP)) {
            return msgStr.split("=").at(1);
        }
    }
    return QString();
}

void DealData::getStatusMsg(QString fileMsg, QMap<QString, QString> *infoMap)
{
    QStringList msgList = fileMsg.split("\n");
    for (QString msgStr : msgList) {
        if (msgStr.startsWith(FILEKEY_NAME)) {
            infoMap->insert(PROCINFOKEY_NAME, msgStr.split("\t").at(1));
            continue;
        }
        if (msgStr.startsWith(FILEKEY_STATE)) {
            infoMap->insert(PROCINFOKEY_STATE, msgStr.split("\t").at(1));
            continue;
        }
        if (msgStr.startsWith(FILEKEY_TGID)) {
            infoMap->insert(PROCINFOKEY_TGID, msgStr.split("\t").at(1));
            continue;
        }
        if (msgStr.startsWith(FILEKEY_UID)) {
            infoMap->insert(PROCINFOKEY_UID, msgStr.split("\t").at(1));
            break;
        }
    }
}

void ProcAddServer::emitSignal(MyMap procmap)
{
    Q_EMIT this->procAdd(procmap);
}

ProcAddServer::ProcAddServer(QObject *parent) : QObject(parent)
{
    qDBusRegisterMetaType<QMap<QString, QString>>();
    m_queDutyPool = QThreadPool::globalInstance();
    m_queDutyPool->setMaxThreadCount(THREAD_MAXNUM);
}
