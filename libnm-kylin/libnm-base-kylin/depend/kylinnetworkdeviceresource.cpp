
#include "kylinnetworkdeviceresource.h"
#include "kylinnetworkmanagerutil.h"
#include "kylinconnectoperation.h"

#define VIRTURAL_DEVICE_PATH "/sys/devices/virtual/net"
#define LOG_FLAG "KyNetworkDeviceResourse"

KyNetworkDeviceResourse::KyNetworkDeviceResourse(QObject *parent) : QObject(parent)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceCarrierChange,
                                       this, &KyNetworkDeviceResourse::carrierChange);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceStateChange,
                                       this, &KyNetworkDeviceResourse::deviceStateChange);
//    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceBitRateChange,
//                                       this, &KyNetworkDeviceResourse::deviceBitRateChange);
//    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceMacAddressChange,
//                                       this, &KyNetworkDeviceResourse::deviceMacAddressChange);
//    connect(m_networkResourceInstance, &KyNetworkResourceManager::deviceActiveChange,
//                                       this, &KyNetworkDeviceResourse::deviceActiveChange);

}

KyNetworkDeviceResourse::~KyNetworkDeviceResourse()
{
    m_networkResourceInstance = nullptr;
}

void KyNetworkDeviceResourse::getNetworkDeviceList(
                KyDeviceType deviceType,
                QStringList &networkDeviceList)
{
    NetworkManager::Device::List deviceList
            = m_networkResourceInstance->getNetworkDeviceList();

    if (deviceList.isEmpty()) {
        qDebug() << LOG_FLAG <<"network device is not exist. device type" << deviceType;
        return;
    }

    NetworkManager::Device::Ptr devicePtr = nullptr;
    for (int index = 0; index < deviceList.size(); ++index) {
        devicePtr = deviceList.at(index);
        if (devicePtr.isNull()) {
            continue;
        }

        if (devicePtr->type() == deviceType) {
            if (DEVICE_TYPE_ETHERNET == deviceType) {
                //为了区分有线网卡和虚拟网卡
                qDebug()<< LOG_FLAG << "device uni" << devicePtr->udi();
                if (devicePtr->udi().startsWith(VIRTURAL_DEVICE_PATH)) {
                    continue;
                }
            }

            networkDeviceList<<devicePtr->interfaceName();
        }
    }

    return;
}

void KyNetworkDeviceResourse::getHardwareInfo(QString ifaceName, QString &hardAddress, int &bandWith)
{
    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceByName(ifaceName);

    if (nullptr == connectDevice || !connectDevice->isValid()) {
        qWarning()<< LOG_FLAG <<"get hardware info failed, the device" << ifaceName << "is not existed";
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
            qWarning()<< LOG_FLAG << "the network device type is undefined" << connectDevice->type();
            break;
        }
    }
}


void KyNetworkDeviceResourse::setWiredDeviceEnable(const QString& devName, bool enable)
{
    saveDeviceEnableState(devName, enable);

    KyNetworkDeviceResourse deviceResource;
    if (enable) {
        deviceResource.openWiredNetworkWithDevice(devName);
    } else {
        deviceResource.closeWiredNetworkWithDevice(devName);
    }
    Q_EMIT wiredDeviceEnableChange(devName, enable);
}

void KyNetworkDeviceResourse::getWiredDeviceEnableState(QMap<QString, bool> &map)
{
    getDeviceEnableState(map);
}

//用于有线开关回连处理 关闭调用
void KyNetworkDeviceResourse::saveActiveConnection(QString &deviceName, QString &connectUuid)
{
    QSettings *p_settings = new QSettings(WIRED_NETWORK_STATE_CONF_FILE, QSettings::IniFormat);

    QString settingValue = p_settings->value(deviceName).toString();
    if (settingValue.isEmpty()) {
        p_settings->setValue(deviceName, connectUuid);
        p_settings->sync();
    }

    delete p_settings;
    p_settings = nullptr;

    return;
}

//用于有线开关回连处理 开启调用
void KyNetworkDeviceResourse::getActiveConnection(QString &deviceName, QString &connectUuid)
{
    QSettings *p_settings = new QSettings(WIRED_NETWORK_STATE_CONF_FILE, QSettings::IniFormat);

    connectUuid = p_settings->value(deviceName).toString();
    p_settings->remove(deviceName);

    delete p_settings;
    p_settings = nullptr;

    return;
}

