#include "kylindbusinterfrace.h"

#define LOG_FLAG  "[KylinDbusInterface]"

KylinDbusInterface::KylinDbusInterface(QObject *parent) : QObject(parent)
{
    m_activeConnectResource = new KyActiveConnectResourse(this);
    connect(m_activeConnectResource, &KyActiveConnectResourse::wiredStateChange,
                                        this, &KylinDbusInterface::lanConnectionStateChanged);
    connect(m_activeConnectResource, &KyActiveConnectResourse::wirelessStateChange,
                                        this, &KylinDbusInterface::onWlanStateChange);

    m_connectResource = new KyConnectResourse(this);
    connect(m_connectResource, &KyConnectResourse::wiredConnectionAdd, this, &KylinDbusInterface::onLanAdd);
    connect(m_connectResource, &KyConnectResourse::wiredConnectionRemove, this, &KylinDbusInterface::lanRemoved);
    connect(m_connectResource, &KyConnectResourse::wiredConnectionUpdate, this, &KylinDbusInterface::onLanUpdate);

    m_deviceResource = new KyNetworkDeviceResourse(this);
    connect(m_deviceResource, &KyNetworkDeviceResourse::wiredDeviceAdd, this, &KylinDbusInterface::wiredDeviceAdd);
    connect(m_deviceResource, &KyNetworkDeviceResourse::wiredDeviceRemove, this, &KylinDbusInterface::wiredDeviceRemove);
    connect(m_deviceResource, &KyNetworkDeviceResourse::wiredDeviceNameUpdate, this, &KylinDbusInterface::wiredDeviceNameUPdate);

    m_wirelessResource = new KyWirelessNetResource(this);
    connect(m_wirelessResource, &KyWirelessNetResource::secuTypeChange, this, &KylinDbusInterface::securityTypeChanged);
    connect(m_wirelessResource, &KyWirelessNetResource::signalStrengthChange, this, &KylinDbusInterface::signalStrengthChanged);

    connect(m_wirelessResource, &KyWirelessNetResource::wifiNetworkAdd, this, &KylinDbusInterface::onWlanAdd);
    connect(m_wirelessResource, &KyWirelessNetResource::wifiNetworkRemove, this, &KylinDbusInterface::wlanRemoved);

    connect(m_wirelessResource, &KyWirelessNetResource::wirelessDeviceAdd, this, &KylinDbusInterface::wirelessDeviceAdd);
    connect(m_wirelessResource, &KyWirelessNetResource::wirelessDeviceRemove, this, &KylinDbusInterface::wirelessDeviceRemove);
    connect(m_wirelessResource, &KyWirelessNetResource::wirelessDeviceNameUpdate, this, &KylinDbusInterface::wirelessDeviceNameUPdate);

    m_wirelessOperation = new KyWirelessConnectOperation(this);
    m_wiredOperation = new KyWiredConnectOperation(this);

    m_secretAgent = KylinAgent::getInstance();
}

KylinDbusInterface::~KylinDbusInterface()
{

}

void KylinDbusInterface::onLanAdd(QString connectUuid, QString connectName, QString connectPath)
{
    QStringList info;
    info << connectName << connectUuid << connectPath;

    QString deviceName = "";
    m_connectResource->getInterfaceByUuid(deviceName, connectUuid);

    Q_EMIT lanAdded(deviceName, info);

    return;
}

void KylinDbusInterface::onLanUpdate(QString connectUuid, QString connectName, QString connectPath)
{
    QStringList info;
    info << connectName << connectUuid << connectPath;

    QString deviceName = "";
    if (m_connectResource->isActivatedConnection(connectUuid)) {
        deviceName = m_activeConnectResource->getDeviceOfActivateConnect(connectUuid);
    } else {
        m_connectResource->getInterfaceByUuid(deviceName, connectUuid);
    }

    Q_EMIT lanUpdated(deviceName, info);

    return;
}

void KylinDbusInterface::onWlanAdd(QString deviceName, KyWirelessNetItem &item)
{
    QStringList info;

    info << item.m_NetSsid
         << QString::number(item.m_signalStrength)
         << item.m_secuType
         << (m_connectResource->isApConnection(item.m_connectUuid) ? IsApConnection : NotApConnection);

    Q_EMIT  wlanAdded(deviceName, info);

    return;
}

void KylinDbusInterface::onWlanStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state)
{
    Q_EMIT wlanConnectionStateChanged(deviceName, ssid, uuid, state);

    if (m_connectResource->isApConnection(uuid)) {
        if (CONNECT_STATE_DEACTIVATED == state) {
            Q_EMIT hotspotDeactivated(deviceName, ssid);
        } else if (CONNECT_STATE_ACTIVATED) {
            Q_EMIT hotspotActivated(deviceName, ssid, uuid);
        }
    }

    return;
}

