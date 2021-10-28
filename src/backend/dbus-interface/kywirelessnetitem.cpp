#include "kywirelessnetitem.h"
#include <NetworkManagerQt/Connection>

QString enumToQstring(NetworkManager::AccessPoint::Capabilities cap, NetworkManager::AccessPoint::WpaFlags wpa_flags,NetworkManager::AccessPoint::WpaFlags rsn_flags)
{
    QString out;
    if (   (cap & NM_802_11_AP_FLAGS_PRIVACY)
           && (wpa_flags == NM_802_11_AP_SEC_NONE)
           && (rsn_flags == NM_802_11_AP_SEC_NONE)) {
        out += "WEP ";
    }
    if (wpa_flags != NM_802_11_AP_SEC_NONE) {
        out += "WPA1 ";
    }
    if ((rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_PSK)
            || (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)) {
        out += "WPA2 ";
    }
    if (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_SAE) {
        out += "WPA3 ";
    }
    if (   (wpa_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)
           || (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)) {
        out += "802.1X ";
    }
    return out;
}

KyWirelessNetItem::KyWirelessNetItem(NetworkManager::WirelessNetwork::Ptr net)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    m_bssid = "";
    m_connectUuid = "";
    m_isConfigured = false;
    m_connName = "";
    m_connDbusPath = "";
    m_secuType = "";
    m_device = "";
    m_channel = 0;

    init(net);
}


KyWirelessNetItem::~KyWirelessNetItem()
{
    m_networkResourceInstance = nullptr;
}


void KyWirelessNetItem::init(NetworkManager::WirelessNetwork::Ptr net)
{
    m_NetSsid = net->ssid();
    m_signalStrength = net->signalStrength();
    m_frequency = net->referenceAccessPoint()->frequency();
    m_channel = NetworkManager::findChannel(m_frequency);
    NetworkManager::AccessPoint::Capabilities cap = net->referenceAccessPoint()->capabilities();
    NetworkManager::AccessPoint::WpaFlags wpaFlag = net->referenceAccessPoint()->wpaFlags();
    NetworkManager::AccessPoint::WpaFlags rsnFlag = net->referenceAccessPoint()->rsnFlags();
    m_secuType = enumToQstring(cap, wpaFlag, rsnFlag);
    m_bssid = net->referenceAccessPoint()->hardwareAddress();
    m_device = net->device();
    initInfoBySsid();
}

void KyWirelessNetItem::initInfoBySsid()
{
    for (auto const & conn : m_networkResourceInstance->m_connections) {
        NetworkManager::ConnectionSettings::Ptr settings = conn->settings();
        if (settings->connectionType() != NetworkManager::ConnectionSettings::Wireless) {
            continue;
        }

        NetworkManager::WirelessSetting::Ptr wifi_sett
            = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
        QString devName = m_networkResourceInstance->findDeviceUni(m_device)->interfaceName();
        if (wifi_sett->ssid() == m_NetSsid
                && (settings->interfaceName().compare(devName) == 0 || settings->interfaceName().isEmpty())) {
            m_connectUuid = settings->uuid();
            m_connName    = conn->name();
            m_connDbusPath = conn->path();
            m_isConfigured = true;
            /*
            * 如果有激活的链接，则取激活的链接，没有则取最后一个，因为一个热点可以创建多个链接
            */
            if (nullptr != m_networkResourceInstance->getActiveConnect(m_connectUuid)) {
                break;
            }
        }
    }

    return;
}
