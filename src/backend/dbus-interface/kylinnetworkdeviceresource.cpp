
#include "kylinnetworkdeviceresource.h"
#include "kywirelessnetitem.h"

KyNetworkDeviceResourse::KyNetworkDeviceResourse(QObject *parent) : QObject(parent)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

//    m_activeConnectUuidList.clear();
    //m_activeConnectUuidMap.clear();
    m_deviceMap.clear();
    //TODO::get uuid from settings for system reboot;

    initDeviceMap();

    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceAdd, this, [=](QString deviceName, QString uni, NetworkManager::Device::Type deviceType) {
        m_deviceMap.insert(uni,deviceName);
        emit deviceAdd(deviceName, deviceType);
    });
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceRemove, this, [=](QString deviceName, QString uni) {
        m_deviceMap.remove(uni);
        emit deviceRemove(deviceName);
    });
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceUpdate, this, &KyNetworkDeviceResourse::onDeviceUpdate);

    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceCarrierChanage, this, &KyNetworkDeviceResourse::carrierChanage);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceBitRateChanage, this, &KyNetworkDeviceResourse::deviceBitRateChanage);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceMacAddressChanaged, this, &KyNetworkDeviceResourse::deviceMacAddressChanaged);

}

KyNetworkDeviceResourse::~KyNetworkDeviceResourse()
{
    m_networkResourceInstance = nullptr;
}

void KyNetworkDeviceResourse::initDeviceMap()
{
    NetworkManager::Device::List deviceList
            = m_networkResourceInstance->getNetworkDeviceList();

    if (deviceList.isEmpty()) {;
        return;
    }

    NetworkManager::Device::Ptr devicePtr = nullptr;
    for (int index = 0; index < deviceList.size(); ++index) {
        devicePtr = deviceList.at(index);
        m_deviceMap.insert(devicePtr->uni(), devicePtr->interfaceName());
    }
}

void KyNetworkDeviceResourse::getNetworkDeviceList(
                NetworkManager::Device::Type deviceType,
                QStringList &networkDeviceList)
{
    qDebug()<<"[KyNetworkDeviceResourse]"<<"get device list";
    NetworkManager::Device::List deviceList
            = m_networkResourceInstance->getNetworkDeviceList();

    if (deviceList.isEmpty()) {
        qDebug()<<"[KyNetworkDeviceResourse]"<<"network device is not exist.";
        return;
    }

    NetworkManager::Device::Ptr devicePtr = nullptr;
    for (int index = 0; index < deviceList.size(); ++index) {
        devicePtr = deviceList.at(index);
        if (devicePtr->type() == deviceType) {
            networkDeviceList<<devicePtr->interfaceName();
        }
    }

    return;
}

void KyNetworkDeviceResourse::getHardwareInfo(QString ifaceName, QString &hardAddress, int &bandWith)
{
    qDebug() << "[KyNetworkDeviceResourse]" << "get wired hardware info"<<ifaceName;

    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->getNetworkDevice(ifaceName);

    if (nullptr == connectDevice || !connectDevice->isValid()) {
        qWarning()<<"[KyNetworkDeviceResourse]"<<"get hardware info failed, the device" << ifaceName << "is not existed";
        hardAddress.clear();
        bandWith = 0;
        return;
    }

    switch (connectDevice->type()) {
        case NetworkManager::Device::Ethernet:
        {
            NetworkManager::WiredDevice *wiredDevicePtr =
                qobject_cast<NetworkManager::WiredDevice *>(connectDevice.data());
            hardAddress = wiredDevicePtr->hardwareAddress();
            bandWith = wiredDevicePtr->bitRate();
            break;
        }
        case NetworkManager::Device::Wifi:
        {
            NetworkManager::WirelessDevice *wirelessDevicePtr =
                qobject_cast<NetworkManager::WirelessDevice *>(connectDevice.data());
            hardAddress = wirelessDevicePtr->hardwareAddress();
            bandWith = wirelessDevicePtr->bitRate();
            break;
        }
        default:
        {
            hardAddress = "";
            bandWith = 0;
            qWarning()<<"[KyNetworkDeviceResourse]" <<"the network device type is undefined"<<connectDevice->type();
            break;
        }
    }

    return;
}