void KylinDbusInterface::getWirelessList(QMap<QString, QVector<QStringList>> &map)
{
    QMap<QString,QStringList> actMap;
    m_wirelessResource->getWirelessActiveConnection(CONNECT_STATE_ACTIVATED, actMap);

    QMap<QString, QList<KyWirelessNetItem> > wlanMap;
    if (!m_wirelessResource->getWifiNetworkOfAllDevice(wlanMap)) {
        return;
    }

    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = wlanMap.begin();
    while (iter != wlanMap.end()) {
        QVector<QStringList> vector;
        QString activeSsid ;
        //先是已连接
        if (actMap.contains(iter.key())) {
            KyWirelessNetItem data;
            QString ssid ="";
            m_wirelessResource->getSsidByUuid(actMap[iter.key()].at(0), ssid);
            if (m_wirelessResource->getWifiNetwork(iter.key(), ssid, data)) {
                vector.append(QStringList() << data.m_NetSsid
                              << QString::number(data.m_signalStrength)
                              << data.m_secuType
                              << data.m_connectUuid
                              << (m_connectResource->isApConnection(data.m_connectUuid) ? IsApConnection : NotApConnection));
                activeSsid = data.m_NetSsid;
            } else {
                vector.append(QStringList("--"));
            }
        } else {
            vector.append(QStringList("--"));
        }
        //未连接
        Q_FOREACH (auto itemData, iter.value()) {
            if (itemData.m_NetSsid == activeSsid) {
                continue;
            }
            vector.append(QStringList()<<itemData.m_NetSsid
                          << QString::number(itemData.m_signalStrength)
                          << itemData.m_secuType
                          << (m_connectResource->isApConnection(itemData.m_connectUuid) ? IsApConnection : NotApConnection));
        }

        map.insert(iter.key(), vector);
        iter++;
    }

    return;
}

void KylinDbusInterface::getWiredList(QMap<QString, QVector<QStringList>> &map)
{
    QStringList devlist;
    m_deviceResource->getNetworkDeviceList(DEVICE_TYPE_ETHERNET, devlist);
    if (devlist.isEmpty()) {
        return;
    }

    Q_FOREACH (auto deviceName, devlist) {
        QList<KyConnectItem *> activedList;
        QList<KyConnectItem *> deactivedList;
        QVector<QStringList> vector;
        m_activeConnectResource->getActiveConnectionList(deviceName, CONNECT_TYPE_WIRED, activedList);
        if (!activedList.isEmpty()) {
            vector.append(QStringList() << activedList.at(0)->m_connectName
                          << activedList.at(0)->m_connectUuid
                          << activedList.at(0)->m_connectPath);
        } else {
            vector.append(QStringList()<<("--"));
        }

        m_connectResource->getConnectionList(deviceName, CONNECT_TYPE_WIRED, deactivedList);      //未激活列表的显示
        if (!deactivedList.isEmpty()) {
            for (int i = 0; i < deactivedList.size(); i++) {
                vector.append(QStringList()<<deactivedList.at(i)->m_connectName
                              <<deactivedList.at(i)->m_connectUuid
                              << deactivedList.at(i)->m_connectPath);
            }
        }
        map.insert(deviceName, vector);
    }

    return;
}

//开启/断开热点
void KylinDbusInterface::activeWirelessAp(const QString apName, const QString apPassword,
                            const QString wirelessBand, const QString apDevice)
{
    QString uuid("");
    QList<KyApConnectItem *> apConnectItemList;
    m_connectResource->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        uuid = apConnectItemList.at(0)->m_connectUuid;
    }

    m_wirelessOperation->activeWirelessAp(uuid, apName, apPassword, apDevice, wirelessBand);

    return;
}

void KylinDbusInterface::deactiveWirelessAp(const QString apName, const QString uuid)
{
    if (!uuid.isEmpty()) {
        m_wirelessOperation->deactiveWirelessAp(apName, uuid);
    }

    return;
}

//获取热点信息
void KylinDbusInterface::getStoredApInfo(QStringList &list)
{
    list.clear();
    QList<KyApConnectItem *> apConnectItemList;
    m_connectResource->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        list << apConnectItemList.at(0)->m_connectSsid;
        list << apConnectItemList.at(0)->m_password;
        list << apConnectItemList.at(0)->m_ifaceName;
        list << (apConnectItemList.at(0)->m_isActivated? "true":"false");
        list << apConnectItemList.at(0)->m_connectUuid;
        list << apConnectItemList.at(0)->m_band;
    }

    return;
}

