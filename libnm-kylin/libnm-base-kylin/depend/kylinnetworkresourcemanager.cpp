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
#include <QMetaType>
#include "kylinnetworkmanagerutil.h"
#include <QThread>
#include <QTimer>

#define SIGNAL_DELAY 80000
#define EMIT_DELAY 10000

#define LOG_FLAG  "[KyNetworkResourceManager]"

//单例部分
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
    //线程 信号槽 类型声明
    qRegisterMetaType<KyConnectState>("KyConnectState");
    qRegisterMetaType<KyConnectivity>("KyConnectivity");
    qRegisterMetaType<KyDeviceType>("KyDeviceType");
}

//start后执行 循环检查m_initFinished 非true阻塞
void KyNetworkResourceManager::onInitNetwork()
{
    insertActiveConnections();
    insertConnections();
    insertDevices();
    insertWifiNetworks();

    //设备
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded,
                                                            this, &KyNetworkResourceManager::onDeviceAdded);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceRemoved,
                                                            this, &KyNetworkResourceManager::onDeviceRemoved);

    //已连接
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionAdded,
                                                            this, &KyNetworkResourceManager::onActiveConnectionAdded);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::activeConnectionRemoved,
                                                            this, &KyNetworkResourceManager::onActiveConnectionRemoved);

    //Connection
    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionAdded,
                                                            this, &KyNetworkResourceManager::onConnectionAdded);
    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionRemoved,
                                                            this, static_cast<void (KyNetworkResourceManager::*)(QString const &)>(&KyNetworkResourceManager::onConnectionRemoved));

    //通用能力
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::connectivityChanged,
                                                            this, &KyNetworkResourceManager::onConnectivityChanged);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::networkingEnabledChanged,
                                                            this, &KyNetworkResourceManager::networkingEnabledChanged);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::wirelessEnabledChanged,
                                                            this, &KyNetworkResourceManager::wifiEnabledChanged);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::primaryConnectionTypeChanged,
                                                            this, &KyNetworkResourceManager::onPrimaryConnectionTypeChanged);

    // Note: the connectionRemoved is never emitted in case network-manager service stop,
    // we need remove the connections manually.
    //NetworkManager服务
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::serviceDisappeared,
                                                            this, &KyNetworkResourceManager::clearConnections);
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::serviceDisappeared,
                                                            this, &KyNetworkResourceManager::clearWifiNetworks);

    QDBusConnection::systemBus().connect(QString("org.freedesktop.DBus"),
                                             QString("/org/freedesktop/DBus"),
                                             QString("org.freedesktop.DBus"),
                                             QString("NameOwnerChanged"), this, SLOT(onServiceAppear(QString,QString,QString)));

    m_initFinished = true;
}

bool KyNetworkResourceManager::NetworkManagerIsInited()
{
    return m_initFinished;
}

KyNetworkResourceManager::~KyNetworkResourceManager()
{

}

//服务重启后 没有add信号 需要重新初始化wifi
void KyNetworkResourceManager::onServiceAppear(QString interface, QString oldOwner, QString newOwner)
{
    if (interface == "org.freedesktop.NetworkManager"
            && oldOwner.isEmpty() && !newOwner.isEmpty()) {
        qDebug() << LOG_FLAG << "org.freedesktop.NetworkManager start";
        QTimer::singleShot(500,this,&KyNetworkResourceManager::insertWifiNetworks);
    }
}

//m_activeConnectList 删除
void KyNetworkResourceManager::removeActiveConnection(int pos)
{
    //active connections signals
    NetworkManager::ActiveConnection::Ptr connectPtr = m_activeConnectList.takeAt(pos);
    connectPtr->disconnect(this);
}

//m_activeConnectList 清空
void KyNetworkResourceManager::clearActiveConnections()
{
    while (0 < m_activeConnectList.size()) {
        removeActiveConnection(0);
    }
}

//m_activeConnectList 增加
void KyNetworkResourceManager::addActiveConnection(NetworkManager::ActiveConnection::Ptr conn)
{
    m_activeConnectList.push_back(conn);

    connect(conn.data(), &NetworkManager::ActiveConnection::stateChanged, this, &KyNetworkResourceManager::onActiveConnectionChanged);

//    if (conn->vpn()) {
//        connect(qobject_cast<NetworkManager::VpnConnection *>(conn.data()), &NetworkManager::VpnConnection::bannerChanged, this, &KyNetworkResourceManager::onActiveConnectionUpdated);
//        connect(qobject_cast<NetworkManager::VpnConnection *>(conn.data()), &NetworkManager::VpnConnection::stateChanged, this, &KyNetworkResourceManager::onVpnActiveConnectChangedReason);
//    }
}