NetworkManager::Device::State KyNetworkDeviceResourse::getDeviceState(QString deviceName)
{
    qDebug()<<"[KyNetworkDeviceResourse]"<<deviceName<<"get device state";

    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceInterface(deviceName);
    if (connectDevice->isValid()) {
        return connectDevice->state();
    }

    qWarning()<<"[KyNetworkDeviceResourse]"<<"the device is not valid.";
    return NetworkManager::Device::State::UnknownState;
}

bool KyNetworkDeviceResourse::wiredDeviceCarriered(QString deviceName)
{
    qDebug()<<"[KyNetworkDeviceResourse]"<<deviceName<<"wired device is carriered";

    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceInterface(deviceName);
    if (connectDevice->isValid()
            && NetworkManager::Device::Type::Ethernet == connectDevice->type()) {
        NetworkManager::WiredDevice *wiredDevicePtr =
            qobject_cast<NetworkManager::WiredDevice *>(connectDevice.data());
        return wiredDevicePtr->carrier();
    }

    qWarning()<<"[KyNetworkDeviceResourse]"<< deviceName <<" can not get carrier state.";
    return false;
}

void KyNetworkDeviceResourse::setDeviceRefreshRate(QString deviceName, int ms)
{
    qDebug()<<"[KyNetworkDeviceResourse]"<<deviceName<<"set device refresh rate"<<ms;

    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceInterface(deviceName);
    if (connectDevice->isValid()) {
        NetworkManager::DeviceStatistics::Ptr deviceStatistics = connectDevice->deviceStatistics();
        deviceStatistics->setRefreshRateMs(ms);
    }

    return;
}

void KyNetworkDeviceResourse::getDeviceActiveAPInfo(const QString devName, QString &strMac, uint &iHz, uint &iChan, QString &secuType)
{
    strMac.clear();
    iHz = 0;
    iChan = 0;
    secuType.clear();

    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->getNetworkDevice(devName);

    if (nullptr == connectDevice || !connectDevice->isValid()) {
        qWarning()<<"[KyNetworkDeviceResourse]"<<"getDeviceActiveAPInfo failed, the device" << devName << "is not existed";
        return;
    }

    switch (connectDevice->type()) {
        case NetworkManager::Device::Wifi:
        {
            NetworkManager::WirelessDevice *wirelessDevicePtr =
                qobject_cast<NetworkManager::WirelessDevice *>(connectDevice.data());
            NetworkManager::AccessPoint::Ptr apPtr = wirelessDevicePtr->activeAccessPoint();
            if (apPtr.isNull()) {
                break;
            }
            strMac = apPtr->hardwareAddress();
            iHz = apPtr->frequency();
            iChan = NetworkManager::findChannel(iHz);
            NetworkManager::AccessPoint::Capabilities cap = apPtr->capabilities();
            NetworkManager::AccessPoint::WpaFlags wpaFlag = apPtr->wpaFlags();
            NetworkManager::AccessPoint::WpaFlags rsnFlag = apPtr->rsnFlags();
            secuType = enumToQstring(cap, wpaFlag, rsnFlag);
            break;
        }
        case NetworkManager::Device::Ethernet:
            break;
        default:
            break;
    }
}

int KyNetworkDeviceResourse::getWirelessDeviceCapability(const QString deviceName)
{
    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceInterface(deviceName);
    if (connectDevice->isValid()
            && NetworkManager::Device::Type::Wifi == connectDevice->type()) {
        NetworkManager::WirelessDevice *wirelessDevicePtr =
            qobject_cast<NetworkManager::WirelessDevice *>(connectDevice.data());

        int cap = 0;
        if (wirelessDevicePtr->wirelessCapabilities() & NetworkManager::WirelessDevice::ApCap) {
            cap = cap | 0x01;
        }
        if (wirelessDevicePtr->wirelessCapabilities() & NetworkManager::WirelessDevice::Freq2Ghz) {
            cap = cap | 0x02;
        }
        if (wirelessDevicePtr->wirelessCapabilities() & NetworkManager::WirelessDevice::Freq5Ghz) {
            cap = cap | 0x04;
        }
        return cap;
    } else {
        qWarning()<<"[KyNetworkDeviceResourse]"<<deviceName<<" is not valid or not wireless.";
    }

    return 0;
}

