#include "kywirelessconnectoperation.h"
#include "kylinnetworkmanagerutil.h"

#include <networkmanagerqt/wirelesssecuritysetting.h>

#define PSK_SETTING_NAME "802-11-wireless-security"
#define PRIVATE_PSK_SETTING_NAME "802-1x"

NetworkManager::ConnectionSettings::Ptr assembleWpaXPskSettings(NetworkManager::AccessPoint::Ptr accessPoint, QString &psk, bool isAutoConnect)
{
    QByteArray rawSsid = accessPoint->rawSsid();
    QString wifiSsid = getSsidFromByteArray(rawSsid);

    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(wifiSsid);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(rawSsid);
    wifi_sett->setSecurity("802-11-wireless-security");

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    if (NetworkManager::AccessPoint::Adhoc == accessPoint->mode()) {
        wifi_sett->setMode(NetworkManager::WirelessSetting::Adhoc);
        security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaNone);
    } else {
        security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaPsk);
    }
    if (!psk.isEmpty()) {
        security_sett->setPsk(psk);
    }

    return settings;
}

NetworkManager::ConnectionSettings::Ptr assembleSaeSettings(NetworkManager::AccessPoint::Ptr accessPoint, QString &psk, bool isAutoConnect)
{
    QByteArray rawSsid = accessPoint->rawSsid();
    QString wifiSsid = getSsidFromByteArray(rawSsid);

    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(wifiSsid);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(rawSsid);
    wifi_sett->setSecurity("802-11-wireless-security");

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    if (NetworkManager::AccessPoint::Adhoc == accessPoint->mode()) {
        wifi_sett->setMode(NetworkManager::WirelessSetting::Adhoc);
        security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaNone);
    } else {
        security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::SAE);
    }
    if (!psk.isEmpty()) {
        security_sett->setPsk(psk);
    }

    return settings;
}

NetworkManager::ConnectionSettings::Ptr assembleWirelessSettings(const NetworkManager::AccessPoint::Ptr accessPointPtr,
                                                                 const KyWirelessConnectSetting &connSettingInfo,
                                                                 KyKeyMgmt type,
                                                                 bool isHidden)
{
    QByteArray rawSsid;
    if (nullptr == accessPointPtr || accessPointPtr.isNull()) {
        rawSsid = connSettingInfo.m_ssid.toUtf8();
    } else {
        rawSsid = accessPointPtr->rawSsid();
    }

    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(connSettingInfo.m_connectName);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(connSettingInfo.m_isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);
    settings->setInterfaceName(connSettingInfo.m_ifaceName);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(rawSsid);
    wifi_sett->setSecurity("802-11-wireless-security");
    wifi_sett->setHidden(isHidden);


    if (type != KyKeyMgmt::WpaNone && type != KyKeyMgmt::Unknown)
    {
        NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
        security_sett->setInitialized(true);
        security_sett->setKeyMgmt((NetworkManager::WirelessSecuritySetting::KeyMgmt)type);
        if (KyKeyMgmt::SAE == type || KyKeyMgmt::WpaPsk == type)
        {
            security_sett->setPsk(connSettingInfo.m_psk);
        }
    }
    return settings;
}


KyWirelessConnectOperation::KyWirelessConnectOperation(QObject *parent) : KyConnectOperation(parent)
{
}

void KyWirelessConnectOperation::requestWirelessScan()
{
    NetworkManager::Device::List deviceList = m_networkResourceInstance->getNetworkDeviceList();

    for (auto const & dev : deviceList) {
        auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
        if (nullptr != spec_dev) {
            m_networkResourceInstance->requestScan(spec_dev);
        }
    }
}

