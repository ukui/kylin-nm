#include "kywirelessnetresource.h"
#include "kylinnetworkmanagerutil.h"

#define LOG_FLAG "[KyWirelessNetResource]"

#define PSK_SETTING_NAME "802-11-wireless-security"
#define PRIVATE_PSK_SETTING_NAME "802-1x"

static bool subWifiListSort(const KyWirelessNetItem info1, const KyWirelessNetItem info2)
{
    if (info1.m_connectUuid.isEmpty() == info2.m_connectUuid.isEmpty()) {
        if (info1.m_signalStrength != info2.m_signalStrength) {
            return info1.m_signalStrength > info2.m_signalStrength;
        } else {
            if (QString::compare(info1.m_NetSsid, info2.m_NetSsid, Qt::CaseInsensitive) > 0) {
                return false;
            } else {
                return true;
            }
        }
    }
    return !info1.m_connectUuid.isEmpty();
}

static void wifiListSort(QList<KyWirelessNetItem> &list)
{
    qSort(list.begin(), list.end(), subWifiListSort);
}

KyWirelessNetResource::KyWirelessNetResource(QObject *parent) : KyNetResource(parent)
{
    qRegisterMetaType<KyWirelessNetItem>("KyWirelessNetItem&");
    qRegisterMetaType<NetworkManager::WirelessNetwork::Ptr>("NetworkManager::WirelessNetwork::Ptr");

    kyWirelessNetItemListInit();

    //无线出现/消失-->item 增删
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkAdded,
                     this, &KyWirelessNetResource::onWifiNetworkAdded);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkRemoved,
                     this, &KyWirelessNetResource::onWifiNetworkRemoved);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkPropertyChange,
                     this, &KyWirelessNetResource::onWifiNetworkPropertyChange);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkDeviceDisappear,
                     this, &KyWirelessNetResource::onWifiNetworkDeviceDisappear);

    //无线配置变化-->item uuid变化
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wirelessConnectionAdd,
                     this, &KyWirelessNetResource::onConnectionAdd);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::connectionRemove,
                     this, &KyWirelessNetResource::onConnectionRemove);

    //设备变化-->map key变化
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wirelessDeviceAdd,
                     this, &KyWirelessNetResource::onDeviceAdd);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceRemove,
                     this, &KyWirelessNetResource::onDeviceRemove);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wirelessDeviceUpdate,
                     this, &KyWirelessNetResource::onDeviceNameUpdate);

    m_updateTimer = new QTimer(this);
    m_updateTimer->start(10*1000);
    connect(m_updateTimer, &QTimer::timeout, this, &KyWirelessNetResource::updateList);

}

//配合activeconnection 此处未单独区分已连接的wifi
void KyWirelessNetResource::getWifiNetworkList(QString devName, QList<KyWirelessNetItem> &list)
{
    list.clear();
    if (m_WifiNetworkList.isEmpty() || !m_WifiNetworkList.contains(devName)) {
        return;
    } else {
        QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
        while (iter != m_WifiNetworkList.end()) {
            if (iter.key() == devName) {
                list = m_WifiNetworkList[iter.key()];
                wifiListSort(list);
                return;
            }
            iter++;
        }
    }
}

void KyWirelessNetResource::kyWirelessNetItemListInit()
{
    m_WifiNetworkList.clear();

    NetworkManager::WirelessNetwork::List m_wifiNetList = m_networkResourceInstance->getWifiNetworkList();
    for (auto const & net : m_wifiNetList) {
        QString devIface = getDeviceIFace(net);
        if (devIface.isEmpty()) {
            continue;
        }

        KyWirelessNetItem item;
        wirelessNetItemInit(item, net);
        if (!m_WifiNetworkList.contains(devIface)){
            QList<KyWirelessNetItem> list;
            list.append(item);
            m_WifiNetworkList.insert(devIface,list);
        } else {
            m_WifiNetworkList[devIface].append(item);
        }
    }

    return;
}

