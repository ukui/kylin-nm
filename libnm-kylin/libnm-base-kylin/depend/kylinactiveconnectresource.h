#ifndef KYLINACTIVECONNECTRESOURCE_H
#define KYLINACTIVECONNECTRESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinconnectitem.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinapconnectitem.h"
#include "kylinnetworkresourcetype.h"

class KyActivateItem
{
public:
    QString m_interface;
    QString m_uuid;
    QString m_ssid;
    QString m_connName;
    QString m_dbusPath;
    KyConnectState m_connStatus;
};

class KyActiveConnectResourse : public QObject
{
    Q_OBJECT

public:
    explicit KyActiveConnectResourse(QObject *parent = nullptr);
    ~KyActiveConnectResourse();

public:
    void getActiveConnectionList(QString deviceName, 
                                 KyConnectionType connectionType, QList<KyActivateItem> &connectItemList);
    
    void getActiveConnectIpInfo(const QString &uuid,
                                QString &ipv4Address, QString &ipv6Address);
    void getActiveConnectDnsInfo(const QString &uuid,
                             QList<QHostAddress> &ipv4Dns, QList<QHostAddress> &ipv6Dns);

    bool wiredConnectIsActived();
    bool wirelessConnectIsActived();

    int getAcivateWifiSignal();

private:
    void getActiveConnectIp(NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                            QString &ipv4Address, QString &ipv6Address);
    void getActiveConnectDns(NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                             QList<QHostAddress> &ipv4Dns, QList<QHostAddress> &ipv6Dns);

    KyActivateItem getActiveConnectionItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr);

Q_SIGNALS:
    void wiredStateChange(QString deviceName, QString uuid, KyConnectState state);
    void wirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};
#endif // KYLINACTIVECONNECTRESOURCE_H
