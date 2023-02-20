#include "kylinnetworkmanager.h"

#define WAIT_US    1000

KyNetworkManager::KyNetworkManager(QObject *parent) : QObject(parent)
{
//    kylinNetworkManagerInit();
    m_init = false;
}

KyNetworkManager::~KyNetworkManager()
{
    m_networkResource = nullptr;
}

void KyNetworkManager::kylinNetworkManagerInit()
{
    if (!m_init) {
        qDebug() << "kylinNetworkManagerInit";
        m_networkResource = KyNetworkResourceManager::getInstance();
        if (!m_networkResource->NetworkManagerIsInited()) {
            m_networkResource->onInitNetwork();
        }
        m_activeConnectResource = new KyActiveConnectResourse(this);
        m_deviceResource = new KyNetworkDeviceResourse(this);
        m_general = new KylinGeneralOpration(this);
        m_wiredConnectResourse = new KyWiredConnectResourse(this);
        m_wirelessNetResource = new KyWirelessNetResource(this);
        m_apNetResource = new KyApNetResource(this);

        initConnect();
    }
    m_init = true;
}

void KyNetworkManager::initConnect()
{
    connect(m_activeConnectResource, &KyActiveConnectResourse::wiredStateChange,
                                        this, &KyNetworkManager::wiredStateChange);
    connect(m_activeConnectResource, &KyActiveConnectResourse::wirelessStateChange,
                                        this, &KyNetworkManager::wirelessStateChange);

    connect(m_deviceResource, &KyNetworkDeviceResourse::carrierChange,
                                        this, &KyNetworkManager::carrierChange);
    connect(m_deviceResource, &KyNetworkDeviceResourse::deviceStateChange,
                                        this, &KyNetworkManager::deviceStateChange);
    connect(m_deviceResource, &KyNetworkDeviceResourse::deviceManagedChange,
                                        this, &KyNetworkManager::deviceManagedChange);

    connect(m_general, &KylinGeneralOpration::connectStatusChanged,
                                        this, &KyNetworkManager::connectStatusChanged);
    connect(m_general, &KylinGeneralOpration::networkingEnabledChanged,
                                        this, &KyNetworkManager::networkingEnabledChanged);
    connect(m_general, &KylinGeneralOpration::wifiEnabledChanged,
                                        this, &KyNetworkManager::wifiEnabledChanged);
    connect(m_general, &KylinGeneralOpration::wiredEnabledChanged,
                                        this, &KyNetworkManager::wiredEnabledChanged);

    connect(m_apNetResource, &KyApNetResource::wirelessApConnectStateChange,
                                        this, &KyNetworkManager::wirelessApConnectStateChange);

    connect(m_wiredConnectResourse, &KyWiredConnectResourse::wiredConnectionAdd,
                                        this, &KyNetworkManager::wiredConnectionAdd);
    connect(m_wiredConnectResourse, &KyWiredConnectResourse::wiredConnectionUpdate,
                                        this, &KyNetworkManager::wiredConnectionUpdate);
    connect(m_wiredConnectResourse, &KyWiredConnectResourse::connectionRemove,
                                        this, &KyNetworkManager::connectionRemove);
    connect(m_wiredConnectResourse, &KyWiredConnectResourse::wiredDeviceAdd,
                                        this, &KyNetworkManager::wiredDeviceAdd);
    connect(m_wiredConnectResourse, &KyWiredConnectResourse::wiredDeviceUpdate,
                                        this, &KyNetworkManager::wiredDeviceUpdate);
    connect(m_wiredConnectResourse, &KyWiredConnectResourse::deviceRemove,
                                        this, &KyNetworkManager::deviceRemove);


    connect(m_wirelessNetResource, &KyWirelessNetResource::signalStrengthChange,
                                        this, &KyNetworkManager::signalStrengthChange);
    connect(m_wirelessNetResource, &KyWirelessNetResource::secuTypeChange,
                                        this, &KyNetworkManager::secuTypeChange);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wirelessConnectionRemove,
                                        this, &KyNetworkManager::wirelessConnectionRemove);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wirelessConnectionAdd,
                                        this, &KyNetworkManager::wirelessConnectionAdd);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wirelessConnectionUpdate,
                                        this, &KyNetworkManager::wirelessConnectionUpdate);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wifiNetworkUpdate,
                                        this, &KyNetworkManager::wifiNetworkUpdate);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wifiNetworkAdd,
                                        this, &KyNetworkManager::wifiNetworkAdd);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wifiNetworkRemove,
                                        this, &KyNetworkManager::wifiNetworkRemove);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wirelessDeviceAdd,
                                        this, &KyNetworkManager::wirelessDeviceAdd);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wirelessDeviceNameUpdate,
                                        this, &KyNetworkManager::wirelessDeviceNameUpdate);
    connect(m_wirelessNetResource, &KyWirelessNetResource::updateWifiList,
                                        this, &KyNetworkManager::updateWifiList);
}

