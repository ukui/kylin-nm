#ifndef KYLINVPNCONNECTITEM_H
#define KYLINVPNCONNECTITEM_H

#include <QObject>
#include <NetworkManagerQt/VpnConnection>
#include <NetworkManagerQt/VpnSetting>

class KyVpnConnectItem : public QObject
{
    Q_OBJECT
public:
    explicit KyVpnConnectItem();
    ~KyVpnConnectItem();
    
public:
    QString m_vpnName;
    QString m_vpnUuid;
    
    QString m_vpnUser;
    QString m_vpnGateWay;
    
    QString m_vpnIpv4Address;
    QString m_vpnIpv6Address;
    
    bool m_vpnMppe;
    NetworkManager::VpnConnection::State m_vpnState;
};


#endif // KYLINVPNCONNECTITEM_H
