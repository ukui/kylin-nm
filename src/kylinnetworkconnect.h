#ifndef KYLINNETWORKCONNECT_H
#define KYLINNETWORKCONNECT_H

#include "kylinnetworkresourcemanager.h"

class KyNetworkConnect : public QObject
{
    Q_OBJECT
public:
    KyNetworkConnect();
    ~KyNetworkConnect();

public:
    int  addAndActivateConnect(const QString );
    int  activateConnection(const QString connectUuid);
    int  deactivateConnection(const QString connectName, const QString &uuid);
    void requestScan(const QString ifaceName);
    void requestAllWifiScan();

signals:
    void activateConnectionFinished(NetworkManager::Connection::Ptr conn);
    void deactivateConnectionFinished(NetworkManager::Connection::Ptr conn);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};

#endif // KYLINNETWORKCONNECT_H
