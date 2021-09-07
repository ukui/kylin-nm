#ifndef KYLINCONNECTRESOURCE_H
#define KYLINCONNECTRESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinconnectitem.h"
#include "kylinvpnconnectitem.h"
#include "kylinbluetoothconnectitem.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinconnectsetting.h"
#include "kylinapconnectitem.h"

class KyConnectResourse : public QObject
{
    Q_OBJECT
public:
    explicit KyConnectResourse(QObject *parent = nullptr);
    ~KyConnectResourse();

public:
    KyConnectItem *getConnectionItemByUuid(QString connectUuid, QString deviceName);
    void getConnectionList(QString deviceName,
                           NetworkManager::ConnectionSettings::ConnectionType connectionType,
                           QList<KyConnectItem *> &connectItemList);
 //   void getWiredConnections(QList<KyWiredConnectItem *> &wiredConnectItemList);
    void getVpnConnections(QList<KyVpnConnectItem *> &vpnConnectItemList);
    void getBluetoothConnections(QList<KyBluetoothConnectItem *> &bluetoothConnectItemList);
    void getApConnections(QList<KyApConnectItem *> &apConnectItemList);
    void getConnectionSetting(QString connectUuid, KyConnectSetting &connectSetting);
    bool getInterfaceByUuid(QString &deviceName, NetworkManager::ConnectionSettings::ConnectionType &type, const QString connUuid);

private:
    KyConnectItem *getConnectionItem(NetworkManager::Connection::Ptr connectPtr);
    void getConnectIp(NetworkManager::ConnectionSettings::Ptr settingPtr,
                      QString &ipv4Address,
                      QString &ipv6Address);
//    KyWiredConnectItem *getWiredConnectItem(NetworkManager::Connection::Ptr connectPtr);

    void getVpnConnectData(NetworkManager::ConnectionSettings::Ptr settingPtr,
                           KyVpnConnectItem *vpnItem);
    KyVpnConnectItem *getVpnConnectItem(NetworkManager::Connection::Ptr connectPtr);
    KyBluetoothConnectItem *getBluetoothConnectItem(NetworkManager::Connection::Ptr connectPtr);
    KyApConnectItem *getApConnectItem(NetworkManager::Connection::Ptr connectPtr);

    void getIpv4ConnectSetting(NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                            KyConnectSetting &connectSetting);
    void getIpv6ConnectSetting(NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                            KyConnectSetting &connectSetting);

signals:
    void connectionAdd(QString uuid);
    void connectionUpdate(QString uuid);
    void connectionRemove(QString path);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    KyNetworkDeviceResourse  *m_networkdevice = nullptr;
};
#endif // KYLINCONNECTRESOURCE_H
