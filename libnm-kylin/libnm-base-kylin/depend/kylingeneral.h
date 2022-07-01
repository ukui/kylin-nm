#ifndef KYLINGENERAL_H
#define KYLINGENERAL_H

#include <QObject>
#include <QGSettings/QGSettings>
#include "kylinnetworkresourcemanager.h"

enum KyConnectStatus {
    NOT_CONNECTED = 0,
    LAN_CONNECTED,
    WLAN_CONNECTED,
    LAN_CONNECTED_LIMITED,
    WLAN_CONNECTED_LIMITED,
};

class KylinGeneralOpration : public QObject
{
    Q_OBJECT
public:
    explicit KylinGeneralOpration(QObject *parent = nullptr);
    ~KylinGeneralOpration();
    void getConnectStatus(KyConnectStatus &status);
    bool getWirelessEnabled();
    void setWirelessNetworkEnabled(bool enabled);
    bool getNetworkingEnabled();
    void setNetworkingEnabled(bool enabled);
    void setWiredEnabled(bool enabled);

Q_SIGNALS:
    void connectStatusChanged(KyConnectStatus);
    void wifiEnabledChanged(bool);
    void networkingEnabledChanged(bool);

private Q_SLOTS:
    void updateConnectStatus();

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    QGSettings *gsettings;
    KyConnectStatus m_status = NOT_CONNECTED;

    void updateGsetting(bool);
    void getConnectivity(KyConnectivity &connectivity);
    void getPrimaryConnectionType(KyConnectionType &connectType);
};

#endif // KYLINGENERAL_H