void KyWirelessNetResource::wirelessNetItemInit(KyWirelessNetItem &wirelessItem,
                                                NetworkManager::WirelessNetwork::Ptr wirelessNetPtr)
{
    if(wirelessNetPtr.isNull()) {
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wirelessNetPtr->referenceAccessPoint();
    QByteArray rawSsid = accessPointPtr->rawSsid();
    wirelessItem.m_NetSsid = getSsidFromByteArray(rawSsid);

    wirelessItem.m_signalStrength = accessPointPtr->signalStrength();
    NetworkManager::AccessPoint::Capabilities cap = accessPointPtr->capabilities();
    NetworkManager::AccessPoint::WpaFlags wpaFlag = accessPointPtr->wpaFlags();
    NetworkManager::AccessPoint::WpaFlags rsnFlag = accessPointPtr->rsnFlags();
    wirelessItem.m_secuType = enumToQstring(cap, wpaFlag, rsnFlag);
    QString interface = getDeviceIFace(wirelessNetPtr);
    getUuidBySsid(wirelessItem.m_NetSsid, interface, wirelessItem.m_connectUuid, wirelessItem.m_dbusPath);
    KyWirelessConnectOperation operation;
    wirelessItem.m_connectSecuType = operation.getConnectSecuType(wirelessItem.m_connectUuid);
    wirelessItem.m_isApConnection = operation.isApConnection(wirelessItem.m_connectUuid);

    //category
    wirelessItem.m_category = getCategory(accessPointPtr->uni());
}

void KyWirelessNetResource::getUuidBySsid(const QString &ssid, QString &deviceName, QString &uuid, QString &dbusPath)
{
    uuid.clear();
    dbusPath.clear();
    NetworkManager::Connection::List connectList;

    connectList.clear();
    connectList = m_networkResourceInstance->getConnectList();

    if (connectList.empty()) {
        qWarning()<<"[KyConnectResourse]"<<"get connection failed, the connect list is empty";
        return;
    }

    NetworkManager::Connection::Ptr connectPtr = nullptr;
    for (int index = 0; index < connectList.size(); index++) {
        connectPtr = connectList.at(index);
        if (connectPtr.isNull()) {
            continue;
        }

        if (NetworkManager::ConnectionSettings::Wireless != connectPtr->settings()->connectionType()) {
            continue;
        }

        QString connectInterface = connectPtr->settings()->interfaceName();
        if (!connectInterface.isEmpty()
               && deviceName != connectInterface) {
            continue;
        }

        NetworkManager::WirelessSetting::Ptr wifi_sett
            = connectPtr->settings()->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
        if (wifi_sett->ssid() == ssid) {
            uuid = connectPtr->settings()->uuid();
            dbusPath = connectPtr->path();
        }
    }
}

void KyWirelessNetResource::onWifiNetworkAdded(QString devIfaceName, QString ssid)
{
    NetworkManager::WirelessNetwork::Ptr wifi = nullptr;
    NetworkManager::WirelessNetwork::List m_wifiNetList = m_networkResourceInstance->getWifiNetworkList();
    for (auto const & net : m_wifiNetList) {
        if (net.isNull()) {
            continue;
        }

        NetworkManager::AccessPoint::Ptr accessPointPtr = net->referenceAccessPoint();
        QByteArray rawSsid = accessPointPtr->rawSsid();
        QString wifiSsid = getSsidFromByteArray(rawSsid);

        if (wifiSsid == ssid
                && m_networkResourceInstance->findDeviceByUni(net->device())->interfaceName() == devIfaceName) {
            wifi = net;
        }
    }

    if (wifi.isNull()) {
        return;
    }

    KyWirelessNetItem item;
    wirelessNetItemInit(item, wifi);

    if (m_WifiNetworkList.contains(devIfaceName)) {
        m_WifiNetworkList[devIfaceName].append(item);
    } else {
        QList<KyWirelessNetItem> list;
        list.append(item);
        m_WifiNetworkList.insert(devIfaceName,list);
    }

    Q_EMIT wifiNetworkAdd(devIfaceName, item);
}

void KyWirelessNetResource::onWifiNetworkRemoved(QString devIfaceName, QString ssid)
{
    if (m_WifiNetworkList.contains(devIfaceName)) {
        int index = 0;
        for ( ; index < m_WifiNetworkList.value(devIfaceName).size(); index++) {
            if ( m_WifiNetworkList[devIfaceName].at(index).m_NetSsid == ssid) {
                m_WifiNetworkList[devIfaceName].removeAt(index);
            }
        }
        //remove后为空则删除
        if (m_WifiNetworkList.value(devIfaceName).isEmpty()) {
            m_WifiNetworkList.remove(devIfaceName);
        }
        Q_EMIT wifiNetworkRemove(devIfaceName, ssid);
    }
}

void KyWirelessNetResource::onWifiNetworkPropertyChange(NetworkManager::WirelessNetwork::Ptr net)
{
    if (nullptr == net) {
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = net->referenceAccessPoint();
    QByteArray rawSsid = accessPointPtr->rawSsid();
    QString wifiSsid = getSsidFromByteArray(rawSsid);

    QString devIface = m_networkResourceInstance->findDeviceByUni(net->device())->interfaceName();
    if (m_WifiNetworkList.contains(devIface)) {
        QList<KyWirelessNetItem>::iterator iter = m_WifiNetworkList[devIface].begin();
         while (iter != m_WifiNetworkList[devIface].end()) {
             if (iter->m_NetSsid == wifiSsid) {
                 if (iter->m_signalStrength != net->signalStrength()) {
                     iter->m_signalStrength = net->signalStrength();
                 }

                 QString secuType = enumToQstring(accessPointPtr->capabilities(),
                                                  accessPointPtr->wpaFlags(),
                                                  accessPointPtr->rsnFlags());
                 if (iter->m_secuType != secuType) {
                     iter->m_secuType = secuType;
                     KyWirelessNetItem item = *iter;
                     Q_EMIT wifiNetworkUpdate(devIface, wifiSsid, item);
                 }
                 break;
             }
             iter++;
         }
    }
}

void KyWirelessNetResource::onWifiNetworkDeviceDisappear()
{
    qWarning() << "wifiNetworkDeviceDisappear!!!!!!!!!";
    for (int i = 0; i < m_WifiNetworkList.keys().size(); ++i) {
        Q_EMIT deviceRemove(m_WifiNetworkList.keys().at(i));
    }
    kyWirelessNetItemListInit();
}

//对应属性页-->安全
bool KyWirelessNetResource::getNormalWifiConnectSecuInfo(QString &uuid, KyWpaPasswordInfo &info)
{
    KyWirelessConnectOperation operation;
    info.pwd = operation.getPsk(uuid);
    return operation.getConnSecretFlags(uuid, info.flag);
}

bool KyWirelessNetResource::getEnterpiseEapMethod(const QString &uuid, KyEapMethodType &type)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (connectPtr.isNull()) {
        qWarning() << "getEnterpiseEapMethod faild.Can't find uuid = " << uuid;
        return false;
    }

    KyWirelessConnectOperation operation;
    KyKeyMgmt keyMgmt = operation.getConnectKeyMgmt(uuid);
    if (keyMgmt != WpaEap) {
        qWarning() << "getEnterpiseEapMethod but not WpaEap.it's " << keyMgmt;
        return false;
    }

    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
            = connectionSettings->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

    QList<NetworkManager::Security8021xSetting::EapMethod> list = wifi_8021x_sett->eapMethods();

    if (list.contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodTls)) {
        type = TLS;
    } else if (list.contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodPeap)) {
        type = PEAP;
    } else if (list.contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodTtls)) {
        type = TTLS;
    } else  if (list.contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodLeap)) {
        type = LEAP;
    } else if (list.contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodPwd)) {
        type = PWD;
    } else if (list.contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodFast)) {
        type = FAST;
    }
    return true;
}


