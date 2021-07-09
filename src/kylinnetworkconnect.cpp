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

#include <NetworkManagerQt/AdslDevice>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>
#include <NetworkManagerQt/WiredSetting>

KyNetworkConnect::KyNetworkConnect()
{
    qDebug()<<"construct network connect";
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
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
