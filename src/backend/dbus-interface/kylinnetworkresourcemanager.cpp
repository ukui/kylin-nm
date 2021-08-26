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

KyNetworkResourceManager::KyNetworkResourceManager(QObject *parent) : QObject(parent)
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

    //todo wifi开关信号
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::wirelessEnabledChanged, this, &KyNetworkResourceManager::wifinEnabledChanged);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::wirelessHardwareEnabledChanged, [=](){

    });

    // Note: the connectionRemoved is never emitted in case network-manager service stop,
    // we need remove the connections manually.
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::serviceDisappeared, this, &KyNetworkResourceManager::clearConnections);
    connect(this, &KyNetworkResourceManager::wifiNetworkAdd, [this] (NetworkManager::Device * dev, QString const & ssid) {
        qDebug() << "wifiNetworkAdd" << dev << dev->interfaceName() << ssid;
        NetworkManager::WirelessDevice * w_dev = qobject_cast<NetworkManager::WirelessDevice *>(dev);
        NetworkManager::WirelessNetwork::Ptr net = w_dev->findNetwork(ssid);
        if (!net.isNull())
        {
            if (0 > m_wifiNets.indexOf(net))
            {
                addWifiNetwork(net);
            } else
            {
                //TODO: onWifiNetworkUpdate
                qDebug() << "add but already exist";
            }
            emit wifiNetworkAdded(dev->interfaceName(), ssid);
        }
    });
    connect(this, &KyNetworkResourceManager::wifiNetworkUpdate, [this] (NetworkManager::WirelessNetwork * net) {

        auto i = std::find(m_wifiNets.cbegin(), m_wifiNets.cend(), net);
        if (m_wifiNets.cend() != i)
        {
            if (net->accessPoints().isEmpty())
            {
                //emit
                bool bFlag = false;
                QString devIface;
                NetworkManager::Device::Ptr dev = findDeviceUni(net->device());
                if(dev.isNull())
                {
                    qDebug() << "device invalid";
                    bFlag = true;
                } else {
                    devIface = dev->interfaceName();
                }
                //remove
                auto pos = i - m_wifiNets.cbegin();
                removeWifiNetwork(pos);
                if(bFlag)
                {
                    //device invalid
                    qDebug() << "wifiNetworkDeviceDisappear";
                    emit wifiNetworkDeviceDisappear();
                } else {
                    qDebug() << "wifiNetwork disappear" << net << net->ssid();
                    emit wifiNetworkRemoved(devIface,net->ssid());
                }
            } else {
                qDebug() << "wifiNetworkPropertyChange " << net << net->ssid();
                emit wifiNetworkPropertyChange(net);
            }
        }
    });
    connect(this, &KyNetworkResourceManager::wifiNetworkRemove, [this] (NetworkManager::Device * dev, QString const & ssid) {
        qDebug() << "wifiNetworkRemove" << dev << dev->interfaceName() << ssid;
        NetworkManager::WirelessNetwork::Ptr net = findWifiNetwork(ssid, dev->uni());
        if (!net.isNull())
        {
            auto pos = m_wifiNets.indexOf(net);
            if (0 <= pos)
            {
                removeWifiNetwork(pos);
                emit wifiNetworkRemoved(dev->interfaceName(), ssid);
            }
        }
    });

    qDebug() <<"[KyNetworkResourceManager]"
            << "active connections:" << m_activeConns.size()
             << "connections:" << m_connections.size()
             << "network device:" << m_devices.size();
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
    if (conn->vpn()) {
        connect(qobject_cast<NetworkManager::VpnConnection *>(conn.data()), &NetworkManager::VpnConnection::bannerChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
        connect(qobject_cast<NetworkManager::VpnConnection *>(conn.data()), &NetworkManager::VpnConnection::stateChanged, this, &KyNetworkResourceManager::onVpnActiveConnectChanagedReason);
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
    while (0 < m_connections.size()) {
        removeConnection(0);
    }
}

void KyNetworkResourceManager::addConnection(NetworkManager::Connection::Ptr conn)
{
    m_connections.push_back(conn);

    //connections signals
    connect(conn.data(), &NetworkManager::Connection::updated, this, &KyNetworkResourceManager::onConnectionUpdated);
    connect(conn.data(), &NetworkManager::Connection::removed, this, &KyNetworkResourceManager::onConnectionRemoved);
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
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::bitRateChanged, this, &KyNetworkResourceManager::onDeviceBitRateChanage);
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::carrierChanged, this, &KyNetworkResourceManager::onDeviceCarrierChanage);
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::hardwareAddressChanged, this, &KyNetworkResourceManager::onDeviceMacAddressChanaged);
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
   int count = 0;
    for (auto const & device : m_devices)
    {
        if (NetworkManager::Device::Wifi == device->type())
        {
            NetworkManager::WirelessDevice::Ptr w_dev = device.objectCast<NetworkManager::WirelessDevice>();
            for (auto const & net : w_dev->networks()) {
                if (!net.isNull()) {
                    addWifiNetwork(net);
                    count++;
                }
            }
        }
    }
    qDebug() << "insertWifiNetworks" << count;
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
    NetworkManager::Device::Ptr devicePtr = nullptr;
    if (ifaceName.isEmpty()) {
        return nullptr;
    }

    for (int index = 0; index < m_devices.size(); ++index) {
        devicePtr = m_devices.at(index);
        if (ifaceName == devicePtr->interfaceName()) {
            return devicePtr;
        }
    }

    return nullptr;
}

