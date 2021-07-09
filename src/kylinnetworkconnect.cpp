#include "kylinnetworkconnect.h"
#include "sys/syslog.h"


NetworkManager::ConnectionSettings::Ptr assembleWpaXPskSettings(const NetworkManager::AccessPoint::Ptr accessPoint, const QString &psk)
{
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(accessPoint->ssid());
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(true);
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


KyNetworkConnect::KyNetworkConnect()
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    connect(this, &KyNetworkConnect::checkActiveonnection, this, &KyNetworkConnect::onCheckActiveonnection);
}

KyNetworkConnect::~KyNetworkConnect()
{
    m_networkResourceInstance = nullptr;
}

int KyNetworkConnect::activateConnection(const QString connectUuid)
{
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;

    qDebug()<<"it will activate connect"<<connectUuid;
    NetworkManager::Connection::Ptr connectPtr = m_networkResourceInstance->getConnect(connectUuid);
    conn_uni = connectPtr->path();
    conn_name = connectPtr->name();
    if (NetworkManager::ConnectionSettings::Vpn == connectPtr->settings()->connectionType()) {
        spec_object = dev_uni = QStringLiteral("/");
        /*
        // find first non-vpn active connection
        const auto act_i = std::find_if(d->mActiveConns.cbegin(), d->mActiveConns.cend(), [] (NetworkManager::ActiveConnection::Ptr const & conn) -> bool
        {
        return nullptr != dynamic_cast<NetworkManager::VpnConnection const *>(conn.data());
        });
        if (act_i != d->mActiveConns.cend() && !(*act_i)->devices().empty())
        {
        dev_uni = (*act_i)->devices().front();
        spec_object = (*act_i)->path();
        }
        */

    } else {
        dev_name = connectPtr->settings()->interfaceName();
        for (auto const & dev : m_networkResourceInstance->m_devices) {
            for (auto const & dev_conn : dev->availableConnections()) {
                if (dev_conn == connectPtr) {
                    dev_uni = dev->uni();
                    dev_name = dev->interfaceName();
                    break;
                }
            }
        }

        if (dev_uni.isEmpty() && !dev_name.isEmpty()) {
            auto dev = m_networkResourceInstance->findDeviceInterface(dev_name);
            if (!dev.isNull())
                dev_uni = dev->uni();
        }
    }

    if (dev_uni.isEmpty()) {
        //TODO: in what form should we output the warning messages
        qWarning() << QStringLiteral("can't find device '%1' to activate connection '%2' on").arg(dev_name).arg(connectPtr->name());
        return -EINVAL;
    }

    qDebug() << __FUNCTION__ << conn_uni << dev_uni << conn_name << dev_name << spec_object;

    //TODO: check vpn type etc..
    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::activateConnection(conn_uni, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [conn_name, dev_name] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            //TODO: in what form should we output the warning messages
            qWarning() << QStringLiteral("activation of connection '%1' on interface '%2' failed: %3").arg(conn_name)
                    .arg(dev_name).arg(watcher->error().message());
         }
         watcher->deleteLater();
    });

    return 0;
}

int KyNetworkConnect::deactivateConnection(const QString connectName, const QString &uuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    qDebug()<<"deactivetate connect name"<<connectName<<"uuid"<<uuid;
    for (index = 0; index < m_networkResourceInstance->m_activeConns.size(); ++index) {
        activateConnectPtr = m_networkResourceInstance->m_activeConns.at(index);
        if (activateConnectPtr->uuid() == uuid) {
            break;
        }
    }

    if (index >= m_networkResourceInstance->m_activeConns.size()) {
        qWarning()<<"it can not find the activate connect"<<connectName << "uuid"<<uuid;
        return -EINVAL;
    }

    qDebug() << __FUNCTION__ <<"dead active connection path:"<< activateConnectPtr->path();

    QDBusPendingReply<> reply = NetworkManager::deactivateConnection(activateConnectPtr->path());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, [activateConnectPtr] (QDBusPendingCallWatcher * watcher) {
        //TODO::it may should send signal deactivateConnectionFinished
        if (watcher->isError() || !watcher->isValid()) {
            //TODO: in what form should we output the warning messages
            qWarning() << QStringLiteral("deactivation of connection '%1' failed: %3").arg(activateConnectPtr->connection()->name())
                    .arg(watcher->error().message());
        } else {
            qWarning()<<"deactive connect finished"<<activateConnectPtr->connection()->name();
        }
         watcher->deleteLater();
    });

    return 0;
}

