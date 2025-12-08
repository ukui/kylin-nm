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

#include <QDebug>
#include "proxyservicemanager.h"

#define PROXY_SCHEMA              "org.gnome.system.proxy"
#define HTTP_PROXY_SCHEMA         "org.gnome.system.proxy.http"
#define HTTPS_PROXY_SCHEMA        "org.gnome.system.proxy.https"
#define FTP_PROXY_SCHEMA          "org.gnome.system.proxy.ftp"
#define SOCKS_PROXY_SCHEMA        "org.gnome.system.proxy.socks"

#define PROXY_MODE_KEY            "mode"
#define PROXY_AUTOCONFIG_URL_KEY  "autoconfigUrl"
#define IGNORE_HOSTS_KEY          "ignoreHosts"
#define PROXY_HOST_KEY       "host"
#define PROXY_PORT_KEY       "port"

QJsonObject readJsonFile(QString filePath)
{
    QJsonObject readObj = QJsonObject();
    if (filePath.isEmpty() || !QFile(filePath).exists()) {
        qWarning() << filePath << "is not exits!";
        return readObj;
    }

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QByteArray readBy = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument readDoc = QJsonDocument::fromJson(readBy, &error);
    if (!readDoc.isEmpty() && error.error == QJsonParseError::NoError) {
        readObj = readDoc.object();
    }

    return readObj;
}

QJsonObject dealJsonObj(const QStringList configList)
{
    QJsonObject configObj = QJsonObject();
    if (configList.isEmpty() && configList.count() < 3) {
        qWarning() << "configList item less!";
        return configObj;
    }

    configObj.insert(PROTOJSON_KEY_TYPE, QJsonValue(configList.at(0)));
    configObj.insert(PROTOJSON_KEY_NAME, QJsonValue("default"));
    configObj.insert(PROTOJSON_KEY_SERVER, QJsonValue(configList.at(1)));
    QString prot = configList.at(2);
    configObj.insert(PROTOJSON_KEY_PORT, QJsonValue(prot.toInt()));
    switch (configList.count()) {
    case 4:
        configObj.insert(PROTOJSON_KEY_USRNAME, QJsonValue(configList.at(3)));
        configObj.insert(PROTOJSON_KEY_PASSWORD, QJsonValue(""));
        break;

    case 5:
        configObj.insert(PROTOJSON_KEY_USRNAME, QJsonValue(configList.at(3)));
        configObj.insert(PROTOJSON_KEY_PASSWORD, QJsonValue(configList.at(4)));
        break;

    default:
        configObj.insert(PROTOJSON_KEY_USRNAME, QJsonValue(""));
        configObj.insert(PROTOJSON_KEY_PASSWORD, QJsonValue(""));
        break;
    }
    configObj.insert(PROTOJSON_KEY_STATE, QJsonValue(true));
    return configObj;
}

void wirteJsonFile(QString filePath, const QJsonObject obj)
{
    if (filePath.isEmpty() || obj.isEmpty()) {
        qWarning() << "function input filePath or obj is error!";
        return;
    }
    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QJsonDocument writeDoc = QJsonDocument(obj);
    file.write(writeDoc.toJson());
    file.flush();
    file.close();
}

QStringList getAppProxyFromFile()
{
    QStringList appProxyList;
    QString jsonPath = QDir::homePath() + "/" + APPPROXY_FILE;
    QJsonObject readObj = readJsonFile(jsonPath);
    QJsonArray appArray = readObj.value("application").toArray();
    if (!appArray.isEmpty()) {
        for (auto appItem : appArray) {
            appProxyList.append(appItem.toString());
        }
    }
    return appProxyList;
}

void ProxyServiceManager::getProxyInfoList()
{
    m_proxyExecList.clear();
    m_proxyNameList.clear();
    for (auto desktop : m_proxyDesktopList) {
        if (m_appInfoMap.contains(desktop)) {
            QMap<QString, QString> map = m_appInfoMap.value(desktop);
            if (map.value(APPINFOMAP_KEY_KEYWORDS).contains("Android")) {
                m_proxyExecList.append(map.value(APPINFOMAP_KEY_COMMENT));
            } else {
                QStringList execlist = map.value(APPINFOMAP_KEY_EXEC).split(" ");
                m_proxyExecList.append(execlist.at(0));
            }
            m_proxyNameList.append(map.value(APPINFOMAP_KEY_ICON));
        }
    }
}

ProcInfoDeal::ProcInfoDeal(QObject *parent) : QObject(parent)
{
    qDBusRegisterMetaType<QMap<QString, QString>>();
}

void ProcInfoDeal::startConnect()
{
    m_proxyInterface = new QDBusInterface("com.kylin.system.proxy",
                                          "/com/kylin/system/proxy/App",
                                          "com.kylin.system.proxy.App",
                                          QDBusConnection::systemBus(), this);
    if (!m_proxyInterface->isValid())
        qDebug() << Q_FUNC_INFO << "========================foijwoiefjoiwejfoiwjefoiwjeofiweiofjoweijfoe";

}