bool KyWirelessNetResource::getEnterPriseInfoTls(QString &uuid, KyEapMethodTlsInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->findConnectByUuid(uuid);
    if (conn.isNull()) {
        qWarning() << LOG_FLAG << "modifyEnterPriseInfoTls connection missing";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett =
            conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qWarning()<< LOG_FLAG << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        return false;
    }
    NetworkManager::Security8021xSetting::Ptr setting =
            conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull()) {
        qWarning()<< LOG_FLAG << "don't have Security8021x connection";
        return false;
    }


    info.identity = setting->identity();
    info.domain = setting->domainSuffixMatch();
    info.caCertPath = setting->caPath();
    if (info.caCertPath.left(7) == "file://") {
        info.caCertPath = info.caCertPath.mid(7);
    }

    info.clientCertPath = setting->clientCertificate();
    if (info.clientCertPath.left(7) == "file://") {
        info.clientCertPath = info.clientCertPath.mid(7);
    }

    info.clientPrivateKey = QString(setting->privateKey());
    if (info.clientPrivateKey.left(7) == "file://") {
        info.clientPrivateKey = info.clientPrivateKey.mid(7);
    }

    info.m_privateKeyPWDFlag = setting->privateKeyPasswordFlags();
    if (!info.m_privateKeyPWDFlag) {
        KyWirelessConnectOperation operation;
        info.clientPrivateKeyPWD = operation.getPrivateKeyPassword(conn->uuid());
    }

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoPeap(QString &uuid, KyEapMethodPeapInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->findConnectByUuid(uuid);
    if (conn.isNull()) {
        qWarning() << LOG_FLAG << "getEnterPriseInfoPeap connection missing";
        return false;
    }
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qWarning() << LOG_FLAG << "don't have WirelessSecurity connection";
        return false;
    }

    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        qWarning() << LOG_FLAG << "keyMgmt not WpaEap " << security_sett->keyMgmt();
        return false;
    }

    NetworkManager::Security8021xSetting::Ptr setting =
            conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodPeap)) {
        qWarning()<< LOG_FLAG << "don't have Security8021x connection";
        return false;
    }

    info.phase2AuthMethod = (KyNoEapMethodAuth)setting->phase2AuthMethod();
    info.userName = setting->identity();
    info.m_passwdFlag = setting->passwordFlags();
    if (!info.m_passwdFlag) {
        KyWirelessConnectOperation operation;
        info.userPWD = operation.get8021xPassword(conn->uuid());
    }

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoTtls(QString &uuid, KyEapMethodTtlsInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->findConnectByUuid(uuid);
    if (conn.isNull()) {
        qWarning() << LOG_FLAG << "modifyEnterPriseInfoTtls connection missing";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qWarning() << LOG_FLAG << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        qWarning() << LOG_FLAG << "not wpaeap"<<security_sett->keyMgmt();
        return false;
    }

    NetworkManager::Security8021xSetting::Ptr setting = conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodTtls)) {
        qWarning() << LOG_FLAG << "don't have Security8021x connection";
        return false;
    }

    info.authEapMethod = (KyEapMethodAuth)setting->phase2AuthEapMethod();
    info.authNoEapMethod = (KyNoEapMethodAuth)setting->phase2AuthMethod();

    info.authType = KyTtlsAuthMethod::AUTH_EAP;
    if (info.authEapMethod != KyAuthEapMethodUnknown) {
        info.authType = KyTtlsAuthMethod::AUTH_EAP;
    } else {
        info.authType = KyTtlsAuthMethod::AUTH_NO_EAP;
    }

    info.userName = setting->identity();
    info.m_passwdFlag = setting->passwordFlags();
    if (!info.m_passwdFlag) {
        KyWirelessConnectOperation operation;
        info.userPWD = operation.get8021xPassword(conn->uuid());
    }

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoLeap(QString &uuid, KyEapMethodLeapInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->findConnectByUuid(uuid);
    if (conn.isNull()) {
        qDebug()<< LOG_FLAG << "getEnterPriseInfoLeap connection missing";
        return false;
    }
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qDebug()<< LOG_FLAG << "don't have WirelessSecurity connection";
        return false;
    }

    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        qDebug()<< LOG_FLAG << "keyMgmt not WpaEap " << security_sett->keyMgmt();
        return false;
    }

    NetworkManager::Security8021xSetting::Ptr setting =
            conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodLeap)) {
        qDebug()<< LOG_FLAG << "don't have Security8021x connection";
        return false;
    }

    info.m_userName = setting->identity();
    info.m_passwdFlag = setting->passwordFlags();
    if (!info.m_passwdFlag) {
        KyWirelessConnectOperation operation;
        info.m_userPwd = operation.get8021xPassword(conn->uuid());
    }

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoPwd(QString &uuid, KyEapMethodPwdInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->findConnectByUuid(uuid);
    if (conn.isNull()) {
        qDebug()<< LOG_FLAG << "getEnterPriseInfoPwd connection missing";
        return false;
    }
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qDebug()<< LOG_FLAG << "don't have WirelessSecurity connection";
        return false;
    }

    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        qDebug()<< LOG_FLAG << "keyMgmt not WpaEap " << security_sett->keyMgmt();
        return false;
    }

    NetworkManager::Security8021xSetting::Ptr setting =
            conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodPwd)) {
        qDebug()<< LOG_FLAG << "don't have Security8021x connection";
        return false;
    }

    info.m_userName = setting->identity();
    info.m_passwdFlag = setting->passwordFlags();
    if (!info.m_passwdFlag) {
        KyWirelessConnectOperation operation;
        info.m_userPwd = operation.get8021xPassword(conn->uuid());
    }

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoFast(QString &uuid, KyEapMethodFastInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->findConnectByUuid(uuid);
    if (conn.isNull()) {
        qDebug()<< LOG_FLAG << "getEnterPriseInfoFast connection missing";
        return false;
    }
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qDebug()<< LOG_FLAG << "don't have WirelessSecurity connection";
        return false;
    }

    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        qDebug()<< LOG_FLAG << "keyMgmt not WpaEap " << security_sett->keyMgmt();
        return false;
    }

    NetworkManager::Security8021xSetting::Ptr setting =
            conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodFast)) {
        qDebug()<< LOG_FLAG << "don't have Security8021x connection";
        return false;
    }

    info.m_anonIdentity = setting->anonymousIdentity();
    info.m_pacProvisioning = (KyFastProvisioning)setting->phase1FastProvisioning();
    info.m_pacFilePath = setting->pacFile();
    if (info.m_pacFilePath.left(7) == "file://") {
        info.m_pacFilePath = info.m_pacFilePath.mid(7);
    }
    info.m_authMethod = (KyNoEapMethodAuth)setting->phase2AuthMethod();

    info.m_userName = setting->identity();
    info.m_passwdFlag = setting->passwordFlags();
    if (!info.m_passwdFlag) {
        KyWirelessConnectOperation operation;
        info.m_userPwd = operation.get8021xPassword(conn->uuid());
    }

    return true;
}

