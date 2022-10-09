
#include "kylinactiveconnectresource.h"
#include "kywirelessconnectoperation.h"
#include "kylinnetworkmanagerutil.h"
#include "kylinnetworkdeviceresource.h"

#include <NetworkManagerQt/IpConfig>
#include <NetworkManagerQt/Dhcp4Config>
#include <NetworkManagerQt/Dhcp6Config>

#define LOG_FLAG "[KyActiveConnectResourse]"

KyActiveConnectResourse::KyActiveConnectResourse(QObject *parent) : QObject(parent)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    connect(m_networkResourceInstance, &KyNetworkResourceManager::wiredConnectStateChange,
                                                    this, &KyActiveConnectResourse::wiredStateChange);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wirelessConnectStateChange,
                                                    this, &KyActiveConnectResourse::wirelessStateChange);
}

KyActiveConnectResourse::~KyActiveConnectResourse()
{
    m_networkResourceInstance = nullptr;
}

KyActivateItem KyActiveConnectResourse::getActiveConnectionItem(NetworkManager::ActiveConnection::Ptr activeConnectPtr)
{
    if (nullptr == activeConnectPtr) {
        qWarning() << LOG_FLAG <<"the active connect is empty";
        return KyActivateItem();
    }

    if (NetworkManager::ActiveConnection::State::Activated != activeConnectPtr->state()) {
        return KyActivateItem();
    }

    KyActivateItem activeConnectItem;
    activeConnectItem.m_uuid = activeConnectPtr->uuid();

    NetworkManager::Connection::Ptr connectPtr = activeConnectPtr->connection();
    activeConnectItem.m_connName = connectPtr->name();
    activeConnectItem.m_dbusPath = connectPtr->path();
    if (activeConnectPtr->type() == CONNECT_TYPE_WIRELESS) {
        NetworkManager::WirelessSetting::Ptr wireless_sett =
                connectPtr->settings()->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
        QByteArray rawSsid = wireless_sett->ssid();
        activeConnectItem.m_ssid = getSsidFromByteArray(rawSsid);
    }

    activeConnectItem.m_connStatus = CONNECT_STATE_ACTIVATED;

    return activeConnectItem;
}

void KyActiveConnectResourse::getActiveConnectionList(QString deviceName,
                             KyConnectionType connectionType,
                             QList<KyActivateItem> &activeConnectItemList)
{
    NetworkManager::ActiveConnection::List activeConnectList = m_networkResourceInstance->getActiveConnectList();

    if (activeConnectList.isEmpty()) {
        qWarning() << LOG_FLAG <<"the active connect list is empty";
        return;
    }
    NetworkManager::ActiveConnection::Ptr activeConnectPtr = nullptr;
    for (int index = 0; index < activeConnectList.size(); index++) {
        activeConnectPtr = nullptr;
        activeConnectPtr = activeConnectList.at(index);
        if (activeConnectPtr.isNull()) {
            continue;
        }

        if (connectionType != activeConnectPtr->type()) {
            continue;
        }

        QStringList interfaces = activeConnectPtr->devices();
        for (int index = 0; index < interfaces.size(); ++index) {
            QString ifaceUni = interfaces.at(index);
            NetworkManager::Device:: Ptr devicePtr =
                        m_networkResourceInstance->findDeviceByUni(ifaceUni);
            if (devicePtr->interfaceName() == deviceName) {
                KyActivateItem activeConnectItem =
                        getActiveConnectionItem(activeConnectPtr);
                activeConnectItem.m_interface = deviceName;
                activeConnectItemList <<  activeConnectItem;
                break;
            }
        }
    }
}

//动态ip地址
void KyActiveConnectResourse::getActiveConnectIpInfo(
                        const QString &connectUuid,
                        QString &ipv4Address,
                        QString &ipv6Address)
{
    ipv4Address.clear();
    ipv6Address.clear();
    NetworkManager::ActiveConnection::Ptr activeConnectPtr =
            m_networkResourceInstance->findActiveConnectByUuid(connectUuid);

    if (activeConnectPtr.isNull()) {
        qWarning() << LOG_FLAG <<"it can not find connect "<< connectUuid;
        return;
    }

    getActiveConnectIp(activeConnectPtr, ipv4Address, ipv6Address);
}

void KyActiveConnectResourse::getActiveConnectIp(
                        NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                        QString &ipv4Address,
                        QString &ipv6Address)
{
    NetworkManager::IpConfig ipv4Config =activeConnectPtr->ipV4Config();
    if (ipv4Config.isValid()) {
        if (!ipv4Config.addresses().isEmpty()) {
            NetworkManager::IpAddress address = ipv4Config.addresses().at(0);
            ipv4Address = address.ip().toString();
        } else {
            qWarning()<<"[KyActiveConnectResourse]"<<"the ipv4 address is empty.";
        }
    } else {
        qWarning()<<"[KyActiveConnectResourse]"<<"ipv4 config is not valid";
    }

    NetworkManager::IpConfig ipv6Config =activeConnectPtr->ipV6Config();
    if (ipv6Config.isValid()) {
        if (!ipv6Config.addresses().isEmpty()) {
             NetworkManager::IpAddress address = ipv6Config.addresses().at(0);
             ipv6Address = address.ip().toString();
        } else {
            qWarning()<<"[KyActiveConnectResourse]"<<"ipv6 address is empty";
        }
    } else {
        qWarning()<<"[KyActiveConnectResourse]"<<"ipv6 config is not valid";
    }
}