void ProcInfoDeal::setProxyState(bool state)
{
    if (m_procAddServerDbus == nullptr) {
        m_procAddServerDbus = new QDBusInterface("com.kylin.network.qt.systemdbus",
                                                 "/com/kylin/proxy",
                                                 "com.kylin.network.proxy",
                                                 QDBusConnection::systemBus(), this);
    }
    if (m_procAddServerDbus->isValid()) {
        if (state) {
            //m_procAddServerDbus->asyncCall("startListen", getpid());
            QDBusReply<void> reply = m_procAddServerDbus->call("startListen", getpid());
            if (!reply.isValid()) {
                qWarning() << "D-Bus call startListen failed: " << reply.error().message();
            }
            //用QObject的connect会报没有procAdd这个信号，使用QDBusConnection可以监听到这个信号，随后调研下为啥在别的地方可以connect到
//            connect(m_procAddServerDbus, SIGNAL(procAdd(QMap<QString, QString>)), this, SLOT(onProcAdd(QMap<QString, QString>)), Qt::QueuedConnection);
            QDBusConnection::systemBus().connect("com.kylin.network.qt.systemdbus",
                                                 "/com/kylin/proxy",
                                                 "com.kylin.network.proxy",
                                                 "procAdd",
                                                 this,
                                                 SLOT(onProcAdd(QMap<QString, QString>)));
        } else {
            m_procAddServerDbus->asyncCall("stopListen", getpid());
            m_procAddServerDbus->deleteLater();
            m_procAddServerDbus = nullptr;
        }
    }
}

void ProcInfoDeal::onProcAdd(QMap<QString, QString> map)
{
    //应用代理去掉命令类的进程、PID小于等于1的进程、uid不等于当前用户的进程
    if (map.value(PROCINFOKEY_TYPE) == "sys" || map.value(PROCINFOKEY_PID).toInt() <= 1 || map.value(PROCINFOKEY_UID).toInt() != getuid()) {
        return;
    }

    //根据name进行匹配，忽略大小写
    if (!map.value(PROCINFOKEY_NAME).isEmpty() && m_proxyNameList.contains(map.value(PROCINFOKEY_NAME), Qt::CaseInsensitive)) {
        addProcDbus(map.value(PROCINFOKEY_PID).toInt());
        return;
    }

    //根据desktop进行匹配
    if (!map.value(PROCINFOKEY_DESKTOP).isEmpty()) {
        if (m_proxyDesktopList.contains(map.value(PROCINFOKEY_DESKTOP))) {
            addProcDbus(map.value(PROCINFOKEY_PID).toInt());
        } else {
            QStringList list = map.value(PROCINFOKEY_DESKTOP).split("/");
            QString desktop = list.at(list.size() - 1);
            for (QString name : m_proxyDesktopList) {
                if (name.contains(desktop)) {
                    addProcDbus(map.value(PROCINFOKEY_PID).toInt());
                    break;
                }
            }
        }
        return;
    }

    //根据exec进行匹配
    if (!map.value(PROCINFOKEY_CMDLINE).isEmpty()) {
        QString cmdline = map.value(PROCINFOKEY_CMDLINE);
        if (cmdline.isEmpty()) {
            return;
        }
        QStringList execlist = cmdline.split(" ");
        if (m_proxyExecList.contains(execlist.at(0), Qt::CaseInsensitive)) {
            addProcDbus(map.value(PROCINFOKEY_PID).toInt());
        }
    }

}

ProxyServiceManager::ProxyServiceManager(QObject *parent) : QObject(parent)
{
    qDBusRegisterMetaType<QStringList>();
    qDBusRegisterMetaType<QMap<QString, QStringList >>();
    start();
}

ProxyServiceManager::~ProxyServiceManager()
{
    if (m_thread && m_thread->isRunning()) {
        m_thread->quit();
        m_thread->wait();
    }
}

void ProxyServiceManager::init()
{
    m_time->stop();
    m_proxyInterface = new QDBusInterface("com.kylin.system.proxy",
                                          "/com/kylin/system/proxy/App",
                                          "com.kylin.system.proxy.App",
                                          QDBusConnection::systemBus(), this);

    m_thread = new QThread();
    m_threadObj = new ProcInfoDeal();
    m_threadObj->moveToThread(m_thread);
    connect(m_thread, &QThread::finished, [=] () {
        m_thread->deleteLater();
        m_threadObj->deleteLater();
    });

    connect(m_thread, &QThread::started, m_threadObj, &ProcInfoDeal::startConnect);
    m_thread->start();

    m_kylinProcessManagerDbus = new QDBusInterface("com.kylin.ProcessManager",
                                                   "/com/kylin/ProcessManager/AppWhitelist",
                                                   "com.kylin.ProcessManager.AppWhitelist",
                                                   QDBusConnection::sessionBus(), this);


    initProxyState();
    initSystemProxyCtlLogSender();
}

void ProxyServiceManager::callSendSysProxyNetCtlLog(const QString& key, const QString& value, const QString& str, QString& lastValue){
    if(!m_pNetCtlInterface || (value == lastValue)){
        return;
    }

    QString preValue = lastValue;
    QString currValue = value;
    /* 变量判空，若是赋值为"空",增强日志可读性 */
    QString emptyStr = QString("空");
    if(currValue.isEmpty()){
        currValue = emptyStr;
    }
    if(preValue.isEmpty()){
        preValue = emptyStr;
    }//end
    QString message = QString("系统代理变化, 原%1：%2，新%1：%3").arg(str).arg(preValue).arg(currValue);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"message:"<<message;

    lastValue = value;
    m_pNetCtlInterface->call("sendSysProxyNetCtlLog", message);
    return;
}