//普通wifi
void KyWirelessConnectOperation::addConnect(const KyWirelessConnectSetting &connSettingInfo, KySecuType &type)
{
    KyKeyMgmt mgmt;
    if (type == NONE) {
        mgmt = WpaNone;
    } else if (type == WPA_AND_WPA2_PERSONAL) {
        mgmt = WpaPsk;
    } else if (type == WPA3_PERSONAL) {
        mgmt = WpaEap;
    } else {
        return;
    }

    NetworkManager::WirelessNetwork::Ptr wifiNet =
                                checkWifiNetExist(connSettingInfo.m_ssid, connSettingInfo.m_ifaceName);
    if (wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid
                                                + " is not exsit in " + connSettingInfo.m_ifaceName;
        qWarning()<<errorMessage;
        Q_EMIT createConnectionError(errorMessage);
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wifiNet->referenceAccessPoint();
    NetworkManager::ConnectionSettings::Ptr connSetting =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, mgmt, false);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->createConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });
}
//tls
void KyWirelessConnectOperation::addTlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTlsInfo &tlsInfo)
{

    NetworkManager::WirelessNetwork::Ptr wifiNet =
                                checkWifiNetExist(connSettingInfo.m_ssid, connSettingInfo.m_ifaceName);
    if (wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid
                                                + " is not exsit in " + connSettingInfo.m_ifaceName;
        qWarning() << errorMessage;
        Q_EMIT createConnectionError(errorMessage);
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wifiNet->referenceAccessPoint();
    NetworkManager::ConnectionSettings::Ptr connSetting =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, false);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodTlsSettings(connSetting, tlsInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless tls connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->createConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });

}
//peap
void KyWirelessConnectOperation::addPeapConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodPeapInfo &peapInfo)
{
    NetworkManager::WirelessNetwork::Ptr wifiNet =
                                checkWifiNetExist(connSettingInfo.m_ssid, connSettingInfo.m_ifaceName);
    if (wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid
                                                + " is not exsit in " + connSettingInfo.m_ifaceName;
        qWarning()<<errorMessage;
        Q_EMIT createConnectionError(errorMessage);
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wifiNet->referenceAccessPoint();
    NetworkManager::ConnectionSettings::Ptr connSetting =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, false);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodPeapSettings(connSetting, peapInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless peap connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->createConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::addTtlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTtlsInfo &ttlsInfo)
{
    NetworkManager::WirelessNetwork::Ptr wifiNet =
                                checkWifiNetExist(connSettingInfo.m_ssid, connSettingInfo.m_ifaceName);
    if (wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid
                                                + " is not exsit in " + connSettingInfo.m_ifaceName;
        qWarning()<<errorMessage;
        Q_EMIT createConnectionError(errorMessage);
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wifiNet->referenceAccessPoint();
    NetworkManager::ConnectionSettings::Ptr connSetting =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, false);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodTtlsSettings(connSetting, ttlsInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless ttls connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->createConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });

    return;
}

void KyWirelessConnectOperation::addLeapConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodLeapInfo &leapInfo)
{
    NetworkManager::WirelessNetwork::Ptr wifiNet =
                                checkWifiNetExist(connSettingInfo.m_ssid, connSettingInfo.m_ifaceName);
    if (wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid
                                                + " is not exsit in " + connSettingInfo.m_ifaceName;
        qWarning() << errorMessage;
        Q_EMIT createConnectionError(errorMessage);
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wifiNet->referenceAccessPoint();
    NetworkManager::ConnectionSettings::Ptr connSetting =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, false);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodLeapSettings(connSetting, leapInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless leap connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->createConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });

    return;
}

void KyWirelessConnectOperation::addPwdConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodPwdInfo &pwdInfo)
{
    NetworkManager::WirelessNetwork::Ptr wifiNet =
                                checkWifiNetExist(connSettingInfo.m_ssid, connSettingInfo.m_ifaceName);
    if (wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid
                                                + " is not exsit in " + connSettingInfo.m_ifaceName;
        qWarning() << errorMessage;
        Q_EMIT createConnectionError(errorMessage);
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wifiNet->referenceAccessPoint();
    NetworkManager::ConnectionSettings::Ptr connSetting =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, false);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodPwdSettings(connSetting, pwdInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless pwd connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->createConnectionError(errorMessage);
        }
        watcher->deleteLater();
    });

    return;
}