//同步方法
//有线 无线 移动热点界面
void KyNetworkManager::getActiveConnectionList(QString deviceName,
                             KyConnectionType connectionType, QList<KyActivateItem> &connectItemList)
{
    m_activeConnectResource->getActiveConnectionList(deviceName, connectionType, connectItemList);
}

int KyNetworkManager::getAcivateWifiSignal()
{
    return m_activeConnectResource->getAcivateWifiSignal();
}

void KyNetworkManager::getNetworkDeviceList(KyDeviceType deviceType, QStringList &networkDeviceList)
{
    m_deviceResource->getNetworkDeviceList(deviceType, networkDeviceList);
}

bool KyNetworkManager::getDeviceManaged(QString devName)
{
    return m_deviceResource->getDeviceManaged(devName);
}

void KyNetworkManager::getConnectStatus(KyConnectStatus &connectType)
{
    m_general->getConnectStatus(connectType);
}

bool KyNetworkManager::getWirelessEnabled()
{
    return m_general->getWirelessEnabled();
}

bool KyNetworkManager::getWiredEnabled()
{
    return m_general->getWiredEnabled();
}

bool KyNetworkManager::getNetworkingEnabled()
{
    return m_general->getNetworkingEnabled();
}

void KyNetworkManager::getWiredList(QMap<QString,QList<KyWiredItem>> &map)
{
    m_wiredConnectResourse->getWiredList(map);
}

void KyNetworkManager::getWifiNetworkList(QString devName, QList<KyWirelessNetItem> &list)
{
    m_wirelessNetResource->getWifiNetworkList(devName, list);
}

void KyNetworkManager::getWirelessConnectInfo(QString deviceName, QString &secuType, int &cateGory)
{
    m_wirelessNetResource->getWirelessConnectInfo(deviceName, secuType, cateGory);
}

void KyNetworkManager::getApConnections(QList<KyApConnectItem> &apConnectItemList)
{
    m_apNetResource->getApConnections(apConnectItemList);
}

bool KyNetworkManager::isApConnection(QString uuid)
{
    return m_wirelessNetResource->isApConnection(uuid);
}

int KyNetworkManager::getWirelessDeviceCapability(const QString deviceName)
{
    return m_deviceResource->getWirelessDeviceCapability(deviceName);
}

void KyNetworkManager::activeWirelessAp(const QString apUuid, const QString apName,
                                        const QString apPassword, const QString apDevice,
                                        const QString wirelessBand)
{
    KyWirelessConnectOperation operate;
    operate.activeWirelessAp(apUuid, apName, apPassword, apDevice, wirelessBand);
}

void KyNetworkManager::deactiveWirelessAp(const QString apName, const QString apUuid)
{
    KyWirelessConnectOperation operate;
    operate.deactiveWirelessAp(apName, apUuid);
}

