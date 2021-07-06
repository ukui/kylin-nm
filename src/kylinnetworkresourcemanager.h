#ifndef KYLINNETWORKRESOURCEMANAGER_H
#define KYLINNETWORKRESOURCEMANAGER_H

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/GenericTypes>
#include <NetworkManagerQt/VpnConnection>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/AdslDevice>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/WimaxDevice>
#include <NetworkManagerQt/VlanDevice>
#include <NetworkManagerQt/BondDevice>
#include <NetworkManagerQt/BridgeDevice>
#include <NetworkManagerQt/GenericDevice>
#include <NetworkManagerQt/InfinibandDevice>
#include <NetworkManagerQt/BluetoothDevice>
#include <NetworkManagerQt/OlpcMeshDevice>
#include <NetworkManagerQt/TeamDevice>
#include <NetworkManagerQt/WirelessSetting>
#include <NetworkManagerQt/WirelessSecuritySetting>
#include <NetworkManagerQt/Utils>
#include <NetworkManagerQt/ConnectionSettings>
#include <QDBusPendingCallWatcher>
#include <QInputDialog>
#include <QMetaEnum>

class KyNetworkResourceManager : public QObject
{
    Q_OBJECT
public:
    static KyNetworkResourceManager* getInstance();
    static void Release();

public:
    explicit KyNetworkResourceManager();
    ~KyNetworkResourceManager();

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

    void removeWifiNetwork(int pos);
    void clearWifiNetworks();
    void insertWifiNetworks();
    void addWifiNetwork(NetworkManager::WirelessNetwork::Ptr net);

    NetworkManager::ActiveConnection::Ptr findActiveConnection(QString const & path);
    template <typename Predicate>
    NetworkManager::Device::Ptr findDevice(Predicate const & pred);
    NetworkManager::Device::Ptr findDeviceUni(QString const & uni);
    NetworkManager::Device::Ptr findDeviceInterface(QString const & interfaceName);
    NetworkManager::WirelessNetwork::Ptr findWifiNetwork(QString const & ssid, QString const & devUni);

    void requestScan(NetworkManager::WirelessDevice * dev);

public:
    NetworkManager::Device::Ptr getNetworkDevice(const QString ifaceName);
    NetworkManager::ActiveConnection::Ptr getActiveConnect(const QString uuid);
    NetworkManager::Connection::Ptr getConnect(const QString uuid);
    NetworkManager::WirelessNetwork::Ptr getWifiNetwork(const QString apName);

signals:
    void connectionAdd(NetworkManager::Connection::Ptr conn);
    void connectionUpdate(NetworkManager::Connection * conn);
    void connectionRemove(NetworkManager::Connection * conn);
    void activeConnectionAdd(NetworkManager::ActiveConnection::Ptr conn);
    void activeConnectionUpdate(NetworkManager::ActiveConnection * conn);
    void activeConnectionRemove(NetworkManager::ActiveConnection * conn);
    void activeConnectionsReset();
    void deviceAdd(NetworkManager::Device::Ptr dev);
    void deviceUpdate(NetworkManager::Device * dev);
    void deviceRemove(NetworkManager::Device * dev);
    void wifiNetworkAdd(NetworkManager::Device * dev, QString const & ssid);
    void wifiNetworkUpdate(NetworkManager::WirelessNetwork * net);
    void wifiNetworkRemove(NetworkManager::Device * dev, QString const & ssid);


private slots:
    //connection
    void onConnectionUpdated();
    void onConnectionRemoved();

    //active connection
    void onActiveConnectionUpdated();

    //device
    void onDeviceUpdated();
    void onWifiNetworkAppeared(QString const & ssid);
    void onWifiNetworkDisappeared(QString const & ssid);

    //wifi network
    void onWifiNetworkUpdated();

    //notifier
    void onDeviceAdded(QString const & uni);
    void onDeviceRemoved(QString const & uni);
    void onActiveConnectionAdded(QString const & path);
    void onActiveConnectionRemoved(QString const & path);
    void onActiveConnectionsChanged();

    //settings notifier
    void onConnectionAdded(QString const & path);
    void onConnectionRemoved(QString const & path);

public:
    static KyNetworkResourceManager* m_pInstance;

    NetworkManager::ActiveConnection::List m_activeConns;
    NetworkManager::Connection::List m_connections;
    NetworkManager::Device::List m_devices;
    NetworkManager::WirelessNetwork::List m_wifiNets;

};


#endif // KYLINNETWORKRESOURCEMANAGER_H