void KyWirelessConnectOperation::addFastConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodFastInfo &fastInfo)
{
    NetworkManager::WirelessNetwork::Ptr wifiNet =
                                checkWifiNetExist(connSettingInfo.m_ssid, connSettingInfo.m_ifaceName);
    if (wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid
                                                + " is not exsit in " + connSettingInfo.m_ifaceName;
        qWarning() << errorMessage;
        Q_EMIT createConnectionError(errorMessage);
        return;
    }

    NetworkManager::AccessPoint::Ptr accessPointPtr = wifiNet->referenceAccessPoint();
    NetworkManager::ConnectionSettings::Ptr connSetting =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, false);
    setIpv4AndIpv6Setting(connSetting, connSettingInfo);
    assembleEapMethodFastSettings(connSetting, fastInfo);

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connSetting->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("create wireless fast connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->createConnectionError(errorMessage);
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
        Q_EMIT updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();
    setAutoConnect(connectionSettings, bAutoConnect);
    connectPtr->update(connectionSettings->toMap());
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
    if (map.contains("802-11-wireless-security")
            && map.value("802-11-wireless-security").contains("psk"))     {
        QString psk = map.value("802-11-wireless-security").value("psk").toString();
        return  psk;
    }
    return "";
}

QString KyWirelessConnectOperation::getPrivateKeyPassword(const QString &connectUuid)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(connectUuid);
    if (connectPtr.isNull()) {
        QString errorMessage = tr("it can not find connection") + connectUuid;
        qWarning()<<errorMessage;
        return "";
    }
    QDBusPendingReply<NMVariantMapMap> reply = connectPtr->secrets(PRIVATE_PSK_SETTING_NAME);
    QMap<QString,QVariantMap> map(reply.value());
    if (map.contains("802-1x")
            && map.value("802-1x").contains("private-key-password")) {
        QString psk = map.value("802-1x").value("private-key-password").toString();
        return  psk;
    }
    return "";
}

QString KyWirelessConnectOperation::get8021xPassword(const QString &connectUuid)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(connectUuid);
    if (connectPtr.isNull()) {
        QString errorMessage = tr("it can not find connection") + connectUuid;
        qWarning()<<errorMessage;
        return "";
    }
    QDBusPendingReply<NMVariantMapMap> reply = connectPtr->secrets(PRIVATE_PSK_SETTING_NAME);
    QMap<QString,QVariantMap> map(reply.value());
    if (map.contains("802-1x") && map.value("802-1x").contains("password"))
    {
        QString psk = map.value("802-1x").value("password").toString();
        return  psk;
    }
    return "";
}

