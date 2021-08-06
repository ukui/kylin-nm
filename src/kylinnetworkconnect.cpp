/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

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


#include <NetworkManagerQt/AdslDevice>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>
#include <NetworkManagerQt/WiredSetting>

KyNetworkConnect::KyNetworkConnect()
{
    qDebug()<<"construct network connect";
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    connect(this, &KyNetworkConnect::checkActiveonnection, this, &KyNetworkConnect::onCheckActiveonnection);
}

KyNetworkConnect::~KyNetworkConnect()
{
    m_networkResourceInstance = nullptr;
}

void KyNetworkConnect::ipv4SettingInit(
        NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
        const KyConnectInfo &connectInfo)
{
    ipv4Setting->setInitialized(true);

    if (CONFIG_IP_DHCP == connectInfo.m_ipv4ConfigIpType) {
        ipv4Setting->setMethod(NetworkManager::Ipv4Setting::Automatic);
        return;
    } else {
        ipv4Setting->setMethod(NetworkManager::Ipv4Setting::Manual);
    }

    if (!connectInfo.m_ipv4Dns.empty()) {
        ipv4Setting->setDns(connectInfo.m_ipv4Dns);
    }

    if (!connectInfo.m_ipv4Address.empty()) {
        ipv4Setting->setAddresses(connectInfo.m_ipv4Address);
    }

    return;
}
void KyNetworkConnect::ipv6SettingInit(
        NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
        const KyConnectInfo &connectInfo)
{
    ipv6Setting->setInitialized(true);

    if (CONFIG_IP_DHCP == connectInfo.m_ipv6ConfigIpType) {
        ipv6Setting->setMethod(NetworkManager::Ipv6Setting::Automatic);
        return;
    }

    ipv6Setting->setMethod(NetworkManager::Ipv6Setting::Manual);
    if (!connectInfo.m_ipv6Dns.empty()) {
        ipv6Setting->setDns(connectInfo.m_ipv6Dns);
    }

    if (!connectInfo.m_ipv6Address.empty()) {
        ipv6Setting->setAddresses(connectInfo.m_ipv6Address);
    }

    return ;
}

void KyNetworkConnect::connectSettingInit(
        NetworkManager::ConnectionSettings::Ptr connectionSettings,
        const KyConnectInfo &connectInfo)
{
    connectionSettings->setId(connectInfo.m_connectName);
    connectionSettings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    connectionSettings->setAutoconnect(true);
    connectionSettings->setAutoconnectPriority(0);
    connectionSettings->setInterfaceName(connectInfo.m_ifaceName);
    return;
}

void KyNetworkConnect::createWiredConnect(const KyConnectInfo &connectInfo)
{
    NetworkManager::ConnectionSettings::Ptr connectionSettings = NetworkManager::ConnectionSettings::Ptr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired));
    connectSettingInit(connectionSettings, connectInfo);

    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connectionSettings->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    ipv4SettingInit(ipv4Setting, connectInfo);

    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connectionSettings->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    ipv6SettingInit(ipv6Setting, connectInfo);

    NetworkManager::WiredSetting::Ptr wiredSetting = connectionSettings->setting(NetworkManager::Setting::Wired).dynamicCast<NetworkManager::WiredSetting>();
    wiredSetting->setInitialized(true);

    qDebug()<<"add wired connect"<<connectInfo.m_connectName;

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::addConnection(connectionSettings->toMap()), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [](QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            //TODO: in what form should we output the warning messages
            qWarning() << "create connection failed: " << watcher->error().message();
         } else {
            qWarning()<<"success"<<watcher->reply().errorName() <<"error msg"<<watcher->reply().errorMessage();
            qWarning()<<"error type"<<watcher->error().type();
         }
         watcher->deleteLater();
    });

    return;
}

void KyNetworkConnect::createConnect(KyConnectType connectType, const KyConnectInfo &connectInfo)
{
    switch (connectType) {
        case WIRED_CONNECT:
            qDebug()<<"create wired connect";
            createWiredConnect(connectInfo);
            break;
        default:
            qWarning()<<"the connect type undefined"<<connectType;
            break;
    }

    return;
}

void KyNetworkConnect::updateConnect(const QString &connectUuid, const KyConnectInfo &connectInfo)
{
    qDebug()<<"update connect"<<connectUuid;
    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(connectUuid);

    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();
    connectSettingInit(connectionSettings, connectInfo);

    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connectionSettings->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    ipv4SettingInit(ipv4Setting, connectInfo);

    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connectionSettings->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    ipv6SettingInit(ipv6Setting, connectInfo);

    NetworkManager::WiredSetting::Ptr wiredSetting = connectionSettings->setting(NetworkManager::Setting::Wired).dynamicCast<NetworkManager::WiredSetting>();
    wiredSetting->setInitialized(true);

    connectPtr->update(connectionSettings->toMap());

    return ;
}

void KyNetworkConnect::deleteConnect(const QString &connectUuid)
{

    qWarning()<<"TODO:delete connect ";

    NetworkManager::Connection::Ptr connectPtr =
            NetworkManager::findConnectionByUuid(connectUuid);

    connectPtr->remove();

    return ;
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
            qWarning() << "activate  connection failed: " << watcher->error().message();
         } else {
            qWarning()<<"success"<<watcher->reply().errorName() <<"error msg"<<watcher->reply().errorMessage();
            qWarning()<<"error type"<<watcher->error().type();
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
        qDebug() << "finshed";
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
        qDebug() << net->ssid() << "   " << connectSsid;
        if (net->ssid() == connectSsid)
        {
            wirelessNet = net;
            break;
        }
    }

    if (wirelessNet.isNull())
    {
        //TODO:隐藏wifi不会存在与AP中，需要新建connection去连接
        qDebug() << "hidewifi";emit noConnection();
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