//初始化m_activeConnectList
void KyNetworkResourceManager::insertActiveConnections()
{
    for (auto const & conn : NetworkManager::activeConnections()) {
        if (conn.isNull()) {
            continue;
        }
        addActiveConnection(conn);
    }
}

//m_connectList 删除
void KyNetworkResourceManager::removeConnection(int pos)
{
    NetworkManager::Connection::Ptr conn = m_connectList.takeAt(pos);
    conn->disconnect(this);
}

//m_connectList 清空
void KyNetworkResourceManager::clearConnections()
{
    while (0 < m_connectList.size()) {
        removeConnection(0);
    }
}

//m_connectList 增加
void KyNetworkResourceManager::addConnection(NetworkManager::Connection::Ptr connectPtr)
{
    m_connectList.push_back(connectPtr);

    connect(connectPtr.data(), &NetworkManager::Connection::updated, this, &KyNetworkResourceManager::onConnectionUpdated);
//    connect(connectPtr.data(), &NetworkManager::Connection::removed, this, &KyNetworkResourceManager::onConnectionRemoved);
}

//m_connectList 初始化
void KyNetworkResourceManager::insertConnections()
{
    for (auto const & connectPtr : NetworkManager::listConnections()) {
        if (connectPtr.isNull()) {
            continue;
        }

        if (connectPtr->name().isEmpty() || connectPtr->uuid().isEmpty()) {
            qWarning() << LOG_FLAG << " the name of connection is empty.";
            continue;
        }

        addConnection(connectPtr);
    }
}

//m_deviceList 删除
void KyNetworkResourceManager::removeDevice(int pos)
{
    //connections signals
    NetworkManager::Device::Ptr devicePtr = m_deviceList.takeAt(pos);
    devicePtr->disconnect(this);

    m_deviceMap.remove(devicePtr->uni());
    Q_EMIT deviceRemove(devicePtr->interfaceName());
}

//m_deviceList 清空
void KyNetworkResourceManager::clearDevices()
{
    while (0 < m_deviceList.size())
        removeDevice(0);
}

//m_deviceList 增加
void KyNetworkResourceManager::addDevice(NetworkManager::Device::Ptr device)
{
    m_deviceList.push_back(device);
    m_deviceMap.insert(device->uni(), device->interfaceName());

    connect(device.data(), &NetworkManager::Device::activeConnectionChanged, this, &KyNetworkResourceManager::onDeviceActiveChange);
    connect(device.data(), &NetworkManager::Device::interfaceNameChanged, this, &KyNetworkResourceManager::onDeviceUpdated);

    switch (device->type())
    {
        case NetworkManager::Device::Ethernet:
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::bitRateChanged,
                                                                            this, &KyNetworkResourceManager::onDeviceBitRateChange);
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()), &NetworkManager::WiredDevice::carrierChanged,
                                                                            this, &KyNetworkResourceManager::onDeviceCarrierChange);
            connect(qobject_cast<NetworkManager::WiredDevice *>(device.data()),
                    &NetworkManager::WiredDevice::hardwareAddressChanged, this, &KyNetworkResourceManager::onDeviceMacAddressChange);
            break;

        case NetworkManager::Device::Wifi:
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()),
                            &NetworkManager::WirelessDevice::networkAppeared, this, &KyNetworkResourceManager::onWifiNetworkAppeared);
            connect(qobject_cast<NetworkManager::WirelessDevice *>(device.data()),
                      &NetworkManager::WirelessDevice::networkDisappeared, this, &KyNetworkResourceManager::onWifiNetworkDisappeared);
            break;

        default:
            //TODO: other device types!
            break;
    }
}

//m_deviceList 初始化
void KyNetworkResourceManager::insertDevices()
{
    for (auto const & device : NetworkManager::networkInterfaces()) {
        if (device.isNull()) {
            continue;
        }
        addDevice(device);
    }
}

//m_wifiNetList 删除
void KyNetworkResourceManager::removeWifiNetwork(int pos)
{
    //network signals
    NetworkManager::WirelessNetwork::Ptr net = m_wifiNetList.takeAt(pos);
    net->disconnect(this);
}

//m_wifiNetList 清空
void KyNetworkResourceManager::clearWifiNetworks()
{
    while (0 < m_wifiNetList.size())
        removeWifiNetwork(0);
}

//m_wifiNetList 增加
void KyNetworkResourceManager::addWifiNetwork(NetworkManager::WirelessNetwork::Ptr net)
{
    m_wifiNetList.push_back(net);

    connect(net.data(), &NetworkManager::WirelessNetwork::signalStrengthChanged, this, &KyNetworkResourceManager::onUpdateWirelessNet);
    connect(net.data(), &NetworkManager::WirelessNetwork::referenceAccessPointChanged, this, &KyNetworkResourceManager::onUpdateWirelessNet);
    connect(net.data(), &NetworkManager::WirelessNetwork::disappeared, this, &KyNetworkResourceManager::onUpdateWirelessNet);
}