void KyWirelessConnectOperation::updateIpv4AndIpv6SettingInfo(const QString &uuid, const KyConnectSetting &connectSettingsInfo)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        Q_EMIT updateConnectionError(errorMessage);
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
        Q_EMIT updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

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
        Q_EMIT updateConnectionError(errorMessage);
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
        Q_EMIT updateConnectionError(errorMessage);
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
        Q_EMIT updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setWirelessSecuWpaXEap(connectionSettings);
    modifyEapMethodTtlsSettings(connectionSettings, ttlsInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::updateWirelessEnterPriseLeapConnect(const QString &uuid, const KyEapMethodLeapInfo &leapInfo)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        Q_EMIT updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setWirelessSecuWpaXEap(connectionSettings);
    modifyEapMethodLeapSettings(connectionSettings, leapInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::updateWirelessEnterPrisePwdConnect(const QString &uuid, const KyEapMethodPwdInfo &pwdInfo)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        Q_EMIT updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setWirelessSecuWpaXEap(connectionSettings);
    modifyEapMethodPwdSettings(connectionSettings, pwdInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::updateWirelessEnterPriseFastConnect(const QString &uuid, const KyEapMethodFastInfo &fastInfo)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (nullptr == connectPtr) {
        QString errorMessage = tr("it can not find connection") + uuid;
        qWarning()<<errorMessage;
        Q_EMIT updateConnectionError(errorMessage);
        return;
    }
    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();

    setWirelessSecuWpaXEap(connectionSettings);
    modifyEapMethodFastSettings(connectionSettings, fastInfo);
    connectPtr->update(connectionSettings->toMap());
    return;
}

void KyWirelessConnectOperation::addAndActiveWirelessConnect(KyWirelessConnectSetting &connSettingInfo, KySecuType &type)
{
    KyKeyMgmt mgmt;
    if (type == NONE) {
        mgmt = WpaNone;
    } else if (type == WPA_AND_WPA2_PERSONAL) {
        mgmt = WpaPsk;
    } else if (type == WPA3_PERSONAL) {
        mgmt = SAE;
    } else {
        return;
    }

    QString devIface = connSettingInfo.m_ifaceName;
    QString conn_uni, dev_uni, conn_name, spec_object;
    NMVariantMapMap map_settings;

    //检测wifi是否存在
    NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
    if (!connSettingInfo.isHidden && wifiNet.isNull()) {
        QString errorMessage = "the ssid " + connSettingInfo.m_ssid + " is not exsit in " + devIface;
        qWarning() << errorMessage;
        Q_EMIT activateConnectionError(errorMessage);
        return;
    }
    //配置生成
    auto dev = m_networkResourceInstance->findDeviceByName(devIface);
    if (dev.isNull()) {
        return;
    }

    dev_uni = dev->uni();
    auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
    if (!connSettingInfo.isHidden) {
        //非隐藏wifi
        auto access_point = wifiNet->referenceAccessPoint();
        conn_uni = access_point->uni();
        conn_name = access_point->ssid();
        spec_object = conn_uni;

        NetworkManager::WirelessSecurityType sec_type = NetworkManager::findBestWirelessSecurity(spec_dev->wirelessCapabilities()
                                                                                                 , true, (spec_dev->mode() == NetworkManager::WirelessDevice::Adhoc)
                                                                                                 , access_point->capabilities(), access_point->wpaFlags(), access_point->rsnFlags());
        NetworkManager::ConnectionSettings::Ptr settings = nullptr;
        switch (sec_type)
        {
        case NetworkManager::UnknownSecurity:
            qWarning() << QStringLiteral("unknown security to use for '%1'").arg(conn_name);
        case NetworkManager::NoneSecurity:
            //nothing to do
            break;
        case NetworkManager::WpaPsk:
        case NetworkManager::Wpa2Psk:
            settings = assembleWpaXPskSettings(access_point, connSettingInfo.m_psk, connSettingInfo.m_isAutoConnect);
            if (nullptr != settings) {
                map_settings = settings->toMap();
            } else {
                qWarning() << QStringLiteral("connection settings assembly for '%1' failed, abandoning activation...").arg(conn_name);
                return;
            }
            break;
        case NetworkManager::SAE:
            settings = assembleSaeSettings(access_point, connSettingInfo.m_psk, connSettingInfo.m_isAutoConnect);
            if (nullptr != settings) {
                map_settings = settings->toMap();
            } else {
                qWarning() << QStringLiteral("connection settings assembly for '%1' failed, abandoning activation...").arg(conn_name);
                return;
            }
            break;
        //TODO:其他加密类型
        default:
            qWarning() << "addAndActiveWirelessConnect not support";
            return;
        }
    } else {
        //隐藏wifi
        map_settings = assembleWirelessSettings(nullptr, connSettingInfo, mgmt, connSettingInfo.isHidden)->toMap();
    }
    //激活
    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qWarning() << "activation of connection failed " << errorMessage;
        }
        watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo &info, KyWirelessConnectSetting &connSettingInfo)
{
    QString devIface = connSettingInfo.m_ifaceName;
    QString conn_uni, dev_uni, spec_object;
    NMVariantMapMap map_settings;
    NetworkManager::AccessPoint::Ptr accessPointPtr = nullptr;

    if (!connSettingInfo.isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            QString errorMessage = "the ssid " + connSettingInfo.m_ssid + " is not exsit in " + devIface;
            qWarning()<<errorMessage;
            Q_EMIT activateConnectionError(errorMessage);
            return;
        }

        accessPointPtr = wifiNet->referenceAccessPoint();
        conn_uni = accessPointPtr->uni();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceByName(devIface);
    if (dev.isNull()) {
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings = assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, connSettingInfo.isHidden);
    assembleEapMethodTlsSettings(settings, info);

    if(settings.isNull()) {
        qWarning() << "assembleEapMethodTlsSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qWarning() << "addAndActiveWirelessEnterPriseTlsConnect failed " << errorMessage;
        }
        watcher->deleteLater();
    });

}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo &info, KyWirelessConnectSetting &connSettingInfo)
{
    qDebug() << "addAndActiveWirelessEnterPrisePeapConnect";
    QString devIface = connSettingInfo.m_ifaceName;
    QString conn_uni, dev_uni, spec_object;
    NMVariantMapMap map_settings;
    NetworkManager::AccessPoint::Ptr accessPointPtr = nullptr;

    if (!connSettingInfo.isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            QString errorMessage = "the ssid " + connSettingInfo.m_ssid + " is not exsit in " + devIface;
            qWarning() << errorMessage;
            Q_EMIT activateConnectionError(errorMessage);
            return;
        }

        accessPointPtr = wifiNet->referenceAccessPoint();
        conn_uni = accessPointPtr->uni();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceByName(devIface);
    if (dev.isNull()) {
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings =
                                    assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, connSettingInfo.isHidden);
    assembleEapMethodPeapSettings(settings, info);

    if(settings.isNull()) {
        qWarning() << "assembleEapMethodPeapSettings failed";
        return;
    }
    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qWarning() << "addAndActiveWirelessEnterPrisePeapConnect failed " << errorMessage;
        }
        watcher->deleteLater();
    });

}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo &info, KyWirelessConnectSetting &connSettingInfo)
{
    QString devIface = connSettingInfo.m_ifaceName;
    QString conn_uni, dev_uni, spec_object;
    NMVariantMapMap map_settings;
    NetworkManager::AccessPoint::Ptr accessPointPtr = nullptr;

    if (!connSettingInfo.isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            QString errorMessage = "the ssid " + connSettingInfo.m_ssid + " is not exsit in " + devIface;
            qWarning()<<errorMessage;
            Q_EMIT activateConnectionError(errorMessage);
            return;
        }

        accessPointPtr = wifiNet->referenceAccessPoint();
        conn_uni = accessPointPtr->uni();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceByName(devIface);
    if (dev.isNull()) {
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings =
                            assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, connSettingInfo.isHidden);
    assembleEapMethodTtlsSettings(settings, info);

    if(settings.isNull()) {
        qWarning() << "assembleEapMethodTtlsSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qWarning() << "addAndActiveWirelessEnterPriseTtlsConnect failed " << errorMessage;
        }
        watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseLeapConnect(KyEapMethodLeapInfo &info, KyWirelessConnectSetting &connSettingInfo)
{
    QString devIface = connSettingInfo.m_ifaceName;
    QString conn_uni, dev_uni, spec_object;
    NMVariantMapMap map_settings;
    NetworkManager::AccessPoint::Ptr accessPointPtr = nullptr;

    if (!connSettingInfo.isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            QString errorMessage = "the ssid " + connSettingInfo.m_ssid + " is not exsit in " + devIface;
            qWarning()<<errorMessage;
            Q_EMIT activateConnectionError(errorMessage);
            return;
        }

        accessPointPtr = wifiNet->referenceAccessPoint();
        conn_uni = accessPointPtr->uni();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceByName(devIface);
    if (dev.isNull()) {
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings =
                        assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, connSettingInfo.isHidden);
    assembleEapMethodLeapSettings(settings, info);

    if(settings.isNull()) {
        qDebug() << "assembleEapMethodLeapSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qDebug() << "addAndActiveWirelessEnterPriseLeapConnect failed " << errorMessage;
        }
        watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPrisePwdConnect(KyEapMethodPwdInfo &info, KyWirelessConnectSetting &connSettingInfo)
{
    QString devIface = connSettingInfo.m_ifaceName;
    QString conn_uni, dev_uni, spec_object;
    NMVariantMapMap map_settings;
    NetworkManager::AccessPoint::Ptr accessPointPtr = nullptr;

    if (!connSettingInfo.isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            QString errorMessage = "the ssid " + connSettingInfo.m_ssid + " is not exsit in " + devIface;
            qWarning()<<errorMessage;
            Q_EMIT activateConnectionError(errorMessage);
            return;
        }

        accessPointPtr = wifiNet->referenceAccessPoint();
        conn_uni = accessPointPtr->uni();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceByName(devIface);
    if (dev.isNull()) {
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings =
                        assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, connSettingInfo.isHidden);
    assembleEapMethodPwdSettings(settings, info);

    if(settings.isNull()) {
        qDebug() << "assembleEapMethodPwdSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qDebug() << "addAndActiveWirelessEnterPrisePwdConnect failed " << errorMessage;
        }
        watcher->deleteLater();
    });
}

