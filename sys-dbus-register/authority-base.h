#ifndef AUTHORITYBASE_H
#define AUTHORITYBASE_H

#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>
#include <errno.h>
#include <QStringList>

static const QStringList programWhiteList={
    "/usr/bin/kylin-nm",
    "/usr/bin/ukui-control-center",
    "/usr/bin/kylin-nm-plugin",
};

#define  checkEnviron(ret,pid)    \
    do {   \
        QString filePath = QString("/proc/%1/environ").arg(pid);    \
        QFile file(filePath);   \
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {    \
        qDebug() << "file.open failed" << filePath; \
        ret = false;  break; \
    }   \
    QTextStream in(&file);  \
    QString data = in.readAll();    \
    file.close();   \
    data.replace('\0', '\n');   \
    QStringList environVars = data.split('\n'); \
    for (const QString &var : environVars){ \
        if (!var.isEmpty()) {   \
            int pos = var.indexOf('='); \
            if (pos != -1) {    \
                QString key = var.left(pos);    \
                if (key == "LD_PRELOAD" || key == "LD_LIBRARY_PATH" || key == "LD_AUDIT"){  \
                    ret = false;break;   \
                }   \
            }   \
        }   \
    }   \
    ret = true;    \
    }while(0);

#define isAllowedCaller(a)   \
do {   \
    bool macroRet = false;\
    QString execPath="";    \
    QString clientService = message().service();    \
    QDBusConnectionInterface *intf = connection().interface();  \
    unsigned int  pid = intf->servicePid(clientService).value();    \
    checkEnviron(macroRet, pid);     \
    if(!macroRet) {   \
        sendErrorReply(QDBusError::ErrorType::Failed,QString("[%1] dbus method control,env forbidden").arg((pid))); \
        a = false; \
        break;   \
    }  \
    QFileInfo file(QString("/proc/%1/exe").arg(pid));   \
    if (file.exists()) {    \
        execPath = file.canonicalFilePath(); \
        bool hadBreakIntisAllowedCaller = false;    \
        Q_FOREACH (const QString& whitePath,programWhiteList){  \
            if (execPath == whitePath){ \
                a = true;  hadBreakIntisAllowedCaller = true; break;\
            }   \
        }   \
        if (hadBreakIntisAllowedCaller) break;  \
    }   \
   sendErrorReply(QDBusError::ErrorType::Failed,QString("[%1] dbus method control,operation not permitted").arg((execPath)));   \
   a = false;    \
} while(0);

#define PEEK_LIMIT() bool checret; \
                            isAllowedCaller(checret)  \
                            if (!checret) return;

#define PEEK_LIMIT_RET(a)   bool checret; \
                            isAllowedCaller(checret)  \
                            if (!checret) {return a;}

#endif // AUTHORITYBASE_H
