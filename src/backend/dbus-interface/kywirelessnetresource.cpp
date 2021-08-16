#include "kywirelessnetresource.h"

KyWirelessNetResource::KyWirelessNetResource(QObject *parent)
    : QObject(parent)
{
    qDebug()<<"KyWirelessNetResource";
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    kyWirelessNetItemListInit();

    //TODO:connect device signal
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkAdded, this, &KyWirelessNetResource::onWifiNetworkAdded);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkRemoved, this, &KyWirelessNetResource::onWifiNetworkRemoved);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkPropertyChange, this, &KyWirelessNetResource::onWifiNetworkPropertyChange);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiNetworkDeviceDisappear, this, &KyWirelessNetResource::onWifiNetworkDeviceDisappear);

    //connect(m_networkResourceInstance, &KyNetworkResourceManager::connectionAdd, this, &KyWirelessNetResource::onConnectionAdd);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::connectionRemove, this, &KyWirelessNetResource::onConnectionRemove);

}

KyWirelessNetResource::~KyWirelessNetResource()
{
    m_networkResourceInstance = nullptr;
}

bool KyWirelessNetResource::getAllDeviceWifiNetwork(QMap<QString,QList<KyWirelessNetItem> > &map)
{
    onWifiNetworkDeviceDisappear();
    if (m_WifiNetworkList.isEmpty()) {
        return false;
    } else {
        map = m_WifiNetworkList;
        return true;
    }
}


bool KyWirelessNetResource::getDeviceWifiNetwork(QString devIfaceName, QList<KyWirelessNetItem> &wirelessNetResource)
{
    onWifiNetworkDeviceDisappear();

    if (!m_WifiNetworkList.contains(devIfaceName)) {
        return false;
    } else {
        wirelessNetResource = m_WifiNetworkList[devIfaceName];
        return true;
    }
}

bool KyWirelessNetResource::getWifiNetwork(QString &devIfaceName, QString &ssid, KyWirelessNetItem &wirelessNetResource)
{
    onWifiNetworkDeviceDisappear();

    if (!m_WifiNetworkList.contains(devIfaceName)) {
        return false;
    } else {
        for (int index = 0; m_WifiNetworkList[devIfaceName].size(); index ++){
            if (m_WifiNetworkList[devIfaceName].at(index).m_NetSsid  == ssid) {
                wirelessNetResource = m_WifiNetworkList[devIfaceName].at(index);
                return true;
            }
        }
    }

    return false;
}

bool KyWirelessNetResource::getWirelessActiveConnection(QMap<QString,QStringList> &map)
{
    int index = 0;
    NetworkManager::ActiveConnection::List activeConnectionList;

    map.clear();
    activeConnectionList.clear();
    activeConnectionList = m_networkResourceInstance->m_activeConns;
    if (activeConnectionList.isEmpty())
    {
        map.clear();
        return true;
    }

    NetworkManager::ActiveConnection::Ptr activeConnectionPtr = nullptr;
    for (; index < activeConnectionList.size(); index++)
    {
        activeConnectionPtr = activeConnectionList.at(index);
        if (NetworkManager::ConnectionSettings::ConnectionType::Wireless != activeConnectionPtr->type())
        {
            continue;
        }
        if (NetworkManager::ActiveConnection::Activated != activeConnectionPtr->state())
        {
            continue;
        }
        QString ssid;
        QString ifaceName = getDeviceIFace(activeConnectionPtr,ssid);
        if (map.contains(ifaceName)) {
            map[ifaceName].append(ssid);
        } else {
            QStringList list;
            list.append(ssid);
            map.insert(ifaceName,list);
        }
    }
    return true;
}

QString KyWirelessNetResource::getDeviceIFace(NetworkManager::ActiveConnection::Ptr actConn,
                                              QString &wirelessNetResourcessid)
{
    if (actConn.isNull()) {
        return "";
    }

    NetworkManager::Connection::Ptr conn = actConn->connection();
    NetworkManager::ConnectionSettings::Ptr sett = conn->settings();
    wirelessNetResourcessid = sett->id();

    return sett->interfaceName();
}