void KyWirelessConnectOperation::addAndActiveWirelessEnterPriseFastConnect(KyEapMethodFastInfo &info, KyWirelessConnectSetting &connSettingInfo)
{
    QString devIface = connSettingInfo.m_ifaceName;
    QString conn_uni, dev_uni, spec_object;
    NMVariantMapMap map_settings;
    NetworkManager::AccessPoint::Ptr accessPointPtr = nullptr;

    if (!connSettingInfo.isHidden) {
        NetworkManager::WirelessNetwork::Ptr wifiNet = checkWifiNetExist(connSettingInfo.m_ssid, devIface);
        if (wifiNet.isNull()) {
            QString errorMessage = "the ssid " + connSettingInfo.m_ssid + " is not exsit in " + devIface;
            qWarning()<<errorMessage;
            Q_EMIT activateConnectionError(errorMessage);
            return;
        }

        accessPointPtr = wifiNet->referenceAccessPoint();
        conn_uni = accessPointPtr->uni();
        spec_object = conn_uni;
    }

    auto dev = m_networkResourceInstance->findDeviceByName(devIface);
    if (dev.isNull()) {
        return;
    }
    dev_uni = dev->uni();

    NetworkManager::ConnectionSettings::Ptr settings =
                        assembleWirelessSettings(accessPointPtr, connSettingInfo, WpaEap, connSettingInfo.isHidden);
    assembleEapMethodFastSettings(settings, info);

    if(settings.isNull()) {
        qDebug() << "assembleEapMethodFastSettings failed";
        return;
    }

    map_settings = settings->toMap();

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = watcher->error().message();
            qDebug() << "addAndActiveWirelessEnterPriseFastConnect failed " << errorMessage;
        }
        watcher->deleteLater();
    });
}