//详情页
//共有ipv4 ipv6 包括autoconnect
void KyNetworkManager::getConnectIpInfo(QString uuid, KyConnectSetting &connectSetting)
{
    KyNetResource resource;
    resource.getConnectionSetting(uuid, connectSetting);
}
//详情页-->有线连接detail
void KyNetworkManager::getWiredConnectDetail(QString interface, QString uuid, KyDetailInfo &connectSetting)
{
    KyWiredConnectResourse resource;
    resource.getWiredConnectDetail(interface, uuid, connectSetting);
}

//详情页-->无线连接detail
void KyNetworkManager::getWirelessConnectDetail(QString interface, QString ssid, QString uuid, KyDetailInfo &connectSetting)
{
    KyWirelessNetResource resource;
    resource.getWirelessConnectDetail(interface, ssid, uuid, connectSetting);
}

//无线连接-->安全
bool KyNetworkManager::getNormalWifiConnectSecuInfo(QString &uuid, KyWpaPasswordInfo &info)
{
    KyWirelessNetResource resource;
    return resource.getNormalWifiConnectSecuInfo(uuid, info);
}

bool KyNetworkManager::getEnterPriseInfoTls(QString &uuid, KyEapMethodTlsInfo &info)
{
    KyWirelessNetResource resource;
    return resource.getEnterPriseInfoTls(uuid, info);
}

bool KyNetworkManager::getEnterPriseInfoPeap(QString &uuid, KyEapMethodPeapInfo &info)
{
    KyWirelessNetResource resource;
    return resource.getEnterPriseInfoPeap(uuid, info);
}

bool KyNetworkManager::getEnterPriseInfoTtls(QString &uuid, KyEapMethodTtlsInfo &info)
{
    KyWirelessNetResource resource;
    return resource.getEnterPriseInfoTtls(uuid, info);
}

//获取企业网类型
bool KyNetworkManager::getEnterpiseEapMethod(const QString &uuid, KyEapMethodType &type)
{
    KyWirelessNetResource resource;
    return resource.getEnterpiseEapMethod(uuid, type);
}

//获取无线网配置安全类型
KySecuType KyNetworkManager::getConnectSecuType(const QString &uuid)
{
    KyWirelessConnectOperation operate;
    return operate.getConnectSecuType(uuid);
}

bool KyNetworkManager::wiredConnectIsActived()
{
    return m_activeConnectResource->wiredConnectIsActived();
}

bool KyNetworkManager::wirelessConnectIsActived()
{
    return m_activeConnectResource->wirelessConnectIsActived();
}

//异步槽
void KyNetworkManager::onSetWirelessNetworkEnabled(bool enabled)
{
    m_general->setWirelessNetworkEnabled(enabled);
}

void KyNetworkManager::onSetNetworkingEnabled(bool enabled)
{
    m_general->setNetworkingEnabled(enabled);
}

void KyNetworkManager::onSetWiredEnabled(bool enabled)
{
    m_general->setWiredEnabled(enabled);
}

void KyNetworkManager::onSetDeviceManaged(QString devName, bool managed)
{
    m_deviceResource->setDeviceManaged(devName, managed);
}

void KyNetworkManager::onSetWiredDeviceEnable(QString devName, bool enable)
{
    m_deviceResource->setDeviceManaged(devName, enable);
}

void KyNetworkManager::onDeleteConnect(const QString &connectUuid)
{
    KyConnectOperation operate;
    operate.deleteConnect(connectUuid);
}

void KyNetworkManager::onActivateConnection(const QString connectUuid, const QString deviceName)
{
    KyConnectOperation operate;
    operate.activateConnection(connectUuid, deviceName);
}

void KyNetworkManager::onDeactivateConnection(const QString &activeConnectUuid)
{
    KyConnectOperation operate;
    operate.deactivateConnection(activeConnectUuid);
}

void KyNetworkManager::onCreateWiredConnect(KyConnectSetting connectSettingsInfo)
{
    KyWiredConnectOperation operate;
    operate.createWiredConnect(connectSettingsInfo);
}

//=====================申请无线扫描===================
void KyNetworkManager::onRequestWirelessScan()
{
    KyWirelessConnectOperation operate;
    operate.requestWirelessScan();
}