NetworkManager::ActiveConnection::Ptr KyNetworkResourceManager::getActiveConnect(const QString activeConnectUuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    qDebug()<<"[KyNetworkResourceManager]"<<"get activetate connect with uuid"<< activeConnectUuid;
    if (activeConnectUuid.isEmpty()) {
        return nullptr;
    }

    for (index = 0; index < m_activeConns.size(); ++index) {
        activateConnectPtr = m_activeConns.at(index);
        if (activateConnectPtr->uuid() == activeConnectUuid) {
            return activateConnectPtr;
        }
    }

    qWarning()<<"[KyNetworkResourceManager]"<<"it can not find the activate connect with uuid" <<activeConnectUuid;

    return nullptr;
}

NetworkManager::Connection::Ptr KyNetworkResourceManager::getConnect(const QString connectUuid)
{
    int index = 0;
    NetworkManager::Connection::Ptr connectPtr = nullptr;

    qDebug()<<"[KyNetworkResourceManager]"<<"get connect with uuid"<<connectUuid;
    if (connectUuid.isEmpty()) {
        return nullptr;
    }

    for (index = 0; index < m_connections.size(); ++index) {
        connectPtr = m_connections.at(index);
        if (connectPtr->uuid() == connectUuid) {
            return connectPtr;
        }
    }

    qWarning()<<"[KyNetworkResourceManager]"<<"it can not find connect with uuid"<<connectUuid;

    return nullptr;
}

NetworkManager::WirelessNetwork::Ptr KyNetworkResourceManager::getWifiNetwork(const QString apName)
{
    return nullptr;
}

NetworkManager::ActiveConnection::List KyNetworkResourceManager::getActiveConnectList()
{
    return m_activeConns;
}

NetworkManager::Connection::List KyNetworkResourceManager::getConnectList()
{
    return m_connections;
}

NetworkManager::Device::List KyNetworkResourceManager::getNetworkDeviceList()
{
    return m_devices;
}

bool KyNetworkResourceManager::isActiveConnection(QString uuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    if (uuid.isEmpty()) {
        return false;
    }

    for (index = 0; index < m_activeConns.size(); ++index) {
        activateConnectPtr = m_activeConns.at(index);
        if (activateConnectPtr->uuid() == uuid
             && NetworkManager::ActiveConnection::State::Activated == activateConnectPtr->state()) {
            return true;
        }
    }

    return false;
}

void KyNetworkResourceManager::requestScan(NetworkManager::WirelessDevice * dev)
{
    qDebug() <<"[KyNetworkResourceManager]"<< dev->interfaceName()<<"start scan wifi ap";
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
    NetworkManager::Connection *connectPtr =
            qobject_cast<NetworkManager::Connection *>(sender());

    qDebug()<<"[KyNetworkResourceManager]"<<"onConnectionUpdate "<<connectPtr->name()<<connectPtr->uuid();

    if (connectPtr->isValid()) {
        emit connectionUpdate(connectPtr->uuid());
    } else {
        qWarning()<< "[KyNetworkResourceManager]"
                  <<"connect update failed, the connect is invalid"
                  <<connectPtr->name()
                  <<connectPtr->uuid()
                  <<connectPtr->path();
    }

    return;
}

void KyNetworkResourceManager::onActiveConnectionUpdated()
{
    //emit activeConnectionUpdate(qobject_cast<NetworkManager::ActiveConnection *>(sender()));
}

