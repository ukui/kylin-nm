#include "kywirelessnetresource.h"

static bool subWifiListSort(const KyWirelessNetItem info1, const KyWirelessNetItem info2)
{
    if (info1.m_isConfigured == info2.m_isConfigured) {
        return info1.m_signalStrength >info2.m_signalStrength;
    }
    return info1.m_isConfigured;
}

static void wifiListSort(QList<KyWirelessNetItem> &list)
{
    qSort(list.begin(), list.end(), subWifiListSort);
}

KyWirelessNetResource::KyWirelessNetResource(QObject *parent)
    : QObject(parent)
{
    qDebug()<<"KyWirelessNetResource";
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
    m_connectResource = new KyConnectResourse(this);
    m_operation = new KyWirelessConnectOperation(this);

    kyWirelessNetItemListInit();

    //TODO:connect device signal
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkAdded, this, &KyWirelessNetResource::onWifiNetworkAdded);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkRemoved, this, &KyWirelessNetResource::onWifiNetworkRemoved);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkPropertyChange, this, &KyWirelessNetResource::onWifiNetworkPropertyChange);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkDeviceDisappear, this, &KyWirelessNetResource::onWifiNetworkDeviceDisappear);

    connect(m_connectResource, &KyConnectResourse::connectionAdd, this, &KyWirelessNetResource::onConnectionAdd);
    connect(m_connectResource, &KyConnectResourse::connectionRemove, this, &KyWirelessNetResource::onConnectionRemove);
    connect(m_connectResource, &KyConnectResourse::connectionUpdate, this, &KyWirelessNetResource::onConnectionUpdate);

    connect(m_device, &KyNetworkDeviceResourse::deviceAdd, this, &KyWirelessNetResource::onDeviceAdd);
    connect(m_device, &KyNetworkDeviceResourse::deviceRemove, this, &KyWirelessNetResource::onDeviceRemove);
    connect(m_device, &KyNetworkDeviceResourse::deviceNameUpdate, this, &KyWirelessNetResource::onDeviceNameUpdate);

}

KyWirelessNetResource::~KyWirelessNetResource()
{
    m_networkResourceInstance = nullptr;
}

bool KyWirelessNetResource::getAllDeviceWifiNetwork(QMap<QString,QList<KyWirelessNetItem> > &map)
{
//    onWifiNetworkDeviceDisappear();
    if (m_WifiNetworkList.isEmpty()) {
        return false;
    } else {
        QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
        while (iter != m_WifiNetworkList.end()) {
            wifiListSort(m_WifiNetworkList[iter.key()]);
            iter++;
        }
        map = m_WifiNetworkList;
        return true;
    }
}


bool KyWirelessNetResource::getDeviceWifiNetwork(QString devIfaceName, QList<KyWirelessNetItem> &wirelessNetResource)
{
//    onWifiNetworkDeviceDisappear();

    if (!m_WifiNetworkList.contains(devIfaceName)) {
        return false;
    } else {
        wifiListSort(m_WifiNetworkList[devIfaceName]);
        wirelessNetResource = m_WifiNetworkList[devIfaceName];
        return true;
    }
}

bool KyWirelessNetResource::getWifiNetwork(const QString &devIfaceName, const QString &ssid, KyWirelessNetItem &wirelessNetResource)
{
//    onWifiNetworkDeviceDisappear();

    if (!m_WifiNetworkList.contains(devIfaceName)) {
         qDebug() << "getWifiNetwork fail,not contain " << devIfaceName;
        return false;
    } else {
        for (int index = 0; index < m_WifiNetworkList[devIfaceName].size(); index ++){
            if (m_WifiNetworkList[devIfaceName].at(index).m_NetSsid  == ssid) {
                wirelessNetResource = m_WifiNetworkList[devIfaceName].at(index);
                qDebug() << "getWifiNetwork success";
                return true;
            }
        }
    }
    qDebug() << "getWifiNetwork fail,not contain " << ssid;
    return false;
}

