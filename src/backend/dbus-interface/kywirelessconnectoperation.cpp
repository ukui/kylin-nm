#include "kywirelessconnectoperation.h"

NetworkManager::ConnectionSettings::Ptr assembleWpaXPskSettings(NetworkManager::AccessPoint::Ptr accessPoint, QString &psk, bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlag)
{
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(accessPoint->ssid());
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(accessPoint->ssid().toUtf8());
    wifi_sett->setSecurity("802-11-wireless-security");

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    if (NetworkManager::AccessPoint::Adhoc == accessPoint->mode())
    {
        wifi_sett->setMode(NetworkManager::WirelessSetting::Adhoc);
        security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaNone);
    } else
    {
        security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaPsk);
    }
    security_sett->setPsk(psk);
    return settings;
}


NetworkManager::ConnectionSettings::Ptr assembleWpaXPskHiddenSettings(QString &ssid, KySecuType &type, QString &psk, bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlag)
{
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(ssid);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(ssid.toUtf8());
    wifi_sett->setHidden(true);
    wifi_sett->setSecurity("802-11-wireless-security");

    if (type != KySecuType::NONE)
    {
        NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
        security_sett->setInitialized(true);
        switch (type) {
        case KySecuType::WPA_AND_WPA2_PERSONAL:
            security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaPsk);
            security_sett->setPskFlags(secretFlag);
            break;
        case KySecuType::WPA_AND_WPA2_ENTERPRISE:
            //TODO:隐藏企业wifi
            break;
        case KySecuType::WPA2_AND_WPA3_PERSONAL:
            security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::SAE);
            security_sett->setPskFlags(secretFlag);
            break;
        default:
            qDebug() << " unsupport security type";
            break;
        }

        security_sett->setPsk(psk);
    }
    return settings;
}

KyWirelessConnectOperation::KyWirelessConnectOperation(QObject *parent) : KyConnectOperation(parent)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifinEnabledChanged, this, &KyWirelessConnectOperation::wifinEnabledChanged);
}

KyWirelessConnectOperation::~KyWirelessConnectOperation()
{
    m_networkResourceInstance = nullptr;
}

void KyWirelessConnectOperation::activeWirelessConnect(QString devIfaceName, QString connUuid)
{
    activateConnection(connUuid);
    return;
#if 0
    NetworkManager::Connection::Ptr conn;
    conn = m_networkResourceInstance->getConnect(connUuid);
    if (conn.isNull())
    {
        qDebug() <<"get failed";
        emit connectFail(connUuid, devIfaceName, "connection do not exist");
        return;
    }

    QString conn_uni;
    QString dev_uni;
    QString spec_object;
    auto dev = m_networkResourceInstance->findDeviceInterface(devIfaceName);
    if (dev.isNull())
    {
        emit connectFail(conn->name(), devIfaceName, "devIface not exist");
        return;
    }
    dev_uni = dev->uni();
    conn_uni = conn->path();
    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::activateConnection(conn_uni, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            qWarning() << QStringLiteral("activation of connection failed: %1").arg(watcher->error().message());
            emit connectFail(connUuid, devIfaceName, "Internal error");
        }
        qDebug() << "5";
        watcher->deleteLater();
    });
#endif
}

void KyWirelessConnectOperation::deactivateWirelessConnection(const QString activeConnectName, const QString &activeConnectUuid)
{
    deactivateConnection(activeConnectName, activeConnectUuid);
    return;
}

//void KyWirelessConnectOperation::activeWirelessConnectWithPwd(QString devIfaceName, QString connUuid, QString psk)
//{

//    //todo:
//    NetworkManager::Connection::Ptr conn;
//    conn = m_networkResourceInstance->getConnect(connUuid);
//    if (conn.isNull())
//    {
//        qDebug() <<"get failed";
//        emit connectFail(connUuid, devIfaceName, "connection do not exist");
//        return;
//    }

//    NetworkManager::WirelessSecuritySetting::Ptr security_sett
//        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
//    security_sett->setPsk(psk);


//    conn->update(conn->settings()->toMap());


//    QString conn_uni;
//    QString dev_uni;
//    QString spec_object;
//    auto dev = m_networkResourceInstance->findDeviceInterface(devIfaceName);
//    if (dev.isNull())
//    {
//        emit connectFail(conn->name(), devIfaceName, "devIface not exist");
//        return;
//    }
//    dev_uni = dev->uni();
//    conn_uni = conn->path();
//    QDBusPendingCallWatcher * watcher;
//    watcher = new QDBusPendingCallWatcher{NetworkManager::activateConnection(conn_uni, dev_uni, spec_object), this};
//    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
//        if (watcher->isError() || !watcher->isValid())
//        {
//            qWarning() << QStringLiteral("activation of connection failed: %1").arg(watcher->error().message());
//            emit connectFail(connUuid, devIfaceName, "Internal error");
//        }
//        watcher->deleteLater();
//    });
//}

