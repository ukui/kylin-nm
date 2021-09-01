#include "wlanpage.h"
#include "kywirelessnetitem.h"
#include "dbusadaptor.h"
#include <QEvent>
#include <QDateTime>
#include <QDebug>
#include <QSettings>

WlanPage::WlanPage(QWidget *parent) : TabPage(parent)
{
    m_resource = new KyWirelessNetResource(this);
    m_connectResource = new KyActiveConnectResourse();
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
    m_netDeviceResource=new KyNetworkDeviceResourse(this);
    m_apConnectResource = new KyConnectResourse(this);
    devList.empty();
    initDevice();
    m_wirelessConnectOpreation = new KyWirelessConnectOperation(this);
    initWlanUI();
    //要在initUI之后调用，保证UI的信号槽顺利绑定
    initConnections();
    getActiveWlan();
    getAllWlan();

    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceAdd, this, &WlanPage::onDeviceAdd);
    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceRemove, this, &WlanPage::onDeviceRemove);
    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceNameUpdate, this, &WlanPage::onDeviceNameUpdate);

    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::activateConnectionError, this, &WlanPage::activateFailed);
    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::addAndActivateConnectionError, this, &WlanPage::activateFailed);
    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::deactivateConnectionError, this, &WlanPage::deactivateFailed);
}

//QString WlanPage::getSsidFromUuid(const QString &uuid)
//{

//}

bool WlanPage::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        if (w == m_hiddenWlanLabel) {
            //ZJP_TODO 打开隐藏WiFi添加弹窗
        } else if (w == m_settingsLabel) {
            //ZJP_TODO 打开控制面板
        }
    }
    return QWidget::eventFilter(w,e);
}

void WlanPage::initWlanUI()
{
    m_titleLabel->setText(tr("WLAN"));
    m_activatedNetLabel->setText(tr("Activated WLAN"));
    m_inactivatedNetLabel->setText(tr("Inactivated WLAN"));

    //一些独有控件
    m_inactivatedWlanListAreaCentralWidget = new QFrame(m_inactivatedNetListArea);
    m_inactivatedNetListArea->setBackgroundRole(QPalette::Base);
    m_inactivatedNetListArea->setWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaLayout = new QVBoxLayout(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaCentralWidget->setLayout(m_inactivatedWlanListAreaLayout);
    m_inactivatedWlanListAreaLayout->setSpacing(MAIN_LAYOUT_SPACING);
    m_inactivatedWlanListAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);

    m_inactivatedNetListWidget = new QListWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_inactivatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_inactivatedNetListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_hiddenWlanWidget = new QFrame(m_inactivatedWlanListAreaCentralWidget);
    m_hiddenWlanLayout = new QHBoxLayout(m_hiddenWlanWidget);
    m_hiddenWlanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_hiddenWlanLabel = new QLabel(m_hiddenWlanWidget);
    m_hiddenWlanLabel->setText(tr("More..."));
    m_hiddenWlanLabel->setContentsMargins(MORE_TEXT_MARGINS);
    m_hiddenWlanLayout->addWidget(m_hiddenWlanLabel);
    m_hiddenWlanLayout->addStretch();

    m_inactivatedWlanListAreaLayout->addWidget(m_inactivatedNetListWidget);
    m_inactivatedWlanListAreaLayout->addWidget(m_hiddenWlanLabel);
    m_inactivatedWlanListAreaLayout->addStretch();
    m_activatedNetListWidget = new QListWidget(m_activatedNetFrame);
    m_activatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_activatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_activatedNetListWidget->setFixedHeight(NORMAL_HEIGHT);
    m_activatedNetListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_activatedNetLayout->addWidget(m_activatedNetListWidget);
//    m_inactivatedNetListArea->setFixedHeight(SCROLLAREA_HEIGHT);
    m_inactivatedNetListArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/**
 * @brief WlanPage::initConnections 初始化一些信号槽
 */
void WlanPage::initConnections()
{
    connect(m_resource, &KyWirelessNetResource::wifiNetworkAdd, this, &WlanPage::onWlanAdded);
    connect(m_resource, &KyWirelessNetResource::wifiNetworkRemove, this, &WlanPage::onWlanRemoved);
    connect(m_resource, &KyWirelessNetResource::wifiNetworkAdd, this, &WlanPage::listUpdate);
    connect(m_resource, &KyWirelessNetResource::wifiNetworkRemove, this, &WlanPage::listUpdate);
//    connect(m_resource, &KyWirelessNetResource::wifiNetworkUpdate, this, &WlanPage::onWlanUpdated);
    connect(m_connectResource, &KyActiveConnectResourse::stateChangeReason, this, &WlanPage::onActivatedWlanChanged);
    connect(m_netSwitch, &SwitchButton::clicked, this, &WlanPage::onWlanSwitchClicked);
    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        m_switchGsettings = new QGSettings(GSETTINGS_SCHEMA);
        if (m_switchGsettings->keys().contains(WIRELESS_SWITCH)) {
            m_netSwitch->setSwitchStatus(m_switchGsettings->get(WIRELESS_SWITCH).toBool());
            connect(m_switchGsettings, &QGSettings::changed, this, [ = ](const QString &key) {
                if (key == WIRELESS_SWITCH) {
                    onWlanSwitchStatusChanged(m_switchGsettings->get(WIRELESS_SWITCH).toBool());
                }
            });
            return;
        }
    }
    qDebug()<<"isSchemaInstalled false" << Q_FUNC_INFO << __LINE__;
    m_netSwitch->setSwitchStatus(m_wirelessConnectOpreation->getWirelessEnabled());
}