void KyNetworkConnect::requestScan(const QString ifaceName)
{
    auto const & dev = m_networkResourceInstance->getNetworkDevice(ifaceName);
    auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
    if (nullptr == spec_dev) {
        //TODO: in what form should we output the warning messages
        qWarning() << "dropping request for scan on non wireles device " << dev->interfaceName();
        return;
    }

    m_networkResourceInstance->requestScan(spec_dev);
}

void KyNetworkConnect::requestAllWifiScan()
{
    return;
}


void KyNetworkConnect::activateWirelessConnection(NetworkManager::WirelessNetwork::Ptr wirelessNet)
{
qDebug() << "activateWirelessConnections" ;
    emit starWaiting();
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;

    auto access_point = wirelessNet->referenceAccessPoint();
    Q_ASSERT(!access_point.isNull());
    dev_uni = wirelessNet->device();
    syslog(LOG_DEBUG,"dev_uni %s",dev_uni.toUtf8().data());
    auto dev = m_networkResourceInstance->findDeviceUni(dev_uni);
    Q_ASSERT(!dev.isNull());
    auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
    Q_ASSERT(nullptr != spec_dev);
    conn_uni = access_point->uni();
    syslog(LOG_DEBUG,"conn_uni %s",conn_uni.toUtf8().data());
    conn_name = access_point->ssid();
    syslog(LOG_DEBUG,"conn_name %s",conn_name.toUtf8().data());
    //find the device name
    NetworkManager::Connection::Ptr conn;
    dev_name = dev->interfaceName();
    syslog(LOG_DEBUG,"dev_name %s",dev_name.toUtf8().data());
    for (auto const & dev_conn : dev->availableConnections())
    {
        syslog(LOG_DEBUG,"dev_conn->settings()->id() %s",dev_conn->settings()->id().toUtf8().data());
        if (dev_conn->settings()->id() == conn_name)
        {
            conn = dev_conn;
        }
    }
    conn_uni = conn->path();
    qDebug()<<conn_uni;

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::activateConnection(conn_uni, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            //TODO: in what form should we output the warning messages
            qWarning() << QStringLiteral("activation of connection failed: %1").arg(watcher->error().message());
         } else {
            emit checkActiveonnection((qdbus_cast<QDBusObjectPath>(watcher->reply().arguments().at(0))).path());
        }
         qDebug() <<"  " << (qdbus_cast<QDBusObjectPath>(watcher->reply().arguments().at(0))).path();

         watcher->deleteLater();
    });
}

void KyNetworkConnect::addAndActivateWirelessConnection(NetworkManager::WirelessNetwork::Ptr wirelessNet, const QString &psk)
{
    qDebug() << "addAndActivateWirelessConnection" ;
    emit starWaiting();
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;
    NMVariantMapMap map_settings;

    auto access_point = wirelessNet->referenceAccessPoint();
    Q_ASSERT(!access_point.isNull());
    dev_uni = wirelessNet->device();
    syslog(LOG_DEBUG,"dev_uni %s",dev_uni.toUtf8().data());
    auto dev = m_networkResourceInstance->findDeviceUni(dev_uni);
    Q_ASSERT(!dev.isNull());
    auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
    Q_ASSERT(nullptr != spec_dev);
    conn_uni = access_point->uni();
    syslog(LOG_DEBUG,"conn_uni %s",conn_uni.toUtf8().data());
    conn_name = access_point->ssid();
    syslog(LOG_DEBUG,"conn_name %s",conn_name.toUtf8().data());
    //find the device name
    NetworkManager::Connection::Ptr conn;
    dev_name = dev->interfaceName();
    syslog(LOG_DEBUG,"dev_name %s",dev_name.toUtf8().data());

    //TODO: in what form should we output the warning messages
    qWarning() << QStringLiteral("can't find connection for '%1' on device '%2', will create new...").arg(conn_name).arg(dev_name);
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
            if (NetworkManager::ConnectionSettings::Ptr settings = assembleWpaXPskSettings(access_point,psk))
            {
                map_settings = settings->toMap();
            } else
            {
                qWarning() << QStringLiteral("connection settings assembly for '%1' failed, abandoning activation...").arg(conn_name);
                return;
            }
            break;
            //TODO: other types...
    }

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addAndActivateConnection(map_settings, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [&] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            //TODO: in what form should we output the warning messages
            qDebug() << "activation of connection failed " << watcher->error().message();
         }
         else{
            emit checkActiveonnection((qdbus_cast<QDBusObjectPath>(watcher->reply().arguments().at(1))).path());

        }
         watcher->deleteLater();
    });
}