void ProxyServiceManager::initSystemProxyCtlLogSender() {
    const QByteArray proxy_id(PROXY_SCHEMA);
    const QByteArray http_id(HTTP_PROXY_SCHEMA);
    const QByteArray https_id(HTTPS_PROXY_SCHEMA);
    const QByteArray ftp_id(FTP_PROXY_SCHEMA);
    const QByteArray sock_id(SOCKS_PROXY_SCHEMA);

    m_pProxySettings = new QGSettings(proxy_id,QByteArray(),this);
    m_pHttpSettings = new QGSettings(http_id,QByteArray(),this);
    m_pHttpsSettings = new QGSettings(https_id,QByteArray(),this);
    m_pFtpSettings = new QGSettings(ftp_id,QByteArray(),this);
    m_pSocksSettings = new QGSettings(sock_id,QByteArray(),this);

    initSystemProxyConfig();

    m_pNetCtlInterface = new QDBusInterface("com.kylin.networkCtrol",
                                       "/com/kylin/networkCtrol",
                                       "com.kylin.networkCtrol",
                                       QDBusConnection::systemBus());

    connect(m_pProxySettings, &QGSettings::changed, this, [=](QString key){
        if(PROXY_MODE_KEY == key){
            callSendSysProxyNetCtlLog(key, m_pProxySettings->get(key).toString(), "代理类型", m_sysProxyType);
        }else if(PROXY_AUTOCONFIG_URL_KEY == key){
            callSendSysProxyNetCtlLog(key, m_pProxySettings->get(key).toString(), "配置URL", m_sysAutoConfigUrl);
        }else if(IGNORE_HOSTS_KEY == key){
            callSendSysProxyNetCtlLog(key, m_pProxySettings->get(key).toStringList().join(";"), "忽略的主机列表", m_sysIgnoreHosts);
        }
    });

    connect(m_pHttpSettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pHttpSettings->get(key).toString(), "HTTP代理的IP", m_sysHttpIP);
        } else if (PROXY_PORT_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pHttpSettings->get(key).toString(), "HTTP代理的端口", m_sysHttpPort);
        } else {
            QString lastValue;
            callSendSysProxyNetCtlLog(key, m_pHttpSettings->get(key).toString(), "HTTP代理", lastValue);
        }
    });

    connect(m_pHttpsSettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pHttpsSettings->get(key).toString(), "HTTPS代理的IP", m_sysHttpsIP);
        } else if (PROXY_PORT_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pHttpsSettings->get(key).toString(), "HTTPS代理的端口", m_sysHttpsPort);
        } else {
            QString lastValue;
            callSendSysProxyNetCtlLog(key, m_pHttpsSettings->get(key).toString(), "HTTPS代理", lastValue);
        }
    });

    connect(m_pFtpSettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pFtpSettings->get(key).toString(), "FTP代理的IP", m_sysFtpIP);
        } else if (PROXY_PORT_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pFtpSettings->get(key).toString(), "FTP代理的端口", m_sysFtpPort);
        } else {
            QString lastValue;
            callSendSysProxyNetCtlLog(key, m_pFtpSettings->get(key).toString(), "FTP代理", lastValue);
        }
    });

    connect(m_pSocksSettings, &QGSettings::changed, this, [=](QString key){
        if (PROXY_HOST_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pSocksSettings->get(key).toString(), "SOCKS代理的IP", m_sysSocksIP);
        } else if (PROXY_PORT_KEY == key) {
            callSendSysProxyNetCtlLog(key, m_pSocksSettings->get(key).toString(), "SOCKS代理的端口", m_sysSocksPort);
        } else {
            QString lastValue;
            callSendSysProxyNetCtlLog(key, m_pSocksSettings->get(key).toString(), "SOCKS代理", lastValue);
        }
    });
}

void ProxyServiceManager::initSystemProxyConfig()
{
    if(m_pProxySettings){
        m_sysProxyType = m_pProxySettings->get(PROXY_MODE_KEY).toString();
        m_sysAutoConfigUrl = m_pProxySettings->get(PROXY_AUTOCONFIG_URL_KEY).toString();
        m_sysIgnoreHosts = m_pProxySettings->get(IGNORE_HOSTS_KEY).toStringList().join(";");
    }
    if(m_pHttpSettings){
        m_sysHttpIP = m_pHttpSettings->get(PROXY_HOST_KEY).toString();
        m_sysHttpPort = m_pHttpSettings->get(PROXY_PORT_KEY).toString();
    }
    if(m_pHttpsSettings){
        m_sysHttpsIP = m_pHttpsSettings->get(PROXY_HOST_KEY).toString();
        m_sysHttpsPort = m_pHttpsSettings->get(PROXY_PORT_KEY).toString();
    }
    if(m_pFtpSettings){
        m_sysFtpIP = m_pFtpSettings->get(PROXY_HOST_KEY).toString();
        m_sysFtpPort = m_pFtpSettings->get(PROXY_PORT_KEY).toString();
    }
    if(m_pSocksSettings){
        m_sysSocksIP = m_pSocksSettings->get(PROXY_HOST_KEY).toString();
        m_sysSocksPort = m_pSocksSettings->get(PROXY_PORT_KEY).toString();
    }
}

void ProxyServiceManager::initAppInfoMapTemp()
{
    QStringList desktopfpList = getDesktopFilePath();
    for (QString fd : m_appInfoMap.keys()) {
        if (!desktopfpList.contains(fd)) {
            m_appInfoMap.remove(fd);
        }
    }

    for (QString desktopfp : desktopfpList) {
        if (!m_appInfoMap.contains(desktopfp)) {
            QMap<QString, QString> infoMap = getDesktopFileInfo(desktopfp);
            m_appInfoMap.insert(desktopfp, infoMap);
        }
    }
}

