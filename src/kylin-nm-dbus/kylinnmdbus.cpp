#include "kylinnmdbus.h"

const QString NotApConnection  = "0";
const QString IsApConnection   = "1";
const QByteArray GSETTINGS_SCHEMA_KYLIN_NM  = "org.ukui.kylin-nm.switch";
const QString    KEY_WIRELESS_SWITCH        = "wirelessswitch";
const QString    KEY_WIRED_SWITCH           = "wiredswitch";
#define LOG_FLAG "[KylinNmDbus]"

KylinNmDbus::KylinNmDbus(MainWindow *parent)
    :QObject(parent)
{
    qDBusRegisterMetaType<QMap<QString, bool> >();
    qDBusRegisterMetaType<QMap<QString, int> >();
    qDBusRegisterMetaType<QVector<QStringList> >();
    qDBusRegisterMetaType<QMap<QString, QVector<QStringList>> >();

    m_thread = new QThread;
    m_manager = new KyNetworkManager();
    m_manager->moveToThread(m_thread);
    connect(m_thread, &QThread::started, m_manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(m_thread, &QThread::finished, m_manager, &KyNetworkManager::deleteLater);
    m_thread->start();

    initDbusConnect();

    while (!m_manager->isInitFinished()) {
        ::usleep(1000);
    }
}

KylinNmDbus::~KylinNmDbus()
{
    m_thread->quit();
}

void KylinNmDbus::initDbusConnect()
{
    //wired
    connect(m_manager, &KyNetworkManager::wiredConnectionAdd, this, &KylinNmDbus::onWiredConnectionAdd);
    connect(m_manager, &KyNetworkManager::connectionRemove, this, &KylinNmDbus::lanRemove);
    connect(m_manager, &KyNetworkManager::wiredConnectionUpdate, this, &KylinNmDbus::onWiredConnectionUpdate);
    connect(m_manager, &KyNetworkManager::wiredStateChange, this, &KylinNmDbus::lanActiveConnectionStateChanged);
    //wireless
    connect(m_manager, &KyNetworkManager::wifiNetworkAdd, this, &KylinNmDbus::onWifiNetworkAdd);
    connect(m_manager, &KyNetworkManager::wifiNetworkRemove, this, &KylinNmDbus::wlanRemove);
    connect(m_manager, &KyNetworkManager::wirelessStateChange, this, &KylinNmDbus::wlanactiveConnectionStateChanged);
    //设备插拔
    connect(m_manager, &KyNetworkManager::wiredDeviceAdd, this, &KylinNmDbus::deviceStatusChanged);
    connect(m_manager, &KyNetworkManager::wirelessDeviceAdd, this, &KylinNmDbus::deviceStatusChanged);
    connect(m_manager, &KyNetworkManager::deviceRemove, this, &KylinNmDbus::deviceStatusChanged);
    connect(m_manager, &KyNetworkManager::wiredDeviceUpdate, this, &KylinNmDbus::onWiredDeviceUpdate);
    connect(m_manager, &KyNetworkManager::wirelessDeviceNameUpdate, this, &KylinNmDbus::onWirelessDeviceNameUpdate);
    //移动热点断开
    connect(m_manager,&KyNetworkManager::wirelessApConnectStateChange, this, &KylinNmDbus::onWirelessApConnectStateChange);
    //信号强度变化
    connect(m_manager, &KyNetworkManager::signalStrengthChange, this, &KylinNmDbus::signalStrengthChange);
    //安全性变化
    connect(m_manager, &KyNetworkManager::secuTypeChange, this, &KylinNmDbus::secuTypeChange);
    //更新列表排序
    connect(m_manager, &KyNetworkManager::updateWifiListInCtrlCenter, this, &KylinNmDbus::updateWifiList);
}

//无线列表
QMap<QString, QVector<QStringList>> KylinNmDbus::getWirelessList()
{
    QMap<QString, QVector<QStringList> > map;
    QStringList deviceList;
    QList<KyActivateItem> activedList;
    QList<KyWirelessNetItem> wlanList;
    QVector<QStringList> vector;
    
    m_manager->getNetworkDeviceList(DEVICE_TYPE_WIFI, deviceList);
    
    if (deviceList.isEmpty()) {
        qDebug() << LOG_FLAG << "getWirelessList return empty, because deviceList is empty";
        return map;
    }

    for (const auto deviceName : deviceList) {
        QString activeSsid;
        vector.clear();
        m_manager->getWifiNetworkList(deviceName, wlanList);
        //已连接
        activedList.clear();
        m_manager->getActiveConnectionList(deviceName, CONNECT_TYPE_WIRELESS, activedList);
        if (!activedList.isEmpty()) {
            QString secuType = "";
            int category = 0;
            activeSsid = activedList.at(0).m_ssid;
            m_manager->getWirelessConnectInfo(deviceName, secuType, category);
            vector.append(QStringList() << activeSsid
                          << QString::number(m_manager->getAcivateWifiSignal())
                          << secuType
                          << activedList.at(0).m_uuid
                          << (m_manager->isApConnection(activedList.at(0).m_uuid) ? IsApConnection : NotApConnection)
                          << QString::number(category));
        } else {
            vector.append(QStringList("--"));
        }
        
        //未连接
        if (!wlanList.isEmpty()) {
            for (const auto itemData : wlanList) {
                if (itemData.m_NetSsid == activeSsid) {
                    continue;
                } else {
                    vector.append(QStringList()<<itemData.m_NetSsid
                                  << QString::number(itemData.m_signalStrength)
                                  << itemData.m_secuType
                                  << (itemData.m_isApConnection ? IsApConnection : NotApConnection)
                                  << QString::number(itemData.m_category));
                }
            }
        }
        map.insert(deviceName, vector);
    }
    return map;
}

QMap<QString, QVector<QStringList> > KylinNmDbus::getWiredList()
{
    QMap<QString, QVector<QStringList> > map;
    QStringList deviceList;
    QMap<QString, QList<KyWiredItem>> lanListMap;
    QList<KyActivateItem> activedList;
    QVector<QStringList> vector;
    QMap<QString, QString> activeMap;
    
    m_manager->getNetworkDeviceList(DEVICE_TYPE_ETHERNET, deviceList);
    
    //已连接
    if (!deviceList.isEmpty()) {
        for (const auto deviceName : deviceList) {
            vector.clear();
            activedList.clear();
            m_manager->getActiveConnectionList(deviceName, CONNECT_TYPE_WIRED, activedList);
            if (!activedList.isEmpty()) {
                vector.append(QStringList() << activedList.at(0).m_connName
                              << activedList.at(0).m_uuid
                              << activedList.at(0).m_dbusPath);
                activeMap.insert(deviceName, activedList.at(0).m_connName);
            } else {
                vector.append(QStringList()<<("--"));
            }
            map.insert(deviceName, vector);
        }
    }
    //未连接
    m_manager->getWiredList(lanListMap);
    if (!lanListMap.isEmpty()) {
        QMap<QString, QList<KyWiredItem> >::iterator iter = lanListMap.begin();
        while (iter != lanListMap.end()) {
            vector.clear();
            for (const auto itemData : lanListMap[iter.key()]) {
                if (activeMap.contains(iter.key()) && itemData.m_connectName == activeMap[iter.key()]) {
                    continue;
                } else {
                    vector.append(QStringList() << itemData.m_connectName
                                  << itemData.m_connectUuid
                                  << itemData.m_connectPath);
                }
            }
            map[iter.key()].append(vector);
            iter++;
        }
    }
    return map;
}

void KylinNmDbus::setWiredSwitchEnable(bool enable)
{
    m_manager->onSetWiredEnabled(enable);
}

void KylinNmDbus::setWirelessSwitchEnable(bool enable)
{
    m_manager->onSetWirelessNetworkEnabled(enable);
}

void KylinNmDbus::setDeviceEnable(QString devName, bool enable)
{
    m_manager->onSetDeviceManaged(devName, enable);
}

void KylinNmDbus::deleteConnect(int type, const QString &connectUuid)
{
    Q_UNUSED(type)
    m_manager->onDeleteConnect(connectUuid);
}

void KylinNmDbus::activateConnect(int type, QString devName, QString uuid)
{
    Q_UNUSED(type)
    m_manager->onActivateConnection(uuid, devName);
}

void KylinNmDbus::deActivateConnect(int type, QString devName, QString activeConnectUuid)
{
    Q_UNUSED(type)
    Q_UNUSED(devName)
    m_manager->onDeactivateConnection(activeConnectUuid);
}

QMap<QString, bool> KylinNmDbus::getDeviceListAndEnabled(int devType)
{
    QMap<QString, bool> map;
    if (devType != WIRED && devType != WIRELESS) {
        qDebug() << LOG_FLAG << "getDeviceEnableState but wrong type";
        return map;
    }
    
    QStringList wiredDevList, wirelessDevList;
    if (devType == WIRED) {
        m_manager->getNetworkDeviceList(KyDeviceType::DEVICE_TYPE_ETHERNET, wiredDevList);
        if (!wiredDevList.isEmpty()) {
            for (const auto devName : wiredDevList) {
                map.insert(devName, m_manager->getDeviceManaged(devName));
            }
        }
    } else if (devType == WIRELESS) {
        m_manager->getNetworkDeviceList(KyDeviceType::DEVICE_TYPE_WIFI, wirelessDevList);
        if (!wirelessDevList.isEmpty()) {
            for (const auto devName : wirelessDevList) {
                map.insert(devName, m_manager->getDeviceManaged(devName));
            }
        }
    }
    return map;
}

QMap<QString, int> KylinNmDbus::getWirelessDeviceCap()
{
    QMap<QString, int> map;
    QStringList wirelessDevList;
    m_manager->getNetworkDeviceList(KyDeviceType::DEVICE_TYPE_WIFI, wirelessDevList);
    for (const auto devName : wirelessDevList) {
        map.insert(devName, m_manager->getWirelessDeviceCapability(devName));
    }
    return map;
}

void KylinNmDbus::activeWirelessAp(const QString apName, const QString apPassword, const QString band, const QString apDevice)
{
    QString uuid("");
    m_manager->activeWirelessAp(uuid, apName, apPassword, apDevice, band);
}

void KylinNmDbus::deactiveWirelessAp(const QString apName, const QString uuid)
{
    m_manager->deactiveWirelessAp(apName, uuid);
}

QStringList KylinNmDbus::getStoredApInfo()
{
    QStringList list;
    QList<KyApConnectItem> apConnectItemList;
    m_manager->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        list << apConnectItemList.at(0).m_connectSsid;
        list << apConnectItemList.at(0).m_password;
        list << apConnectItemList.at(0).m_ifaceName;
        list << (apConnectItemList.at(0).m_isActivated? "true":"false");
        list << apConnectItemList.at(0).m_connectUuid;
        list << apConnectItemList.at(0).m_band;
    }
    return list;
}