#if 0
void KyNetworkDeviceResourse::DeviceSpeed(QString deviceName, KyConnectItem *wiredItem)
{
   // qDebug()<<"[KyNetworkDeviceResourse]"<<deviceName<<"get deivce up and down speed.";

    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceInterface(deviceName);
    if (connectDevice->isValid()) {
        NetworkManager::DeviceStatistics::Ptr deviceStatistics = connectDevice->deviceStatistics();
        wiredItem->m_upSpeed = deviceStatistics->txBytes();
        wiredItem->m_downSpeed = deviceStatistics->rxBytes();
    }

    return;
}

void KyNetworkDeviceResourse::disconnectDevice()
{
    NetworkManager::Device::List networkDeviceList =
                        m_networkResourceInstance->getNetworkDeviceList();
    qDebug() << "[KyNetworkDeviceResourse]:disconnectDevice" << networkDeviceList.size();

    if (networkDeviceList.isEmpty()) {
        qDebug()<<"[KyNetworkDeviceResourse]"<<"the network device is empty, no need disconnect.";
        return;
    }

//    m_activeConnectUuidList.clear();
    m_activeConnectUuidMap.clear();

    for (int index = 0; index < networkDeviceList.size(); ++index) {
        qDebug() << "[KyNetworkDeviceResourse]:disconnectDevice" << index;
        NetworkManager::Device::Ptr networkDevicePtr = networkDeviceList.at(index);
        if (networkDevicePtr->isValid() &&
                NetworkManager::Device::Type::Ethernet == networkDevicePtr->type()) {
            NetworkManager::ActiveConnection::Ptr activeConnectPtr = networkDevicePtr->activeConnection();
            if (nullptr == activeConnectPtr) {
                continue;
            }

            QString activeConnectUuid = activeConnectPtr->uuid();
            if (!activeConnectUuid.isEmpty()) {
//                m_activeConnectUuidList<<activeConnectUuid;
                m_activeConnectUuidMap.insert(networkDevicePtr->interfaceName(),activeConnectUuid);
                //TODO:save uuid for system reboot.
            }

            networkDevicePtr->disconnectInterface();
        }

        networkDevicePtr = nullptr;
    }

     qDebug() << "[KyNetworkDeviceResourse]:disconnectDevice finished.";
    return;
}

void KyNetworkDeviceResourse::setDeviceAutoConnect()
{
    NetworkManager::Device::List networkDeviceList =
                        m_networkResourceInstance->getNetworkDeviceList();

    if (networkDeviceList.isEmpty()) {
        qDebug()<<"[KyNetworkDeviceResourse]" << "the network device is empty,so no need set auto connect.";
        return;
    }

    QMap<QString, QString>::iterator iter = m_activeConnectUuidMap.begin();
    while (iter != m_activeConnectUuidMap.end())
    {
        qDebug() << "Iterator " << iter.key() << ":" << iter.value();
        wiredOperation.activateConnection(iter.key(), iter.value());
        qDebug()<<"[KyNetworkDeviceResourse]" << "active connect uuid "<< iter.key() << " device " << iter.value();
        iter++;
    }

    for (int index = 0; index < networkDeviceList.size(); ++index) {
        NetworkManager::Device::Ptr networkDevicePtr = networkDeviceList.at(index);
        if (networkDevicePtr->isValid()
            && NetworkManager::Device::Type::Ethernet == networkDevicePtr->type()) {
            networkDevicePtr->setAutoconnect(true);
        }
        networkDevicePtr = nullptr;
    }

    return;
}

#endif

void KyNetworkDeviceResourse::onDeviceUpdate(NetworkManager::Device * dev)
{
    QString dbusPath = dev->uni();
    QString interface = dev->interfaceName();
    if (m_deviceMap.contains(dbusPath)) {
        if (m_deviceMap[dbusPath] != interface) {
            QString oldName = m_deviceMap[dbusPath];
            m_deviceMap[dbusPath] = interface;
            emit deviceNameUpdate(oldName, interface);
        }
    }
}

bool KyNetworkDeviceResourse::checkWirelessDeviceExist(const QString devName)
{
    QStringList list;
    getNetworkDeviceList(NetworkManager::Device::Type::Wifi, list);
    return list.contains(devName);
}