QMap<QString, QString> ProxyServiceManager::getDesktopFileInfo(QString desktopfp)
{
    QMap<QString, QString> filemsg;
    if (desktopfp.isEmpty()) {
        qWarning()<<Q_FUNC_INFO<<"desktop path is empty!";
        return filemsg;
    }
    GError **error = nullptr;
    GKeyFileFlags flags = G_KEY_FILE_NONE;
    GKeyFile *keyfile = g_key_file_new();
    QByteArray fpbyte = desktopfp.toLocal8Bit();
    char *filepath = fpbyte.data();
    g_key_file_load_from_file(keyfile, filepath, flags, error);
    char *name = g_key_file_get_string(keyfile, "Desktop Entry", APPINFOMAP_KEY_NAME, nullptr);
    QString namestr = QString::fromLocal8Bit(name);
    char *localname = g_key_file_get_locale_string(keyfile, "Desktop Entry", APPINFOMAP_KEY_NAME, nullptr, nullptr);
    QString localnamestr = QString::fromLocal8Bit(localname);
    char *icon = g_key_file_get_string(keyfile, "Desktop Entry", APPINFOMAP_KEY_ICON, nullptr);
    QString iconstr = QString::fromLocal8Bit(icon);
    char *exec = g_key_file_get_string(keyfile, "Desktop Entry", APPINFOMAP_KEY_EXEC, nullptr);
    QString execstr = QString::fromLocal8Bit(exec);
    char *keywords = g_key_file_get_string(keyfile, "Desktop Entry", APPINFOMAP_KEY_KEYWORDS, nullptr);
    QString keywordsstr = QString::fromLocal8Bit(keywords);
    char *comment = g_key_file_get_string(keyfile, "Desktop Entry", APPINFOMAP_KEY_COMMENT, nullptr);
    QString commentstr = QString::fromLocal8Bit(comment);

    filemsg.insert(APPINFOMAP_KEY_NAME, namestr);
    filemsg.insert(APPINFOMAP_KEY_LOCALNAME, localnamestr);
    filemsg.insert(APPINFOMAP_KEY_ICON, iconstr);
    filemsg.insert(APPINFOMAP_KEY_KEYWORDS, keywordsstr);
    filemsg.insert(APPINFOMAP_KEY_EXEC, execstr);
    filemsg.insert(APPINFOMAP_KEY_COMMENT, commentstr);
    g_key_file_free(keyfile);
    return filemsg;
}

void ProxyServiceManager::initProxyState()
{
    QString jsonPath = QDir::homePath() + "/" + PROXYCONF_FILE;
    QJsonObject readObj = readJsonFile(jsonPath);
    if (!readObj.value(PROTOJSON_KEY_TYPE).toString().isNull()
        && !readObj.value(PROTOJSON_KEY_SERVER).toString().isNull()
        && !readObj.value(PROTOJSON_KEY_PORT).isNull()
        && readObj.value(PROTOJSON_KEY_STATE).toBool()) {

        m_proxyDesktopList = getAppProxyFromFile();
        if (!initAppIntoProcessManager()) {
            qWarning() << "init app into kylin-process-manager false";
            return;
        }

        startProxy(readObj);

        initAppInfoMapTemp();
        getProxyInfoList();
        m_threadObj->setDesktopList(m_proxyDesktopList);
        m_threadObj->setExecList(m_proxyExecList);
        m_threadObj->setNameList(m_proxyNameList);
    } else {
        stopProxy();
    }
}

void ProxyServiceManager::start()
{
    qDebug() << "ProxyServiceManager ---------------start";
    m_time = new QTimer(this);
    connect(m_time, SIGNAL(timeout()), this, SLOT(init()));
    m_time->start(10);
}

void ProxyServiceManager::stop()
{
    qDebug() << "ProxyServiceManager ---------------stop";
}

//添加应用desktopfile到分级冻结管控白名单
bool ProxyServiceManager::addDesktopFileIntoProcessManager(QString desktopfp)
{
    if (desktopfp.isEmpty()) {
        qWarning() << "desktopfile string is empty!";
        return false;
    }

    if (m_kylinProcessManagerDbus == nullptr || !m_kylinProcessManagerDbus->isValid()) {
        qWarning() << Q_FUNC_INFO << "kylin-process-manager dbus is not valid!";
        return false;
    }

    QDBusReply<bool> reply = m_kylinProcessManagerDbus->call("AddApp", desktopfp);
    if (!reply) {
        qDebug() << "add kylin-process-manager error!";
        return false;
    }

    return true;
}

//从分级冻结管控白名单删除应用
bool ProxyServiceManager::delDesktopFileFromProcessManager(QString desktopfp)
{
    if (desktopfp.isEmpty()) {
        qWarning() << "desktopfile string is empty!";
        return false;
    }

    if (m_kylinProcessManagerDbus == nullptr || !m_kylinProcessManagerDbus->isValid()) {
        qWarning() << Q_FUNC_INFO << "kylin-process-manager dbus is not valid!";
        return false;
    }

    QDBusReply<bool> reply = m_kylinProcessManagerDbus->call("RemoveApp", desktopfp);
    if (!reply) {
        qDebug() << "remove kylin-process-manager error!";
        return false;
    }

    return true;
}

//获取分级冻结应用desktopfile列表
QStringList ProxyServiceManager::getProcessManagerDesktopFile()
{
    QStringList appList;

    if (m_kylinProcessManagerDbus == nullptr || !m_kylinProcessManagerDbus->isValid()) {
        qWarning() << Q_FUNC_INFO << "kylin-process-manager dbus is not valid!";
        return appList ;
    }

    QDBusReply<QStringList> reply = m_kylinProcessManagerDbus->call("AppList");

    if (!reply.isValid()) {
        qWarning() << "get kylin-process-manager applist error!";
        return appList;
    }

    appList =  reply.value();
    if (appList.isEmpty()) {
        qInfo() << "kylin-process-manager applist is empty";
    }

    return appList;
}

