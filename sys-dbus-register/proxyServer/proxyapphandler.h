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

#ifndef PROXYAPPHANDLER_H
#define PROXYAPPHANDLER_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QDir>
#include <QMap>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QtDBus/QDBusMetaType>
#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>
#include <errno.h>
#include <QThreadPool>
#include <QRunnable>
#include <QMetaObject>

#include <QTime>

extern "C"
{
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <linux/cn_proc.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

}

#define THREAD_MAXNUM 5
#define READLINE_MAXSIZE 256

#define PROCINFOKEY_TYPE "type"
#define PROCINFOKEY_PID "pid"
#define PROCINFOKEY_CMDLINE "cmdline"
#define PROCINFOKEY_UID "uid"
#define PROCINFOKEY_NAME "name"
#define PROCINFOKEY_STATE "state"
#define PROCINFOKEY_TGID "tgid"
#define PROCINFOKEY_DESKTOP "desktop"

#define FILEKEY_DESKTOP "GIO_LAUNCHED_DESKTOP_FILE="
#define FILEKEY_NAME "Name:"
#define FILEKEY_STATE "State:"
#define FILEKEY_TGID "Tgid:"
#define FILEKEY_UID "Uid:"

typedef QMap<QString, QString> ProcessInfoMap;

class ListenThObject : public QObject,protected QDBusContext
{
    Q_OBJECT
public:
    explicit ListenThObject(QObject *parent = nullptr);

    inline void setExitStat(bool stat)
    {
        m_exitStat = stat;
    }

    inline void setSockFd(int sockFd)
    {
        m_sockFd = sockFd;
    }

private:
    bool m_exitStat = false;
    int m_sockFd;

public Q_SLOTS:
    int handleProcEv();

Q_SIGNALS:
    void procAdd(int pid);
};

class DealData : public QRunnable
{
public:
    explicit DealData(int pid, QObject *obj);
    void run() override;

private:
    QString getFileMsg(QString filePath);

    bool isSystemCmd(QString cmdline);
    QString getEnvironMsg(QString environ);
    void getStatusMsg(QString fileMsg, QMap<QString, QString> *infoMap);

private:
    int m_pid;
    QObject *m_obj;
};

class ProcAddServer : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface","com.kylin.network.proxy")
public:
    explicit ProcAddServer(QObject *parent = nullptr);

private:
    bool netlinkConnect();
    bool setProcEvListen(int nl_sock, bool enable);

private Q_SLOTS:
    void emitSignal(ProcessInfoMap procmap);

public Q_SLOTS:
    void startListen(int pid);
    void stopListen(int pid);

private:
    int m_sockFd = -1;
    QThread *m_listenTh = nullptr;
    ListenThObject *m_listenThObj = nullptr;

    QThreadPool *m_queDutyPool = nullptr;

    QList<int> m_usedModelList;
Q_SIGNALS:
    void procAdd(ProcessInfoMap procinfo);
};

#endif // PROXYAPPHANDLER_H