void KyNetworkConnect::onActivateWirelessConnection(const QString &connectSsid, const QString &connectUuid)
{
    qDebug() << "onActivateWirelessConnection" << connectSsid;
    NetworkManager::WirelessNetwork::Ptr wirelessNet = nullptr;
    for (auto const & net : m_networkResourceInstance->m_wifiNets)
    {
        if (net->ssid() == connectSsid)
        {
            wirelessNet = net;
            break;
        }
    }

    if (wirelessNet.isNull())
    {
        //TODO:隐藏wifi不会存在与AP中，需要新建connection去连接
        qDebug() << "hidewifi";
        return;
    }

    qDebug() << "非隐藏wifi";
    NetworkManager::Connection::Ptr connectPtr = m_networkResourceInstance->getConnect(connectUuid);
    if (connectPtr.isNull())
    {
        //无配置文件，前段输入完密码直接调用addAndActivateWirelessConnection
        qDebug() << "emit noConnection";
        emit noConnection();
        return;
    }

//    //获取安全类型
//    NetworkManager::ConnectionSettings::Ptr settings = connectPtr->settings();
//    NetworkManager::WirelessSecuritySetting::Ptr wifiSecurity =
//            settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
//    int key_mgmt = wifiSecurity->keyMgmt();

    //获取密码存储策略  0:所有用户存储 1:当前用户存储 2:每次连接询问
    NetworkManager::ConnectionSettings::Ptr settings = connectPtr->settings();
    NetworkManager::WirelessSecuritySetting::Ptr wifiSecurity =
            settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    int psk_flag = wifiSecurity->pskFlags();

    qDebug() << "psk_flag=" <<psk_flag;

    if(psk_flag == NetworkManager::Setting::SecretFlagType::NotSaved)
    {
        //每次都要询问，前端弹出窗口
        qDebug() << "emit notSavedConnection";
        emit notSavedConnection();
        return;
    }

    activateWirelessConnection(wirelessNet);
}

void KyNetworkConnect::onActivateWirelessConnectionWithPWD(const QString &connectSsid, const QString &psk, bool isNotSaved, const QString &connectUuid)
{
    qDebug() << "onActivateWirelessConnectionWithPWD" << connectSsid;

    if(isNotSaved && !connectUuid.isEmpty())
    {
        qDebug() << "每次都要询问";
        //修改connection psk
        NetworkManager::Connection::Ptr connectPtr = m_networkResourceInstance->getConnect(connectUuid);
        if (connectPtr.isNull())
        {
            return;
        }

        NetworkManager::ConnectionSettings::Ptr settings = connectPtr->settings();
        NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
        qDebug() << connectPtr->path();
        security_sett->setPsk(psk);

        //
        NetworkManager::WirelessNetwork::Ptr wirelessNet = nullptr;
        for (auto const & net : m_networkResourceInstance->m_wifiNets)
        {
            if (net->ssid() == connectSsid)
            {
                wirelessNet = net;
                break;
            }
        }

        activateWirelessConnection(wirelessNet);
        return;
    }

    NetworkManager::WirelessNetwork::Ptr wirelessNet = nullptr;
    for (auto const & net : m_networkResourceInstance->m_wifiNets)
    {
        if (net->ssid() == connectSsid)
        {
            wirelessNet = net;
            break;
        }
    }

    if (wirelessNet.isNull())
    {
        //TODO:隐藏wifi不会存在与AP中，需要新建connection去连接
        qDebug() << "hidewifi";
        return;
    }

    addAndActivateWirelessConnection(wirelessNet,psk);
}


void KyNetworkConnect::onCheckActiveonnection(const QString &activeConnPath)
{
    if (m_networkResourceInstance->m_activeConns.size() == 0)
    {
        qDebug() << "不存在activeConnPath";
        emit connResult(1);
        return;
    }

    for (auto const & activeConn : m_networkResourceInstance->m_activeConns)
    {
        if (activeConn->path() == activeConnPath)
        {
            qDebug() << "存在activeConnPath,且匹配"<<activeConnPath;
            qDebug() << activeConn->state();
            connect(activeConn.data(), &NetworkManager::ActiveConnection::stateChangedReason, this, &KyNetworkConnect::onStateChangedReason);
            return;
        }
    }
    qDebug() << "存在activeConnPath,但不匹配"<<activeConnPath;
    emit connResult(1);
}

void KyNetworkConnect::onStateChangedReason(NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason)
{
    qDebug() << "onStateChangedReason state:" << state << " reason" << reason;
    if (state == NetworkManager::ActiveConnection::State::Activated)
    {
        emit connResult(0);
    } else if (state == NetworkManager::ActiveConnection::State::Deactivating || state == NetworkManager::ActiveConnection::State::Deactivated) {
        emit connResult(1);
    }
}