bool ProxyServiceManager::initAppIntoProcessManager()
{
    QStringList procManagerList = getProcessManagerDesktopFile();
    for (QString desktopfp : m_proxyDesktopList) {
        if (procManagerList.contains(desktopfp)) {
            continue;
        }

        addDesktopFileIntoProcessManager(desktopfp);
    }
    return true;
}

bool ProxyServiceManager::clearProcessManagerApp()
{
    QStringList procManagerList = getProcessManagerDesktopFile();
    for (QString desktopfp : m_proxyDesktopList) {
        if (!procManagerList.contains(desktopfp)) {
            continue;
        }

        delDesktopFileFromProcessManager(desktopfp);
    }
    return true;
}

void ProcInfoDeal::addProcDbus(const qint32 pid)
{
    if (!m_proxyInterface->isValid()) {
        qWarning() << "m_proxyInterface dbus is not valid!";
        return;
    }

    m_proxyInterface->asyncCall("AddProc", pid);
}

void ProxyServiceManager::startProxyDbus(const QJsonObject obj)
{
    if (obj.isEmpty()) {
        qWarning() << "obj is error!";
        return;
    }

    if (!m_proxyInterface->isValid()) {
        qWarning() << "m_proxyInterface dbus is not valid!";
        return;
    }

    QString proto = obj.value(PROTOJSON_KEY_TYPE).toString();
    m_proxyInterface->asyncCall("StartProxy", proto, "default", false);
}

void ProxyServiceManager::addProxyDbus(const QJsonObject obj)
{
    if (!m_proxyInterface->isValid()) {
        qWarning() << "m_proxyInterface dbus is not valid!";
        return;
    }

    QString proto = obj.value(PROTOJSON_KEY_TYPE).toString();
    QByteArray objArray = QJsonDocument(obj).toJson();
    qInfo() << "AddProxy proto:" + proto + ", default, objArray:" + objArray;
    m_proxyInterface->asyncCall("AddProxy", proto, "default", objArray);
}

void ProxyServiceManager::addProxy(const QJsonObject obj)
{
    if (obj.isEmpty()) {
        qWarning() << "obj is error!";
        return;
    }

    QJsonObject confObj = obj;
    confObj.remove(PROTOJSON_KEY_STATE);
    QString serverStr = confObj.value(PROTOJSON_KEY_SERVER).toString();

    if (QHostAddress(serverStr).protocol() != QAbstractSocket::UnknownNetworkLayerProtocol) {
        addProxyDbus(confObj);
    } else {
        QHostInfo info = QHostInfo::fromName(serverStr);
        if (info.error() != QHostInfo::NoError) {
            qWarning() << "Error occurred while looking up host:" << info.errorString();
            return ;
        }
        QHostAddress address = info.addresses().takeFirst();
        qInfo() << "Found address:" << address.toString();
        confObj.remove(PROTOJSON_KEY_SERVER);
        confObj.insert(PROTOJSON_KEY_SERVER, QJsonValue(address.toString()));
        addProxyDbus(confObj);
    }
}

void ProxyServiceManager::clearProxyDbus()
{
    if (!m_proxyInterface->isValid()) {
        qWarning() << "m_proxyInterface dbus is not valid!";
        return;
    }

    m_proxyInterface->asyncCall("ClearProxy");
}

void ProxyServiceManager::startProxy(const QJsonObject obj)
{
    clearProxyDbus();
    addProxy(obj);
    startProxyDbus(obj);
    m_threadObj->setProxyState(true);
    setProxyState(true);
}

void ProxyServiceManager::stopProxyDbus()
{
    if (!m_proxyInterface->isValid()) {
        qWarning() << "m_proxyInterface dbus is not valid!";
        return;
    }

    m_proxyInterface->asyncCall("StopProxy");
}

void ProxyServiceManager::stopProxy()
{
    stopProxyDbus();
    QString jsonPath = QDir::homePath() + "/" + PROXYCONF_FILE;
    QJsonObject protoObj = readJsonFile(jsonPath);
    protoObj.insert(PROTOJSON_KEY_STATE, QJsonValue(false));
    wirteJsonFile(jsonPath, protoObj);
    m_threadObj->setProxyState(false);
    setProxyState(false);
}


QStringList ProxyServiceManager::getProxyConfig()
{
    QStringList proxyConfList;
    proxyConfList.clear();

    QString jsonPath = QDir::homePath() + "/" + PROXYCONF_FILE;
    QJsonObject readObj = readJsonFile(jsonPath);
    proxyConfList.append(readObj.value(PROTOJSON_KEY_TYPE).toString());
    proxyConfList.append(readObj.value(PROTOJSON_KEY_SERVER).toString());
    QString portStr = QString::number(readObj.value(PROTOJSON_KEY_PORT).toInt());
    proxyConfList.append(portStr);
    proxyConfList.append(readObj.value(PROTOJSON_KEY_USRNAME).toString());
    proxyConfList.append(readObj.value(PROTOJSON_KEY_PASSWORD).toString());

    return proxyConfList;
}

void ProxyServiceManager::setProxyConfig(const QStringList configList)
{
    QString jsonPath = QDir::homePath() + "/" + PROXYCONF_FILE;
    QJsonObject readObj = readJsonFile(jsonPath);
    QJsonObject configObj = dealJsonObj(configList);
    startProxy(configObj);
    if (configObj != readObj) {
        wirteJsonFile(jsonPath, configObj);
    }
}