void KyWirelessNetResource::getWirelessConnectDetail(QString interface, QString ssid, QString uuid, KyDetailInfo &connectSetting)
{
    getBaseInfo(interface, ssid, true, !uuid.isEmpty(), connectSetting);
    if (!uuid.isEmpty()) {
        getDynamicIpInfo(uuid, connectSetting);
    }
}

bool KyWirelessNetResource::isApConnection(QString uuid)
{
    bool state = false;
    KyWirelessConnectOperation operation;
    state = operation.isApConnection(uuid);
    return state;
}

void KyWirelessNetResource::getWirelessConnectInfo(QString devName, QString &secuType, int &cateGory)
{
    NetworkManager::Device::Ptr connectDevice =
            m_networkResourceInstance->findDeviceByName(devName);
    if (nullptr == connectDevice || !connectDevice->isValid()) {
        qWarning()<< LOG_FLAG <<"getWirelessConnectInfo failed, the device" << devName << "is not existed";
        return;
    }
    if (connectDevice->type() == NetworkManager::Device::Wifi) {
        NetworkManager::WirelessDevice *wirelessDevicePtr =
                qobject_cast<NetworkManager::WirelessDevice *>(connectDevice.data());
        NetworkManager::AccessPoint::Ptr apPtr = wirelessDevicePtr->activeAccessPoint();
        if (apPtr.isNull()) {
            return;
        }
        NetworkManager::AccessPoint::Capabilities cap = apPtr->capabilities();
        NetworkManager::AccessPoint::WpaFlags wpaFlag = apPtr->wpaFlags();
        NetworkManager::AccessPoint::WpaFlags rsnFlag = apPtr->rsnFlags();
        secuType = enumToQstring(cap, wpaFlag, rsnFlag);

        QString uni = "";
        uni = apPtr->uni();
        cateGory = getCategory(uni);
    }
}