//m_wifiNetList 初始化
void KyNetworkResourceManager::insertWifiNetworks()
{
    for (auto const & device : m_deviceList) {
        if (device.isNull()) {
            continue;
        }

        if (NetworkManager::Device::Wifi == device->type()) {
            NetworkManager::WirelessDevice::Ptr w_dev = device.objectCast<NetworkManager::WirelessDevice>();
            for (auto const & net : w_dev->networks()) {
                if (!net.isNull()) {
                    addWifiNetwork(net);
                    Q_EMIT wifiNetworkAdded(device->interfaceName(),net->ssid());
                }
            }
        }
    }
}

//get
NetworkManager::ActiveConnection::List KyNetworkResourceManager::getActiveConnectList()
{
    return m_activeConnectList;
}

NetworkManager::Connection::List KyNetworkResourceManager::getConnectList()
{
    return m_connectList;
}

NetworkManager::Device::List KyNetworkResourceManager::getNetworkDeviceList()
{
    return m_deviceList;
}

NetworkManager::WirelessNetwork::List KyNetworkResourceManager::getWifiNetworkList()
{
    return m_wifiNetList;
}

//查找类接口=====================================================================================================
NetworkManager::ActiveConnection::Ptr KyNetworkResourceManager::findActiveConnectionByPath(QString const & path)
{
    NetworkManager::ActiveConnection::Ptr activeConnectPtr = nullptr;

    if (path.isEmpty()) {
        return nullptr;
    }

    if (m_activeConnectList.isEmpty()) {
        return nullptr;
    }

    for (int index = 0; index < m_activeConnectList.size(); ++index) {
        activeConnectPtr = m_activeConnectList.at(index);
        if (activeConnectPtr.isNull()) {
            continue;
        }

        if (activeConnectPtr->path() == path) {
            return activeConnectPtr;
        }
    }

    return nullptr;
}

NetworkManager::Device::Ptr KyNetworkResourceManager::findDeviceByUni(QString const & deviceUni)
{
    NetworkManager::Device::Ptr devicePtr = nullptr;

    if (deviceUni.isEmpty()) {
        return nullptr;
    }

    for (int index = 0; index < m_deviceList.size(); ++index) {
        devicePtr = m_deviceList.at(index);
        if (devicePtr.isNull()) {
            continue;
        }

        if (devicePtr->uni() == deviceUni) {
            return devicePtr;
        }
    }

    return nullptr;
}

NetworkManager::Device::Ptr KyNetworkResourceManager::findDeviceByName(QString const &interfaceName)
{
    NetworkManager::Device::Ptr devicePtr = nullptr;

    if (interfaceName.isEmpty()) {
        return nullptr;
    }

    for (int index = 0; index < m_deviceList.size(); ++index) {
        devicePtr = m_deviceList.at(index);
        if (devicePtr.isNull()) {
            continue;
        }

        if (devicePtr->interfaceName() == interfaceName) {
            return devicePtr;
        }
    }

    return nullptr;
}

NetworkManager::WirelessNetwork::Ptr KyNetworkResourceManager::findWifiNetwork(QString const & ssid, QString const & deviceUni)
{
    NetworkManager::WirelessNetwork::Ptr  wifiNetPtr = nullptr;

    if (ssid.isEmpty() || deviceUni.isEmpty()) {
        return nullptr;
    }

    if (m_wifiNetList.isEmpty()) {
        return nullptr;
    }

    for (int index = 0; index < m_wifiNetList.size(); ++index) {
        wifiNetPtr = m_wifiNetList.at(index);
        if (wifiNetPtr.isNull()) {
            continue;
        }

        if (wifiNetPtr->ssid() == ssid && wifiNetPtr->device() == deviceUni) {
            return wifiNetPtr;
        }
    }

    return nullptr;
}

NetworkManager::ActiveConnection::Ptr KyNetworkResourceManager::findActiveConnectByUuid(const QString activeConnectUuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    qDebug()<<"[KyNetworkResourceManager]"<<"get activetate connect with uuid"<< activeConnectUuid;
    if (activeConnectUuid.isEmpty()) {
        return nullptr;
    }

    for (index = 0; index < m_activeConnectList.size(); ++index) {
        activateConnectPtr = m_activeConnectList.at(index);
        if (activateConnectPtr.isNull()) {
            continue;
        }

        if (activateConnectPtr->uuid() == activeConnectUuid) {
            return activateConnectPtr;
        }
    }

    qWarning()<<"[KyNetworkResourceManager]"<<"it can not find the activate connect with uuid" <<activeConnectUuid;

    return nullptr;
}