void ProxyServiceManager::delValueFromArray(QJsonArray *array, const QJsonValue item)
{
    if (array != nullptr && !array->isEmpty()) {
        for (int i = 0; i < array->count(); i++) {
            if (array->at(i) == item) {
                array->removeAt(i);
                return;
            }
        }
    }
}

void ProxyServiceManager::setProxyFile(QString desktopfp, bool create)
{
    QString jsonPath = QDir::homePath() + "/" + APPPROXY_FILE;
    QJsonObject readObj = readJsonFile(jsonPath);
    QJsonObject writeObj = readObj;

    if (readObj.isEmpty()) {
        if (create) {
            QJsonArray appArray = QJsonArray();
            appArray.append(QJsonValue(desktopfp));
            writeObj.insert(JSON_KEY_APPLICATION, appArray);
            qDebug()<<Q_FUNC_INFO<<__LINE__<<desktopfp<<" add to proxy list";
        } else {
            qDebug()<<Q_FUNC_INFO<<__LINE__<<jsonPath<<"is error!";
        }
    } else {
        QJsonArray appArray = readObj.value(JSON_KEY_APPLICATION).toArray();
        QJsonValue arrayItem = QJsonValue(desktopfp);
        if (create && !appArray.contains(arrayItem)) {
            appArray.append(arrayItem);
        }
        if (!create && appArray.contains(arrayItem)) {
            delValueFromArray(&appArray, arrayItem);
        }
        writeObj.insert(JSON_KEY_APPLICATION, appArray);
    }

    if (writeObj != readObj) {
        wirteJsonFile(jsonPath, writeObj);
    }
}

void ProxyServiceManager::addAppIntoProxy(QString desktopfp)
{
    if (desktopfp.isEmpty()) {
        qWarning() << "desktopfp is Empty!";
        return;
    }

    if (!addDesktopFileIntoProcessManager(desktopfp)) {
        return;
    }

    setProxyFile(desktopfp, true);
    m_proxyDesktopList = getAppProxyFromFile();
    getProxyInfoList();
    m_threadObj->setDesktopList(m_proxyDesktopList);
    m_threadObj->setExecList(m_proxyExecList);
    m_threadObj->setNameList(m_proxyNameList);
}

void ProxyServiceManager::delAppIntoProxy(QString desktopfp)
{
    if (desktopfp.isEmpty()) {
        qWarning() << "desktopfp is Empty!";
        return;
    }

    if (!delDesktopFileFromProcessManager(desktopfp)) {
        return;
    }

    setProxyFile(desktopfp, false);
    m_proxyDesktopList = getAppProxyFromFile();
    getProxyInfoList();
    m_threadObj->setDesktopList(m_proxyDesktopList);
    m_threadObj->setExecList(m_proxyExecList);
    m_threadObj->setNameList(m_proxyNameList);
}

void ProxyServiceManager::setProxyStateDbus(bool state)
{
    if (state) {
        initAppIntoProcessManager();

        QString jsonPath = QDir::homePath() + "/" + PROXYCONF_FILE;
        QJsonObject proxyObj = readJsonFile(jsonPath);
        proxyObj.insert(PROTOJSON_KEY_STATE, QJsonValue(true));
        wirteJsonFile(jsonPath, proxyObj);
        startProxy(proxyObj);
    } else {
        clearProcessManagerApp();
        stopProxy();
    }
}

bool ProxyServiceManager::getProxyStateDbus()
{
    QString jsonPath = QDir::homePath() + "/" + PROXYCONF_FILE;
    QJsonObject readObj = readJsonFile(jsonPath);
    return readObj.value(PROTOJSON_KEY_STATE).toBool();
}

QMap<QString, QStringList> ProxyServiceManager::getAppProxy()
{
    initAppInfoMapTemp();
    m_proxyDesktopList = getAppProxyFromFile();
    getProxyInfoList();

    QStringList customAppList = getCustomizedAppList(CUSTOMAPP_FILE);
    QMap<QString, QStringList> appPathMap;
    for (auto desktopfp : m_appInfoMap.keys()) {
        QMap<QString, QString> map = m_appInfoMap.value(desktopfp);
        QString name = map.value(APPINFOMAP_KEY_LOCALNAME);
        QString icon = map.value(APPINFOMAP_KEY_ICON);

        //定制软件判断，若文档是空的，则认为没有开定制功能
        if (!customAppList.isEmpty() && !customAppList.contains(desktopfp)) {
            continue;
        }

        QStringList appInfoList;
        appInfoList.append(name);
        appInfoList.append(icon);
        if (!m_proxyDesktopList.isEmpty() && m_proxyDesktopList.contains(desktopfp)) {
            appInfoList.append("true");
        } else {
            appInfoList.append("false");
        }
        appPathMap.insert(desktopfp, appInfoList);
    }

    return appPathMap;
}

//获取定制软件列表
QStringList ProxyServiceManager::getCustomizedAppList(QString filePath)
{
    if (!QFile(filePath).exists()) {
        return QStringList();
    }
    QStringList customizedAppList;
    QJsonObject readObj = readJsonFile(filePath);
    QJsonArray appArray = readObj.value(JSON_KEY_APPLICATION).toArray();
    for (auto arrayItem : appArray) {
        customizedAppList.append(arrayItem.toString());
    }

    return customizedAppList;
}