void KyWirelessNetResource::getWirelessActiveConnection(NetworkManager::ActiveConnection::State state, QMap<QString,QStringList> &map)
{
    int index = 0;
    map.clear();
    NetworkManager::ActiveConnection::List activeConnectionList;

    activeConnectionList.clear();
    activeConnectionList = m_networkResourceInstance->getActiveConnectList();
    if (activeConnectionList.isEmpty()) {
        return;
    }

    NetworkManager::ActiveConnection::Ptr activeConnectionPtr = nullptr;
    for (; index < activeConnectionList.size(); index++) {
        activeConnectionPtr = activeConnectionList.at(index);
        if (activeConnectionPtr.isNull()) {
            continue;
        }

        if (NetworkManager::ConnectionSettings::ConnectionType::Wireless != activeConnectionPtr->type()) {
            continue;
        }

        if (state != activeConnectionPtr->state()) {
            continue;
        }

        qDebug() << "getWirelessActiveConnection " << activeConnectionPtr->uuid();
        QString ssid;
        QString ifaceName = getDeviceIFace(activeConnectionPtr,ssid);
        if(ifaceName.isEmpty() || ssid.isNull()) {
            continue;
        }
        if (map.contains(ifaceName)) {
            map[ifaceName].append(activeConnectionPtr->uuid());
        } else {
            QStringList list;
            list.append(activeConnectionPtr->uuid());
            map.insert(ifaceName,list);
        }
    }
    return;
}

QString KyWirelessNetResource::getDeviceIFace(NetworkManager::ActiveConnection::Ptr actConn,
                                              QString &wirelessNetResourcessid)
{
    if (actConn.isNull()) {
        return "";
    }

    NetworkManager::Connection::Ptr conn = actConn->connection();
    if (conn.isNull()) {
        return "";
    }

    NetworkManager::ConnectionSettings::Ptr sett = conn->settings();
    if (sett.isNull()) {
        return "";
    }

    NetworkManager::WirelessSetting::Ptr wireless_sett = sett->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    if (wireless_sett.isNull()) {
        return "";
    }
    wirelessNetResourcessid = wireless_sett->ssid();

    return sett->interfaceName();
}

void KyWirelessNetResource::getSsidByUuid(const QString uuid, QString &ssid, QString &devName)
{
    ssid.clear();
    devName.clear();
    NetworkManager::Connection::Ptr connectPtr = m_networkResourceInstance->getConnect(uuid);
    if (connectPtr.isNull()) {
        return;
    }
    NetworkManager::WirelessSetting::Ptr wireless_sett
        = connectPtr->settings()->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    if (wireless_sett.isNull()) {
        qDebug() << "don't have WirelessSetting connection";
        return;
    }
    ssid = wireless_sett->ssid();
    devName = connectPtr->settings()->interfaceName();
    qDebug() << "getSsidByUuid success " << ssid;
    return;
}

