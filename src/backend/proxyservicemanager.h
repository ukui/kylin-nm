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


#ifndef PROXYSERVICEMANAGER_H
#define PROXYSERVICEMANAGER_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDir>
#include <unistd.h>
#include <glib.h>
#include <QtCore>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDBus/QDBusMetaType>
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>
#include <QGSettings>

#define APPPROXY_FILE ".config/application-proxy.json"
#define PROXYCONF_FILE ".config/proto-config.json"
#define CUSTOMAPP_FILE "/usr/share/custom_app/custom_app.json"

#define JSON_KEY_APPLICATION "application"
#define DESKTOP_FILE_PATH       "/usr/share/applications/"
#define USR_SHARE_APP_CURRENT   "/usr/share/applications/."
#define USR_SHARE_APP_UPER      "/usr/share/applications/.."

#define DESKTOP_EXEC_KEY "Exec="
#define ANDROID_FILE_PATH       "/.local/share/applications/"
#define ANDROID_APP_CURRENT     "/.local/share/applications/."
#define ANDROID_APP_UPER        "/.local/share/applications/.."

#define PROTOJSON_KEY_TYPE "type"
#define PROTOJSON_KEY_SERVER "Server"
#define PROTOJSON_KEY_PORT "Port"
#define PROTOJSON_KEY_USRNAME "UserName"
#define PROTOJSON_KEY_PASSWORD "Password"
#define PROTOJSON_KEY_NAME "name"
#define PROTOJSON_KEY_STATE "state"

#define APPINFOMAP_KEY_NAME "Name"
#define APPINFOMAP_KEY_LOCALNAME "Localname"
#define APPINFOMAP_KEY_ICON "Icon"
#define APPINFOMAP_KEY_EXEC "Exec"
#define APPINFOMAP_KEY_KEYWORDS "Keywords"
#define APPINFOMAP_KEY_COMMENT "Comment"

#define PROCINFOKEY_TYPE "type"
#define PROCINFOKEY_PID "pid"
#define PROCINFOKEY_CMDLINE "cmdline"
#define PROCINFOKEY_UID "uid"
#define PROCINFOKEY_NAME "name"
#define PROCINFOKEY_STATE "state"
#define PROCINFOKEY_TGID "tgid"
#define PROCINFOKEY_DESKTOP "desktop"

class ProcInfoDeal : public QObject
{
    Q_OBJECT
public:
    explicit ProcInfoDeal(QObject *parent = nullptr);

    void setProxyState(bool state);

    inline void setDesktopList(QStringList list) {
        m_proxyDesktopList = list;
    }

    inline void setExecList(QStringList list) {
        m_proxyExecList = list;
    }

    inline void setNameList(QStringList list) {
        m_proxyNameList = list;
    }

public Q_SLOTS:
    void startConnect();

private:

    void addProcDbus(const qint32 pid);

private:
    QDBusInterface  *m_proxyInterface = nullptr;
    QDBusInterface  *m_procAddServerDbus = nullptr;

    QStringList m_proxyDesktopList;
    QStringList m_proxyExecList;
    QStringList m_proxyNameList;

private Q_SLOTS:
    void onProcAdd(QMap<QString, QString> map);
};

class ProxyServiceManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface","com.kylin.network.proxy")
public:
    explicit ProxyServiceManager(QObject *parent = nullptr);
    ~ProxyServiceManager();
    void ProxyServiceManagerNew();
    void start();
    void stop();