//获取系统desktop文件路径
QStringList ProxyServiceManager::getDesktopFilePath()
{
    m_filePathList.clear();
    QString jsonPath = QDir::homePath() + "/.config/ukui-menu-security-config.json";
    QFile file(jsonPath);

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QByteArray readBy = file.readAll();
        QJsonParseError error;
        QJsonDocument readDoc = QJsonDocument::fromJson(readBy, &error);

        if (!readDoc.isNull() && error.error == QJsonParseError::NoError) {
            QJsonObject obj = readDoc.object().value("ukui-menu").toObject();

            if (obj.value("mode").toString() == "whitelist") {
                QJsonArray blArray = obj.value("whitelist").toArray();
                QJsonArray enArray = blArray.at(0).toObject().value("entries").toArray();

                for (int index = 0; index < enArray.size(); index++) {
                    QJsonObject obj = enArray.at(index).toObject();
                    m_filePathList.append(obj.value("path").toString());
                }

                return m_filePathList;
            } else if (obj.value("mode").toString() == "blacklist") {
#ifdef ENABLE_ANDROIDAPP
                getAndroidApp();
#endif
                recursiveSearchFile("/usr/share/applications/");
                recursiveSearchFile("/opt/kaiming/share/applications/");
                recursiveSearchFile("/var/lib/snapd/desktop/applications/");
                recursiveSearchFile("/var/lib/flatpak/exports/share/applications/");
                QJsonArray blArray = obj.value("blacklist").toArray();
                QJsonArray enArray = blArray.at(0).toObject().value("entries").toArray();

                for (int index = 0; index < enArray.size(); index++) {
                    QJsonObject obj = enArray.at(index).toObject();
                    m_filePathList.removeAll(obj.value("path").toString());
                }
            } else {
#ifdef ENABLE_ANDROIDAPP
                getAndroidApp();
#endif
                recursiveSearchFile("/usr/share/applications/");
                recursiveSearchFile("/opt/kaiming/share/applications/");
                recursiveSearchFile("/var/lib/snapd/desktop/applications/");
                recursiveSearchFile("/var/lib/flatpak/exports/share/applications/");
            }
        }

        file.close();
    } else {
#ifdef ENABLE_ANDROIDAPP
        getAndroidApp();
#endif
        recursiveSearchFile("/usr/share/applications/");
        recursiveSearchFile("/opt/kaiming/share/applications/");
        recursiveSearchFile("/var/lib/snapd/desktop/applications/");
        recursiveSearchFile("/var/lib/flatpak/exports/share/applications/");
    }

    m_filePathList.removeAll("/usr/share/applications/software-properties-livepatch.desktop");
    m_filePathList.removeAll("/usr/share/applications/mate-color-select.desktop");
    m_filePathList.removeAll("/usr/share/applications/blueman-adapters.desktop");
    m_filePathList.removeAll("/usr/share/applications/mate-user-guide.desktop");
    m_filePathList.removeAll("/usr/share/applications/nm-connection-editor.desktop");
    m_filePathList.removeAll("/usr/share/applications/debian-uxterm.desktop");
    m_filePathList.removeAll("/usr/share/applications/debian-xterm.desktop");
    m_filePathList.removeAll("/usr/share/applications/im-config.desktop");
    m_filePathList.removeAll("/usr/share/applications/fcitx.desktop");
    m_filePathList.removeAll("/usr/share/applications/fcitx-configtool.desktop");
    m_filePathList.removeAll("/usr/share/applications/onboard-settings.desktop");
    m_filePathList.removeAll("/usr/share/applications/info.desktop");
    m_filePathList.removeAll("/usr/share/applications/ukui-power-preferences.desktop");
    m_filePathList.removeAll("/usr/share/applications/ukui-power-statistics.desktop");
    m_filePathList.removeAll("/usr/share/applications/software-properties-drivers.desktop");
    m_filePathList.removeAll("/usr/share/applications/software-properties-gtk.desktop");
    m_filePathList.removeAll("/usr/share/applications/gnome-session-properties.desktop");
    m_filePathList.removeAll("/usr/share/applications/org.gnome.font-viewer.desktop");
    m_filePathList.removeAll("/usr/share/applications/xdiagnose.desktop");
    m_filePathList.removeAll("/usr/share/applications/gnome-language-selector.desktop");
    m_filePathList.removeAll("/usr/share/applications/mate-notification-properties.desktop");
    m_filePathList.removeAll("/usr/share/applications/transmission-gtk.desktop");
    m_filePathList.removeAll("/usr/share/applications/mpv.desktop");
    m_filePathList.removeAll("/usr/share/applications/system-config-printer.desktop");
    m_filePathList.removeAll("/usr/share/applications/org.gnome.DejaDup.desktop");
    m_filePathList.removeAll("/usr/share/applications/yelp.desktop");
    //v10
    m_filePathList.removeAll("/usr/share/applications/mate-about.desktop");
    m_filePathList.removeAll("/usr/share/applications/time.desktop");
    m_filePathList.removeAll("/usr/share/applications/network.desktop");
    m_filePathList.removeAll("/usr/share/applications/shares.desktop");
    m_filePathList.removeAll("/usr/share/applications/mate-power-statistics.desktop");
    m_filePathList.removeAll("/usr/share/applications/display-im6.desktop");
    m_filePathList.removeAll("/usr/share/applications/display-im6.q16.desktop");
    m_filePathList.removeAll("/usr/share/applications/openjdk-8-policytool.desktop");
    m_filePathList.removeAll("/usr/share/applications/kylin-io-monitor.desktop");
    m_filePathList.removeAll("/usr/share/applications/wps-office-uninstall.desktop");
    m_filePathList.removeAll("/usr/share/applications/wps-office-misc.desktop");
    QStringList desktopList;

    for (int i = 0; i < m_filePathList.count(); ++i) {
        QString filepath = m_filePathList.at(i);
        int list_index = filepath.lastIndexOf('/');
        QString desktopName = filepath.right(filepath.length() - list_index - 1);

        if (desktopList.contains(desktopName)) {
            m_filePathList.removeAll(filepath);
            i--;
        } else {
            desktopList.append(desktopName);
        }
    }
    return m_filePathList;
}

