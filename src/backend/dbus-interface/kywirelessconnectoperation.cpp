#include "kywirelessconnectoperation.h"

#include <networkmanagerqt/wirelesssecuritysetting.h>

#define PSK_SETTING_NAME "802-11-wireless-security"

NetworkManager::ConnectionSettings::Ptr assembleWpaXPskSettings(NetworkManager::AccessPoint::Ptr accessPoint, QString &psk, bool isAutoConnect)
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
    if (!psk.isEmpty())
    {
        security_sett->setPsk(psk);
    }
    return settings;
}

NetworkManager::ConnectionSettings::Ptr assembleWirelessSettings(const KyWirelessConnectSetting &connSettingInfo)
{
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(connSettingInfo.m_connectName);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(connSettingInfo.isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(connSettingInfo.m_ssid.toUtf8());
    wifi_sett->setSecurity("802-11-wireless-security");

    if (connSettingInfo.m_type != KyKeyMgmt::WpaNone && connSettingInfo.m_type != KyKeyMgmt::Unknown)
    {
        NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
        security_sett->setInitialized(true);
        security_sett->setKeyMgmt((NetworkManager::WirelessSecuritySetting::KeyMgmt)connSettingInfo.m_type);
        if (KyKeyMgmt::SAE == connSettingInfo.m_type || KyKeyMgmt::WpaPsk == connSettingInfo.m_type)
        {
            security_sett->setPsk(connSettingInfo.m_psk);
        }
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
    activateConnection(connUuid, devIfaceName);
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
        watcher->deleteLater();
    });
#endif
}

void KyWirelessConnectOperation::deActivateWirelessConnection(const QString activeConnectName, const QString &activeConnectUuid)
{
    deactivateConnection(activeConnectName, activeConnectUuid);
    return;
}

//普通wifi
void KyWirelessConnectOperation::addConnect(const KyWirelessConnectSetting &connSettingInfo)
{
    NetworkManager::ConnectionSettings::Ptr connSetting = assembleWirelessSettings(connSettingInfo);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            emit this->createConnectionError(errorMessage);
        } else {
            qDebug()<<"create wireless connect complete";
        }
        watcher->deleteLater();
    });

    return;
}
//tls
void KyWirelessConnectOperation::addTlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTlsInfo &tlsInfo)
{
    NetworkManager::ConnectionSettings::Ptr connSetting = assembleWirelessSettings(connSettingInfo);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodTlsSettings(connSetting, tlsInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless tls connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            emit this->createConnectionError(errorMessage);
        } else {
            qDebug()<<"create wireless connect complete";
        }
        watcher->deleteLater();
    });

    return;
}
//peap
void KyWirelessConnectOperation::addPeapConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodPeapInfo &peapInfo)
{
    NetworkManager::ConnectionSettings::Ptr connSetting = assembleWirelessSettings(connSettingInfo);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodPeapSettings(connSetting, peapInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless peap connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            emit this->createConnectionError(errorMessage);
        } else {
            qDebug()<<"create wireless connect complete";
        }
        watcher->deleteLater();
    });

    return;
}

void KyWirelessConnectOperation::addTtlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTtlsInfo &ttlsInfo)
{
    NetworkManager::ConnectionSettings::Ptr connSetting = assembleWirelessSettings(connSettingInfo);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodTtlsSettings(connSetting, ttlsInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless ttls connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            emit this->createConnectionError(errorMessage);
        } else {
            qDebug()<<"create wireless connect complete";
        }
        watcher->deleteLater();
    });

    return;
}

void KyWirelessConnectOperation::setWirelessAutoConnect(const QString &uuid, bool bAutoConnect)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        emit updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();
    setAutoConnect(connectionSettings, bAutoConnect);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::deleteWirelessConnect(const QString &connectUuid)
{
    qDebug()<<"delete wireless connect uuid " << connectUuid;

    deleteConnect(connectUuid);

    return ;
}

QString KyWirelessConnectOperation::getPsk(const QString &connectUuid)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(connectUuid);
    if (connectPtr.isNull()) {
        QString errorMessage = tr("it can not find connection") + connectUuid;
        qWarning()<<errorMessage;
        return "";
    }
    QDBusPendingReply<NMVariantMapMap> reply = connectPtr->secrets(PSK_SETTING_NAME);
    QMap<QString,QVariantMap> map(reply.value());
    if (map.contains("802-11-wireless-security") && map.value("802-11-wireless-security").contains("psk"))
    {
        QString psk = map.value("802-11-wireless-security").value("psk").toString();
        return  psk;
    }
    return "";
}