private:
    void initProxyState();
    void initSystemProxyCtlLogSender();
    void initSystemProxyConfig();
    void startProxyDbus(const QJsonObject obj);
    void addProxy(const QJsonObject obj);
    void addProxyDbus(const QJsonObject obj);
    void clearProxyDbus();
    void startProxy(const QJsonObject obj);
    void stopProxyDbus();
    void stopProxy();
    void delValueFromArray(QJsonArray *array, const QJsonValue item);
    void setProxyFile(QString desktopfp, bool create);

    QStringList getDesktopFilePath();

    QStringList getCustomizedAppList(QString filePath);
    void recursiveSearchFile(const QString &_filePath);

    // 日志上报函数
    void callSendSysProxyNetCtlLog(const QString& key, const QString& value, const QString& str, QString& lastValue);    
    void sendAppProxyNetCtlLog(const QStringList& preAppInfo, bool preStatus,
                              const QStringList& currAppInfo, bool status,
                              bool addApp = true, const QString& appName = QString());
    void sendAptProxyNetCtlLog(const QHash<QString, QVariant>& preAptInfo,
                              QString http_host, QString http_port,
                              QString https_host, QString https_port, bool status);
    void sendProxyNetCtlLog(const QString& logMessage);


#ifdef ENABLE_ANDROIDAPP
    void getAndroidApp();
#endif

    inline bool getProxyState() {
        return m_proxyState;
    }
    inline void setProxyState(bool state) {
        m_proxyState = state;
    }

    void initAppInfoMapTemp();
    QMap<QString, QString> getDesktopFileInfo(QString desktopfp);
    void getProxyInfoList();

    bool addDesktopFileIntoProcessManager(QString desktopfp);
    bool delDesktopFileFromProcessManager(QString desktopfp);
    QStringList getProcessManagerDesktopFile();

    bool initAppIntoProcessManager();
    bool clearProcessManagerApp();

private:
    QDBusInterface  *m_proxyInterface = nullptr;
    QDBusInterface  *m_kylinProcessManagerDbus = nullptr;

    QStringList m_filePathList;
    QStringList m_androidDesktopfnList;

    GError **m_error = nullptr;
    GKeyFileFlags m_flags = G_KEY_FILE_NONE;
    GKeyFile *m_keyfile = nullptr;

    bool m_proxyState = false;

    QTimer *m_time = nullptr;
    QThread *m_thread = nullptr;
    ProcInfoDeal *m_threadObj = nullptr;

    QMap<QString, QMap<QString, QString>> m_appInfoMap;

    QStringList m_proxyDesktopList;
    QStringList m_proxyExecList;
    QStringList m_proxyNameList;

    QDBusInterface *m_pNetCtlInterface;

    QGSettings * m_pProxySettings;
    QGSettings * m_pHttpSettings;
    QGSettings * m_pHttpsSettings;
    QGSettings * m_pFtpSettings;
    QGSettings * m_pSocksSettings;

    QString m_sysProxyType;
    QString m_sysAutoConfigUrl;
    QString m_sysHttpIP;
    QString m_sysHttpPort;
    QString m_sysHttpsIP;
    QString m_sysHttpsPort;
    QString m_sysFtpIP;
    QString m_sysFtpPort;
    QString m_sysSocksIP;
    QString m_sysSocksPort;
    QString m_sysIgnoreHosts;

Q_SIGNALS:  // 添加信号声明
    void proxyConfigChanged(const QStringList &configList);
    void appProxyStateChanged(bool state);
    void appProxyAppListChanged(const QString &desktopfp, bool added);  // 单个应用变化
    void appProxyAllListChanged();  // 整个列表变化
    void aptProxyChanged(const QHash<QString, QVariant> &aptInfo);

public Q_SLOTS:
    QStringList getAppProxyConfig();
    void setAppProxyConfig(const QStringList configList);
    QMap<QString, QStringList> getAppProxyAppsInfo();
    void addAppIntoProxy(QString desktopfp);
    void delAppIntoProxy(QString desktopfp);
    void setAppProxyState(bool state);
    bool getAppProxyState();
    void setAptProxy(QString host , QString port , QString https_host, QString https_port, bool status); //  apt代理对应的配置文件的写入或删除
    QHash<QString, QVariant> getAptProxy();
    void setAptProxyState(bool state);

private Q_SLOTS:
    void init();
};

#endif // PROXYSERVICEMANAGER_H
