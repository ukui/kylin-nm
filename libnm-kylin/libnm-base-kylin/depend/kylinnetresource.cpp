#include "kylinnetresource.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinactiveconnectresource.h"
#include "kywirelessnetresource.h"
#include "kylinnetworkmanagerutil.h"

KyNetResource::KyNetResource(QObject *parent) : QObject(parent)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
}

KyNetResource::~KyNetResource() {
    m_networkResourceInstance = nullptr;
}

void KyNetResource::getIpv4ConnectSetting(
                        NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                        KyConnectSetting &connectSetting)
{
    if (NetworkManager::Ipv4Setting::Automatic == ipv4Setting->method()) {
        connectSetting.m_ipv4ConfigIpType = CONFIG_IP_DHCP;
        connectSetting.m_ipv4Dns = ipv4Setting->dns();
        return;
    }

    connectSetting.m_ipv4ConfigIpType = CONFIG_IP_MANUAL;

    connectSetting.m_ipv4Address = ipv4Setting->addresses();
    connectSetting.m_ipv4Dns = ipv4Setting->dns();
}

void KyNetResource::getIpv6ConnectSetting(
                        NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                        KyConnectSetting &connectSetting)
{

    if (NetworkManager::Ipv6Setting::Automatic == ipv6Setting->method()) {
        connectSetting.m_ipv6ConfigIpType = CONFIG_IP_DHCP;
        connectSetting.m_ipv6Dns = ipv6Setting->dns();
        return;
    }

    connectSetting.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    connectSetting.m_ipv6Address = ipv6Setting->addresses();
    connectSetting.m_ipv6Dns = ipv6Setting->dns();
}

void KyNetResource::getConnectionSetting(QString connectUuid, KyConnectSetting &connectSetting)
{
    NetworkManager::Connection::Ptr connectPtr =
                            m_networkResourceInstance->findConnectByUuid(connectUuid);

    if (nullptr == connectPtr
               || !connectPtr->isValid()) {
        qWarning() <<"[KyConnectResourse]" << "it can not find valid connection" << connectUuid;
        return;
    }

    connectSetting.m_connectName = connectPtr->name();

    NetworkManager::ConnectionSettings::Ptr connectionSettings = connectPtr->settings();
    connectSetting.m_ifaceName = connectionSettings->interfaceName();
    connectSetting.m_isAutoConnect = connectionSettings->autoconnect();

    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connectionSettings->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    getIpv4ConnectSetting(ipv4Setting, connectSetting);

    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connectionSettings->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    getIpv6ConnectSetting(ipv6Setting, connectSetting);
}

void KyNetResource::getBaseInfo(QString devName,QString ssid, bool isWlan, bool isActive, KyDetailInfo &conInfo)
{
    //有线无线公有
    KyNetworkDeviceResourse devResource;

    QString hardAddress;
    int bandWith;
    devResource.getHardwareInfo(devName, hardAddress, bandWith);

    if (!hardAddress.isEmpty()) {
        conInfo.strBandWidth = QString("%1").arg(bandWith/1000) + "Mbps";
        conInfo.strMac = hardAddress;
    }

    if (isWlan) {
        if (!isActive) {
            NetworkManager::Device::Ptr devPtr =
                    KyNetworkResourceManager::getInstance()->findDeviceByName(devName);
            if (devPtr.isNull()) {
                return;
            }

            NetworkManager::WirelessNetwork::Ptr netPtr =
                    KyNetworkResourceManager::getInstance()->findWifiNetwork(ssid, devPtr->uni());
            if (netPtr.isNull()) {
                return;
            }

            NetworkManager::AccessPoint::Ptr bestApPtr = netPtr->referenceAccessPoint();
            conInfo.strHz = QString::number(bestApPtr->frequency()) + "MHz";
            conInfo.strChan = QString::number(NetworkManager::findChannel(bestApPtr->frequency()));
            NetworkManager::AccessPoint::Capabilities cap = bestApPtr->capabilities();
            NetworkManager::AccessPoint::WpaFlags wpaFlag = bestApPtr->wpaFlags();
            NetworkManager::AccessPoint::WpaFlags rsnFlag = bestApPtr->rsnFlags();
            conInfo.strSecType = enumToQstring(cap, wpaFlag, rsnFlag);
        } else {
            uint iHz,iChan;
            QString strMac;
            devResource.getDeviceActiveAPInfo(devName, strMac, iHz, iChan, conInfo.strSecType);

            conInfo.strHz = QString::number(iHz);
            conInfo.strChan = QString::number(iChan);
        }

        if (conInfo.strSecType.isEmpty()) {
            conInfo.strSecType = tr("None");
        }
    }
}

//详情ipv4 ipv6 ipv4Dns
void KyNetResource::getDynamicIpInfo(QString uuid, KyDetailInfo &conInfo)
{
    //已激活的网络 详情页显示动态ipv4 ipv6 dns
    QString ipv4,ipv6;
    QList<QHostAddress> ipv4Dns,ipv6Dns;
    KyActiveConnectResourse activeResourse;
    activeResourse.getActiveConnectIpInfo(uuid,ipv4,ipv6);
    activeResourse.getActiveConnectDnsInfo(uuid,ipv4Dns,ipv6Dns);

    //Ipv6
    if (!ipv6.isEmpty()) {
        conInfo.strDynamicIpv6 = ipv6;
    }

    //IPv4
    if (!ipv4.isEmpty()) {
        conInfo.strDynamicIpv4 = ipv4;
    }

    if (!ipv4Dns.isEmpty()) {
        conInfo.strDynamicIpv4Dns = ipv4Dns.at(0).toString();
    }
}
