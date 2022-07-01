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

#ifndef KYLINNETWORKRESOURCEMANAGER_H
#define KYLINNETWORKRESOURCEMANAGER_H

#include "NetworkManagerQt/Manager"
#include "NetworkManagerQt/Settings"
#include "NetworkManagerQt/WirelessDevice"
#include "NetworkManagerQt/GenericTypes"
#include "NetworkManagerQt/VpnConnection"
#include "NetworkManagerQt/WirelessDevice"
#include "NetworkManagerQt/AdslDevice"
#include "NetworkManagerQt/WiredDevice"
#include "NetworkManagerQt/WimaxDevice"
#include "NetworkManagerQt/VlanDevice"
#include "NetworkManagerQt/BondDevice"
#include "NetworkManagerQt/BridgeDevice"
#include "NetworkManagerQt/GenericDevice"
#include "NetworkManagerQt/InfinibandDevice"
#include "NetworkManagerQt/BluetoothDevice"
#include "NetworkManagerQt/OlpcMeshDevice"
#include "NetworkManagerQt/TeamDevice"
#include "NetworkManagerQt/WirelessSetting"
#include "NetworkManagerQt/WirelessSecuritySetting"
#include "NetworkManagerQt/Utils"
#include "NetworkManagerQt/ConnectionSettings"
#include "NetworkManagerQt/Security8021xSetting"
#include <QDBusPendingCallWatcher>
#include <QMetaEnum>

#include "kylinnetworkresourcetype.h"

#define VIRTURAL_DEVICE_PATH "/sys/devices/virtual/net"

class KyNetworkResourceManager : public QObject
{
    Q_OBJECT
public:
    static KyNetworkResourceManager* getInstance();
    static void Release();

public:
    explicit KyNetworkResourceManager(QObject *parent = nullptr);
    ~KyNetworkResourceManager();

public:
    NetworkManager::ActiveConnection::Ptr findActiveConnectionByPath(QString const & path);
    NetworkManager::ActiveConnection::Ptr findActiveConnectByUuid(const QString activeConnectUuid);

    NetworkManager::Connection::Ptr findConnectByUuid(const QString uuid);

    NetworkManager::Device::Ptr findDeviceByUni(QString const &deviceUni);
    NetworkManager::Device::Ptr findDeviceByName(QString const &interfaceName);

    NetworkManager::WirelessNetwork::Ptr findWifiNetwork(QString const &ssid, QString const &deviceUni);

    NetworkManager::ActiveConnection::List getActiveConnectList();
    NetworkManager::Connection::List getConnectList();
    NetworkManager::Device::List getNetworkDeviceList();
    NetworkManager::WirelessNetwork::List getWifiNetworkList();

    bool isActiveConnection(QString uuid);
    bool isActivatingConnection(QString uuid);

    void getConnectivity(KyConnectivity &connectivity);

    bool NetworkManagerIsInited();

    void requestScan(NetworkManager::WirelessDevice *p_wirelessDevice);

    bool isWiredConnect(QString connectPath);
    bool isApConnection(NetworkManager::Connection::Ptr connectPtr);

private:
    void removeActiveConnection(int pos);
    void clearActiveConnections();
    void insertActiveConnections();
    void addActiveConnection(NetworkManager::ActiveConnection::Ptr conn);

    void removeConnection(int pos);
    void clearConnections();
    void insertConnections();
    void addConnection(NetworkManager::Connection::Ptr conn);

    void removeDevice(int pos);
    void clearDevices();
    void insertDevices();
    void addDevice(NetworkManager::Device::Ptr conn);
    void updateDeviceName(NetworkManager::Device *p_device);

    void removeWifiNetwork(int pos);
    void clearWifiNetworks();
    void insertWifiNetworks();
    void addWifiNetwork(NetworkManager::WirelessNetwork::Ptr net);

    void connectionDump();

    void wifiNetworkAdd(NetworkManager::Device * dev, QString const & ssid);
    void wifiNetworkUpdate(NetworkManager::WirelessNetwork * net);
    void wifiNetworkRemove(NetworkManager::Device * dev, QString const & ssid);


    void wirelessActiveStateChange(NetworkManager::ConnectionSettings::Ptr connectSettingsPtr,
                                            QString deviceName, QString uuid, KyConnectState state);
    void wiredActiveStateChange(QString connectPath, QString deviceName,
                                            QString uuid, KyConnectState state);
    void connectActiveStateChange(NetworkManager::Connection::Ptr connectPtr,
                                            QString deviceName, KyConnectState state);

private Q_SLOTS:
    void onServiceAppear(QString, QString, QString);

Q_SIGNALS:

