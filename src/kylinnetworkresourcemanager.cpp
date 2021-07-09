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

#include "kylinnetworkresourcemanager.h"

KyNetworkResourceManager* KyNetworkResourceManager::m_pInstance = nullptr;

KyNetworkResourceManager* KyNetworkResourceManager::getInstance()
{
    if (m_pInstance == NULL) {
        m_pInstance = new KyNetworkResourceManager();
    }
    return m_pInstance;
}

void KyNetworkResourceManager::Release()
{
    if (m_pInstance != NULL) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

KyNetworkResourceManager::KyNetworkResourceManager()
{
    insertActiveConnections();
    insertConnections();
    insertDevices();
    insertWifiNetworks();

    //initialize NetworkManager signals
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded, this, &KyNetworkResourceManager::onDeviceAdded);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceRemoved, this, &KyNetworkResourceManager::onDeviceRemoved);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionAdded, this, &KyNetworkResourceManager::onActiveConnectionAdded);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionRemoved, this, &KyNetworkResourceManager::onActiveConnectionRemoved);
    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionAdded, this, &KyNetworkResourceManager::onConnectionAdded);
    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionRemoved, this, static_cast<void (KyNetworkResourceManager::*)(QString const &)>(&KyNetworkResourceManager::onConnectionRemoved));

    // Note: the connectionRemoved is never emitted in case network-manager service stop,
    // we need remove the connections manually.
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::serviceDisappeared, this, &KyNetworkResourceManager::clearConnections);

    qDebug() << m_activeConns.size() << m_connections.size() << m_devices.size();
}

KyNetworkResourceManager::~KyNetworkResourceManager()
{

}

void KyNetworkResourceManager::removeActiveConnection(int pos)
{
    //active connections signals
    NetworkManager::ActiveConnection::Ptr conn = m_activeConns.takeAt(pos);
    conn->disconnect(this);
}

void KyNetworkResourceManager::clearActiveConnections()
{
    while (0 < m_activeConns.size())
        removeActiveConnection(0);
}

