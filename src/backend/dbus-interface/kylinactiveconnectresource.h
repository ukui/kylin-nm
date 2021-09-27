#ifndef KYLINACTIVECONNECTRESOURCE_H
#define KYLINACTIVECONNECTRESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinconnectitem.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinvpnconnectitem.h"
#include "kylinbluetoothconnectitem.h"
#include "kylinapconnectitem.h"

class KyActiveConnectResourse : public QObject
{
    Q_OBJECT
public:
    explicit KyActiveConnectResourse(QObject *parent = nullptr);
    ~KyActiveConnectResourse();

public:
    KyConnectItem *getActiveConnectionByUuid(QString connectUuid);
    KyConnectItem *getActiveConnectionByUuid(QString connectUuid, QString deviceName);
    void getActiveConnectionList(QString deviceName,
                                 NetworkManager::ConnectionSettings::ConnectionType connectionType,
                                 QList<KyConnectItem *> &connectItemList);
    void getActiveConnectIpInfo(const QString &uuid,
                            QString &ipv4Address,
                            QString &ipv6Address);
    void getActiveConnectDnsInfo(const QString &uuid,
                             QList<QHostAddress> &ipv4Dns,
                             QList<QHostAddress> &ipv6Dns);

    //void getWiredActivateConnect(QList<KyWiredConnectItem *> &wiredActiveConnectItemList);
    void getVpnActivateConnect(QList<KyVpnConnectItem *> &vpnActiveConnectItemList);
    void getBtActivateConnect(QList<KyBluetoothConnectItem *> &btActiveConnectItemList);

    void getApActivateConnect(QList<KyApConnectItem *> &apConnectItemList);

    bool isActiveConnection(QString uuid, QStringList &devName);

private:
    void getActiveConnectIp(NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                            QString &ipv4Address,
                            QString &ipv6Address);
    void getActiveConnectDns(NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                             QList<QHostAddress> &ipv4Dns,
                             QList<QHostAddress> &ipv6Dns);

    KyConnectItem *getActiveConnectionItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);

   // KyWiredConnectItem *getWiredActiveConnectItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);
    KyVpnConnectItem *getVpnActiveConnectItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);
    KyBluetoothConnectItem *getBtActiveConnectItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);
    KyApConnectItem *getApActiveConnectItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);

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