//=======================连接操作====================
//连接无本地配置的非企业网热点
void KyNetworkManager::onAddAndActivateNormalWifi(KyWirelessConnectSetting connSettingInfo, KySecuType type)
{
    KyWirelessConnectOperation operate;
    operate.addAndActiveWirelessConnect(connSettingInfo, type);
}
//连接无本地配置的企业网热点
void KyNetworkManager::onAddAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo info,
                                                KyWirelessConnectSetting connSettingInfo)
{
    KyWirelessConnectOperation operate;
    operate.addAndActiveWirelessEnterPriseTlsConnect(info, connSettingInfo);
}
void KyNetworkManager::onAddAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo info,
                                                KyWirelessConnectSetting connSettingInfo)
{
    KyWirelessConnectOperation operate;
    operate.addAndActiveWirelessEnterPrisePeapConnect(info, connSettingInfo);
}
void KyNetworkManager::onAddAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo info,
                                                 KyWirelessConnectSetting connSettingInfo)
{
    KyWirelessConnectOperation operate;
    operate.addAndActiveWirelessEnterPriseTtlsConnect(info, connSettingInfo);
}

//=====================新增配置操作====================
//新增普通连接
void KyNetworkManager::onAddNormalConnect(const KyWirelessConnectSetting &connSettingInfo, KySecuType &type)
{
    KyWirelessConnectOperation operate;
    operate.addConnect(connSettingInfo, type);
}
//新增TLS连接
void KyNetworkManager::onAddTlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTlsInfo &tlsinfo)
{
    KyWirelessConnectOperation operate;
    operate.addTlsConnect(connSettingInfo, tlsinfo);
}
//新增PEAP连接
void KyNetworkManager::onAddPeapConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodPeapInfo &peapInfo)
{
    KyWirelessConnectOperation operate;
    operate.addPeapConnect(connSettingInfo, peapInfo);
}
//新增TTLS连接
void KyNetworkManager::onAddTtlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTtlsInfo &ttlsInfo)
{
    KyWirelessConnectOperation operate;
    operate.addTtlsConnect(connSettingInfo, ttlsInfo);
}

//=====================更新配置操作====================
void KyNetworkManager::onUpdateIpv4AndIpv6SettingInfo(const QString &uuid, const KyConnectSetting &connectSettingsInfo)
{
    KyWirelessConnectOperation operate;
    operate.updateIpv4AndIpv6SettingInfo(uuid, connectSettingsInfo);
}
//连接修改(安全改为个人/None)
void KyNetworkManager::onUpdateWirelessPersonalConnect(const QString &uuid,
                                                                 const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange)
{
    KyWirelessConnectOperation operate;
    operate.updateWirelessPersonalConnect(uuid, connSettingInfo, bPwdChange);
}
//连接修改(安全改为改为企业)
void KyNetworkManager::onUpdateWirelessEnterPriseTlsConnect(const QString &uuid, const KyEapMethodTlsInfo &tlsinfo)
{
    KyWirelessConnectOperation operate;
    operate.updateWirelessEnterPriseTlsConnect(uuid, tlsinfo);
}
void KyNetworkManager::onUpdateWirelessEnterPrisePeapConnect(const QString &uuid, const KyEapMethodPeapInfo &peapInfo)
{
    KyWirelessConnectOperation operate;
    operate.updateWirelessEnterPrisePeapConnect(uuid, peapInfo);
}
void KyNetworkManager::onUpdateWirelessEnterPriseTtlsConnect(const QString &uuid, const KyEapMethodTtlsInfo &ttlsInfo)
{
    KyWirelessConnectOperation operate;
    operate.updateWirelessEnterPriseTtlsConnect(uuid, ttlsInfo);
}

void KyNetworkManager::onUpdateWirelessAutoConnectState(const QString &uuid, bool bAutoConnect)
{
    KyWirelessConnectOperation operate;
    operate.setWirelessAutoConnect(uuid, bAutoConnect);
}