void KyWirelessConnectOperation::deActiveWirelessConnect(QString &uuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    qDebug()<<"deactivetate connect uuid "<<uuid;
    for (index = 0; index < m_networkResourceInstance->m_activeConns.size(); ++index) {
        activateConnectPtr = m_networkResourceInstance->m_activeConns.at(index);
        if (activateConnectPtr->uuid() == uuid) {
            break;
        }
    }

    if (index >= m_networkResourceInstance->m_activeConns.size()) {
        qWarning()<<"it can not find the activate connect uuid "<<uuid;
        emit disConnectFail(uuid,"no","connection do not exist");
        return;
    }

    qDebug() << __FUNCTION__ <<"dead active connection path:"<< activateConnectPtr->path();

    QDBusPendingReply<> reply = NetworkManager::deactivateConnection(activateConnectPtr->path());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            qWarning() << QStringLiteral("deactivation of connection failed");
            emit disConnectFail(uuid, "no", "Internal error");
        }
        watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::addAndActiveWirelessConnect(QString & ssid, QString & devIface,
                                                             QString psk, bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    qDebug() << "addAndActiveWirelessConnect" << ssid << devIface <<psk;
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;
    NMVariantMapMap map_settings;
    bool bFind = false;

    NetworkManager::WirelessNetwork::Ptr wifiNet = nullptr;

    for (auto const & net : m_networkResourceInstance->m_wifiNets)
    {
        auto dev = m_networkResourceInstance->findDeviceUni(net->device());
        if (dev == nullptr)
        {
           continue;
        }
        if (dev->type() != NetworkManager::Device::Wifi || dev->interfaceName() != devIface)
        {
            continue;
        }
        if (ssid == net->ssid())
        {
            wifiNet = net;
            bFind = true;
        }
    }

    if (!bFind)
    {
        qDebug() << "addAndActiveWirelessConnect can not find " << ssid << " in " << devIface;
        return;
    }

    auto access_point = wifiNet->referenceAccessPoint();
    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull())
    {
        emit connectFail(ssid, devIface, "devIface not exist");
        return;
    }
    auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
    Q_ASSERT(nullptr != spec_dev);
    conn_uni = access_point->uni();
    conn_name = access_point->ssid();
    dev_name = dev->interfaceName();
    dev_uni = dev->uni();

    spec_object = conn_uni;
    NetworkManager::WirelessSecurityType sec_type = NetworkManager::findBestWirelessSecurity(spec_dev->wirelessCapabilities()
            , true, (spec_dev->mode() == NetworkManager::WirelessDevice::Adhoc)
            , access_point->capabilities(), access_point->wpaFlags(), access_point->rsnFlags());

    qDebug() << "findBestWirelessSecurity type "<< sec_type;
    switch (sec_type)
    {
        case NetworkManager::UnknownSecurity:
            qWarning() << QStringLiteral("unknown security to use for '%1'").arg(conn_name);
        case NetworkManager::NoneSecurity:
            //nothing to do
            break;
        case NetworkManager::WpaPsk:
        case NetworkManager::Wpa2Psk:
            if (NetworkManager::ConnectionSettings::Ptr settings = assembleWpaXPskSettings(access_point, psk, isAutoConnect ,secretFlags))
            {
                map_settings = settings->toMap();
            } else
            {
                qWarning() << QStringLiteral("connection settings assembly for '%1' failed, abandoning activation...").arg(conn_name);
                return;
            }
            break;
            //TODO: other types...
        default:
            qDebug() << "not support";
            break;
    }

    qDebug() << dev_uni;
    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            qDebug() << "activation of connection failed " << watcher->error().message();
            emit connectFail(ssid, devIface, "Internal error");
        }
         watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::addAndActiveWirelessHiddenConnect(KySecuType &type, QString &ssid, QString &devIface,
                                                                   QString &psk, bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    qDebug() << "addAndActiveWirelessHiddenConnect";
    QString dev_uni;
    QString dev_name;
    QString spec_object;
    NMVariantMapMap map_settings;

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull())
    {
        emit connectFail(ssid, devIface, "devIface not exist");
        return;
    }
    dev_name = dev->interfaceName();
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = assembleWpaXPskHiddenSettings(ssid, type, psk, isAutoConnect, secretFlags);
    map_settings = settings->toMap();


    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            qDebug() << "activation of connection failed " << watcher->error().message();
            emit connectFail(ssid, devIface, "Internal error");
        }
         watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::requestWirelessScan()
{
    for (auto const & dev : m_networkResourceInstance->m_devices)
    {
        auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
        if (nullptr != spec_dev)
        {
            m_networkResourceInstance->requestScan(spec_dev);
        }
    }

}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo &info, QString & devIface,
                                                                          bool isHidden, bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;
    NMVariantMapMap map_settings;
    bool bFind = false;

    if (!isHidden)
    {
        NetworkManager::WirelessNetwork::Ptr wifiNet = nullptr;

        for (auto const & net : m_networkResourceInstance->m_wifiNets)
        {
            auto dev = m_networkResourceInstance->findDeviceUni(net->device());
            if (dev == nullptr)
            {
               continue;
            }
            if (dev->type() != NetworkManager::Device::Wifi || dev->interfaceName() != devIface)
            {
                continue;
            }
            if (info.connName == net->ssid())
            {
                wifiNet = net;
                bFind = true;
            }
        }

        if (!bFind)
        {
            qDebug() << "addAndActiveWirelessEnterPriseTlsConnect can not find " << info.connName << " in " << devIface;
            return;
        }

        auto access_point = wifiNet->referenceAccessPoint();
        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull())
    {
        emit connectFail(info.connName, devIface, "devIface not exist");
        return;
    }
    dev_name = dev->interfaceName();
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = KyEnterPriceSettingInfo::assembleEapMethodTlsSettings(info, isAutoConnect, secretFlags);
    if(settings.isNull())
    {
        qDebug() << "assembleEapMethodPeapSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            qDebug() << "activation of connection failed " << watcher->error().message();
            emit connectFail(info.connName, devIface, "Internal error");
        }
         watcher->deleteLater();
    });

}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo &info, QString &devIface,
                                                                           bool isHidden, bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    qDebug() <<"addAndActiveWirelessEnterPrisePeapConnect";
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;
    NMVariantMapMap map_settings;
    bool bFind = false;

    if (!isHidden)
    {
        NetworkManager::WirelessNetwork::Ptr wifiNet = nullptr;

        for (auto const & net : m_networkResourceInstance->m_wifiNets)
        {
            auto dev = m_networkResourceInstance->findDeviceUni(net->device());
            if (dev == nullptr)
            {
               continue;
            }
            if (dev->type() != NetworkManager::Device::Wifi || dev->interfaceName() != devIface)
            {
                continue;
            }
            if (info.connName == net->ssid())
            {
                wifiNet = net;
                bFind = true;
            }
        }

        if (!bFind)
        {
            qDebug() << "addAndActiveWirelessEnterPrisePeapConnect can not find " << info.connName << " in " << devIface;
            return;
        }

        auto access_point = wifiNet->referenceAccessPoint();

        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull())
    {
        emit connectFail(info.connName, devIface, "devIface not exist");
        return;
    }
    dev_name = dev->interfaceName();
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = KyEnterPriceSettingInfo::assembleEapMethodPeapSettings(info, isAutoConnect, secretFlags);
    if(settings.isNull())
    {
        qDebug() << "assembleEapMethodPeapSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            qDebug() << "activation of connection failed " << watcher->error().message();
            emit connectFail(info.connName, devIface, "Internal error");
        }
         watcher->deleteLater();
    });

}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo &info, QString &devIface,
                                                                           bool isHidden, bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;
    NMVariantMapMap map_settings;
    bool bFind = false;

    if (!isHidden)
    {
        NetworkManager::WirelessNetwork::Ptr wifiNet = nullptr;

        for (auto const & net : m_networkResourceInstance->m_wifiNets)
        {
            auto dev = m_networkResourceInstance->findDeviceUni(net->device());
            if (dev == nullptr)
            {
               continue;
            }
            if (dev->type() != NetworkManager::Device::Wifi || dev->interfaceName() != devIface)
            {
                continue;
            }
            if (info.connName == net->ssid())
            {
                wifiNet = net;
                bFind = true;
            }
        }

        if (!bFind)
        {
            qDebug() << "addAndActiveWirelessEnterPriseTlsConnect can not find " << info.connName << " in " << devIface;
            return;
        }

        auto access_point = wifiNet->referenceAccessPoint();
        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull())
    {
        emit connectFail(info.connName, devIface, "devIface not exist");
        return;
    }
    dev_name = dev->interfaceName();
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = KyEnterPriceSettingInfo::assembleEapMethodTtlsSettings(info, isAutoConnect, secretFlags);
    if(settings.isNull())
    {
        qDebug() << "assembleEapMethodPeapSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            qDebug() << "activation of connection failed " << watcher->error().message();
            emit connectFail(info.connName, devIface, "Internal error");
        }
         watcher->deleteLater();
    });
}