NetworkManager::Connection::Ptr KyNetworkResourceManager::findConnectByUuid(const QString connectUuid)
{
    int index = 0;
    NetworkManager::Connection::Ptr connectPtr = nullptr;

    qDebug() <<"[KyNetworkResourceManager]" << "get connect with uuid" << connectUuid;
    if (connectUuid.isEmpty()) {
        return nullptr;
    }

    for (index = 0; index < m_connectList.size(); ++index) {
        connectPtr = m_connectList.at(index);
        if (connectPtr.isNull()) {
            continue;
        }

        if (connectPtr->uuid() == connectUuid) {
            return connectPtr;
        }
    }

    qWarning()<<"[KyNetworkResourceManager]"<<"it can not find connect with uuid"<<connectUuid;

    return nullptr;
}

bool KyNetworkResourceManager::isActiveConnection(QString uuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    if (uuid.isEmpty()) {
        return false;
    }

    for (index = 0; index < m_activeConnectList.size(); ++index) {
        activateConnectPtr = m_activeConnectList.at(index);

        if (activateConnectPtr.isNull()) {
            continue;
        }

        if (activateConnectPtr->uuid() == uuid
             && NetworkManager::ActiveConnection::State::Activated == activateConnectPtr->state()) {
            return true;
        }
    }

    return false;
}

//属性判断类接口=====================================================================
bool KyNetworkResourceManager::isWiredConnect(QString connectPath)
{
    QString connectionType = getConnectTypeByDbus(connectPath);
    if (ETHERNET_TYPE == connectionType) {
        return true;
    }

    return false;
}

bool KyNetworkResourceManager::isActivatingConnection(QString uuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    if (uuid.isEmpty()) {
        return false;
    }

    for (index = 0; index < m_activeConnectList.size(); ++index) {
        activateConnectPtr = m_activeConnectList.at(index);
        if (activateConnectPtr.isNull()) {
            continue;
        }

        if (activateConnectPtr->uuid() == uuid
             && NetworkManager::ActiveConnection::State::Activating == activateConnectPtr->state()) {
            return true;
        }
    }

    return false;
}

bool KyNetworkResourceManager::isApConnection(NetworkManager::Connection::Ptr connectPtr)
{
    if (nullptr == connectPtr) {
        return false;
    }

    if (NetworkManager::ConnectionSettings::ConnectionType::Wireless
            != connectPtr->settings()->connectionType()) {
        return false;
    }

    NetworkManager::ConnectionSettings::Ptr settingPtr = connectPtr->settings();
    NetworkManager::WirelessSetting::Ptr wirelessSetting
        = settingPtr->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    if (NetworkManager::WirelessSetting::NetworkMode::Ap
                                    != wirelessSetting->mode()) {
        return false;
    }

    return true;
}

void KyNetworkResourceManager::getConnectivity(KyConnectivity &connectivity)
{
    connectivity = (KyConnectivity)NetworkManager::connectivity();
}

void KyNetworkResourceManager::requestScan(NetworkManager::WirelessDevice *p_wirelessDevice)
{
    if (nullptr == p_wirelessDevice) {
        qWarning() << LOG_FLAG << "request scan failed, wireless device is invalid.";
        return;
    }

    QDBusPendingReply<> reply = p_wirelessDevice->requestScan();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, p_wirelessDevice);
    connect(watcher, &QDBusPendingCallWatcher::finished, [p_wirelessDevice] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            //TODO: in what form should we output the warning messages
            qWarning() << QStringLiteral("requestScan on device '%1' failed: %3").arg(p_wirelessDevice->interfaceName())
                    .arg(watcher->error().message());
        }
        watcher->deleteLater();
    });

    return;
}

void KyNetworkResourceManager::onConnectionUpdated()
{
    NetworkManager::Connection *connectPtr =
            qobject_cast<NetworkManager::Connection *>(sender());
    if (nullptr != connectPtr && connectPtr->isValid()) {
        NetworkManager::ConnectionSettings::Ptr connectSettingPtr = connectPtr->settings();
        if (NetworkManager::ConnectionSettings::ConnectionType::Wireless == connectSettingPtr->connectionType()) {
            Q_EMIT wirelessConnectionUpdate(connectSettingPtr->interfaceName(), connectPtr->uuid(), connectPtr->name(), connectPtr->path());
        } else if (isWiredConnect(connectPtr->path())) {
            Q_EMIT wiredConnectionUpdate(connectSettingPtr->interfaceName(), connectPtr->uuid(), connectPtr->name(), connectPtr->path());
        } else {
            Q_EMIT connectionUpdate(connectPtr->uuid(), connectPtr->name(), connectPtr->path());
        }
    } else {
        qWarning()<< LOG_FLAG
                  <<"onConnectionUpdate failed, the connect is invalid";
    }
}

