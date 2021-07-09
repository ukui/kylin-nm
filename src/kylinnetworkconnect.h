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
    void activateWirelessConnection(NetworkManager::WirelessNetwork::Ptr wirelessNet);
    void addAndActivateWirelessConnection(NetworkManager::WirelessNetwork::Ptr wirelessNet, const QString &psk);
    int  deactivateConnection(const QString connectName, const QString &uuid);
    void requestScan(const QString ifaceName);
    void requestAllWifiScan();

signals:
    void activateConnectionFinished(NetworkManager::Connection::Ptr conn);
    void deactivateConnectionFinished(NetworkManager::Connection::Ptr conn);

    void noConnection();
    void notSavedConnection();

signals:
    void checkActiveonnection(const QString &activeConn);
    void connResult(int);
    void starWaiting();

public slots:
    void onActivateWirelessConnection(const QString &connectSsid, const QString &connectUuid);
    void onActivateWirelessConnectionWithPWD(const QString &connectSsid, const QString &psk, bool isNotSaved, const QString &connectUuid);
    void onCheckActiveonnection(const QString &activeConn);
    void onStateChangedReason(NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};

#endif // KYLINNETWORKCONNECT_H