bool KyWirelessConnectOperation::getConnSecretFlags(QString &connUuid, NetworkManager::Setting::SecretFlags &flag)
{
    NetworkManager::Connection::Ptr conn;
    conn = m_networkResourceInstance->findConnectByUuid(connUuid);
    if (conn.isNull()) {
        qWarning() <<"get failed";
        return false;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = conn->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    flag = security_sett->pskFlags();
    return true;
}

NetworkManager::ConnectionSettings::Ptr
    KyWirelessConnectOperation::createWirelessApSetting(const QString apSsid,
                                                        const QString apPassword,
                                                        const QString apDevice,
                                                        const QString wirelessBand)
{
    NetworkManager::ConnectionSettings::Ptr connectionSettings =
                NetworkManager::ConnectionSettings::Ptr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    connectionSettings->setId(apSsid);
    connectionSettings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    connectionSettings->setAutoconnect(false);
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
    if (wirelessBand == WIFI_BAND_2_4GHZ) {
        wirelessSetting->setBand(NetworkManager::WirelessSetting::FrequencyBand::Bg);
    } else if (wirelessBand == WIFI_BAND_5GHZ) {
        wirelessSetting->setBand(NetworkManager::WirelessSetting::FrequencyBand::A);
    } else {
        qWarning()<<"[KyWirelessConnectOperation] the band type undefined"<<wirelessBand;
        wirelessSetting->setBand(NetworkManager::WirelessSetting::FrequencyBand::Automatic);
    }


    NetworkManager::WirelessSecuritySetting::Ptr wirelessSecuritySetting
        = connectionSettings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (apPassword.isEmpty()) {
        wirelessSecuritySetting->setInitialized(false);
    } else {
        wirelessSecuritySetting->setInitialized(true);
        wirelessSecuritySetting->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaPsk);
        wirelessSecuritySetting->setPsk(apPassword);
    }

    return connectionSettings;
}

void KyWirelessConnectOperation::updateWirelessApSetting(
        NetworkManager::Connection::Ptr apConnectPtr,
        const QString apName, const QString apPassword,
        const QString apDevice, const QString wirelessBand)