void KyNetworkResourceManager::onActiveConnectionChangedReason(
                                      NetworkManager::ActiveConnection::State state,
                                      NetworkManager::ActiveConnection::Reason reason)
{
    NetworkManager::ActiveConnection * activeConnect =
        qobject_cast<NetworkManager::ActiveConnection *>(sender());
    if (activeConnect->isValid()) {
        qDebug()<<"state change activate connect"<<activeConnect->uuid();
        qDebug()<<"the active connect state"<<state;
        qDebug()<<"the active connect state chanager reason:"<<reason;
        emit activeConnectStateChangeReason(activeConnect->uuid(), state, reason);
    } else {

    }

    return;
}

void KyNetworkResourceManager::onVpnActiveConnectChanagedReason(NetworkManager::VpnConnection::State state,
                                      NetworkManager::VpnConnection::StateChangeReason reason)
{
    NetworkManager::ActiveConnection *activeConnect =
        qobject_cast<NetworkManager::ActiveConnection *>(sender());

    if (activeConnect->isValid()) {
        qDebug()<<"state change activate connect"<<activeConnect->uuid();
        qDebug()<<"the active connect state"<<state;
        qDebug()<<"the active connect state chanager reason:"<<reason;
        emit vpnActiveConnectStateChangeReason(activeConnect->uuid(), state, reason);
    } else {

    }

    return;
}

void KyNetworkResourceManager::onDeviceUpdated()
{
    emit deviceUpdate(qobject_cast<NetworkManager::Device *>(sender()));
}

void KyNetworkResourceManager::onDeviceCarrierChanage(bool pluged)
{
    NetworkManager::WiredDevice * networkDevice
            = qobject_cast<NetworkManager::WiredDevice *>(sender());

    qDebug()<<"device carrier chanage"<< pluged;
    if (networkDevice->isValid()) {
        emit deviceCarrierChanage(networkDevice->interfaceName(), pluged);
    } else {
        qWarning()<<"the device is not invalid";
    }

    return;
}

void KyNetworkResourceManager::onDeviceBitRateChanage(int bitRate)
{
    NetworkManager::WiredDevice * networkDevice
            = qobject_cast<NetworkManager::WiredDevice *>(sender());

    if (networkDevice->isValid()) {
        emit deviceBitRateChanage(networkDevice->interfaceName(), bitRate);
    } else {
        qWarning()<<"the device is not invalid";
    }

    return;
}

void KyNetworkResourceManager::onDeviceMacAddressChanaged(const QString &hwAddress)
{
    NetworkManager::WiredDevice * networkDevice
            = qobject_cast<NetworkManager::WiredDevice *>(sender());

    if (networkDevice->isValid()) {
        emit deviceMacAddressChanaged(networkDevice->interfaceName(), hwAddress);
    } else {
        qWarning()<<"the device is not invalid";
    }

    return;
}

void KyNetworkResourceManager::onDeviceStateChanged(
                    NetworkManager::Device::State newstate,
                    NetworkManager::Device::State oldstate,
                    NetworkManager::Device::StateChangeReason reason)
{
    qWarning()<<"the device state "<<oldstate << "to" <<newstate << "reason"<< reason;
    return;
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
    qDebug()<< "onDeviceAdded"<<uni;

    NetworkManager::Device::Ptr networkDevice = NetworkManager::findNetworkInterface(uni);
    if (networkDevice.isNull()) {
        qWarning()<<"the device is not exist in networkmanager."<< uni;
        return;
    }

    if (!networkDevice->isValid()) {
        qWarning() << uni << " is currently not invalid";
        return;
    }

    if (0 > m_devices.indexOf(networkDevice)) {
        addDevice(networkDevice);
        emit deviceAdd(networkDevice->interfaceName(), networkDevice->uni(), networkDevice->type());
    } else {
        qWarning() << networkDevice->interfaceName() <<"the device is exist in network device list.";
    }

    return;
}

void KyNetworkResourceManager::onDeviceRemoved(QString const & uni)
{
    NetworkManager::Device::Ptr networkDevice = findDeviceUni(uni);
    if (networkDevice.isNull()) {
        qWarning()<<"the device is not exist in network device list ."<< uni;
        return;
    }

    qDebug()<<"remove network device"<< uni;

    auto index = std::find(m_devices.cbegin(), m_devices.cend(), networkDevice);
    if (m_devices.cend() != index) {
        const int pos = index - m_devices.cbegin();
        removeDevice(pos);
        emit deviceRemove(networkDevice->interfaceName(), networkDevice->uni());
    }

    return;
}