/**
 * @brief WlanPage::initDevice 初始化默认网卡
 */
void WlanPage::initDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wireless");
    QString deviceName = m_settings->value(key, "").toString();
    m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, devList);
    if (deviceName.isEmpty()) {
        qDebug() << "initDevice but  defalut wireless card is null";
        if (!devList.isEmpty()) {
            deviceName = devList.at(0);
            m_settings->setValue(key, deviceName);
        }
    }
    updateDefaultDevice(deviceName);
    qDebug() << "[WlanPage] initDevice defaultDevice = " << deviceName;
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;

}

/**
 * @brief WlanPage::getActiveWlan 获取所有已激活连接
 */
void WlanPage::getActiveWlan()
{
    QMap<QString,QStringList> actMap;
    m_activatedNetListWidget->clear();
    m_resource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);
    QMap<QString,QStringList>::iterator iter = actMap.begin();
    int height = 0;
    while (iter != actMap.end()) {
        if (iter.key() == defaultDevice && !iter.value().isEmpty()) {
            QString ssid = iter.value().at(0);
            appendActiveWlan(ssid, height);
            break;
        }
        iter ++;
    }
    if (height > 0) {
        m_activatedNetListWidget->setFixedHeight(height);
    } else {
        //未连接任何WiFi的情况
        m_activatedWlanSSid.clear();
        WlanListItem *wlanItemWidget = new WlanListItem();
        qDebug() << "There is no activated wlan." << Q_FUNC_INFO << __LINE__ ;
        QListWidgetItem *wlanItem = new QListWidgetItem(m_activatedNetListWidget);
        wlanItem->setSizeHint(QSize(m_activatedNetListWidget->width(), wlanItemWidget->height()));
        m_activatedNetListWidget->addItem(wlanItem);
        m_activatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
        height += wlanItemWidget->height();
    }
}

void WlanPage::appendActiveWlan(const QString &ssid, int &height)
{
    m_activatedWlanSSid = ssid;

    KyWirelessNetItem data;
    if (!m_resource->getWifiNetwork(defaultDevice, ssid, data)) {
        return;
    }
    KyWirelessNetItem *item_data = new KyWirelessNetItem(data);
    WlanListItem *wlanItemWidget = new WlanListItem(m_resource, item_data, defaultDevice);
    wlanItemWidget->setActivated(true);
    qDebug() << "Activated wlan: ssid = " << item_data->m_NetSsid;
    QListWidgetItem *wlanItem = new QListWidgetItem(m_activatedNetListWidget);
    wlanItem->setSizeHint(QSize(m_activatedNetListWidget->width(), wlanItemWidget->height()));
    m_activatedNetListWidget->addItem(wlanItem);
    m_activatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);

    height += wlanItemWidget->height();
}

/**
 * @brief WlanPage::getAllWlan 获取所有Wlan //ZJP_TODO 需要针对网卡进行筛选
 */