{
    NetworkManager::ConnectionSettings::Ptr apConnectSettingPtr = apConnectPtr->settings();
    apConnectSettingPtr->setId(apName);
    apConnectSettingPtr->setInterfaceName(apDevice);
    apConnectSettingPtr->setAutoconnect(false);

    NetworkManager::WirelessSetting::Ptr wirelessSetting
        = apConnectSettingPtr->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wirelessSetting->setInitialized(true);
    wirelessSetting->setSsid(apName.toUtf8());
    if (wirelessBand == WIFI_BAND_2_4GHZ) {
        wirelessSetting->setBand(NetworkManager::WirelessSetting::FrequencyBand::Bg);
    } else if (wirelessBand == WIFI_BAND_5GHZ) {
        wirelessSetting->setBand(NetworkManager::WirelessSetting::FrequencyBand::A);
    } else {
        qWarning()<<"[KyWirelessConnectOperation] the band type undefined"<<wirelessBand;
        wirelessSetting->setBand(NetworkManager::WirelessSetting::FrequencyBand::Automatic);
    }


    NetworkManager::WirelessSecuritySetting::Ptr wirelessSecuritySetting
        = apConnectSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    if (apPassword.isEmpty()) {
        wirelessSecuritySetting->setInitialized(false);
    } else {
        wirelessSecuritySetting->setInitialized(true);
        wirelessSecuritySetting->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaPsk);
        wirelessSecuritySetting->setPsk(apPassword);
    }

    apConnectPtr->update(apConnectSettingPtr->toMap());
}

void KyWirelessConnectOperation::activeWirelessAp(const QString apUuid, const QString apName,
                                                  const QString apPassword, const QString apDevice,
                                                  const QString wirelessBand)

{
    //1、检查连接是否存在
    NetworkManager::Connection::Ptr connectPtr = m_networkResourceInstance->findConnectByUuid(apUuid);
    if (nullptr == connectPtr) {
        NetworkManager::Device::Ptr devicePtr = m_networkResourceInstance->findDeviceByName(apDevice);
        if (devicePtr.isNull()) {
            QString errorMsg ="Create hotspot faild. " + apDevice + " is not existed";
            qWarning()<< errorMsg;
            return;
        }

        QString deviceIdentifier = devicePtr->uni();

        NetworkManager::ConnectionSettings::Ptr apConnectSettingPtr =
                                createWirelessApSetting(apName, apPassword, apDevice, wirelessBand);
        QString specificObject = "";
        QDBusPendingCallWatcher * watcher;
        watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(apConnectSettingPtr->toMap(), deviceIdentifier, specificObject), this};
        connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
            if (watcher->isError() || !watcher->isValid()) {
                QString errorMsg = "Create hotspot faild. " + watcher->error().message();
                qWarning() << errorMsg;
            }
             watcher->deleteLater();
        });
    } else {
        updateWirelessApSetting(connectPtr, apName, apPassword, apDevice, wirelessBand);
        QTimer::singleShot(500, this, [=](){
            activateApConnectionByUuid(apUuid, apDevice);
        });
    }
}

void KyWirelessConnectOperation::deactiveWirelessAp(const QString apName, const QString apUuid)
{
    deactivateConnection(apUuid);
}

//private
NetworkManager::WirelessNetwork::Ptr KyWirelessConnectOperation::checkWifiNetExist(QString ssid, QString devName)
{
    NetworkManager::WirelessNetwork::List wifiNetList = m_networkResourceInstance->getWifiNetworkList();

    for (auto const & net : wifiNetList) {
        auto dev = m_networkResourceInstance->findDeviceByUni(net->device());
        if (dev == nullptr) {
            continue;
        }

        if (dev->type() != NetworkManager::Device::Wifi || dev->interfaceName() != devName) {
            continue;
        }

        NetworkManager::AccessPoint::Ptr accessPointPtr = net->referenceAccessPoint();
        QByteArray rawSsid = accessPointPtr->rawSsid();
        QString wifiSsid = getSsidFromByteArray(rawSsid);

        if (ssid == wifiSsid) {
            return net;
        }
    }

    return nullptr;
}

