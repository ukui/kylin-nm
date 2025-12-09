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
#include <cstddef>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <QDebug>
#include <vector>

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
        qDebug() << "Failed to create netlink socket, errno:" << errno;
        return false;
    }
    qDebug() << "Netlink socket created, fd:" << m_sockFd;

    memset(&sa_nl, 0, sizeof(sa_nl));
    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_groups = CN_IDX_PROC;
    sa_nl.nl_pid = getpid();

    qDebug() << "Binding netlink socket, pid:" << sa_nl.nl_pid;

    if (bind(m_sockFd, (struct sockaddr *)&sa_nl, sizeof(sa_nl)) < 0) {
        perror("bind");
        qDebug() << "Failed to bind netlink socket, errno:" << errno;
        close(m_sockFd);
        m_sockFd = -1;
        return false;
    }

    qDebug() << "Netlink socket bound successfully";
    return true;
}

/*
 * subscribe on proc events (process notifications)
 */
bool ProcAddServer::setProcEvListen(int nl_sock, bool enable)
{
    /* 动态分配消息内存 */
    std::size_t msg_size = NLMSG_SPACE(sizeof(struct cn_msg) + sizeof(enum proc_cn_mcast_op));
    std::vector<char> buf(msg_size);
    memset(buf.data(), 0, msg_size);

    struct nlmsghdr *nlh = (struct nlmsghdr *)buf.data();
    struct cn_msg *cn = (struct cn_msg *)NLMSG_DATA(nlh);
    enum proc_cn_mcast_op *data = (enum proc_cn_mcast_op *)(cn + 1);

    nlh->nlmsg_len = msg_size;
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_type = NLMSG_DONE;

    cn->id.idx = CN_IDX_PROC;
    cn->id.val = CN_VAL_PROC;
    cn->len = sizeof(enum proc_cn_mcast_op);

    *data = enable ? PROC_CN_MCAST_LISTEN : PROC_CN_MCAST_IGNORE;

    if (send(nl_sock, buf.data(), msg_size, 0) < 0) {
        perror("netlink send");
        qDebug() << "Failed to send netlink message, errno:" << errno;
        return false;
    } else {
        qDebug() << "Netlink message sent successfully";
    }

    return true;
}

/*
 * handle a single process event
 */
int ListenThObject::handleProcEv()
{
    /* 使用动态分配的缓冲区，避免栈溢出 */
    const std::size_t BUFFER_SIZE = 8192;  // 8KB 缓冲区
    std::vector<char> buffer(BUFFER_SIZE);
    char *buf = buffer.data();

    struct nlmsghdr *nlh;
    struct cn_msg *cn;
    struct proc_event *proc_ev;

    while (!m_exitStat) {
        ssize_t rc = recv(m_sockFd, buf, BUFFER_SIZE, 0);
        if (rc == 0) {
            /* shutdown? */
            qDebug() << "连接关闭";
            return 0;
        } else if (rc == -1) {
            if (errno == EINTR) continue;
            perror("netlink recv");
            qDebug() << "接收错误, errno:" << errno;
            return -1;
        }

        /* 解析消息 */
        nlh = (struct nlmsghdr *)buf;

        /* 检查消息长度 */
        if ((std::size_t)rc < sizeof(struct nlmsghdr)) {
            qDebug() << "消息过短，无法解析nlmsghdr";
            continue;
        }

        /* 获取cn_msg */
        cn = (struct cn_msg *)NLMSG_DATA(nlh);

        /* 检查cn_msg长度 */
        if ((char *)cn + sizeof(struct cn_msg) > buf + rc) {
            qDebug() << "消息不完整，无法解析cn_msg";
            continue;
        }

        /* 验证connector消息 */
        if (cn->id.idx != CN_IDX_PROC || cn->id.val != CN_VAL_PROC) {
            qDebug() << "非进程事件消息, 忽略";
            continue;
        }

        /* 获取proc_event */
        proc_ev = (struct proc_event *)(cn->data);

        /* 检查proc_event长度 */
        if ((char *)proc_ev + sizeof(struct proc_event) > buf + rc) {
            qDebug() << "消息不完整，无法解析proc_event";
            continue;
        }

        /* 处理各种事件类型 */
        switch (proc_ev->what) {
            case PROC_EVENT_FORK:
                if (proc_ev->event_data.fork.parent_pid > 1) {
                    Q_EMIT this->procAdd(proc_ev->event_data.fork.parent_pid);
                }
                break;

            case PROC_EVENT_EXEC:
                if (proc_ev->event_data.exec.process_pid > 1) {
                    Q_EMIT this->procAdd(proc_ev->event_data.exec.process_pid);
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

    /* 确保ProcessInfoMap类型已注册 */
    static bool registered = false;
    if (!registered) {
        qRegisterMetaType<ProcessInfoMap>("ProcessInfoMap");
        qDBusRegisterMetaType<ProcessInfoMap>();
        registered = true;
    }

    m_pid = pid;
    m_obj = obj;
}

bool DealData::isSystemCmd(QString cmdline)
{
    if (cmdline.isEmpty()) {
        return true;
    }
    QStringList fileLine = cmdline.split(" ");
    //QSet<QString> intersection = systemCmdList.toSet().intersect(fileLine.toSet());
    QSet<QString> systemCmdSet(systemCmdList.begin(), systemCmdList.end());
    QSet<QString> fileLineSet(fileLine.begin(), fileLine.end());
    QSet<QString> intersection = systemCmdSet.intersect(fileLineSet);
    if (intersection.isEmpty()) {
        return false;
    }
    return true;
}

void DealData::run()
{
    ProcessInfoMap procInfo;
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

        /* 确保在主线程中调用 */
        QMetaObject::invokeMethod(m_obj, "emitSignal", Qt::QueuedConnection, Q_ARG(ProcessInfoMap, procInfo));
    }
}

QString DealData::getFileMsg(QString filePath)
{
    QFile file(filePath);
    if (file.exists() && file.open(QFile::ReadOnly)) {
        QByteArray fileMsg = file.readAll();
        // 去掉fileMsg中的乱码，用空格替代
        fileMsg.replace('\0', ' ');
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

void ProcAddServer::emitSignal(ProcessInfoMap procmap)
{
    qDebug() << "ProcAddServer::emitSignal procAdd"<<procmap;
    Q_EMIT this->procAdd(procmap);
}

ProcAddServer::ProcAddServer(QObject *parent) : QObject(parent)
{
    /* 确保ProcessInfoMap类型已注册 */
    qRegisterMetaType<ProcessInfoMap>("ProcessInfoMap");
    qDBusRegisterMetaType<ProcessInfoMap>();

    m_queDutyPool = QThreadPool::globalInstance();
    m_queDutyPool->setMaxThreadCount(THREAD_MAXNUM);
}