void KyNetworkResourceManager::wirelessActiveStateChange(NetworkManager::ConnectionSettings::Ptr connectSettingsPtr,
                                                         QString deviceName,
                                                         QString uuid,
                                                         KyConnectState state)
{
    NetworkManager::WirelessSetting::Ptr wirelessSettingsPtr
        = connectSettingsPtr->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    if (wirelessSettingsPtr.isNull()) {
        qDebug()<< LOG_FLAG << "don't have WirelessSetting connection";
        return;
    }

    QByteArray rawSsid = wirelessSettingsPtr->ssid();
    QString ssid = getSsidFromByteArray(rawSsid);

    Q_EMIT wirelessConnectStateChange(deviceName, ssid, uuid, state);
}

void KyNetworkResourceManager::wiredActiveStateChange(QString connectPath,
                                                       QString deviceName,
                                                       QString uuid,
                                                       KyConnectState state)
{
    QString connectionType = getConnectTypeByDbus(connectPath);
    if (ETHERNET_TYPE == connectionType) {
        Q_EMIT wiredConnectStateChange(deviceName, uuid, state);
    } else {
        qWarning() << LOG_FLAG << "the connect type is not wired" << connectionType;
    }
}

void KyNetworkResourceManager::connectActiveStateChange(NetworkManager::Connection::Ptr connectPtr,
                                                           QString deviceName,
                                                           KyConnectState state)
{
    QString uuid = connectPtr->uuid();
    NetworkManager::ConnectionSettings::Ptr connectSettingsPtr = connectPtr->settings();
    KyConnectionType connectionType = (KyConnectionType)connectSettingsPtr->connectionType();

    if (connectionType == CONNECT_TYPE_WIRELESS) {
        wirelessActiveStateChange(connectSettingsPtr, deviceName, uuid, state);
        NetworkManager::WirelessSetting::Ptr wirelessSetting
            = connectSettingsPtr->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
        if (NetworkManager::WirelessSetting::NetworkMode::Ap
                                        == wirelessSetting->mode()) {
            Q_EMIT wirelessApConnectStateChange();
        }
    } else if (connectionType == CONNECT_TYPE_WIRED) {
        wiredActiveStateChange(connectPtr->path(), deviceName, uuid, state);
    } else {
        qWarning() << LOG_FLAG << "connect type is undefined" << connectionType;
    }
}

void KyNetworkResourceManager::onActiveConnectionChanged(
                                      NetworkManager::ActiveConnection::State state)
{
    NetworkManager::ActiveConnection * activeConnect =
                        qobject_cast<NetworkManager::ActiveConnection *>(sender());
    if (nullptr != activeConnect && activeConnect->isValid()) {
        QString deviceName = "";
        QStringList interfaces = activeConnect->devices();
        if (interfaces.size() > 0) {
            QString ifaceUni = interfaces.at(0);
            NetworkManager::Device:: Ptr devicePtr = findDeviceByUni(ifaceUni);
            deviceName = devicePtr->interfaceName();
        }

        NetworkManager::Connection::Ptr connectPtr = activeConnect->connection();
        connectActiveStateChange(connectPtr, deviceName, (KyConnectState)state);
    } else {
        qWarning() << LOG_FLAG << "onActiveConnectionChanged failed, the connection is invalid.";
    }
}

void KyNetworkResourceManager::onVpnActiveConnectChangedReason(NetworkManager::VpnConnection::State state,
                                      NetworkManager::VpnConnection::StateChangeReason reason)
{
    NetworkManager::ActiveConnection *activeConnect =
        qobject_cast<NetworkManager::ActiveConnection *>(sender());

    if (nullptr != activeConnect && activeConnect->isValid()) {
        qDebug()<<"vpn connect uuid" << activeConnect->uuid()
                <<"state change " <<state <<"reason " << reason;
       // Q_EMIT vpnActiveConnectStateChangeReason(activeConnect->uuid(), state, reason);
    } else {
        qWarning() << LOG_FLAG << "onVpnActiveConnectChangedReason failed, the connection is invalid.";
    }
}

void KyNetworkResourceManager::onDeviceActiveChange()
{
    NetworkManager::Device *p_device = qobject_cast<NetworkManager::Device *>(sender());
    if (nullptr == p_device) {
        return;
    }

    QString deviceName = p_device->interfaceName();
    //此处需要取反，因为激活连接的网卡状态是false，断开连接的网卡状态是true
    bool isActive = !p_device->isActive();

    qDebug()<< LOG_FLAG << "device active change, device name " << deviceName
            << "active state" << isActive;

    Q_EMIT deviceActiveChange(deviceName, isActive);
}