QStringList KylinNmDbus::getApInfoBySsid(QString devName, QString ssid)
{
    QStringList list;
    QList<KyApConnectItem> apConnectItemList;
    m_manager->getApConnections(apConnectItemList);
    for (const auto itemData : apConnectItemList) {
        if (itemData.m_connectSsid == ssid && itemData.m_ifaceName == devName) {
            list << itemData.m_password;
            list << itemData.m_band;
        }
    }
    return list;
}

//申请触发扫描，初始化执行&定时执行
void KylinNmDbus::reScan()
{
    m_manager->onRequestWirelessScan();
}

void KylinNmDbus::keyRingInit()
{
    //    system("kylin-keyring --start &");
}

void KylinNmDbus::keyRingClear()
{
    //    system("kylin-keyring --stop");
}

void KylinNmDbus::showKylinNM(int type)
{
    parent()->onShowMainWindow(type);
}

void KylinNmDbus::onWiredConnectionAdd(QString devName, QString connectUuid, QString connectName, QString connectPath)
{
    QStringList info;
    info << connectName << connectUuid << connectPath;
    qDebug() << LOG_FLAG << "emit signal lanAdd, devName"<< devName << ", connectName" << connectName;
    Q_EMIT lanAdd(devName, info);
}

void KylinNmDbus::onWiredConnectionUpdate(QString devName, QString connectUuid, QString connectName, QString connectPath)
{
    QStringList info;
    info << connectName << connectUuid << connectPath;
    Q_EMIT lanUpdate(devName, info);
}