void KyWirelessNetResource::kyWirelessNetItemListInit()
{
    qDebug() << m_networkResourceInstance->m_wifiNets.size();
    for (auto const & net : m_networkResourceInstance->m_wifiNets) {
        QString devIface = getDeviceIFace(net);
        if (devIface.isEmpty()) {
            continue;
        }

        if (!m_WifiNetworkList.contains(devIface)){
            QList<KyWirelessNetItem> list;
            KyWirelessNetItem item(net);
            list.append(item);
            m_WifiNetworkList.insert(devIface,list);
        } else {
            KyWirelessNetItem item(net);
            m_WifiNetworkList[devIface].append(item);
        }
    }

    return;
}

QString KyWirelessNetResource::getDeviceIFace(NetworkManager::WirelessNetwork::Ptr net)
{
    if (net.isNull())
    {
        return "";
    }
    QString devUni = net->device();
    NetworkManager::Device::Ptr dev = m_networkResourceInstance->findDeviceUni(devUni);
    if (dev.isNull())
    {
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
    for (auto const & net : m_networkResourceInstance->m_wifiNets)
    {
        if (net->ssid() == ssid && m_networkResourceInstance->findDeviceUni(net->device())->interfaceName() == devIfaceName )
        {
            wifi = net;
        }
    }

    if (wifi.isNull())
    {
        return;
    }
    KyWirelessNetItem item(wifi);

    if (m_WifiNetworkList.contains(devIfaceName))
    {
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
    if (m_WifiNetworkList.contains(devIfaceName))
    {
        int index = 0;
        for ( ; index < m_WifiNetworkList.value(devIfaceName).size(); index++)
        {
            if ( m_WifiNetworkList[devIfaceName].at(index).m_NetSsid == ssid)
            {
                m_WifiNetworkList[devIfaceName].removeAt(index);
            }
        }
        //remove后为空则删除
        if (m_WifiNetworkList.value(devIfaceName).isEmpty())
        {
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
    if (m_WifiNetworkList.contains(devIface))
    {
        QList<KyWirelessNetItem>::iterator iter = m_WifiNetworkList[devIface].begin();
         while (iter != m_WifiNetworkList[devIface].end())
         {
             if (iter->m_NetSsid == net->ssid())
             {
                 qDebug()<<"recive properity changed signal, sender is" << iter->m_NetSsid;
                 if (iter->m_signalStrength != net->signalStrength())
                 {
                     iter->m_signalStrength = net->signalStrength();
                     emit signalStrengthChange(devIface, net->ssid(), iter->m_signalStrength);
                 }

                 if (iter->m_bssid != net->referenceAccessPoint()->hardwareAddress())
                 {
                     iter->m_bssid = net->referenceAccessPoint()->hardwareAddress();
                     emit bssidChange(devIface, net->ssid(), iter->m_bssid);
                 }

                 QString secuType = enumToQstring(net->referenceAccessPoint()->capabilities(), net->referenceAccessPoint()->wpaFlags(), net->referenceAccessPoint()->rsnFlags());
                 if (iter->m_secuType != secuType)
                 {
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
    if (conn.isNull())
    {
        qDebug() << "modifyEnterPriseInfoTls connection missing";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull())
    {
        qDebug() << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap)
    {
        return false;
    }
    NetworkManager::Security8021xSetting::Ptr setting = conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull())
    {
        qDebug() << "don't have Security8021x connection";
        return false;
    }

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodTls);
    setting->setEapMethods(list);
    setting->setIdentity(info.identity);
    if(!info.domain.isEmpty())
    {
        setting->setDomainSuffixMatch(info.domain);
    }
    setting->setCaPath(info.caCertPath);
    setting->setClientCertificate(info.clientCertPath.toLocal8Bit());
    setting->setPrivateKey(info.clientPrivateKey.toLocal8Bit());
    setting->setPrivateKeyPassword(info.clientPrivateKeyPWD);

    conn->update(conn->settings()->toMap());

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoPeap(QString &uuid, KyEapMethodPeapInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->getConnect(uuid);
    if (conn.isNull())
    {
        qDebug() << "getEnterPriseInfoPeap connection missing";
        return false;
    }
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull())
    {
        qDebug() << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap)
    {
        qDebug() << "keyMgmt not WpaEap " << security_sett->keyMgmt();
        return false;
    }
    NetworkManager::Security8021xSetting::Ptr setting = conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodPeap))
    {
        qDebug() << "don't have Security8021x connection";
        return false;
    }

    info.phase2AuthMethod = (KyNoEapMethodAuth)setting->phase2AuthMethod();
    info.userName = setting->identity();
    info.userPWD = setting->password();

    return true;
}

bool KyWirelessNetResource::getEnterPriseInfoTtls(QString &uuid, KyEapMethodTtlsInfo &info)
{
    NetworkManager::Connection::Ptr conn = m_networkResourceInstance->getConnect(uuid);
    if (conn.isNull())
    {
        qDebug() << "modifyEnterPriseInfoTtls connection missing";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (security_sett.isNull())
    {
        qDebug() << "don't have WirelessSecurity connection";
        return false;
    }
    if (security_sett->keyMgmt() != NetworkManager::WirelessSecuritySetting::WpaEap)
    {
        qDebug() << "not wpaeap"<<security_sett->keyMgmt();
        return false;
    }

    NetworkManager::Security8021xSetting::Ptr setting = conn->settings()->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    if (setting.isNull() || !setting->eapMethods().contains(NetworkManager::Security8021xSetting::EapMethod::EapMethodTtls))
    {
        qDebug() << "don't have Security8021x connection";
        return false;
    }

    info.authEapMethod = (KyEapMethodAuth)setting->phase2AuthEapMethod();
    info.authNoEapMethod = (KyNoEapMethodAuth)setting->phase2AuthMethod();

    info.authType = KyTtlsAuthMethod::AUTH_EAP;
    if (info.authEapMethod != KyAuthEapMethodUnknown)
    {
        info.authType = KyTtlsAuthMethod::AUTH_EAP;
    } else {
        info.authType = KyTtlsAuthMethod::AUTH_NO_EAP;
    }
    info.userName = setting->identity();
    info.userPWD = setting->password();

    return true;
}


void KyWirelessNetResource::onConnectionAdd(NetworkManager::Connection::Ptr conn)
{
    qDebug() << "onConnectionAdd add " << conn->name();
    QString devIfaceName;
    QString ssid;
    if (conn.isNull())
    {
        return;
    }
    NetworkManager::ConnectionSettings::Ptr sett= conn->settings();
    if (sett->connectionType() != NetworkManager::ConnectionSettings::ConnectionType::Wireless)
    {
        return;
    }
    NetworkManager::WirelessSetting::Ptr wireless_sett = sett->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    bool isFind = false;
    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
    while (iter != m_WifiNetworkList.end())
    {
        for(int i = 0; i < iter.value().size(); i++)
        {
            if (iter.value().at(i).m_NetSsid == wireless_sett->ssid())
            {
                isFind = true;
                m_WifiNetworkList[iter.key()][i].m_isConfigured = true;
                m_WifiNetworkList[iter.key()][i].m_connName = conn->name();
                m_WifiNetworkList[iter.key()][i].m_connectUuid = conn->uuid();
                m_WifiNetworkList[iter.key()][i].m_connDbusPath = conn->path();

                devIfaceName = sett->interfaceName();
                ssid = iter.value().at(i).m_NetSsid;
            }
        }
        iter++;
    }
    if (isFind)
    {
        emit connectionAdd(devIfaceName, ssid);
    }
}

void KyWirelessNetResource::onConnectionRemove(QString path)
{
    qDebug() << "onConnectionRemove remove " << path;
    bool isFind = false;
    QString devIfaceName;
    QString ssid;

    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = m_WifiNetworkList.begin();
    while (iter != m_WifiNetworkList.end())
    {
        qDebug() << iter.key();
        for(int i = 0; i < iter.value().size(); i++)
        {
            qDebug() << iter.value().at(i).m_connDbusPath;
            if (iter.value().at(i).m_connDbusPath == path)
            {
                isFind = true;
                m_WifiNetworkList[iter.key()][i].m_isConfigured = false;
                m_WifiNetworkList[iter.key()][i].m_connName = "";
                m_WifiNetworkList[iter.key()][i].m_connectUuid = "";
                m_WifiNetworkList[iter.key()][i].m_connDbusPath = "";

                devIfaceName = iter.key();
                ssid = iter.value().at(i).m_NetSsid;
                break;
            }
        }
        iter++;
    }

    if (isFind)
    {
        emit connectionRemove(devIfaceName, ssid);
    }
}
