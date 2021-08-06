
#include "kylinnetworkdeviceresource.h"

KyNetworkDeviceResourse::KyNetworkDeviceResourse()
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceAdd, this, &KyNetworkDeviceResourse::deviceAdd);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceRemove, this, &KyNetworkDeviceResourse::deviceRemove);

    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceCarrierChanage, this, &KyNetworkDeviceResourse::carrierChanage);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceBitRateChanage, this, &KyNetworkDeviceResourse::deviceBitRateChanage);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceMacAddressChanaged, this, &KyNetworkDeviceResourse::deviceMacAddressChanaged);

}

KyNetworkDeviceResourse::~KyNetworkDeviceResourse()
{
    m_networkResourceInstance = nullptr;
}

void KyNetworkDeviceResourse::getNetworkDevices(QStringList &networkDeviceList)
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
        networkDeviceList<<devicePtr->interfaceName();
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
            qWarning()<<"the network device type is undefined"<<connectDevice->type();
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
            && NetworkManager::Ethernet == connectDevice->type()) {
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
#endif