void KyNetworkResourceManager::updateDeviceName(NetworkManager::Device *p_device)
{
    QString newDeviceName = p_device->interfaceName();
    QString deviceUni = p_device->uni();

    if (m_deviceMap.contains(deviceUni)) {
        if (m_deviceMap[deviceUni] != newDeviceName) {
            QString oldDeviceName = m_deviceMap[deviceUni];
            m_deviceMap[deviceUni] = newDeviceName;
            if (DEVICE_TYPE_WIFI == (KyDeviceType)p_device->type()) {
                Q_EMIT wirelessDeviceUpdate(oldDeviceName, newDeviceName);
            } else if (DEVICE_TYPE_ETHERNET == (KyDeviceType)p_device->type()
                       && !p_device->udi().startsWith(VIRTURAL_DEVICE_PATH)) {
                Q_EMIT wiredDeviceUpdate(oldDeviceName, newDeviceName);;
            } else {
                Q_EMIT deviceUpdate(oldDeviceName, newDeviceName);
            }
        }
    }
}

void KyNetworkResourceManager::onDeviceUpdated()
{
    NetworkManager::Device *p_device = qobject_cast<NetworkManager::Device *>(sender());
    if (nullptr == p_device) {
        return;
    }

    updateDeviceName(p_device);
}

void KyNetworkResourceManager::onDeviceCarrierChange(bool pluged)
{
    NetworkManager::WiredDevice * networkDevice
            = qobject_cast<NetworkManager::WiredDevice *>(sender());

    qDebug()<< LOG_FLAG<<"device carrier change"<< pluged;
    if (nullptr !=networkDevice && networkDevice->isValid()) {
        Q_EMIT deviceCarrierChange(networkDevice->interfaceName(), pluged);
    } else {
        qWarning()<< LOG_FLAG<<"onDeviceCarrierChange failed.";
    }
}

void KyNetworkResourceManager::onDeviceBitRateChange(int bitRate)
{
    NetworkManager::WiredDevice * networkDevice
            = qobject_cast<NetworkManager::WiredDevice *>(sender());

    if (nullptr != networkDevice
            && networkDevice->isValid()) {
        Q_EMIT deviceBitRateChange(networkDevice->interfaceName(), bitRate);
    } else {
        qWarning()<< LOG_FLAG <<"the device is not invalid with bitrate" << bitRate;
    }
}

//暂未用到
void KyNetworkResourceManager::onDeviceMacAddressChange(const QString &hwAddress)
{
    NetworkManager::WiredDevice * networkDevice
            = qobject_cast<NetworkManager::WiredDevice *>(sender());

    if (nullptr != networkDevice && networkDevice->isValid()) {
        Q_EMIT deviceMacAddressChange(networkDevice->interfaceName(), hwAddress);
    } else {
        qWarning()<< LOG_FLAG <<"the device is not invalid with mac" << hwAddress;
    }
}

//设备状态变化
void KyNetworkResourceManager::onDeviceStateChanged(
                    NetworkManager::Device::State newstate,
                    NetworkManager::Device::State oldstate,
                    NetworkManager::Device::StateChangeReason reason)
{
    qDebug() << LOG_FLAG <<"the device state "<<oldstate << "to" <<newstate << "reason"<< reason;
    NetworkManager::WiredDevice * networkDevice
            = qobject_cast<NetworkManager::WiredDevice *>(sender());
    Q_EMIT deviceStateChange(networkDevice->interfaceName(), (KyDeviceState)newstate, (KyDeviceState)oldstate, reason);
}


void KyNetworkResourceManager::wifiNetworkAdd(NetworkManager::Device * dev, QString const & ssid)
{
    if (nullptr == dev) {
        return;
    }

    NetworkManager::WirelessDevice * w_dev = qobject_cast<NetworkManager::WirelessDevice *>(dev);
    NetworkManager::WirelessNetwork::Ptr net = w_dev->findNetwork(ssid);
    if (!net.isNull()) {
        if (0 > m_wifiNetList.indexOf(net)){
            addWifiNetwork(net);
        } else {
            //TODO: onWifiNetworkUpdate
            qDebug()<< LOG_FLAG << "add but already exist";
        }

        NetworkManager::AccessPoint::Ptr accessPoitPtr = net->referenceAccessPoint();
        QByteArray rawSsid = accessPoitPtr->rawSsid();
        QString wifiSsid = getSsidFromByteArray(rawSsid);
        Q_EMIT wifiNetworkAdded(dev->interfaceName(), wifiSsid);
    }

    return;
}