void KyWirelessNetResource::kyWirelessNetItemListInit()
{
    qDebug() << m_networkResourceInstance->m_wifiNets.size();
    for (auto const & net : m_networkResourceInstance->m_wifiNets) {
        QString devIface = getDeviceIFace(net);
        if (devIface.isEmpty()) {
            continue;
        }
        KyWirelessNetItem item(net);
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

QString KyWirelessNetResource::getDeviceIFace(NetworkManager::WirelessNetwork::Ptr net)
{
    if (net.isNull()) {
        return "";
    }
    QString devUni = net->device();
    NetworkManager::Device::Ptr dev = m_networkResourceInstance->findDeviceUni(devUni);
    if (dev.isNull()) {
        qDebug() << "KyWirelessNetResource: can't find " << net->ssid() << " find in device list";
        return "";
    }
    return dev->interfaceName();
}

//void KyWirelessNetResource::onWifiNetworkChange(QString devIfaceName)
//{
//    //创建新加入的的device key
//    if(!m_WifiNetworkList.contains(devIfaceName))
//    {
//        QList<KyWirelessNetItem> list;
//        m_WifiNetworkList.insert(devIfaceName,list);
//    }

//    //清空重新append
//    m_WifiNetworkList[devIfaceName].clear();
//    for (auto const & net : m_networkResourceInstance->m_wifiNets)
//    {
//        if (m_networkResourceInstance->findDeviceUni(net->device())->interfaceName() == devIfaceName)
//        {
//            qDebug() << net->ssid();
//            KyWirelessNetItem item(net);
//            m_WifiNetworkList[devIfaceName].append(item);
//        }
//    }

//    //若仍为空则remove
//    if (m_WifiNetworkList.value(devIfaceName).isEmpty())
//    {
//        m_WifiNetworkList.remove(devIfaceName);
//    }

//    emit updateWifiNetworkList(devIfaceName);
//}

void KyWirelessNetResource::onWifiNetworkAdded(QString devIfaceName, QString ssid)
{
    NetworkManager::WirelessNetwork::Ptr wifi = nullptr;
    for (auto const & net : m_networkResourceInstance->m_wifiNets) {
        if (net->ssid() == ssid && m_networkResourceInstance->findDeviceUni(net->device())->interfaceName() == devIfaceName) {
            wifi = net;
        }
    }

    if (wifi.isNull()) {
        return;
    }
    KyWirelessNetItem item(wifi);

    if (m_WifiNetworkList.contains(devIfaceName)) {
        m_WifiNetworkList[devIfaceName].append(item);
    } else {
        QList<KyWirelessNetItem> list;
        list.append(item);
        m_WifiNetworkList.insert(devIfaceName,list);
    }

    emit wifiNetworkAdd(devIfaceName, item);
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
        emit wifiNetworkRemove(devIfaceName,ssid);
    }
}

void KyWirelessNetResource::onWifiNetworkPropertyChange(NetworkManager::WirelessNetwork * net)
{
    if (nullptr == net) {
        return;
    }

    QString devIface = m_networkResourceInstance->findDeviceUni(net->device())->interfaceName();
    if (m_WifiNetworkList.contains(devIface)) {
        QList<KyWirelessNetItem>::iterator iter = m_WifiNetworkList[devIface].begin();
         while (iter != m_WifiNetworkList[devIface].end()) {
             if (iter->m_NetSsid == net->ssid()) {
                 qDebug()<<"recive properity changed signal, sender is" << iter->m_NetSsid;
                 if (iter->m_signalStrength != net->signalStrength()) {
                     iter->m_signalStrength = net->signalStrength();
                     emit signalStrengthChange(devIface, net->ssid(), iter->m_signalStrength);
                 }

                 if (iter->m_bssid != net->referenceAccessPoint()->hardwareAddress()) {
                     iter->m_bssid = net->referenceAccessPoint()->hardwareAddress();
                     emit bssidChange(devIface, net->ssid(), iter->m_bssid);
                 }

                 QString secuType = enumToQstring(net->referenceAccessPoint()->capabilities(),
                                                  net->referenceAccessPoint()->wpaFlags(), net->referenceAccessPoint()->rsnFlags());
                 if (iter->m_secuType != secuType) {
                     iter->m_secuType = secuType;
                     emit secuTypeChange(devIface, net->ssid(), secuType);
                 }

                 break;
             }
             iter++;
         }
    }
}

void KyWirelessNetResource::onWifiNetworkDeviceDisappear()
{
    m_WifiNetworkList.clear();
    kyWirelessNetItemListInit();
}

bool KyWirelessNetResource::getEnterPriseInfoTls(QString &uuid, KyEapMethodTlsInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->getConnect(uuid);
    if (conn.isNull()) {
        qDebug() << "modifyEnterPriseInfoTls connection missing";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qDebug() << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        return false;
    }
    NetworkManager::Security8021xSetting::Ptr setting = conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull()) {
        qDebug() << "don't have Security8021x connection";
        return false;
    }


    info.identity = setting->identity();
    info.domain = setting->domainSuffixMatch();
    info.caCertPath = setting->caPath();
    info.clientCertPath = setting->clientCertificate();
    info.clientPrivateKey = QString(setting->privateKey());
    info.clientPrivateKeyPWD = m_operation->getPrivateKeyPassword(conn->uuid());

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoPeap(QString &uuid, KyEapMethodPeapInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->getConnect(uuid);
    if (conn.isNull()) {
        qDebug() << "getEnterPriseInfoPeap connection missing";
        return false;
    }
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qDebug() << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        qDebug() << "keyMgmt not WpaEap " << security_sett->keyMgmt();
        return false;
    }
    NetworkManager::Security8021xSetting::Ptr setting = conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodPeap)) {
        qDebug() << "don't have Security8021x connection";
        return false;
    }

    info.phase2AuthMethod = (KyNoEapMethodAuth)setting->phase2AuthMethod();
    info.userName = setting->identity();
    info.userPWD = m_operation->get8021xPassword(conn->uuid());

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoTtls(QString &uuid, KyEapMethodTtlsInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->getConnect(uuid);
    if (conn.isNull()) {
        qDebug() << "modifyEnterPriseInfoTtls connection missing";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull()) {
        qDebug() << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap) {
        qDebug() << "not wpaeap"<<security_sett->keyMgmt();
        return false;
    }

    NetworkManager::Security8021xSetting::Ptr setting = conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodTtls)) {
        qDebug() << "don't have Security8021x connection";
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
    info.userPWD = m_operation->get8021xPassword(conn->uuid());


    return true;
}