    // wired connect
    void wiredConnectionAdd(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void wiredConnectionUpdate(QString devName, QString connectUuid, QString connectName, QString connectPath);
//    void wiredConnectionRemove(QString path);

    void connectionAdd(QString connectUuid, QString connectName, QString connectPath);
    void connectionUpdate(QString connectUuid, QString connectName, QString connectPath);
    void connectionRemove(QString path);

    //device
    void wiredDeviceAdd(QString deviceName);
    void wiredDeviceUpdate(QString oldDeviceName, QString newDeviceName);

    void wirelessDeviceAdd(QString deviceName);
    void wirelessDeviceUpdate(QString oldDeviceName, QString newDeviceName);

    void deviceAdd(QString deviceName, KyDeviceType deviceType);
    void deviceUpdate(QString oldDeviceName, QString newDeviceName);
    void deviceRemove(QString deviceName);

    void deviceActiveChange(QString deviceName, bool deviceActive);
    void deviceCarrierChange(QString deviceName, bool pluged);
    void deviceBitRateChange(QString deviceName, int bitRate);
    void deviceMacAddressChange(QString deviceName, const QString &hwAddress);
    void deviceStateChange(QString deviceName, KyDeviceState newstate, KyDeviceState oldstate,
                                NetworkManager::Device::StateChangeReason reason);

    //wireless
    void wifiNetworkRemoved(QString, QString);
    void wifiNetworkAdded(QString, QString);
    void wifiNetworkPropertyChange(NetworkManager::WirelessNetwork::Ptr net);
    void wifiNetworkDeviceDisappear();

    void wirelessConnectionAdd(QString connectUuid, QString connectName, QString connectPath);
    void wirelessConnectionUpdate(QString devName,QString connectUuid, QString connectName, QString connectPath);
//    void wirelessConnectionRemove(QString path);

//    void wiredConnectionUpdate(QString connectUuid, QString connectName, QString connectPath);


    //activate connection
    void activeConnectionAdd(QString uuid);
    void activeConnectionRemove(QString uuid);

    void wiredConnectStateChange(QString deviceName, QString uuid, KyConnectState state);
    void wirelessConnectStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state);
    void wirelessApConnectStateChange();


    //general
    void connectivityChanged(KyConnectivity connectivity);
    void wifiEnabledChanged(bool);
    void networkingEnabledChanged(bool);
    void primaryConnectionTypeChanged(KyConnectionType type);

public Q_SLOTS:
    void onInitNetwork();

    //connection
    void onConnectionUpdated();
    //void onConnectionRemoved();

    //active connection
    void onActiveConnectionChanged(NetworkManager::ActiveConnection::State state);
    void onVpnActiveConnectChangedReason(NetworkManager::VpnConnection::State state,
                                          NetworkManager::VpnConnection::StateChangeReason reason);

    void onDeviceActiveChange();
    void onDeviceCarrierChange(bool pluged);
    void onDeviceBitRateChange(int bitRate);
    void onDeviceMacAddressChange(const QString &hwAddress);
    void onDeviceStateChanged(NetworkManager::Device::State newstate,
                              NetworkManager::Device::State oldstate, NetworkManager::Device::StateChangeReason reason);

    void onWifiNetworkAppeared(QString const & ssid);
    void onWifiNetworkDisappeared(QString const & ssid);

    //wifi network
    void onUpdateWirelessNet();
    void onConnectivityChanged(NetworkManager::Connectivity connectivity);

    //notifier
    void onDeviceAdded(QString const & uni);
    void onDeviceRemoved(QString const & uni);
    void onDeviceUpdated();

    //active connection notifier
    void onActiveConnectionAdded(QString const & path);
    void onActiveConnectionRemoved(QString const & path);

    //settings notifier
    void onConnectionAdded(QString const & path);
    void onConnectionRemoved(QString const & path);

    void onPrimaryConnectionTypeChanged(NetworkManager::ConnectionSettings::ConnectionType);

public:
    static KyNetworkResourceManager* m_pInstance;

private:
    bool m_initFinished = false;
    NetworkManager::ActiveConnection::List m_activeConnectList;
    NetworkManager::Connection::List m_connectList;
    NetworkManager::Device::List m_deviceList;
    NetworkManager::WirelessNetwork::List m_wifiNetList;

    QMap<QString, QString> m_deviceMap;
};


#endif // KYLINNETWORKRESOURCEMANAGER_H