void KyWirelessNetResource::onConnectionAdd(QString connectUuid, QString connectName, QString connectPath)
{
    Q_UNUSED(connectName);
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->findConnectByUuid(connectUuid);
    if (conn.isNull()) {
        qWarning() << LOG_FLAG << "onConnectionAdd can not find connection" << connectUuid;
        return;
    }

    NetworkManager::ConnectionSettings::Ptr sett = conn->settings();
    NetworkManager::WirelessSetting::Ptr wireless_sett =
            sett->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();

    QByteArray rawSsid = wireless_sett->ssid();
    QString wifiSsid = getSsidFromByteArray(rawSsid);

    QMap<QString, QString> map;
    map.clear();
    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
    while (iter != m_WifiNetworkList.end()) {
        for(int i = 0; i < iter.value().size(); i++) {
            if (iter.value().at(i).m_NetSsid == wifiSsid
                    && (sett->interfaceName() == iter.key() || sett->interfaceName().isEmpty())) {
                QString devIfaceName;
                QString ssid;
                KyWirelessConnectOperation operation;
                m_WifiNetworkList[iter.key()][i].m_connectUuid = conn->uuid();
                m_WifiNetworkList[iter.key()][i].m_dbusPath = conn->path();
                m_WifiNetworkList[iter.key()][i].m_connectSecuType = operation.getConnectSecuType(conn->uuid());

                devIfaceName = iter.key();
                ssid = iter.value().at(i).m_NetSsid;

                Q_EMIT wirelessConnectionAdd(devIfaceName, ssid, connectUuid, connectPath);
                map.insert(devIfaceName, ssid);

                break;
            }
        }
        iter++;
    }
}

