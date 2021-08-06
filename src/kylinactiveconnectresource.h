#ifndef KYLINACTIVECONNECTRESOURCE_H
#define KYLINACTIVECONNECTRESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinconnectitem.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinvpnconnectitem.h"
#include "kylinbluetoothconnectitem.h"

class KyActiveConnectResourse : public QObject
{
    Q_OBJECT
public:
    KyActiveConnectResourse();
    ~KyActiveConnectResourse();

public:
    void getActiveConnectionList(QString DeviceName,
                                 NetworkManager::ConnectionSettings::ConnectionType connectionType,
                                 QList<KyConnectItem *> &connectItemList);
    //void getWiredActivateConnect(QList<KyWiredConnectItem *> &wiredActiveConnectItemList);
    void getVpnActivateConnect(QList<KyVpnConnectItem *> &vpnActiveConnectItemList);
    void getBtActivateConnect(QList<KyBluetoothConnectItem *> &btActiveConnectItemList);

private:
    KyConnectItem *getActiveConnectionItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);
    void getActiveConnectIp(NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                            QString &ipv4Address,
                            QString &ipv6Address);
    void getActiveConnectDns(NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                             QList<QHostAddress> &ipv4Dns,
                             QList<QHostAddress> &ipv6Dns);
   // KyWiredConnectItem *getWiredActiveConnectItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);
    KyVpnConnectItem *getVpnActiveConnectItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);
    KyBluetoothConnectItem *getBtActiveConnectItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);

signals:
    void activeConnectAdd(QString activeConnectUuid);
    void updateActiveConnect(QString activeConnectUuid);
    void activeConnectRemove(QString activeConnectUuid);
    void stateChangeReason(QString uuid,
                                    NetworkManager::ActiveConnection::State state,
                                    NetworkManager::ActiveConnection::Reason reason);
    void vpnConnectChangeReason(QString uuid,
                                NetworkManager::VpnConnection::State state,
                                NetworkManager::VpnConnection::StateChangeReason reason);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    KyNetworkDeviceResourse *m_networkdevice = nullptr;
};
#endif // KYLINACTIVECONNECTRESOURCE_H