void KylinNmDbus::onWifiNetworkAdd(QString deviceName, KyWirelessNetItem &item)
{
    QStringList info;
    info << item.m_NetSsid
         << QString::number(item.m_signalStrength)
         << item.m_secuType
         << (item.m_isApConnection ? IsApConnection : NotApConnection)
         << QString::number(item.m_category);
    Q_EMIT wlanAdd(deviceName, info);
}

void KylinNmDbus::onWiredDeviceUpdate(QString oldDeviceName, QString newDeviceName)
{
    Q_EMIT deviceNameChanged(oldDeviceName, newDeviceName, WIRED);
}

void KylinNmDbus::onWirelessDeviceNameUpdate(QString oldDeviceName, QString newDeviceName)
{
    Q_EMIT deviceNameChanged(oldDeviceName, newDeviceName, WIRELESS);
}

void KylinNmDbus::onWirelessApConnectStateChange(QString devName, QString ssid, QString uuid, KyConnectState state)
{
    if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        qDebug() << LOG_FLAG << "hotspot deactivated";
        Q_EMIT hotspotDeactivated(devName, ssid);
    } else if (state == NetworkManager::ActiveConnection::State::Activated) {
        qDebug() << LOG_FLAG << "hotspot activated";
        Q_EMIT hotspotActivated(devName, ssid, uuid);
    }
}