void KyWirelessNetResource::onConnectionAdd(QString uuid)
{
    qDebug() << "onConnectionAdd " << uuid;
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->getConnect(uuid);
    if (conn.isNull()) {
        qDebug() << "onConnectionAdd can not find connection" << uuid;
        return;
    }
    NetworkManager::ConnectionSettings::Ptr sett= conn->settings();
    if (sett->connectionType() != NetworkManager::ConnectionSettings::ConnectionType::Wireless) {
        return;
    }
    NetworkManager::WirelessSetting::Ptr wireless_sett = sett->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    QMap<QString, QString> map;
    map.clear();
    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
    while (iter != m_WifiNetworkList.end()) {
        for(int i = 0; i < iter.value().size(); i++) {
            if (iter.value().at(i).m_NetSsid == wireless_sett->ssid() && (sett->interfaceName() == iter.key() || sett->interfaceName().isEmpty())) {
                QString devIfaceName;
                QString ssid;
                m_WifiNetworkList[iter.key()][i].m_isConfigured = true;
                m_WifiNetworkList[iter.key()][i].m_connName = conn->name();
                m_WifiNetworkList[iter.key()][i].m_connectUuid = conn->uuid();
                m_WifiNetworkList[iter.key()][i].m_connDbusPath = conn->path();
                m_WifiNetworkList[iter.key()][i].m_channel = wireless_sett->channel();

                devIfaceName = sett->interfaceName();
                ssid = iter.value().at(i).m_NetSsid;
                map.insert(devIfaceName, ssid);

                break;
            }
        }
        iter++;
    }
    if (!map.isEmpty()) {
        for(auto var = map.cbegin(); var != map.cend(); var++) {
            QString devIfaceName = var.key();
            QString ssid = var.value();
            emit connectionAdd(devIfaceName, ssid);
        }
    }
}

void KyWirelessNetResource::onConnectionRemove(QString path)
{
    qDebug() << "onConnectionRemove remove " << path;
    QMap<QString, QString> map;
    map.clear();

    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
    while (iter != m_WifiNetworkList.end())
    {
        qDebug() << iter.key();
        for(int i = 0; i < iter.value().size(); i++) {
            qDebug() << iter.value().at(i).m_connDbusPath;
            if (iter.value().at(i).m_connDbusPath == path)
            {
                QString devIfaceName;
                QString ssid;
                m_WifiNetworkList[iter.key()][i].m_isConfigured = false;
                m_WifiNetworkList[iter.key()][i].m_connName = "";
                m_WifiNetworkList[iter.key()][i].m_connectUuid = "";
                m_WifiNetworkList[iter.key()][i].m_connDbusPath = "";
                m_WifiNetworkList[iter.key()][i].m_channel = 0;

                devIfaceName = iter.key();
                ssid = iter.value().at(i).m_NetSsid;
                map.insert(devIfaceName, ssid);
                break;
            }
        }
        iter++;
    } 
    if (!map.isEmpty()) {
        for(auto var = map.cbegin(); var != map.cend(); var++) {
            QString devIfaceName = var.key();
            QString ssid = var.value();
            emit connectionRemove(devIfaceName, ssid);
        }
    }

}


void KyWirelessNetResource::onConnectionUpdate(QString uuid)
{
    qDebug() << "onConnectionUpdate " << uuid;
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->getConnect(uuid);
    if (conn.isNull()) {
        qDebug() << "onConnectionAdd can not find connection" << uuid;
        return;
    }

    NetworkManager::ConnectionSettings::Ptr sett= conn->settings();
    if (sett->connectionType() != NetworkManager::ConnectionSettings::ConnectionType::Wireless) {
        return;
    }
    m_WifiNetworkList.clear();
    kyWirelessNetItemListInit();
    emit wifiNetworkUpdate();
}


void KyWirelessNetResource::onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType)
{
    if(deviceType == NetworkManager::Device::Type::Wifi) {
        if (!m_WifiNetworkList.contains(deviceName)) {
            m_WifiNetworkList.insert(deviceName,QList<KyWirelessNetItem>());
        }
    }
}

void KyWirelessNetResource::onDeviceRemove(QString deviceName)
{
    if (m_WifiNetworkList.contains(deviceName)) {
        m_WifiNetworkList.remove(deviceName);
    }
}

void KyWirelessNetResource::onDeviceNameUpdate(QString oldName, QString newName)
{
    if (!m_WifiNetworkList.contains(oldName)) {
        return;
    }

    QMap<QString, QList<KyWirelessNetItem> >      newWifiNetworkList(m_WifiNetworkList);
    QList<KyWirelessNetItem> list = m_WifiNetworkList[oldName];
    newWifiNetworkList.remove(oldName);
    newWifiNetworkList.insert(newName,list);
    m_WifiNetworkList = newWifiNetworkList;

}