void KyNetworkResourceManager::addActiveConnection(NetworkManager::ActiveConnection::Ptr conn)
{
    m_activeConns.push_back(conn);
    connect(conn.data(), &NetworkManager::ActiveConnection::connectionChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::default4Changed, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::default6Changed, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::dhcp4ConfigChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::dhcp6ConfigChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::ipV4ConfigChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::ipV6ConfigChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::idChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::typeChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::masterChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::specificObjectChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
        connect(conn.data(), &NetworkManager::ActiveConnection::stateChangedReason, this, &KyNetworkResourceManager::onActiveConnectionChangedReason);
    connect(conn.data(), &NetworkManager::ActiveConnection::stateChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::vpnChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::uuidChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    connect(conn.data(), &NetworkManager::ActiveConnection::devicesChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    if (conn->vpn())
    {
        connect(qobject_cast<NetworkManager::VpnConnection *>(conn.data()), &NetworkManager::VpnConnection::bannerChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
        connect(qobject_cast<NetworkManager::VpnConnection *>(conn.data()), &NetworkManager::VpnConnection::stateChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
    }
}

void KyNetworkResourceManager::insertActiveConnections()
{
    for (auto const & conn : NetworkManager::activeConnections())
        addActiveConnection(conn);
}

void KyNetworkResourceManager::removeConnection(int pos)
{
    //connections signals
    NetworkManager::Connection::Ptr conn = m_connections.takeAt(pos);
    conn->disconnect(this);
}

void KyNetworkResourceManager::clearConnections()
{
    while (0 < m_connections.size())
        removeConnection(0);
}

void KyNetworkResourceManager::addConnection(NetworkManager::Connection::Ptr conn)
{
    m_connections.push_back(conn);
    //connections signals
    connect(conn.data(), &NetworkManager::Connection::updated, this, &KyNetworkResourceManager::onConnectionUpdated);
    connect(conn.data(), &NetworkManager::Connection::removed, this, static_cast<void (KyNetworkResourceManager::*)()>(&KyNetworkResourceManager::onConnectionRemoved));
}

void KyNetworkResourceManager::insertConnections()
{
    for (auto const & conn : NetworkManager::listConnections())
        addConnection(conn);
}

void KyNetworkResourceManager::removeDevice(int pos)
{
    //connections signals
    NetworkManager::Device::Ptr device = m_devices.takeAt(pos);
    device->disconnect(this);
}

void KyNetworkResourceManager::clearDevices()
{
    while (0 < m_devices.size())
        removeDevice(0);
}

void KyNetworkResourceManager::addDevice(NetworkManager::Device::Ptr device)
{
    m_devices.push_back(device);
    //device signals
    //connect(device.data(), &NetworkManager::Device::stateChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::activeConnectionChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::autoconnectChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::availableConnectionChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::availableConnectionAppeared, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::availableConnectionDisappeared, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::capabilitiesChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::dhcp4ConfigChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::dhcp6ConfigChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::driverChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::driverVersionChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::firmwareMissingChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::firmwareVersionChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::interfaceNameChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::ipV4AddressChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::ipV4ConfigChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::ipV6ConfigChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::ipInterfaceChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::managedChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::physicalPortIdChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::mtuChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::nmPluginMissingChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::meteredChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::connectionStateChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::stateReasonChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    connect(device.data(), &NetworkManager::Device::stateChanged, this, &KyNetworkResourceManager::onDeviceStateChanged);
    connect(device.data(), &NetworkManager::Device::udiChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
    switch (device->type())
    {
        case NetworkManager::Ethernet:
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::bitRateChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::carrierChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::hardwareAddressChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::permanentHardwareAddressChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            break;

        case NetworkManager::Device::Wifi:
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::bitRateChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::activeAccessPointChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::modeChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::wirelessCapabilitiesChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::hardwareAddressChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::permanentHardwareAddressChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::wirelessPropertiesChanged, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::accessPointAppeared, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::accessPointDisappeared, this, &KyNetworkResourceManager::onDeviceUpdated);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::networkAppeared, this, &KyNetworkResourceManager::onWifiNetworkAppeared);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()), &NetworkManager::WirelessDevice::networkDisappeared, this, &KyNetworkResourceManager::onWifiNetworkDisappeared);
            break;
        default:
            //TODO: other device types!
            break;
    }
}

void KyNetworkResourceManager::insertDevices()
{
    for (auto const & device : NetworkManager::networkInterfaces())
        addDevice(device);
}

void KyNetworkResourceManager::removeWifiNetwork(int pos)
{
    //network signals
    NetworkManager::WirelessNetwork::Ptr net = m_wifiNets.takeAt(pos);
    net->disconnect(this);
}

void KyNetworkResourceManager::clearWifiNetworks()
{
    while (0 < m_wifiNets.size())
        removeWifiNetwork(0);
}

void KyNetworkResourceManager::addWifiNetwork(NetworkManager::WirelessNetwork::Ptr net)
{
    m_wifiNets.push_back(net);
    //device signals
    connect(net.data(), &NetworkManager::WirelessNetwork::signalStrengthChanged, this, &KyNetworkResourceManager::onWifiNetworkUpdated);
    connect(net.data(), &NetworkManager::WirelessNetwork::referenceAccessPointChanged, this, &KyNetworkResourceManager::onWifiNetworkUpdated);
    connect(net.data(), &NetworkManager::WirelessNetwork::disappeared, this, &KyNetworkResourceManager::onWifiNetworkUpdated);
}

void KyNetworkResourceManager::insertWifiNetworks()
{
    for (auto const & device : m_devices)
    {
        if (NetworkManager::Device::Wifi == device->type())
        {
            NetworkManager::WirelessDevice::Ptr w_dev = device.objectCast<NetworkManager::WirelessDevice>();
            for (auto const & net : w_dev->networks())
            {
                if (!net.isNull())
                {
                    addWifiNetwork(net);
                }
            }
        }
    }
}

NetworkManager::ActiveConnection::Ptr KyNetworkResourceManager::findActiveConnection(QString const & path)
{
    auto i = std::find_if(m_activeConns.cbegin(), m_activeConns.cend(), [&path] (NetworkManager::ActiveConnection::Ptr const & conn) -> bool {
        return conn->path() == path;
    });
    return m_activeConns.cend() == i ? NetworkManager::ActiveConnection::Ptr{} : *i;
}

template <typename Predicate>
NetworkManager::Device::Ptr KyNetworkResourceManager::findDevice(Predicate const & pred)
{
    auto i = std::find_if(m_devices.cbegin(), m_devices.cend(), pred);
    return m_devices.cend() == i ? NetworkManager::Device::Ptr{} : *i;
}

NetworkManager::Device::Ptr KyNetworkResourceManager::findDeviceUni(QString const & uni)
{
    return findDevice([&uni] (NetworkManager::Device::Ptr const & dev) { return dev->uni() == uni; });
}

NetworkManager::Device::Ptr KyNetworkResourceManager::findDeviceInterface(QString const & interfaceName)
{
    return findDevice([&interfaceName] (NetworkManager::Device::Ptr const & dev) { return dev->interfaceName() == interfaceName; });
}

NetworkManager::WirelessNetwork::Ptr KyNetworkResourceManager::findWifiNetwork(QString const & ssid, QString const & devUni)
{
    auto i = std::find_if(m_wifiNets.cbegin(), m_wifiNets.cend(), [&ssid, &devUni] (NetworkManager::WirelessNetwork::Ptr const & net) -> bool {
        return net->ssid() == ssid && net->device() == devUni;
    });
    return m_wifiNets.cend() == i ? NetworkManager::WirelessNetwork::Ptr{} : *i;
}

NetworkManager::Device::Ptr KyNetworkResourceManager::getNetworkDevice(const QString ifaceName)
{
    return nullptr;
}

NetworkManager::ActiveConnection::Ptr KyNetworkResourceManager::getActiveConnect(const QString connectUuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    qDebug()<<"get activetate connect with uuid"<<connectUuid;
    for (index = 0; index < m_activeConns.size(); ++index) {
        activateConnectPtr = m_activeConns.at(index);
        if (activateConnectPtr->uuid() == connectUuid) {
            return activateConnectPtr;
        }
    }

    qWarning()<<"it can not find the activate connect with uuid" <<connectUuid;

    return nullptr;
}

NetworkManager::Connection::Ptr KyNetworkResourceManager::getConnect(const QString connectUuid)
{
    int index = 0;
    NetworkManager::Connection::Ptr connectPtr = nullptr;

    qDebug()<<"get connect with uuid"<<connectUuid;

    for (index = 0; index < m_connections.size(); ++index) {
        connectPtr = m_connections.at(index);
        if (connectPtr->uuid() == connectUuid) {
            return connectPtr;
        }
    }

    qWarning()<<"it can not find connect with uuid"<<connectUuid;

    return nullptr;
}

NetworkManager::WirelessNetwork::Ptr KyNetworkResourceManager::getWifiNetwork(const QString apName)
{
    return nullptr;
}

void KyNetworkResourceManager::requestScan(NetworkManager::WirelessDevice * dev)
{
    qDebug() << __FUNCTION__ << dev->interfaceName();
    QDBusPendingReply<> reply = dev->requestScan();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, dev);
    connect(watcher, &QDBusPendingCallWatcher::finished, [dev] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid())
        {
            //TODO: in what form should we output the warning messages
            qWarning() << QStringLiteral("requestScan on device '%1' failed: %3").arg(dev->interfaceName())
                    .arg(watcher->error().message());
         }
         watcher->deleteLater();
    });
}