void WlanPage::getAllWlan()
{
    qDebug() << "Started loading wlan list! time=" << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");
    m_inactivatedNetListWidget->clear();
    m_itemsMap.clear();
    QList<KyWirelessNetItem> wlanList;
//    if (!m_resource->getAllDeviceWifiNetwork(map))
    if (!m_resource->getDeviceWifiNetwork(defaultDevice, wlanList)) //ZJP_TODO 获取默认网卡并传入
    {
        return;
    }
    int height = 0;
    foreach (auto itemData, wlanList) {
        if (itemData.m_NetSsid == this->m_activatedWlanSSid) {
            continue;
        }

        KyWirelessNetItem *data = new KyWirelessNetItem(itemData);
        WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data, defaultDevice);
        QListWidgetItem *wlanItem = new QListWidgetItem(m_inactivatedNetListWidget);
        connect(wlanItemWidget, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
        connect(wlanItemWidget, &WlanListItem::connectButtonClicked, this, &WlanPage::onConnectButtonClicked);
        m_itemsMap.insert(data->m_NetSsid, wlanItem);
        wlanItem->setSizeHint(QSize(m_inactivatedNetListWidget->width(), wlanItemWidget->height()));
        m_inactivatedNetListWidget->addItem(wlanItem);
        m_inactivatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
        if (height == 0) {
            height += wlanItemWidget->height();
        }
        height += wlanItemWidget->height() + NET_LIST_SPACING;
    }
    m_inactivatedNetListWidget->setFixedHeight(height);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
    qDebug() << "Stopped loading wlan list! time=" << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");
}

void WlanPage::onWlanAdded(QString interface, KyWirelessNetItem &item)
{
    qDebug() << "A Wlan Added! interface = " << interface << "; ssid = " << item.m_NetSsid << Q_FUNC_INFO <<__LINE__;

    KyWirelessNetItem *data = new KyWirelessNetItem(item);
    WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data, defaultDevice);
    connect(wlanItemWidget, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
    QListWidgetItem *wlanItem = new QListWidgetItem(m_inactivatedNetListWidget);
    wlanItem->setSizeHint(QSize(m_inactivatedNetListWidget->width(), wlanItemWidget->height()));
    m_inactivatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
//    m_inactivatedNetListWidget->insertItem(N, wlanItem);
    m_inactivatedNetListWidget->addItem(wlanItem); //ZJP_TODO 目前会添加到列表尾部
    m_inactivatedNetListWidget->setFixedHeight(m_inactivatedNetListWidget->height() + wlanItemWidget->height() + NET_LIST_SPACING);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());

    m_itemsMap.insert(data->m_NetSsid, wlanItem);
}

void WlanPage::onWlanRemoved(QString interface, QString ssid)
{
    if (!m_itemsMap.contains(ssid)) { return; }
    if (m_expandedItem == m_itemsMap.value(ssid)) { m_expandedItem = nullptr; }
    qDebug() << "A Wlan Removed! interface = " << interface << "; ssid = " << ssid << Q_FUNC_INFO <<__LINE__;
    int height = m_inactivatedNetListWidget->itemWidget(m_itemsMap.value(ssid))->height();
    m_inactivatedNetListWidget->takeItem(m_inactivatedNetListWidget->row(m_itemsMap.value(ssid)));
    m_inactivatedNetListWidget->setFixedHeight(m_inactivatedNetListWidget->height() - height - NET_LIST_SPACING);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
    m_itemsMap.remove(ssid);
}

void WlanPage::onWlanUpdated()
{
    //ZJP_TODO 某些特定情况下不可重绘整个列表，此处代码需要修改
    m_expandedItem = nullptr;
    getActiveWlan();
    getAllWlan();
}


void WlanPage::onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType)
{
    qDebug() << "deviceAdd" << deviceName;
    if (deviceType !=  NetworkManager::Device::Type::Wifi) {
        return;
    }
    devList << deviceName;
    if (getDefaultDevice().isEmpty())
    {
        updateDefaultDevice(deviceName);
        setDefaultDevice(WIRELESS, deviceName);

    }
    emit deviceStatusChanged();
}

