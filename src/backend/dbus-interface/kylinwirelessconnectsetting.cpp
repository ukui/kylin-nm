#include "kylinwirelessconnectsetting.h"

//KyWirelessConnectSetting::KyWirelessConnectSetting()
//{
//    m_ssid = "";
//    isAutoConnect = false;
//    m_psk = "";
//}

//KyWirelessConnectSetting::~KyWirelessConnectSetting()
//{

//}

NetworkManager::ConnectionSettings::Ptr assembleWirelessSettings(const KyWirelessConnectSetting &connSettingInfo)
{
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(connSettingInfo.m_connectName);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(connSettingInfo.isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(connSettingInfo.m_ssid.toUtf8());
    wifi_sett->setSecurity("802-11-wireless-security");

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    security_sett->setKeyMgmt((NetworkManager::WirelessSecuritySetting::KeyMgmt)connSettingInfo.m_type);
    if (KyKeyMgmt::SAE == connSettingInfo.m_type || KyKeyMgmt::WpaPsk == connSettingInfo.m_type)
    {
        security_sett->setPsk(connSettingInfo.m_psk);
        security_sett->setPskFlags(connSettingInfo.m_secretFlag);
    }
    return settings;
}

void ipv4SettingSet(NetworkManager::Ipv4Setting::Ptr &ipv4Setting, const KyWirelessConnectSetting &connectSettingsInfo)
{
    ipv4Setting->setInitialized(true);

    if (CONFIG_IP_DHCP == connectSettingsInfo.m_ipv4ConfigIpType) {
        ipv4Setting->setMethod(NetworkManager::Ipv4Setting::Automatic);
        return;
    } else {
        ipv4Setting->setMethod(NetworkManager::Ipv4Setting::Manual);
    }

    if (!connectSettingsInfo.m_ipv4Dns.empty()) {
        ipv4Setting->setDns(connectSettingsInfo.m_ipv4Dns);
    }

    if (!connectSettingsInfo.m_ipv4Address.empty()) {
        ipv4Setting->setAddresses(connectSettingsInfo.m_ipv4Address);
    }

    return;
}
void ipv6SettingSet(NetworkManager::Ipv6Setting::Ptr &ipv6Setting, const KyWirelessConnectSetting &connectSettingsInfo)
{
    ipv6Setting->setInitialized(true);

    if (CONFIG_IP_DHCP == connectSettingsInfo.m_ipv6ConfigIpType) {
        ipv6Setting->setMethod(NetworkManager::Ipv6Setting::Automatic);
        return;
    }

    ipv6Setting->setMethod(NetworkManager::Ipv6Setting::Manual);
    if (!connectSettingsInfo.m_ipv6Dns.empty()) {
        ipv6Setting->setDns(connectSettingsInfo.m_ipv6Dns);
    }

    if (!connectSettingsInfo.m_ipv6Address.empty()) {
        ipv6Setting->setAddresses(connectSettingsInfo.m_ipv6Address);
    }

    return ;
}

void setIpv4AndIpv6Setting(NetworkManager::ConnectionSettings::Ptr connSetting, const KyWirelessConnectSetting &connSettingInfo)
{
    KyConnectOperation kco;
    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connSetting->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    kco.ipv4SettingSet(ipv4Setting, connSettingInfo);

    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connSetting->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    kco.KyConnectOperation::ipv6SettingSet(ipv6Setting, connSettingInfo);
}