KyKeyMgmt KyWirelessConnectOperation::getConnectKeyMgmt(const QString &uuid)
{
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(uuid);
    if (connectPtr.isNull()) {
        return KyKeyMgmt::Unknown;
    }

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connectPtr->settings()->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();

    if(security_sett.isNull()) {
        return KyKeyMgmt::Unknown;
    }
    return (KyKeyMgmt)security_sett->keyMgmt();
}

KySecuType KyWirelessConnectOperation::getConnectSecuType(const QString &uuid)
{
    KyKeyMgmt type = getConnectKeyMgmt(uuid);
    if (type == WpaNone) {
        return KySecuType::NONE;
    } else if (type == WpaPsk) {
        return KySecuType::WPA_AND_WPA2_PERSONAL;
    } else if (type == SAE) {
        return  KySecuType::WPA3_PERSONAL;
    } else if (type == WpaEap) {
        return  KySecuType::WPA_AND_WPA2_ENTERPRISE;
    } else {
        qWarning() << "KeyMgmt not support now " << type;
        return KySecuType::UNKNOWN;
    }
}

void KyWirelessConnectOperation::updateWirelessSecu(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange)
{
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();

    KyKeyMgmt type = getConnectKeyMgmt(connSettingPtr->uuid());
    if (type == WpaNone) {
        security_sett->setInitialized(false);
        return;
    } else {
        security_sett->setInitialized(true);
    }
    security_sett->setKeyMgmt((NetworkManager::WirelessSecuritySetting::KeyMgmt)type);
    if (bPwdChange) {
        security_sett->setPsk(connSettingInfo.m_psk);
        NetworkManager::Setting::SecretFlags flag = NetworkManager::Setting::None;
        security_sett->setPskFlags(flag);
    }
}

void KyWirelessConnectOperation::setWirelessSecuWpaXEap(NetworkManager::ConnectionSettings::Ptr connSettingPtr)
{
    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();

    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::KeyMgmt::WpaEap);
}

void KyWirelessConnectOperation::setIpv4AndIpv6Setting(NetworkManager::ConnectionSettings::Ptr connSetting, const KyConnectSetting &connSettingInfo)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connSetting->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    ipv4SettingSet(ipv4Setting, connSettingInfo);

    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connSetting->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    ipv6SettingSet(ipv6Setting, connSettingInfo);
}

void KyWirelessConnectOperation::activateApConnectionByUuid(const QString apUuid, const QString apDevice)
{
    QString connectPath = "";
    QString deviceIdentifier = "";
    QString connectName = "";
    QString specificObject = "";

    NetworkManager::Connection::Ptr  connectPtr = m_networkResourceInstance->findConnectByUuid(apUuid);
    if (connectPtr.isNull())
    {
        QString errorMessage = tr("Create hotspot faild.UUID is empty, its name") + apUuid;
        qWarning() << errorMessage;
        Q_EMIT activateConnectionError(errorMessage);
        return;
    }
    connectPath = connectPtr->path();
    connectName = connectPtr->name();

    auto dev = m_networkResourceInstance->findDeviceByName(apDevice);
    if (!dev.isNull()) {
        deviceIdentifier = dev->uni();
    }

    if (deviceIdentifier.isEmpty()) {
        QString errorMessage = tr("Create hotspot faild.Device Identifier is empty, its name") + apDevice;
        qWarning() << errorMessage;
        Q_EMIT activateConnectionError(errorMessage);
        return ;
    }

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::activateConnection(connectPath, deviceIdentifier, specificObject), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this, connectName, apDevice] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("Create hotspot faild. ") + watcher->error().message();
            qWarning()<<errorMessage;
            Q_EMIT this->activateConnectionError(errorMessage);
         } else {
            qWarning()<<"active wired connect complete.";
         }

         watcher->deleteLater();
    });
}

bool KyWirelessConnectOperation::isApConnection(QString uuid)
{
    NetworkManager::Connection::Ptr connectPtr = nullptr;

    connectPtr = m_networkResourceInstance->findConnectByUuid(uuid);
    return m_networkResourceInstance->isApConnection(connectPtr);
}