//用于有线开关回连处理 关闭调用
int KyNetworkDeviceResourse::closeWiredNetworkWithDevice(QString deviceName)
{
    NetworkManager::Device::Ptr wiredDevicePtr =
                            m_networkResourceInstance->findDeviceByName(deviceName);

    if (wiredDevicePtr.isNull()) {
        qWarning()<<"[KyWiredConnectOperation]"<<"the network device" << deviceName <<"is not exist.";
        return -ENXIO;
    }

    if (NetworkManager::Device::Type::Ethernet != wiredDevicePtr->type()) {
        qWarning()<<"[KyWiredConnectOperation]"<<"the device type"
                  << wiredDevicePtr->type() <<"is not Ethernet.";
        return -EINVAL;
    }

    NetworkManager::ActiveConnection::Ptr activeConnectPtr = wiredDevicePtr->activeConnection();
    if (nullptr != activeConnectPtr) {
        QString activeConnectUuid = activeConnectPtr->uuid();
        if (!activeConnectUuid.isEmpty()) {
            qDebug()<<"[KyWiredConnectOperation]" <<"close wired network save connection uuid"
                   << activeConnectUuid <<"device name " << deviceName;
            saveActiveConnection(deviceName, activeConnectUuid);
        }
    }

    wiredDevicePtr->disconnectInterface();

    return 0;
}

//用于有线开关回连处理 开启调用
int KyNetworkDeviceResourse::openWiredNetworkWithDevice(QString deviceName)
{
    NetworkManager::Device::Ptr wiredDevicePtr =
                            m_networkResourceInstance->findDeviceByName(deviceName);

    if (wiredDevicePtr.isNull() || !wiredDevicePtr->isValid()) {
        qWarning()<<"[KyWiredConnectOperation]"<<"the network device" << deviceName <<"is not exist.";
        return -ENXIO;
    }

    if (NetworkManager::Device::Type::Ethernet != wiredDevicePtr->type()) {
        qWarning()<<"[KyWiredConnectOperation]"<<"the device type"
                  << wiredDevicePtr->type() <<"is not Ethernet.";
        return -EINVAL;
    }

    NetworkManager::WiredDevice *p_wiredDevice =
        qobject_cast<NetworkManager::WiredDevice *>(wiredDevicePtr.data());

    if (p_wiredDevice->carrier()) {
        QString connectUuid;
        getActiveConnection(deviceName, connectUuid);
        if (!connectUuid.isEmpty()) {
            KyConnectOperation operate;
            operate.activateConnection(connectUuid, deviceName);
        }
    }

    return 0;
}


KyDeviceState KyNetworkDeviceResourse::getDeviceState(QString deviceName)
{
    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceByName(deviceName);
    if (!connectDevice.isNull() && connectDevice->isValid()) {
        return (KyDeviceState)connectDevice->state();
    }

    qWarning()<< LOG_FLAG <<"get device state failed, the device is " << deviceName;

    return DEVICE_STATE_UNKNOWN;
}

bool KyNetworkDeviceResourse::wiredDeviceIsCarriered(QString deviceName)
{
    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceByName(deviceName);
    if (connectDevice.isNull()) {
        qWarning()<< LOG_FLAG << "check device carriered failed.";
        return false;
    }

    if (connectDevice->isValid()
            && NetworkManager::Device::Type::Ethernet == connectDevice->type()) {
        NetworkManager::WiredDevice *wiredDevicePtr =
            qobject_cast<NetworkManager::WiredDevice *>(connectDevice.data());
        return wiredDevicePtr->carrier();
    }

    qWarning()<< LOG_FLAG << deviceName <<" can not get carrier state.";

    return false;
}

void KyNetworkDeviceResourse::getDeviceActiveAPInfo(const QString devName, QString &strMac, uint &iHz, uint &iChan, QString &secuType)
{
    strMac.clear();
    iHz = 0;
    iChan = 0;
    secuType.clear();

    NetworkManager::Device::Ptr connectDevice =
                        m_networkResourceInstance->findDeviceByName(devName);

    if (nullptr == connectDevice || !connectDevice->isValid()) {
        qWarning()<< LOG_FLAG <<"getDeviceActiveAPInfo failed, the device" << devName << "is not existed";
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
            NetworkManager::AccessPoint::Capabilities capability = apPtr->capabilities();
            NetworkManager::AccessPoint::WpaFlags wpaFlag = apPtr->wpaFlags();
            NetworkManager::AccessPoint::WpaFlags rsnFlag = apPtr->rsnFlags();
            secuType = enumToQstring(capability, wpaFlag, rsnFlag);
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
                        m_networkResourceInstance->findDeviceByName(deviceName);
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

bool KyNetworkDeviceResourse::wirelessDeviceIsExist(const QString devName)
{
    QStringList list;
    getNetworkDeviceList(DEVICE_TYPE_WIFI, list);
    return list.contains(devName);
}