void KyWirelessConnectOperation::updateIpv4AndIpv6SettingInfo(const QString &uuid, const KyConnectSetting &connectSettingsInfo)
{
    qDebug()<<"updateIpv4AndIpv6SettingInfo wireless connect uuid " << uuid;

    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        emit updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setIpv4AndIpv6Setting(connectionSettings,connectSettingsInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::updateWirelessPersonalConnect(const QString &uuid, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        emit updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    if (connSettingInfo.m_type != Unknown &&connSettingInfo.m_type != WpaNone && connSettingInfo.m_type != WpaPsk && connSettingInfo.m_type != SAE)
    {
        qDebug() << "updateWirelessPersonalConnect " << connSettingInfo.m_type << " not support";
        return;
    }

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
            = connectionSettings->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    wifi_8021x_sett->setInitialized(false);

    updateWirelessSecu(connectionSettings, connSettingInfo, bPwdChange);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::updateWirelessEnterPriseTlsConnect(const QString &uuid, const KyEapMethodTlsInfo &tlsInfo)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (connectPtr.isNull()) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        emit updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setWirelessSecuWpaXEap(connectionSettings);
    modifyEapMethodTlsSettings(connectionSettings, tlsInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::updateWirelessEnterPrisePeapConnect(const QString &uuid, const KyEapMethodPeapInfo &peapInfo)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        emit updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setWirelessSecuWpaXEap(connectionSettings);
    modifyEapMethodPeapSettings(connectionSettings, peapInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::updateWirelessEnterPriseTtlsConnect(const QString &uuid, const KyEapMethodTtlsInfo &ttlsInfo)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        emit updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setWirelessSecuWpaXEap(connectionSettings);
    modifyEapMethodTtlsSettings(connectionSettings, ttlsInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::addAndActiveWirelessConnect(QString & devIface,KyWirelessConnectSetting &connSettingInfo,bool isHidden)
{
    qDebug() << "addAndActiveWirelessConnect" << connSettingInfo.m_ssid << devIface <<connSettingInfo.m_psk;
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString spec_object;
    NMVariantMapMap map_settings;

    NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
    if (!isHidden && wifiNet.isNull()) {
        qDebug() << "addAndActiveWirelessConnect can not find " << connSettingInfo.m_ssid << " in " << devIface;
        return;
    }

    qDebug() << "addAndActiveWirelessConnect  find " << connSettingInfo.m_ssid << " in " << devIface << " or hide";

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull()) {
        emit andAndActivateConnectionError("can not find device");
        return;
    }

    qDebug() << "addAndActiveWirelessConnect device " << devIface << " exist";
    dev_uni = dev->uni();
    auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
    if (!isHidden) {
        qDebug() << "start assemble";
        auto access_point = wifiNet->referenceAccessPoint();
        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;
    qDebug() << "start findBestWirelessSecurity";
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
            if (NetworkManager::ConnectionSettings::Ptr settings = assembleWpaXPskSettings(access_point, connSettingInfo.m_psk, connSettingInfo.isAutoConnect)) {
                map_settings = settings->toMap();
            } else {
                qWarning() << QStringLiteral("connection settings assembly for '%1' failed, abandoning activation...").arg(conn_name);
                return;
            }
            break;
            //TODO: other types...
        default:
            qDebug() << "addAndActiveWirelessConnect not support";
            break;
        }
        qDebug() << "finish assemble";
    } else {
        qDebug() << "start assembleWirelessSettings";
        map_settings = assembleWirelessSettings(connSettingInfo)->toMap();
        qDebug() << "finish assembleWirelessSettings";
    }

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qDebug() << "activation of connection failed " << errorMessage;
            emit andAndActivateConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::requestWirelessScan()
{
    for (auto const & dev : m_networkResourceInstance->m_devices) {
        auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
        if (nullptr != spec_dev) {
            m_networkResourceInstance->requestScan(spec_dev);
        }
    }
}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo &info, KyWirelessConnectSetting &connSettingInfo,
                                                                          QString & devIface, bool isHidden)
{
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString spec_object;
    NMVariantMapMap map_settings;

    if (!isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            qDebug() << "addAndActiveWirelessEnterPriseTlsConnect can not find " << connSettingInfo.m_ssid << " in " << devIface;
            return;
        }

        auto access_point = wifiNet->referenceAccessPoint();
        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull()) {
        emit andAndActivateConnectionError("can not find device");
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = assembleWirelessSettings(connSettingInfo);
    assembleEapMethodTlsSettings(settings, info);

    if(settings.isNull()) {
        qDebug() << "assembleEapMethodTlsSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qDebug() << "addAndActiveWirelessEnterPriseTlsConnect failed " << errorMessage;
            emit andAndActivateConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });

}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo &info, KyWirelessConnectSetting &connSettingInfo,
                                                                           QString & devIface, bool isHidden)
{
    qDebug() <<"addAndActiveWirelessEnterPrisePeapConnect";
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString spec_object;
    NMVariantMapMap map_settings;

    if (!isHidden)
    {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            qDebug() << "addAndActiveWirelessEnterPrisePeapConnect can not find " << connSettingInfo.m_ssid << " in " << devIface;
            return;
        }

        auto access_point = wifiNet->referenceAccessPoint();

        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull()) {
        emit andAndActivateConnectionError("can not find device");
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = assembleWirelessSettings(connSettingInfo);
    assembleEapMethodPeapSettings(settings, info);

    if(settings.isNull()) {
        qDebug() << "assembleEapMethodPeapSettings failed";
        return;
    }
    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qDebug() << "addAndActiveWirelessEnterPrisePeapConnect failed " << errorMessage;
            emit andAndActivateConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });

}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo &info, KyWirelessConnectSetting &connSettingInfo,
                                                                           QString & devIface, bool isHidden)
{
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString spec_object;
    NMVariantMapMap map_settings;

    if (!isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            qDebug() << "addAndActiveWirelessEnterPriseTtlsConnect can not find " << connSettingInfo.m_ssid << " in " << devIface;
            return;
        }

        auto access_point = wifiNet->referenceAccessPoint();
        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceInterface(devIface);
    if (dev.isNull()) {
        emit andAndActivateConnectionError("can not find device");
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = assembleWirelessSettings(connSettingInfo);
    assembleEapMethodTtlsSettings(settings, info);

    if(settings.isNull()) {
        qDebug() << "assembleEapMethodTtlsSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qDebug() << "addAndActiveWirelessEnterPriseTtlsConnect failed " << errorMessage;
            emit andAndActivateConnectionError(errorMessage);
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
    if (conn.isNull()) {
        qDebug() <<"get failed";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    flag = security_sett->pskFlags();
    return true;
}

//private
NetworkManager::WirelessNetwork::Ptr KyWirelessConnectOperation::checkWifiNetExist(QString ssid, QString devName)
{
    for (auto const & net : m_networkResourceInstance->m_wifiNets) {
        auto dev = m_networkResourceInstance->findDeviceUni(net->device());
        if (dev == nullptr) {
            continue;
        }
        if (dev->type() != NetworkManager::Device::Wifi || dev->interfaceName() != devName) {
            continue;
        }
        if (ssid == net->ssid()) {
            return net;
        }
    }
    return nullptr;
}

KyKeyMgmt KyWirelessConnectOperation::getConnectKeyMgmt(const QString &uuid)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connectPtr->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();

//    if(security_sett.isNull())
//    {
//        return KyKeyMgmt::Unknown;
//    }
    return (KyKeyMgmt)security_sett->keyMgmt();
}

void KyWirelessConnectOperation::updateWirelessSecu(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange)
{
    qDebug() << "updateWirelessSecu " << connSettingInfo.m_type;
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();

    KyKeyMgmt type = connSettingInfo.m_type;
    qDebug() << "set keyMgmt " << type;
    if (type == WpaNone) {
        security_sett->setInitialized(false);
        return;
    } else {
        security_sett->setInitialized(true);
    }
    security_sett->setKeyMgmt((NetworkManager::WirelessSecuritySetting::KeyMgmt)type);
    if (bPwdChange) {
        qDebug() << "get psk " << security_sett->psk();
        security_sett->setPsk(connSettingInfo.m_psk);
    }
    return;
}

void KyWirelessConnectOperation::setWirelessSecuWpaXEap(NetworkManager::ConnectionSettings::Ptr connSettingPtr)
{
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();

    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::KeyMgmt::WpaEap);
    return;
}

void KyWirelessConnectOperation::setIpv4AndIpv6Setting(NetworkManager::ConnectionSettings::Ptr connSetting, const KyConnectSetting &connSettingInfo)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connSetting->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    ipv4SettingSet(ipv4Setting, connSettingInfo);

    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connSetting->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    ipv6SettingSet(ipv6Setting, connSettingInfo);
}