//文件递归查询
void ProxyServiceManager::recursiveSearchFile(const QString &_filePath)
{
    QDir dir(_filePath);

    if (!dir.exists()) {
        return;
    }

    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    list.removeAll(QFileInfo("/usr/share/applications/screensavers"));

    if (list.size() < 1) {
        return;
    }

    int i = 0;

    //递归算法的核心部分
    do {
        QFileInfo fileInfo = list.at(i);
        //如果是文件夹，递归
        bool isDir = fileInfo.isDir();

        if (isDir) {
            recursiveSearchFile(fileInfo.filePath());
        } else {
            //过滤后缀不是.desktop的文件
            QString filePathStr = fileInfo.filePath();

            if (!filePathStr.endsWith(".desktop")) {
                i++;
                continue;
            }

            QByteArray fpbyte = filePathStr.toLocal8Bit();
            char *filepath = fpbyte.data();

            if (0 != access(filepath, R_OK)) { //判断文件是否可读
                i++;
                continue;
            }

            m_keyfile = g_key_file_new();

            if (!g_key_file_load_from_file(m_keyfile, filepath, m_flags, m_error)) {
                return;
            }

            char *ret_0 = g_key_file_get_locale_string(m_keyfile, "Desktop Entry", "Categories", nullptr, nullptr);

            if (ret_0 != nullptr) {
                QString str = QString::fromLocal8Bit(ret_0);

                if (str.contains("Android")) {
                    g_key_file_free(m_keyfile);
                    i++;
                    continue;
                }
            }

            char *ret_1 = g_key_file_get_locale_string(m_keyfile, "Desktop Entry", "NoDisplay", nullptr, nullptr);

            if (ret_1 != nullptr) {
                QString str = QString::fromLocal8Bit(ret_1);

                if (str.contains("true")) {
                    g_key_file_free(m_keyfile);
                    i++;
                    continue;
                }
            }

            char *ret_2 = g_key_file_get_locale_string(m_keyfile, "Desktop Entry", "NotShowIn", nullptr, nullptr);

            if (ret_2 != nullptr) {
                QString str = QString::fromLocal8Bit(ret_2);

                if (str.contains("UKUI")) {
                    g_key_file_free(m_keyfile);
                    i++;
                    continue;
                }
            }

            //过滤LXQt、KDE
            char *ret = g_key_file_get_locale_string(m_keyfile, "Desktop Entry", "OnlyShowIn", nullptr, nullptr);

            if (ret != nullptr) {
                QString str = QString::fromLocal8Bit(ret);

                if (str.contains("LXQt") || str.contains("KDE")) {
                    g_key_file_free(m_keyfile);
                    i++;
                    continue;
                }
            }

            g_key_file_free(m_keyfile);
            m_filePathList.append(filePathStr);
        }

        i++;
    } while (i < list.size());
}

#ifdef ENABLE_ANDROIDAPP
void ProxyServiceManager::getAndroidApp()
{
    m_androidDesktopfnList.clear();
    QVector<QStringList> androidVector;
    androidVector.clear();
    QString path = QDir::homePath() + "/.local/share/applications/";
    QDir dir(path);

    if (!dir.exists()) {
        return;
    }

    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();

    if (list.size() < 1) {
        return;
    }

    int i = 0;
    GKeyFile *keyfile = g_key_file_new();

    do {
        QFileInfo fileInfo = list.at(i);

        if (!fileInfo.isFile()) {
            i++;
            continue;
        }

        //过滤后缀不是.desktop的文件
        QString filePathStr = fileInfo.filePath();

        if (!filePathStr.endsWith(".desktop")) {
            i++;
            continue;
        }

        QByteArray fpbyte = filePathStr.toLocal8Bit();
        char *filepath = fpbyte.data();

        if (0 != access(filepath, R_OK)) { //判断文件是否可读
            i++;
            continue;
        }

        m_keyfile = g_key_file_new();

        if (!g_key_file_load_from_file(m_keyfile, filepath, m_flags, m_error)) {
            return;
        }

        char *ret_1 = g_key_file_get_locale_string(m_keyfile, "Desktop Entry", "NoDisplay", nullptr, nullptr);

        if (ret_1 != nullptr) {
            QString str = QString::fromLocal8Bit(ret_1);

            if (str.contains("true")) {
                g_key_file_free(m_keyfile);
                i++;
                continue;
            }
        }

        char *ret_2 = g_key_file_get_locale_string(m_keyfile, "Desktop Entry", "NotShowIn", nullptr, nullptr);

        if (ret_2 != nullptr) {
            QString str = QString::fromLocal8Bit(ret_2);

            if (str.contains("UKUI")) {
                g_key_file_free(m_keyfile);
                i++;
                continue;
            }
        }

        //过滤LXQt、KDE
        char *ret = g_key_file_get_locale_string(m_keyfile, "Desktop Entry", "OnlyShowIn", nullptr, nullptr);

        if (ret != nullptr) {
            QString str = QString::fromLocal8Bit(ret);

            if (str.contains("LXQt") || str.contains("KDE")) {
                g_key_file_free(m_keyfile);
                i++;
                continue;
            }
        }

        m_filePathList.append(filePathStr);
        m_androidDesktopfnList.append(fileInfo.fileName());
        i++;
    } while (i < list.size());

    g_key_file_free(keyfile);
}
#endif