void KyNetworkResourceManager::onConnectionUpdated()
{
    emit connectionUpdate(qobject_cast<NetworkManager::Connection *>(sender()));
}

void KyNetworkResourceManager::onConnectionRemoved()
{
    emit connectionRemove(qobject_cast<NetworkManager::Connection *>(sender()));
}

void KyNetworkResourceManager::onActiveConnectionUpdated()
{
    emit activeConnectionUpdate(qobject_cast<NetworkManager::ActiveConnection *>(sender()));
}

void KyNetworkResourceManager::onActiveConnectionChangedReason(NetworkManager::ActiveConnection::State state,
                                      NetworkManager::ActiveConnection::Reason reason)
{
    qWarning()<<"the active connect state"<<state;
    qWarning()<<"the active connect state chanager reason:"<<reason;
    return;
}

void KyNetworkResourceManager::onDeviceUpdated()
{
    emit deviceUpdate(qobject_cast<NetworkManager::Device *>(sender()));
}

void KyNetworkResourceManager::onDeviceStateChanged(
                    NetworkManager::Device::State newstate,
                    NetworkManager::Device::State oldstate,
                    NetworkManager::Device::StateChangeReason reason)
{
    NetworkManager::WiredDevice *wiredDevice = qobject_cast<NetworkManager::WiredDevice *>(sender());
    if (wiredDevice->carrier()) {
        qWarning()<<"the device carrier true";
    } else {
        qWarning()<<"the device carrier false";
    }

    qWarning()<<"the device state "<<oldstate << "to" <<newstate << "reason"<< reason;
}