void KyActiveConnectResourse::getActiveConnectDnsInfo(
                        const QString &connectUuid,
                        QList<QHostAddress> &ipv4Dns,
                        QList<QHostAddress> &ipv6Dns)
{
    NetworkManager::ActiveConnection::Ptr activeConnectPtr =
            m_networkResourceInstance->findActiveConnectByUuid(connectUuid);
    if (activeConnectPtr.isNull()) {
        qWarning()<< "[KyActiveConnectResourse]" <<"it can not find connect "<< connectUuid;
        return;
    }

    getActiveConnectDns(activeConnectPtr, ipv4Dns, ipv6Dns);
}

void KyActiveConnectResourse::getActiveConnectDns(
                         NetworkManager::ActiveConnection::Ptr activeConnectPtr,
                         QList<QHostAddress> &ipv4Dns,
                         QList<QHostAddress> &ipv6Dns)
{
    NetworkManager::IpConfig ipv4Config = activeConnectPtr->ipV4Config();
    if (ipv4Config.isValid()) {
        ipv4Dns = ipv4Config.nameservers();
    } else {
        qWarning()<<"[KyActiveConnectResourse]"<<"ipv4 config is not valid";
    }

    NetworkManager::IpConfig ipv6Config =activeConnectPtr->ipV6Config();
    if (ipv6Config.isValid()) {
        ipv6Dns = ipv6Config.nameservers();
    } else {
        qWarning()<<"[KyActiveConnectResourse]"<<"ipv6 config is not valid";
    }

    return;
}

bool KyActiveConnectResourse::wiredConnectIsActived()
{
    int index = 0;
    NetworkManager::ActiveConnection::List activeConnectList;

    activeConnectList.clear();
    activeConnectList = m_networkResourceInstance->getActiveConnectList();

    if (activeConnectList.empty()) {
        qWarning()<<"[KyActiveConnectResourse]"
                 <<"get active connect failed, the active connect list is empty";
        return false;
    }

    NetworkManager::ActiveConnection::Ptr activeConnectPtr = nullptr;
    for (index = 0; index < activeConnectList.size(); index++) {
        activeConnectPtr = activeConnectList.at(index);
        if (activeConnectPtr.isNull()) {
            continue;
        }

        if (NetworkManager::ConnectionSettings::ConnectionType::Wired
                != activeConnectPtr->type()) {
            continue;
        }

        QString path = activeConnectPtr->connection()->path();
        if (m_networkResourceInstance->isWiredConnect(path)) {
            continue;
        }

        if (activeConnectPtr->state() == NetworkManager::ActiveConnection::State::Activated) {
            return true;
        }
    }

    return false;
}

bool KyActiveConnectResourse::wirelessConnectIsActived()
{
    int index = 0;
    NetworkManager::ActiveConnection::List activeConnectList;

    activeConnectList.clear();
    activeConnectList = m_networkResourceInstance->getActiveConnectList();

    if (activeConnectList.empty()) {
        qWarning()<<"[KyActiveConnectResourse]"
                 <<"get active connect failed, the active connect list is empty";
        return false;
    }

    NetworkManager::ActiveConnection::Ptr activeConnectPtr = nullptr;
    for (index = 0; index < activeConnectList.size(); index++) {
        activeConnectPtr = activeConnectList.at(index);
        if (activeConnectPtr.isNull()) {
            continue;
        }

        if (NetworkManager::ConnectionSettings::ConnectionType::Wireless
                != activeConnectPtr->type()) {
            continue;
        }

        if (activeConnectPtr->state() == NetworkManager::ActiveConnection::State::Activated) {
            return true;
        }
    }

    return false;
}

int KyActiveConnectResourse::getAcivateWifiSignal()
{
    int signalStrength = 0;
    KyNetworkDeviceResourse devResource;
    QStringList devList;
    devResource.getNetworkDeviceList(DEVICE_TYPE_WIFI, devList);

    for (int i = 0; i < devList.size(); ++i) {

        NetworkManager::Device::Ptr connectDevice =
                            m_networkResourceInstance->findDeviceByName(devList.at(i));

        if (nullptr == connectDevice || !connectDevice->isValid()) {
            qWarning()<< LOG_FLAG <<"getDeviceActiveAPInfo failed, the device" << devList.at(i) << "is not existed";
            continue;
        }

        if (connectDevice->type() == NetworkManager::Device::Wifi) {
            NetworkManager::WirelessDevice *wirelessDevicePtr =
                qobject_cast<NetworkManager::WirelessDevice *>(connectDevice.data());
            NetworkManager::AccessPoint::Ptr apPtr = wirelessDevicePtr->activeAccessPoint();
            if (apPtr.isNull()) {
                continue;
            }
            signalStrength = apPtr->signalStrength();
            break;
        }
    }

    return signalStrength;
}