//无线网络开关设置
void KyWirelessConnectOperation::setWirelessEnabled(bool enabled)
{
    NetworkManager::setWirelessEnabled(enabled);
}

bool KyWirelessConnectOperation::getWirelessEnabled()
{
    return NetworkManager::isWirelessEnabled();
}

bool KyWirelessConnectOperation::getConnSecretFlags(QString &connUuid, NetworkManager::Setting::SecretFlags &flag)
{
    NetworkManager::Connection::Ptr conn;
    conn = m_networkResourceInstance->getConnect(connUuid);
    if (conn.isNull())
    {
        qDebug() <<"get failed";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    flag = security_sett->pskFlags();
    return true;
}

NetworkManager::ConnectionSettings::Ptr
    KyWirelessConnectOperation::createWirelessApSetting(const QString apSsid, const QString apPassword, const QString apDevice)
{
    NetworkManager::ConnectionSettings::Ptr connectionSettings =
                NetworkManager::ConnectionSettings::Ptr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    connectionSettings->setId(apSsid);
    connectionSettings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    connectionSettings->setAutoconnect(true);
    connectionSettings->setAutoconnectPriority(0);
    connectionSettings->setInterfaceName(apDevice);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    connectionSettings->setGatewayPingTimeout(0);

    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connectionSettings->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    ipv4Setting->setInitialized(true);
    ipv4Setting->setMethod(NetworkManager::Ipv4Setting::Shared);

    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connectionSettings->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    ipv6Setting->setInitialized(true);
    ipv6Setting->setMethod(NetworkManager::Ipv6Setting::Ignored);

    NetworkManager::WirelessSetting::Ptr wirelessSetting
        = connectionSettings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wirelessSetting->setInitialized(true);
    wirelessSetting->setSsid(apSsid.toUtf8());
    wirelessSetting->setMode(NetworkManager::WirelessSetting::NetworkMode::Ap);
    wirelessSetting->setSecurity("802-11-wireless-security");

    NetworkManager::WirelessSecuritySetting::Ptr wirelessSecuritySetting
        = connectionSettings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    wirelessSecuritySetting->setInitialized(true);
    if (apPassword.isEmpty()) {
        wirelessSecuritySetting->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaNone);
    } else {
        wirelessSecuritySetting->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaPsk);
        wirelessSecuritySetting->setPsk(apPassword);
    }

    return connectionSettings;
}

