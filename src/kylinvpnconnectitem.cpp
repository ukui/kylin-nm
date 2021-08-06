
#include "kylinvpnconnectitem.h"


KyVpnConnectItem::KyVpnConnectItem()
{
    m_vpnName = "";
    m_vpnUuid = "";

    m_vpnUser = "";
    m_vpnGateWay = "";

    m_vpnIpv4Address = "";
    m_vpnIpv6Address = "";

    m_vpnMppe = false;
    m_vpnState = NetworkManager::VpnConnection::State::Disconnected;
}

KyVpnConnectItem::~KyVpnConnectItem()
{
    
}