void WlanPage::onDeviceRemove(QString deviceName)
{
    qDebug() << "deviceRemove" << deviceName;
    if (getDefaultDevice() == deviceName)
    {
        QStringList list;
        QString newDefaultDevice = "";
        list.empty();
        m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, list);
        if (!list.isEmpty()) {
            newDefaultDevice = list.at(0);
        }
        updateDefaultDevice(newDefaultDevice);
        setDefaultDevice(WIRELESS, newDefaultDevice);
    }
    if (devList.contains(deviceName)) {
        devList.removeOne(deviceName);
        emit deviceStatusChanged();
    }
}

void WlanPage::onDeviceNameUpdate(QString oldName, QString newName)
{
   if (getDefaultDevice() == oldName) {
       updateDefaultDevice(newName);
       setDefaultDevice(WIRELESS, newName);
   }

   if (devList.contains(oldName)) {
       devList.removeOne(oldName);
       devList.append(newName);
       qDebug() << "WlanPage emit deviceNameUpdate "  << oldName << newName;
       emit deviceNameChanged(oldName, newName);
   }
}

void WlanPage::onActivatedWlanChanged(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason)
{
    qDebug()<< "Activated wlan changed, uuid = " << uuid << "; state = " << state << "; reason = " << reason << Q_FUNC_INFO <<__LINE__;

    QString ssid,devName;
    NetworkManager::ConnectionSettings::ConnectionType type;

    if(m_apConnectResource->getInterfaceByUuid(devName, type, uuid)) {
        if (type != NetworkManager::ConnectionSettings::ConnectionType::Wireless) {
            qDebug() << "[WLanPage] updateLanlist but type is not Wireless";
            return;
        }
    } else {
        qDebug() << "[WlanPage] onActivatedWlanChanged but uuid is invalid";
    }

    m_resource->getSsidByUuid(uuid, ssid);

    if (state == NetworkManager::ActiveConnection::State::Activating) {
        qDebug() << "[WlanPage] wirelessActivating" << devName << ssid;
        emit wirelessActivating(devName, ssid);
    }

    if (state == NetworkManager::ActiveConnection::State::Activated || state == NetworkManager::ActiveConnection::State::Deactivated)
    {
        qDebug() << "[WlanPage] because ActivatedWlanChanged " << devName;
        emit listUpdate(devName);
    }

    if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        QList<KyApConnectItem *> apConnectItemList;
        m_apConnectResource->getApConnections(apConnectItemList);
        if (!apConnectItemList.isEmpty()) {
            foreach (auto item, apConnectItemList) {
                if (item->m_connectUuid == uuid) {
                    qDebug() << "[WlanPage] hotpot Deactivated";
                    emit hotPotDeactivated(item->m_ifaceName, ssid);
                    break;
                }
            }
        }
    }

    if (state == NetworkManager::ActiveConnection::State::Activated) {
        QList<KyApConnectItem *> apConnectItemList;
        m_apConnectResource->getApConnections(apConnectItemList);
        if (!apConnectItemList.isEmpty()) {
            foreach (auto item, apConnectItemList) {
                if (item->m_connectUuid == uuid) {
                    qDebug() << "[WlanPage] hotpot Deactivated";
                    emit hotPotActivated(item->m_ifaceName, ssid);
                    break;
                }
            }
        }
    }

    if (state == NetworkManager::ActiveConnection::State::Activated) {
        //onWlanRemoved(m_wlanDevice, ssid);
        m_activatedNetListWidget->clear();
        int height = 0;
        appendActiveWlan(ssid, height);
        onWlanRemoved(defaultDevice, ssid);
    } else {
        onWlanUpdated();
    }
}

void WlanPage::onItemHeightChanged(const QString &ssid)
{
    if (!m_itemsMap.contains(ssid)) { return; }
    QListWidgetItem *item = m_itemsMap.value(ssid);

    if (m_expandedItem && m_expandedItem != item) {
        QSize size(m_inactivatedNetListWidget->itemWidget(m_expandedItem)->size().width(), NORMAL_HEIGHT);
        //旧的收起
        m_expandedItem->setSizeHint(size);
        m_inactivatedNetListWidget->itemWidget(m_expandedItem)->setFixedHeight(NORMAL_HEIGHT);
        //新的展开
        m_expandedItem = item;
        item->setSizeHint(m_inactivatedNetListWidget->itemWidget(item)->size());
    } else if (!m_expandedItem) {
        m_expandedItem = item;
        m_inactivatedNetListWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_inactivatedNetListWidget->itemWidget(item)->height() - item->sizeHint().height());
        m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
        item->setSizeHint(m_inactivatedNetListWidget->itemWidget(item)->size());
    }
}