void KyNetworkResourceManager::wifiNetworkUpdate(NetworkManager::WirelessNetwork * net)
{
    if (nullptr == net) {
        return;
    }

    auto index = std::find(m_wifiNetList.cbegin(), m_wifiNetList.cend(), net);
    if (m_wifiNetList.cend() != index) {
        if (net->accessPoints().isEmpty()) {
            //emit
            bool bFlag = false;
            QString devIface;
            NetworkManager::Device::Ptr dev = findDeviceByUni(net->device());
            if(dev.isNull()) {
                qDebug()<< LOG_FLAG << "device invalid";
                bFlag = true;
            } else {
                devIface = dev->interfaceName();
            }

            //remove
            auto pos = index - m_wifiNetList.cbegin();
            removeWifiNetwork(pos);
            if(bFlag) {
                Q_EMIT wifiNetworkDeviceDisappear();
            } else {
                NetworkManager::AccessPoint::Ptr accessPoitPtr = net->referenceAccessPoint();
                if (accessPoitPtr.isNull()) {
                    return;
                }
                QByteArray rawSsid = accessPoitPtr->rawSsid();
                QString wifiSsid = getSsidFromByteArray(rawSsid);
                Q_EMIT wifiNetworkRemoved(devIface, wifiSsid);
            }
        } else {
            Q_EMIT wifiNetworkPropertyChange(*index);
        }
    }

    return;
}

void KyNetworkResourceManager::wifiNetworkRemove(NetworkManager::Device * dev, QString const & ssid)
{
    if (nullptr == dev) {
        return;
    }

    NetworkManager::WirelessNetwork::Ptr net = findWifiNetwork(ssid, dev->uni());
    if (!net.isNull()) {
        auto pos = m_wifiNetList.indexOf(net);
        if (0 <= pos) {
            removeWifiNetwork(pos);
            NetworkManager::AccessPoint::Ptr accessPoitPtr = net->referenceAccessPoint();
            QByteArray rawSsid = accessPoitPtr->rawSsid();
            QString wifiSsid = getSsidFromByteArray(rawSsid);
            Q_EMIT wifiNetworkRemoved(dev->interfaceName(), wifiSsid);
        }
    }

    return;
}

void KyNetworkResourceManager::onWifiNetworkAppeared(QString const & ssid)
{
    NetworkManager::Device * p_device = qobject_cast<NetworkManager::Device *>(sender());
    if (nullptr != p_device) {
        wifiNetworkAdd(p_device, ssid);
        updateDeviceName(p_device);
    } else {
        qWarning()<< LOG_FLAG << "onWifiNetworkAppeared failed.";
    }
}

void KyNetworkResourceManager::onWifiNetworkDisappeared(QString const & ssid)
{
    NetworkManager::Device *p_device = qobject_cast<NetworkManager::Device *>(sender());
    if (nullptr != p_device) {
        wifiNetworkRemove(p_device, ssid);
        updateDeviceName(p_device);
    } else {
        qWarning()<< LOG_FLAG << "onWifiNetworkDisappeared failed.";
    }
}

void KyNetworkResourceManager::onUpdateWirelessNet()
{
    NetworkManager::WirelessNetwork *p_wirelessNet =
                    qobject_cast<NetworkManager::WirelessNetwork *>(sender());
    if (nullptr != p_wirelessNet) {
        wifiNetworkUpdate(p_wirelessNet);
    }
}

void KyNetworkResourceManager::onDeviceAdded(QString const & uni)
{
    NetworkManager::Device::Ptr networkDevicePtr = NetworkManager::findNetworkInterface(uni);
    if (networkDevicePtr.isNull()) {
        qWarning()<<"the device is not exist in networkmanager."<< uni;
        return;
    }

    if (!networkDevicePtr->isValid()) {
        qWarning() << uni << " is currently not invalid";
        return;
    }

    if (0 > m_deviceList.indexOf(networkDevicePtr)) {
        addDevice(networkDevicePtr);
        if (DEVICE_TYPE_WIFI == (KyDeviceType)networkDevicePtr->type()) {
            Q_EMIT wirelessDeviceAdd(networkDevicePtr->interfaceName());
        } else if (DEVICE_TYPE_ETHERNET == (KyDeviceType)networkDevicePtr->type()
                   && !networkDevicePtr->udi().startsWith(VIRTURAL_DEVICE_PATH)) {
            Q_EMIT wiredDeviceAdd(networkDevicePtr->interfaceName());
            saveDeviceEnableState(networkDevicePtr->interfaceName(), true);
        } else {
            Q_EMIT deviceAdd(networkDevicePtr->interfaceName(), (KyDeviceType)networkDevicePtr->type());
        }
    } else {
        qWarning() << networkDevicePtr->interfaceName() <<"the device is exist in network device list.";
    }
}

void KyNetworkResourceManager::onDeviceRemoved(QString const & uni)
{
    NetworkManager::Device::Ptr networkDevice = findDeviceByUni(uni);
    if (networkDevice.isNull()) {
        qWarning()<<"the device is not exist in network device list ."<< uni;
        return;
    }

    auto index = std::find(m_deviceList.cbegin(), m_deviceList.cend(), networkDevice);
    if (m_deviceList.cend() != index) {
        const int pos = index - m_deviceList.cbegin();
        removeDevice(pos);
    }
}