void KylinDbusInterface::getApInfoBySsid(QString deviceName, QString ssid, QStringList &list)
{
    list.clear();
    QList<KyApConnectItem *> apConnectItemList;
    m_connectResource->getApConnections(apConnectItemList);
    for (int i = 0; i < apConnectItemList.size(); i++) {
        if (apConnectItemList.at(i)->m_connectSsid == ssid
                && apConnectItemList.at(i)->m_ifaceName == deviceName) {
            list << apConnectItemList.at(i)->m_password;
            list << apConnectItemList.at(i)->m_band;
        }
    }

    return;
}

//激活/断开有线连接
void KylinDbusInterface::activateWiredConnection(const QString &deviceName, const QString &connUuid)
{
    if (!m_deviceResource->wiredDeviceIsCarriered(deviceName)) {
        qDebug() << LOG_FLAG << deviceName << "is not carried, so can not activate connection";
        //showDesktopNotify(tr("Wired Device not carried"));
    } else {
        m_wiredOperation->activateConnection(connUuid, deviceName);
    }

    return;
}

void KylinDbusInterface::deactivateWiredConnection(const QString &deviceName, const QString &connUuid)
{
    qDebug()<< LOG_FLAG << "deactivateWired, device:" << deviceName <<"uuid:" << connUuid;
    QString connecName = "";
    m_wiredOperation->deactivateWiredConnection(connecName, connUuid);

    return;
}

/*
* 无线网络激活，需要密码，需要和kylin-nm进行交互，如果改成后台服务的话需要对下面的两个函数功能进行重新定义
* 包括参数的内容
*/
void KylinDbusInterface::activateWirelessConnection(const QString &deviceName, const QString &ssid)
{
//    KyWirelessNetItem wirelessNetItem;
//    if (!m_wirelessResource->getWifiNetwork(devName, ssid, wirelessNetItem)) {
//        qDebug() << "[WlanPage] no such wifi " << ssid << " in " << devName;
//        return;
//    }

//    QListWidgetItem *p_listWidgetItem = nullptr;
//    WlanListItem *p_wlanItem = nullptr;

//    if (m_wirelessNetItemMap.contains(ssid)) {
//        p_listWidgetItem = m_wirelessNetItemMap.value(ssid);
//        p_wlanItem = (WlanListItem*)m_inactivatedNetListWidget->itemWidget(p_listWidgetItem);

//        m_inactivatedNetListWidget->scrollToItem(p_listWidgetItem, QAbstractItemView::EnsureVisible);


//        QMouseEvent *event = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//        QApplication::postEvent(p_wlanItem, event);
//        Q_EMIT showMainWindow();
//    } else {
//        qDebug() << "[WlanPage]activateWirelessConnection no such " << ssid << "in" << devName;
//    }

//    return;
}

void KylinDbusInterface::deactivateWirelessConnection(const QString &deviceName, const QString &ssid)
{
//    KyWirelessNetItem wirelessNetItem;
//    if (!m_wirelessResource->getWifiNetwork(devName, ssid, wirelessNetItem)) {
//        qDebug()<< LOG_FLAG << "no such wifi " << ssid << " in " << devName;
//        return;
//    }

//    if (!m_connectResource->isActivatedConnection(wirelessNetItem.m_connectUuid)) {
//        qDebug()<<LOG_FLAG<<wirelessNetItem.m_connName << "is not activated, so not deactivated.";
//        return;
//    }

//    m_wirelessOperation->deActivateWirelessConnection(wirelessNetItem.m_connName, wirelessNetItem.m_connectUuid);

//    return;
}

//无线总开关
void KylinDbusInterface::setWirelessSwitchEnable(bool enable)
{
    m_wirelessOperation->setWirelessEnabled(enable);

    return;
}

void KylinDbusInterface::setWiredDeviceEnable(const QString &devName, bool enable)
{
    //3.1版本需要保存网卡的状态在、到配置文件，此处由于涉及到登录界面的问题，无法确定配置文件的位置，有待优化
    if (enable) {
        m_wiredOperation->openWiredNetworkWithDevice(devName);
    } else {
        m_wiredOperation->closeWiredNetworkWithDevice(devName);
    }

    return;
}


void KylinDbusInterface::getWirelessDeviceCapability(QMap<QString, int> &map)
{
    QStringList deviceList;

    deviceList.clear();
    m_deviceResource->getNetworkDeviceList(DEVICE_TYPE_WIFI, deviceList);
    for (int i = 0; i < deviceList.size(); ++i) {
        QString deviceName = deviceList.at(i);
        int wirelessDeviceCapability = m_deviceResource->getWirelessDeviceCapability(deviceName);
        map.insert(deviceName, wirelessDeviceCapability);
    }

    return;
}

void KylinDbusInterface::rescanWireless()
{
    m_wirelessOperation->requestWirelessScan();

    return;
}

void KylinDbusInterface::startAgent()
{
    m_secretAgent->startKylinAgent();

    return;
}

void KylinDbusInterface::stopAgent()
{
    m_secretAgent->stopKylinAgent();

    return;
}