void KyWirelessNetResource::onConnectionRemove(QString path)
{
    QMap<QString, QString> map;
    map.clear();

    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
    while (iter != m_WifiNetworkList.end()) {
        for(int i = 0; i < iter.value().size(); i++) {
            if (iter.value().at(i).m_dbusPath == path) {
                KyWirelessConnectOperation operation;
                QString devIfaceName = iter.key();
                QString ssid = iter.value().at(i).m_NetSsid;

                getUuidBySsid(ssid, devIfaceName, m_WifiNetworkList[iter.key()][i].m_connectUuid, m_WifiNetworkList[iter.key()][i].m_dbusPath);
                if (m_WifiNetworkList[iter.key()][i].m_connectUuid.isEmpty()) {
                    Q_EMIT wirelessConnectionRemove(devIfaceName, ssid);
                } else {
                    Q_EMIT wirelessConnectionUpdate(devIfaceName, ssid,
                                                    m_WifiNetworkList[iter.key()][i].m_connectUuid,
                                                    m_WifiNetworkList[iter.key()][i].m_dbusPath,
                                                    operation.getConnectSecuType(m_WifiNetworkList[iter.key()][i].m_connectUuid));
                }

                map.insert(devIfaceName, ssid);
                break;

            }
        }
        iter++;
    } 
}