void KyNetworkResourceManager::onActiveConnectionAdded(QString const & path)
{
    NetworkManager::ActiveConnection::Ptr activeConnectPtr = NetworkManager::findActiveConnection(path);
    if (activeConnectPtr.isNull()) {
        qWarning() << "[KyNetworkResourceManager]" << "the active connect is null, so do not add it."<<path;
        return;
    }

    if (!activeConnectPtr->isValid()) {
        qWarning() << "[KyNetworkResourceManager]" << path << " connect is currently not valid";
        return;
    }

//    qDebug()<<"add active connect"<<activeConnectPtr->connection()->name();

    if (0 > m_activeConnectList.indexOf(activeConnectPtr)) {
        addActiveConnection(activeConnectPtr);
        Q_EMIT activeConnectionAdd(activeConnectPtr->uuid());
    } else {
        //TODO: onActiveConnectionUpdate
        qWarning() << "[KyNetworkResourceManager]" << "update active connection to do";
        //emit activeConnectionUpdate(conn->uuid());
    }
}

void KyNetworkResourceManager::onActiveConnectionRemoved(QString const & path)
{
    NetworkManager::ActiveConnection::Ptr activeConnectPtr = findActiveConnectionByPath(path);//XXX: const QString &uni
    if (activeConnectPtr.isNull()) {
        qWarning() <<"[KyNetworkResourceManager]"
                  <<"the active connect is null, so do not remove it. "<< path;
        return;
    }

    auto index = std::find(m_activeConnectList.cbegin(), m_activeConnectList.cend(), activeConnectPtr);
    if (m_activeConnectList.cend() != index) {
        const int pos = index - m_activeConnectList.cbegin();
        removeActiveConnection(pos);
        Q_EMIT activeConnectionRemove(activeConnectPtr->uuid());
    }
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

    if (connectPtr->name().isEmpty() || connectPtr->uuid().isEmpty()) {
        qWarning() <<"[KyNetworkResourceManager]" << "the name or uuid of connection is empty";
        return;
    }

    if (0 > m_connectList.indexOf(connectPtr)) {
        addConnection(connectPtr);
        NetworkManager::ConnectionSettings::Ptr connectSettingPtr = connectPtr->settings();
        if (NetworkManager::ConnectionSettings::ConnectionType::Wireless == connectSettingPtr->connectionType()) {
            Q_EMIT wirelessConnectionAdd(connectPtr->uuid(), connectPtr->name(), connectPtr->path());
        } else if (isWiredConnect(connectPtr->path())) {
            Q_EMIT wiredConnectionAdd(connectPtr->settings()->interfaceName(), connectPtr->uuid(), connectPtr->name(), connectPtr->path());
        } else {
            Q_EMIT connectionAdd(connectPtr->uuid(), connectPtr->name(), connectPtr->path());
        }
    } else {
        //TODO::updateconnect
        qWarning() << "[KyNetworkResourceManager]" << connectPtr->uuid() <<" connect is exist in connect list.";
    }
}

void KyNetworkResourceManager::onConnectionRemoved(QString const & path)
{
    if (path.isEmpty()) {
        qDebug() <<"[KyNetworkResourceManager]" <<"the connect path is empty";
        return;
    }
    NetworkManager::Connection::Ptr connectionPtr = nullptr;
    for (int index = 0; index < m_connectList.size(); ++index) {
        connectionPtr = m_connectList.at(index);
        if (connectionPtr->path() == path) {
            removeConnection(index);
            Q_EMIT connectionRemove(path);
            return;
        }
    }

    qWarning() <<"[KyNetworkResourceManager]" << path <<" connect is no exist in connect list";
}

void KyNetworkResourceManager::onConnectivityChanged(NetworkManager::Connectivity connectivity)
{
    qDebug() << "onConnectivityChanged";
    Q_EMIT connectivityChanged((KyConnectivity)connectivity);
}

void KyNetworkResourceManager::onPrimaryConnectionTypeChanged(NetworkManager::ConnectionSettings::ConnectionType type)
{
    Q_EMIT primaryConnectionTypeChanged((KyConnectionType)type);
}

void KyNetworkResourceManager::connectionDump()
{
    NetworkManager::Connection::Ptr connectionPtr = nullptr;
    for (int index = 0; index < m_connectList.size(); ++index) {
        connectionPtr = m_connectList.at(index);
        qDebug()<<"connection info**********************";
        qDebug()<<"connection name"<< connectionPtr->name();
        qDebug()<<"connection uuid"<< connectionPtr->uuid();
        qDebug()<<"connection path"<< connectionPtr->path();
    }
}