void WlanPage::onConnectButtonClicked(KyWirelessConnectSetting &connSettingInfo, const bool &isHidden)
{
    qDebug() << "Received signal of connecting wlan, ssid = " << connSettingInfo.m_ssid << Q_FUNC_INFO << __LINE__;
    m_wirelessConnectOpreation->addAndActiveWirelessConnect(defaultDevice, connSettingInfo, isHidden);
}

/**
 * @brief WlanPage::onWlanSwitchClicked 点击无线网开关
 */
void WlanPage::onWlanSwitchClicked()
{
    qDebug() << "On wlan switch button clicked! old state = " << !m_netSwitch->getSwitchStatus() << Q_FUNC_INFO << __LINE__;
    m_wirelessConnectOpreation->setWirelessEnabled(m_netSwitch->getSwitchStatus());
}

void WlanPage::onWlanSwitchStatusChanged(const bool &checked)
{
    m_netSwitch->setSwitchStatus(checked);
    qDebug() << "On wlan switch status changed! new state = " << m_netSwitch->getSwitchStatus() << Q_FUNC_INFO << __LINE__;
    onWlanUpdated();
}


//for dbus
void WlanPage::getWirelessList(QMap<QString, QVector<QStringList> > &map)
{
    QMap<QString,QStringList> actMap;
    m_resource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);

    QMap<QString, QList<KyWirelessNetItem> > wlanMap;
    if (!m_resource->getAllDeviceWifiNetwork(wlanMap))
    {
        return;
    }

    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = wlanMap.begin();
    while (iter != wlanMap.end()) {
        QVector<QStringList> vector;
        QString activeSsid ;
        //先是已连接
        if (actMap.contains(iter.key())) {
            KyWirelessNetItem data;
            if (m_resource->getWifiNetwork(iter.key(), actMap[iter.key()].at(0), data)) {
                vector.append(QStringList()<<data.m_NetSsid<<QString::number(data.m_signalStrength)<<data.m_secuType);
                activeSsid = data.m_NetSsid;
            }

        } else {
            vector.append(QStringList("--"));
        }
        //未连接
        foreach (auto itemData, iter.value()) {
            if (itemData.m_NetSsid == activeSsid) {
                continue;
            }
            vector.append(QStringList()<<itemData.m_NetSsid<<QString::number(itemData.m_signalStrength)<<itemData.m_secuType);
        }

        map.insert(iter.key(), vector);
        iter++;
    }

    return;
}

//开启热点
void WlanPage::activeWirelessAp(const QString apName, const QString apPassword, const QString apDevice)
{
    QString uuid;
    getApUuid(apName, apPassword, apDevice, uuid);
    m_wirelessConnectOpreation->activeWirelessAp(uuid, apName, apPassword, apDevice);
}

//断开热点
void WlanPage::deactiveWirelessAp(const QString apName, const QString apPassword, const QString apDevice)
{
    QString uuid;
    getApUuid(apName, apPassword, apDevice, uuid);
    if (!uuid.isEmpty()) {
        m_wirelessConnectOpreation->deactiveWirelessAp(apName, uuid);
    } else {
        qDebug() << "[WlanPage] deactiveWirelessAp can not find apName " << apName;
        emit deactivateFailed("invalid info");
    }
}

void WlanPage::getApUuid(const QString apName, const QString apPassword, const QString apDevice, QString &uuid)
{
    uuid.clear();
    QList<KyApConnectItem *> apConnectItemList;
    m_apConnectResource->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        foreach (auto item, apConnectItemList) {
            if (item->m_connectName == apName && item->m_password == apPassword
                    && item->m_ifaceName == apDevice) {
                uuid = item->m_connectUuid;
                break;
            }
        }
    }
    return;
}

void WlanPage::activateWireless(const QString& devName, const QString& ssid)
{
    //todo
}

void WlanPage::deactivateWireless(const QString& devName, const QString& ssid)
{
    //todo
}