void KyWirelessConnectOperation::updateWirelessApSetting(
        NetworkManager::Connection::Ptr apConnectPtr,
        const QString apName, const QString apPassword, const QString apDevice)
{
    NetworkManager::ConnectionSettings::Ptr apConnectSettingPtr = apConnectPtr->settings();
    apConnectSettingPtr->setId(apName);
    apConnectSettingPtr->setInterfaceName(apDevice);

    NetworkManager::WirelessSetting::Ptr wirelessSetting
        = apConnectSettingPtr->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wirelessSetting->setInitialized(true);
    wirelessSetting->setSsid(apName.toUtf8());

    NetworkManager::WirelessSecuritySetting::Ptr wirelessSecuritySetting
        = apConnectSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    wirelessSecuritySetting->setInitialized(true);
    if (apPassword.isEmpty()) {
        wirelessSecuritySetting->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaNone);
    } else {
        wirelessSecuritySetting->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaPsk);
        wirelessSecuritySetting->setPsk(apPassword);
    }

    apConnectPtr->update(apConnectSettingPtr->toMap());
}

void KyWirelessConnectOperation::activeWirelessAp(const QString apUuid, const QString apName,
                                                  const QString apPassword, const QString apDevice)
{
    //1、检查连接是否存在
    NetworkManager::Connection::Ptr connectPtr = m_networkResourceInstance->getConnect(apUuid);
    if (nullptr == connectPtr) {
        NetworkManager::Device::Ptr devicePtr = m_networkResourceInstance->findDeviceInterface(apDevice);
        if (devicePtr.isNull())
        {
            emit connectFail(apName, apDevice, "devIface not exist");
            return;
        }

        QString deviceIdentifier = devicePtr->uni();

        NetworkManager::ConnectionSettings::Ptr apConnectSettingPtr =
                                createWirelessApSetting(apName, apPassword, apDevice);
        QString specificObject = "";
        QDBusPendingCallWatcher * watcher;
        watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(apConnectSettingPtr->toMap(), deviceIdentifier, specificObject), this};
        connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
            if (watcher->isError() || !watcher->isValid())
            {
                qDebug() << "activation of connection failed " << watcher->error().message();
                emit connectFail(apName, apDevice, "Internal error");
            }
             watcher->deleteLater();
        });
    } else {
        updateWirelessApSetting(connectPtr, apName, apPassword, apDevice);
        activateConnection(apUuid);
    }

    return;
}

void KyWirelessConnectOperation::deactiveWirelessAp(const QString apName, const QString apUuid)
{
    deactivateConnection(apName, apUuid);
    return;
}