void KyNetworkResourceManager::onActiveConnectionAdded(QString const & path)
{
    NetworkManager::ActiveConnection::Ptr activeConnectPtr = NetworkManager::findActiveConnection(path);//XXX: const QString &uni
    if (activeConnectPtr.isNull()) {
        qWarning() << "[KyNetworkResourceManager]" << "the active connect is null, so do not add it."<<path;
        return;
    }

    if (!activeConnectPtr->isValid()) {
        qWarning() << "[KyNetworkResourceManager]" << path << " connect is currently not valid";
        return;
    }

    qDebug()<<"add active connect"<<activeConnectPtr->connection()->name();

    if (0 > m_activeConns.indexOf(activeConnectPtr)) {
        addActiveConnection(activeConnectPtr);
        emit activeConnectionAdd(activeConnectPtr->uuid());
    } else {
        //TODO: onActiveConnectionUpdate
        qWarning() << "[KyNetworkResourceManager]" << "update active connection to do";
        //emit activeConnectionUpdate(conn->uuid());
    }

    return;
}

void KyNetworkResourceManager::onActiveConnectionRemoved(QString const & path)
{
    NetworkManager::ActiveConnection::Ptr activeConnectPtr = findActiveConnection(path);//XXX: const QString &uni
    if (activeConnectPtr.isNull()) {
        qWarning() <<"[KyNetworkResourceManager]"
                  <<"the active connect is null, so do not remove it. "<< path;
        return;
    }

    qDebug() <<"[KyNetworkResourceManager]"<<"remove active connect"<<activeConnectPtr->uuid();

    auto index = std::find(m_activeConns.cbegin(), m_activeConns.cend(), activeConnectPtr);
    if (m_activeConns.cend() != index) {
        const int pos = index - m_activeConns.cbegin();
        removeActiveConnection(pos);
        emit activeConnectionRemove(activeConnectPtr->uuid());
    }

    return;
}

void KyNetworkResourceManager::onConnectionAdded(QString const & path)
{
    NetworkManager::Connection::Ptr connectPtr = NetworkManager::findConnection(path);
    if (connectPtr.isNull()) {
        qWarning() <<"[KyNetworkResourceManager]" <<"it can not find in networkmanager."<<path;
        return;
    }

    if (!connectPtr->isValid()) {
        qWarning() <<"[KyNetworkResourceManager]" << path << " is currently not invalid";
        return;
    }

    qDebug() <<"[KyNetworkResourceManager]" <<"add connect "<< connectPtr->name() << connectPtr->path();

    if (0 > m_connections.indexOf(connectPtr)) {
        addConnection(connectPtr);
        emit connectionAdd(connectPtr->uuid());
    } else {
        //TODO::updateconnect
        qWarning() << "[KyNetworkResourceManager]" << connectPtr->uuid() <<" connect is exist in connect list.";
    }

    return;
}

void KyNetworkResourceManager::onConnectionRemoved(QString const & path)
{
    if (path.isEmpty()) {
        qDebug() <<"[KyNetworkResourceManager]" <<"the connect path is empty";
        return;
    }

    qDebug() <<"[KyNetworkResourceManager]" << "remove connection path" << path;
    NetworkManager::Connection::Ptr connectionPtr = nullptr;
    for (int index = 0; index < m_connections.size(); ++index) {
        connectionPtr = m_connections.at(index);
        if (connectionPtr->path() == path) {
            removeConnection(index);
            emit connectionRemove(path);
            return;
        }
    }

    qWarning() <<"[KyNetworkResourceManager]" << path <<" connect is no exist in connect list";

    return;
}

void KyNetworkResourceManager::removeConnection(QString const & uuid)
{
    NetworkManager::Connection::Ptr conn = this->getConnect(uuid);
    if(!conn.isNull())
    {
        conn->disconnect(this);
        conn->remove();
    }
}


void KyNetworkResourceManager::connectionDump()
{
    NetworkManager::Connection::Ptr connectionPtr = nullptr;
    for (int index = 0; index < m_connections.size(); ++index) {
        connectionPtr = m_connections.at(index);
        qDebug()<<"connection info**********************";
        qDebug()<<"connection name"<< connectionPtr->name();
        qDebug()<<"connection uuid"<< connectionPtr->uuid();
        qDebug()<<"connection path"<< connectionPtr->path();
    }
}