void KyWirelessNetResource::onDeviceAdd(QString deviceName)
{
    if (!m_WifiNetworkList.contains(deviceName)) {
        m_WifiNetworkList.insert(deviceName,QList<KyWirelessNetItem>());
        Q_EMIT wirelessDeviceAdd(deviceName);
    }
}

void KyWirelessNetResource::onDeviceRemove(QString deviceName)
{
    if (m_WifiNetworkList.contains(deviceName)) {
        m_WifiNetworkList.remove(deviceName);
        Q_EMIT deviceRemove(deviceName);
    }
}

void KyWirelessNetResource::onDeviceNameUpdate(QString oldName, QString newName)
{
    if (m_WifiNetworkList.contains(oldName)) {
        QMap<QString, QList<KyWirelessNetItem>> newWifiNetworkList(m_WifiNetworkList);
        QList<KyWirelessNetItem> list = m_WifiNetworkList[oldName];
        newWifiNetworkList.remove(oldName);
        newWifiNetworkList.insert(newName,list);
        m_WifiNetworkList = newWifiNetworkList;
        Q_EMIT wirelessDeviceNameUpdate(oldName, newName);
    }
}

void KyWirelessNetResource::updateList()
{
    QMap<QString, QVector<QStringList> > map;
    QVector<QStringList> vector;

    for (int i = 0; i< m_WifiNetworkList.keys().size(); ++i) {
        QString devName = m_WifiNetworkList.keys().at(i);
        QList<KyActivateItem> connectItemList;
        KyActiveConnectResourse actResource;
        actResource.getActiveConnectionList(devName, CONNECT_TYPE_WIRELESS, connectItemList);
        QList<KyWirelessNetItem> list = m_WifiNetworkList[devName];
        wifiListSort(list);
        Q_EMIT updateWifiList(devName, connectItemList, list);

        QString activeSsid;
        vector.clear();
        if (!connectItemList.isEmpty()) {
            QString secuType = "";
            int category = 0;
            activeSsid = connectItemList.at(0).m_ssid;
            getWirelessConnectInfo(devName, secuType, category);
            vector.append(QStringList() << activeSsid
                          << QString::number(actResource.getAcivateWifiSignal())
                          << secuType
                          << connectItemList.at(0).m_uuid
                          << QString::number(isApConnection(connectItemList.at(0).m_uuid))
                          << QString::number(category));
        } else {
            vector.append(QStringList("--"));
        }
        if (!list.isEmpty()) {
            for (const auto itemData : list) {
                if (itemData.m_NetSsid == activeSsid) {
                    continue;
                } else {
                    vector.append(QStringList()<<itemData.m_NetSsid
                                  << QString::number(itemData.m_signalStrength)
                                  << itemData.m_secuType
                                  << QString::number(itemData.m_isApConnection)
                                  << QString::number(itemData.m_category));
                }
            }
        }
        map.insert(devName, vector);
    }
    Q_EMIT updateWifiListInCtrlCenter(map);
}

QString KyWirelessNetResource::getDeviceIFace(NetworkManager::WirelessNetwork::Ptr net)
{
    if (net.isNull()) {
        return "";
    }

    QString devUni = net->device();
    NetworkManager::Device::Ptr dev = m_networkResourceInstance->findDeviceByUni(devUni);
    if (dev.isNull()) {
        qDebug()<< LOG_FLAG << "KyWirelessNetResource: can't find " << net->ssid() << " find in device list";
        return "";
    }

    return dev->interfaceName();
}

int KyWirelessNetResource::getCategory(QString uni)
{

    QDBusInterface interface( "org.freedesktop.NetworkManager", uni, "org.freedesktop.DBus.Properties", QDBusConnection::systemBus() );
    if (!interface.isValid()) {
        qDebug() << Q_FUNC_INFO << "dbus is invalid";
        return -1;
    }

    QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.AccessPoint", "Category");
    if (!reply.isValid()) {
        //qDebug()<<"can not get the attribute 'Category' in func getCategory()";
        return 0;
    } else {
        return reply.value().toInt();
    }

}