void KyNetworkResourceManager::onWifiNetworkAppeared(QString const & ssid)
{
    NetworkManager::Device * dev = qobject_cast<NetworkManager::Device *>(sender());
    emit wifiNetworkAdd(dev, ssid);
    emit deviceUpdate(dev);
}

void KyNetworkResourceManager::onWifiNetworkDisappeared(QString const & ssid)
{
    NetworkManager::Device * dev = qobject_cast<NetworkManager::Device *>(sender());
    emit wifiNetworkRemove(dev, ssid);
    emit deviceUpdate(dev);
}

void KyNetworkResourceManager::onWifiNetworkUpdated()
{
    emit wifiNetworkUpdate(qobject_cast<NetworkManager::WirelessNetwork *>(sender()));
}

void KyNetworkResourceManager::onDeviceAdded(QString const & uni)
{
    qDebug()<<Q_FUNC_INFO<<"onDeviceAdded"<<uni;

    NetworkManager::Device::Ptr dev = NetworkManager::findNetworkInterface(uni);
    if (!dev.isNull()) {
        if (dev->isValid())
            qWarning() << Q_FUNC_INFO << uni << " is currently invalid...";

        if (0 > m_devices.indexOf(dev)) {
            addDevice(dev);
        }

        emit deviceAdd(dev);
    }
}

void KyNetworkResourceManager::onDeviceRemoved(QString const & uni)
{
    qDebug()<<Q_FUNC_INFO<<"onDeviceRemoved"<<uni;
    NetworkManager::Device::Ptr dev = findDeviceUni(uni);
    if (!dev.isNull())
    {
        if (dev->isValid())
            qWarning() << Q_FUNC_INFO << uni << " is currently invalid...";

        auto index = std::find(m_devices.cbegin(), m_devices.cend(), dev);
        if (m_devices.cend() != index) {
            const int pos = index - m_devices.cbegin();
            removeDevice(pos);
        }

        emit deviceRemove(dev.data());
    }
}

void KyNetworkResourceManager::onActiveConnectionAdded(QString const & path)
{
    NetworkManager::ActiveConnection::Ptr conn = NetworkManager::findActiveConnection(path);//XXX: const QString &uni
    if (!conn.isNull()) {
        if (conn->isValid())
            qWarning() << Q_FUNC_INFO << path << " is currently invalid...";

        if (0 > m_activeConns.indexOf(conn)) {
            addActiveConnection(conn);
        } else {
            //TODO: onActiveConnectionUpdate
            qWarning() << Q_FUNC_INFO << "update active connection tp do";
        }

        emit activeConnectionAdd(conn);
    }
}

void KyNetworkResourceManager::onActiveConnectionRemoved(QString const & path)
{
    NetworkManager::ActiveConnection::Ptr conn = findActiveConnection(path);//XXX: const QString &uni
    if (!conn.isNull()) {
        if (conn->isValid())
            qWarning() << Q_FUNC_INFO << path << " is currently invalid...";

        auto index = std::find(m_activeConns.cbegin(), m_activeConns.cend(), conn);
        if (m_activeConns.cend() != index) {
            const int pos = index - m_activeConns.cbegin();
            removeActiveConnection(pos);
        }

        emit activeConnectionRemove(conn.data());
    }
}

void KyNetworkResourceManager::onActiveConnectionsChanged()
{
    emit activeConnectionsReset();
}

void KyNetworkResourceManager::onConnectionAdded(QString const & path)
{
    NetworkManager::Connection::Ptr conn = NetworkManager::findConnection(path);
    if (!conn.isNull())
    {
        if (conn->isValid())
            qWarning() << Q_FUNC_INFO << path << " is currently invalid...";

        if (0 > m_connections.indexOf(conn)) {
            addConnection(conn);
        } else {
            //TODO: onConnectionUpdate
        }

        emit connectionAdd(conn);
    }
}

void KyNetworkResourceManager::onConnectionRemoved(QString const & path)
{
    NetworkManager::Connection::Ptr conn = NetworkManager::findConnection(path);
    if (!conn.isNull()) {
        if (conn->isValid())
            qWarning() << Q_FUNC_INFO << path << " is currently invalid...";

        auto index = std::find(m_connections.cbegin(), m_connections.cend(), conn);
        if (m_connections.cend() != index) {
            const int pos = index - m_connections.cbegin();
            removeConnection(pos);
        }

        emit connectionRemove(conn.data());
    }
}
