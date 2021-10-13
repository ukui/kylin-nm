#include "wlanpage.h"
#include "kywirelessnetitem.h"
#include <QEvent>
#include <QDateTime>
#include <QDebug>
#include <QSettings>
#include <QScrollBar>

WlanPage::WlanPage(QWidget *parent) : TabPage(parent)
{
    m_resource = new KyWirelessNetResource(this);
    m_connectResource = new KyActiveConnectResourse();
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
    m_netDeviceResource=new KyNetworkDeviceResourse(this);
    m_apConnectResource = new KyConnectResourse(this);
    m_devList.empty();
    initDevice();
    m_wirelessConnectOpreation = new KyWirelessConnectOperation(this);
    m_connectoperation = new KyConnectOperation(this);
    m_settingsLabel->installEventFilter(this);
    m_connectResource = new KyActiveConnectResourse();
    initWlanUI();
    //要在initUI之后调用，保证UI的信号槽顺利绑定
    initConnections();
    initTimer();
    getActiveWlan();
    getAllWlan();

    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceAdd, this, &WlanPage::onDeviceAdd);
    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceRemove, this, &WlanPage::onDeviceRemove);
    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceNameUpdate, this, &WlanPage::onDeviceNameUpdate);

    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::activateConnectionError, this, &WlanPage::activateFailed);
    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::addAndActivateConnectionError, this, &WlanPage::activateFailed);
    connect(this, &WlanPage::activateFailed, this, &WlanPage::onActiveFailed);
    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::deactivateConnectionError, this, &WlanPage::deactivateFailed);

    connect(this, &WlanPage::hiddenWlanClicked, this, &WlanPage::onHiddenWlanClicked);
    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::wifinEnabledChanged, this, &WlanPage::onWifiEnabledChanged);
}

//QString WlanPage::getSsidFromUuid(const QString &uuid)
//{

//}

bool WlanPage::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        if (w == m_hiddenWlanLabel) {
            //ZJP_TODO 打开隐藏WiFi添加弹窗
            emit this->hiddenWlanClicked();
        } else if (w == m_settingsLabel) {
            //ZJP_TODO 打开控制面板
            showControlCenter();
        }
    }
    return QWidget::eventFilter(w,e);
}

void WlanPage::initWlanUI()
{
    m_titleLabel->setText(tr("WLAN"));
    m_activatedNetLabel->setText(tr("Activated WLAN"));
    m_inactivatedNetLabel->setText(tr("Other WLAN"));

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
    m_inactivatedNetListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);     //用了SCrollArea的滚动条

    m_hiddenWlanWidget = new QFrame(m_inactivatedWlanListAreaCentralWidget);
    m_hiddenWlanLayout = new QHBoxLayout(m_hiddenWlanWidget);
    m_hiddenWlanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_hiddenWlanLabel = new QLabel(m_hiddenWlanWidget);
    m_hiddenWlanLabel->setText(tr("More..."));
    m_hiddenWlanLabel->setContentsMargins(MORE_TEXT_MARGINS);
    m_hiddenWlanLabel->installEventFilter(this);
    m_hiddenWlanLabel->setCursor(Qt::PointingHandCursor);
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

    m_tipsLabel->hide(); //设备关闭的label，要用的话可以放出来
}

/**
 * @brief WlanPage::initConnections 初始化一些信号槽
 */
void WlanPage::initConnections()
{
    connect(m_resource, &KyWirelessNetResource::wifiNetworkAdd, this, &WlanPage::onWlanAdded);
    connect(m_resource, &KyWirelessNetResource::wifiNetworkAdd, this, [=](QString interface, KyWirelessNetItem &item){
        //for dbus
        QStringList info;
        info <<item.m_NetSsid<<QString::number(item.m_signalStrength)<<item.m_secuType;
        emit wlanAdd(interface, info);
    });
    connect(m_resource, &KyWirelessNetResource::wifiNetworkRemove, this, &WlanPage::onWlanRemoved);
    connect(m_resource, &KyWirelessNetResource::wifiNetworkRemove, this, &WlanPage::wlanRemove);
//    connect(m_resource, &KyWirelessNetResource::signalStrengthChange, this, &WlanPage::signalStrengthChange);

//    connect(m_resource, &KyWirelessNetResource::signalStrengthChange, this, &WlanPage::onsignalStrengthChange);

    connect(m_resource, &KyWirelessNetResource::secuTypeChange, this, &WlanPage::secuTypeChange);

//    connect(m_resource, &KyWirelessNetResource::wifiNetworkUpdate, this, &WlanPage::onWlanUpdated);
    connect(m_connectResource, &KyActiveConnectResourse::stateChangeReason, this, &WlanPage::onActivatedWlanChanged);
//    connect(m_connectoperation, &KyConnectOperation::activateConnectionError, this, &WlanPage::showDesktopNotify);
//    connect(m_connectoperation, &KyConnectOperation::createConnectionError, this, &WlanPage::showDesktopNotify);
//    connect(m_connectoperation, &KyConnectOperation::deactivateConnectionError, this, &WlanPage::showDesktopNotify);
//    connect(m_connectoperation, &KyConnectOperation::deleteConnectionError, this, &WlanPage::showDesktopNotify);
    connect(m_netSwitch, &SwitchButton::clicked, this, &WlanPage::onWlanSwitchClicked);
    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        m_switchGsettings = new QGSettings(GSETTINGS_SCHEMA);
        if (m_switchGsettings->keys().contains(WIRELESS_SWITCH)) {
            m_netSwitch->setSwitchStatus(m_switchGsettings->get(WIRELESS_SWITCH).toBool());
            connect(m_switchGsettings, &QGSettings::changed, this, [ = ](const QString &key) {
                if (key == WIRELESS_SWITCH) {
                    bool status = m_switchGsettings->get(WIRELESS_SWITCH).toBool();
                    if (!status) {
//                        m_deviceFrame->hide();
                        m_activatedNetFrame->hide();
                        m_inactivatedNetFrame->hide();
                        m_activatedNetDivider->hide();
                        m_inactivatedNetDivider->hide();
                    } else {
//                        m_deviceFrame->show();
                        m_activatedNetFrame->show();
                        m_inactivatedNetFrame->show();
                        m_activatedNetDivider->show();
                        m_inactivatedNetDivider->show();
                    }
                    m_wirelessConnectOpreation->setWirelessEnabled(status);
                    m_netSwitch->setSwitchStatus(m_switchGsettings->get(WIRELESS_SWITCH).toBool());
                    onWlanSwitchStatusChanged(m_switchGsettings->get(WIRELESS_SWITCH).toBool());
                }
            });
            return;
        }
    }
    qDebug()<<"isSchemaInstalled false" << Q_FUNC_INFO << __LINE__;
    m_netSwitch->setSwitchStatus(m_wirelessConnectOpreation->getWirelessEnabled());
}

void WlanPage::initTimer()
{
    m_scanTimer = new QTimer(this);
    m_refreshIconTimer = new QTimer(this);
    connect(m_scanTimer, &QTimer::timeout, this, &WlanPage::requestScan);
    connect(m_refreshIconTimer, &QTimer::timeout, this, &WlanPage::updateByStrength);
}

/**
 * @brief WlanPage::initDevice 初始化默认网卡
 */
void WlanPage::initDevice()
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    QString key("wireless");
    QString deviceName;
    m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, m_devList);
    if (!m_devList.isEmpty()) {
        deviceName = m_devList.at(0);
        m_settings->setValue(key, deviceName);
    } else {
        qDebug() << "have no device to use "  << Q_FUNC_INFO << __LINE__;
        //检测不到无线网卡不再触发click信号
        m_netSwitch->setSwitchStatus(false);
        m_netSwitch->setEnabled(false);
    }
    updateDefaultDevice(deviceName);
    qDebug() << "[WlanPage] initDevice defaultDevice = " << deviceName;
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;
    //获取完m_devList后调用，减少重复获取
    initDeviceCombox();
}

void WlanPage::initDeviceCombox()
{
    //TODO 获取设备列表，单设备时隐藏下拉框，多设备时添加到下拉框
    disconnect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &WlanPage::onDeviceComboxIndexChanged);
    if (m_devList.length() <= 1) {
        m_deviceFrame->hide();
        foreach (QString device, m_devList) {
            m_deviceComboBox->addItem(device, device);
        }
    } else {
        m_deviceFrame->show();
        foreach (QString device, m_devList) {
            //空时addItem 会触发currentIndexChanged
            m_deviceComboBox->addItem(device, device);
        }
        int index = m_deviceComboBox->findData(m_defaultDevice);
        qDebug() << index;
        m_deviceComboBox->setCurrentIndex(index);
    }
    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &WlanPage::onDeviceComboxIndexChanged);

}

/**
 * @brief WlanPage::getActiveWlan 获取所有已激活连接
 */
void WlanPage::getActiveWlan()
{
    if (!m_activatedNetListWidget) {
        return;
    }
    QMap<QString,QStringList> actMap;
    m_activatedNetListWidget->clear();
    m_resource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);
    QMap<QString,QStringList>::iterator iter = actMap.begin();
    int height = 0;
    while (iter != actMap.end()) {
        if (iter.key() == m_defaultDevice && !iter.value().isEmpty()) {
            QString uuid = iter.value().at(0);
            appendActiveWlan(uuid, height);
            break;
        }
        iter ++;
    }
    if (height > 0) {
        m_activatedNetListWidget->setFixedHeight(height);
    } else {
        //未连接任何WiFi的情况
        m_activatedWlanSSid.clear();
        m_activatedWlanUuid.clear();
        WlanListItem *wlanItemWidget = new WlanListItem();
        connect(wlanItemWidget, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
        connect(wlanItemWidget, &WlanListItem::connectButtonClicked, this, &WlanPage::onConnectButtonClicked);
        qDebug() << "There is no activated wlan." << Q_FUNC_INFO << __LINE__ ;
        QListWidgetItem *wlanItem = new QListWidgetItem();
        wlanItem->setSizeHint(QSize(m_activatedNetListWidget->width(), wlanItemWidget->height()));
        m_activatedNetListWidget->addItem(wlanItem);
//        emit this->wlanConnectChanged();
        m_activatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
        height += wlanItemWidget->height();
    }
}

void WlanPage::appendActiveWlan(const QString &uuid, int &height)
{
    qDebug() << "appendActiveWlan" << uuid;
    m_activatedWlanUuid = uuid;
    m_resource->getSsidByUuid(uuid, m_activatedWlanSSid, m_defaultDevice);

    KyWirelessNetItem data;
    if (!m_resource->getWifiNetwork(m_defaultDevice, m_activatedWlanSSid, data)) {
        qWarning() << "Get activated wlan failed! ssid = " << m_activatedWlanSSid <<"; device = " << m_defaultDevice << "; uuid = " << m_activatedWlanUuid;
        return;
    }
    KyWirelessNetItem *item_data = new KyWirelessNetItem(data);
    WlanListItem *wlanItemWidget = new WlanListItem(m_resource, item_data, m_defaultDevice);
    connect(wlanItemWidget, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
    connect(wlanItemWidget, &WlanListItem::connectButtonClicked, this, &WlanPage::onConnectButtonClicked);
    qDebug() << "Activated wlan: ssid = " << item_data->m_NetSsid;
    QListWidgetItem *wlanItem = new QListWidgetItem();
    wlanItem->setSizeHint(QSize(m_activatedNetListWidget->width(), wlanItemWidget->height()));
    m_activatedNetListWidget->addItem(wlanItem);
    qDebug() << "[wlanpage]emit wlanConnectChanged()" << Q_FUNC_INFO << __LINE__ ;
    m_activatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
    wlanItemWidget->setActive(true);

    height += wlanItemWidget->height();
}

/**
 * @brief WlanPage::getAllWlan 获取所有Wlan //ZJP_TODO 需要针对网卡进行筛选
 */
void WlanPage::getAllWlan()
{
    if (!m_inactivatedNetListWidget) {
        return;
    }
    qDebug() << "Started loading wlan list!" << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");
    m_inactivatedNetListWidget->clear();
    m_itemsMap.clear();
    m_expandedItem = nullptr; //每次重绘整个列表时要清空扩展项，否则会导致再展开某一项item的时候进程崩溃
    QList<KyWirelessNetItem> wlanList;
//    if (!m_resource->getAllDeviceWifiNetwork(map))
    if (!m_resource->getDeviceWifiNetwork(m_defaultDevice, wlanList)) { //ZJP_TODO 获取默认网卡并传入
        return;
    }
    int height = 0;
    foreach (auto itemData, wlanList) {
        if (itemData.m_NetSsid == this->m_activatedWlanSSid) {
            continue;
        }
        KyWirelessNetItem *data = new KyWirelessNetItem(itemData);
        WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data, m_defaultDevice);
        QListWidgetItem *wlanItem = new QListWidgetItem();
        qDebug() << itemData.m_NetSsid << itemData.m_isConfigured;
        connect(wlanItemWidget, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
        connect(wlanItemWidget, &WlanListItem::connectButtonClicked, this, &WlanPage::onConnectButtonClicked);
        QPair<QListWidgetItem*,WlanListItem*> pair (wlanItem, wlanItemWidget);
        m_itemsMap.insert(data->m_NetSsid, pair);
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
    qDebug() << "A Wlan Added! interface = " << interface << "; ssid = " << item.m_NetSsid << "interface" << interface << Q_FUNC_INFO <<__LINE__;
    if (interface != m_defaultDevice) {
        qDebug() << "wlan add interface not equal defaultdevice,ignore";
        return;
    }
    KyWirelessNetItem *data = new KyWirelessNetItem(item);
    WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data, m_defaultDevice);
    connect(wlanItemWidget, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
    connect(wlanItemWidget, &WlanListItem::connectButtonClicked, this, &WlanPage::onConnectButtonClicked);

    QListWidgetItem *wlanItem = new QListWidgetItem();      //不要指定父对象，否则insert会出错
    wlanItem->setSizeHint(QSize(m_inactivatedNetListWidget->width(), wlanItemWidget->height()));
    int index = getIndexByStrength(interface, data->m_NetSsid);

    m_inactivatedNetListWidget->insertItem(index, wlanItem);
    m_inactivatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);

//    m_inactivatedNetListWidget->addItem(wlanItem); //ZJP_TODO 目前会添加到列表尾部
    m_inactivatedNetListWidget->setFixedHeight(m_inactivatedNetListWidget->height() + wlanItemWidget->height() + NET_LIST_SPACING);
    m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
    QPair<QListWidgetItem*,WlanListItem*> pair (wlanItem, wlanItemWidget);
    m_itemsMap.insert(data->m_NetSsid, pair);
}

int WlanPage::getIndexByStrength(QString interface, QString ssid)
{
    QList<KyWirelessNetItem> wlanList;
    m_resource->getDeviceWifiNetwork(interface, wlanList);
    for (int i = 0; i< wlanList.count(); i++) {
        if (wlanList.at(i).m_NetSsid == ssid) {
            qDebug() << "append" << wlanList.at(i).m_NetSsid << i;
            return i;
        }
    }
}

void WlanPage::onWlanRemoved(QString interface, QString ssid)
{
    if (!m_itemsMap.contains(ssid)) { return; }
    if (m_expandedItem == (m_itemsMap.value(ssid)).first) { m_expandedItem = nullptr; }
    qDebug() << "A Wlan Removed! interface = " << interface << "; ssid = " << ssid << Q_FUNC_INFO <<__LINE__;
    if (interface != m_defaultDevice) {
        qDebug() << "wlan remove interface not equal defaultdevice,ignore";
    }
    if (m_itemsMap.value(ssid).second) {
        m_inactivatedNetListWidget->takeItem(m_inactivatedNetListWidget->row((m_itemsMap.value(ssid)).first));
        int height = m_itemsMap.value(ssid).second->height();
        m_inactivatedNetListWidget->setFixedHeight(m_inactivatedNetListWidget->height() - height - NET_LIST_SPACING);
        m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
    }
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
    m_netSwitch->setEnabled(true);
    m_netSwitch->setSwitchStatus(true);
    m_devList << deviceName;
    if (getDefaultDevice().isEmpty())
    {
        m_devList.clear();
        updateDefaultDevice(deviceName);
        setDefaultDevice(WIRELESS, deviceName);

    }

    //往下拉框添加新的网卡
    if (m_deviceComboBox->findData(deviceName) == -1) {
        if (m_devList.length() > 1 && !m_deviceFrame->isVisible()) {
            m_deviceFrame->show();
        }
        m_deviceComboBox->addItem(deviceName, deviceName);
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
        } else {
            m_netSwitch->setSwitchStatus(false);
            m_netSwitch->setEnabled(false);
        }
        updateDefaultDevice(newDefaultDevice);
        setDefaultDevice(WIRELESS, newDefaultDevice);
    }

    if (m_devList.contains(deviceName)) {
        m_devList.removeOne(deviceName);
        emit deviceStatusChanged();
    }

    //从下拉框删除已消失的网卡
    if (m_deviceComboBox->findData(deviceName) != -1) {
        if (m_devList.length() <= 1 && m_deviceFrame->isVisible()) {
            m_deviceFrame->hide();
        }
        m_deviceComboBox->removeItem(m_deviceComboBox->findData(deviceName));
    }

}

void WlanPage::onDeviceNameUpdate(QString oldName, QString newName)
{
   if (getDefaultDevice() == oldName) {
       updateDefaultDevice(newName);
       setDefaultDevice(WIRELESS, newName);
   }

   if (m_devList.contains(oldName)) {
       m_devList.removeOne(oldName);
       m_devList.append(newName);
       qDebug() << "WlanPage emit deviceNameUpdate "  << oldName << newName;
       emit deviceNameChanged(oldName, newName);
   }

   if (m_deviceComboBox->findData(oldName) != -1) {
       m_deviceComboBox->removeItem(m_deviceComboBox->findData(oldName));
       m_deviceComboBox->addItem(newName, newName);
   }
}

void WlanPage::onActivatedWlanChanged(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason)
{
    qDebug()<< "Activated wlan changed, uuid = " << uuid << "; state = " << state << "; reason = " << reason << Q_FUNC_INFO <<__LINE__;

    //wlanpage函数内持续监听连接状态的变化并记录供其他函数调用获取状态
    QString devName,ssid;
    NetworkManager::ConnectionSettings::ConnectionType type;
    if(m_apConnectResource->getInterfaceByUuid(devName, type, uuid)) {
        if (type != NetworkManager::ConnectionSettings::ConnectionType::Wireless) {
            return;
        }
    }
    if(NetworkManager::ActiveConnection::State::Activated == state){
        m_wlanIsConnected = true;
        qDebug() << "[wlanpage] wlanIsConnected status : "  << m_wlanIsConnected << Q_FUNC_INFO << __LINE__ ;
        emit this->wlanConnectChanged();
    } else {
        m_wlanIsConnected = false;
        qDebug() << "[wlanpage] wlanIsConnected status : "  << m_wlanIsConnected << Q_FUNC_INFO << __LINE__ ;
        emit this->wlanConnectChanged();
    }

    //弹窗显示wifi连接状况
    qDebug() << "[WlanPage] State changed to :"  << state <<  reason << Q_FUNC_INFO <<__LINE__;
    //QString ssid;
    QString device = m_defaultDevice;
    m_resource->getSsidByUuid(uuid, ssid, device);
    if (m_activatedWlanSSid == ssid && state == NetworkManager::ActiveConnection::State::Activated) {
        if(m_activedssid != m_activatedWlanSSid){
            m_activedssid = m_activatedWlanSSid;
            qDebug() << "[WlanPage] State changed to :" << state << ssid <<  reason << Q_FUNC_INFO <<__LINE__;
            this->showDesktopNotify(tr("WLAN Connected Successfully"));
        }
    }
    if (state == NetworkManager::ActiveConnection::State::Deactivating){
        m_disconnectuuid = uuid;
        m_disconnectingflag = true;
    }
    if (state == NetworkManager::ActiveConnection::State::Deactivated && m_disconnectuuid == uuid){
        if(m_disconnectingflag){
            m_disconnectingflag = false;
            this->showDesktopNotify(tr("WLAN Disconnected Successfully"));
            qDebug() << "clear the activessid"  << Q_FUNC_INFO <<__LINE__;
            m_activedssid = "";
        }
    }

    //QString ssid,devName;
    //NetworkManager::ConnectionSettings::ConnectionType type;

    if(m_apConnectResource->getInterfaceByUuid(devName, type, uuid)) {
        if (type != NetworkManager::ConnectionSettings::ConnectionType::Wireless) {
            qDebug() << "[WLanPage] updateLanlist but type is not Wireless";
            return;
        }
    } else {
        qDebug() << "[WlanPage] onActivatedWlanChanged but uuid is invalid";
    }

    m_resource->getSsidByUuid(uuid, ssid, devName);

    qDebug() << "emit wlanActiveConnectionStateChanged" << devName << ssid << state;
    emit wlanActiveConnectionStateChanged(devName, ssid,uuid, state);

    if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        QList<KyApConnectItem *> apConnectItemList;
        m_apConnectResource->getApConnections(apConnectItemList);
        if (!apConnectItemList.isEmpty()) {
            foreach (auto item, apConnectItemList) {
                if (item->m_connectUuid == uuid) {
                    qDebug() << "[WlanPage] hotspot Deactivated";
                    emit hotspotDeactivated(item->m_ifaceName, ssid);
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
                    qDebug() << "[WlanPage] hotspot Deactivated";
                    emit hotspotActivated(item->m_ifaceName, ssid);
                    break;
                }
            }
        }
    }

    if (state == NetworkManager::ActiveConnection::State::Activated && m_defaultDevice == devName) {
        //onWlanRemoved(m_wlanDevice, ssid);
        m_activatedNetListWidget->clear();
        int height = 0;
        appendActiveWlan(uuid, height);
//        emit this->wlanConnectChanged();
        onWlanRemoved(m_defaultDevice, ssid);
//        this->showDesktopNotify(tr("Connect WLAN succeed"));
    } else if (state == NetworkManager::ActiveConnection::State::Deactivated && (uuid.isEmpty() || (!uuid.isEmpty() && uuid == m_activatedWlanUuid))) {
        QString oldActWlanSsid = m_activatedWlanSSid;
        getActiveWlan();
        QString newActWlanSsid = m_activatedWlanSSid;
        qDebug() << "Refresh active wlan succeed, old ssid = " << oldActWlanSsid << "; new ssid = " << newActWlanSsid << Q_FUNC_INFO << __LINE__;
        if (oldActWlanSsid != newActWlanSsid) {
            if (!oldActWlanSsid.isEmpty()) {
                KyWirelessNetItem item;
                if(m_resource->getWifiNetwork(m_defaultDevice, oldActWlanSsid, item)) {
                    onWlanAdded(m_defaultDevice, item);
                }
            }
            if (!newActWlanSsid.isEmpty()) {
                onWlanRemoved(m_defaultDevice, newActWlanSsid);
            }
        }
//        this->showDesktopNotify(tr("Disconnect WLAN succeed"));
    } else {
        qDebug() << "Unexpected wlan state, will do nothing." << Q_FUNC_INFO << __LINE__;
    }
}

void WlanPage::onItemHeightChanged(const QString &ssid)
{
    if (!m_itemsMap.contains(ssid)) { return; }
    QListWidgetItem *item = (m_itemsMap.value(ssid)).first;

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
    qDebug() << "Received signal of connecting wlan, ssid = " << connSettingInfo.m_ssid << "device name = " << m_defaultDevice << Q_FUNC_INFO << __LINE__;
    m_wirelessConnectOpreation->addAndActiveWirelessConnect(m_defaultDevice, connSettingInfo, isHidden);
}

/**
 * @brief WlanPage::onWlanSwitchClicked 点击无线网开关
 */
void WlanPage::onWlanSwitchClicked()
{
    qDebug() << "On wlan switch button clicked! old state = " << !m_netSwitch->getSwitchStatus() << Q_FUNC_INFO << __LINE__;
    //应该先检测是否有无线网卡可用，才改变开关状态
    m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, m_devList);
    if (m_devList.isEmpty()) {
        qDebug() << "have no device to use "  << Q_FUNC_INFO << __LINE__;
        //检测不到无线网卡不再触发click信号
        m_netSwitch->setSwitchStatus(false);
        m_netSwitch->setEnabled(false);
    }else{
        m_wirelessConnectOpreation->setWirelessEnabled(m_netSwitch->getSwitchStatus());
    }
}

void WlanPage::onWlanSwitchStatusChanged(const bool &checked)
{
    qDebug() << "On wlan switch status changed! new state = " << m_netSwitch->getSwitchStatus() << Q_FUNC_INFO << __LINE__;
    onWlanUpdated();
}

void WlanPage::onDeviceComboxIndexChanged(int currentIndex)
{
    if (!m_deviceComboBox || currentIndex < 0) {
        return;
    }
    //TODO 设备变更时更新设备和列表
    QString currentDevice = m_deviceComboBox->itemText(currentIndex);
    qDebug() << "Current device changed! device = " << currentDevice << Q_FUNC_INFO << __LINE__;
    m_defaultDevice = currentDevice;
    getActiveWlan();
    getAllWlan();
}

//申请触发扫描，初始化执行&定时执行
void WlanPage::requestScan()
{
    if (!m_wirelessConnectOpreation) {
        qWarning() << "Scan failed! m_wirelessConnectOpreation is nullptr!" << Q_FUNC_INFO << __LINE__;
        return;
    }
    m_wirelessConnectOpreation->requestWirelessScan();
}

void WlanPage::onHiddenWlanClicked()
{
    qDebug() << "[wlanPage] AddHideWifi Clicked! " << Q_FUNC_INFO << __LINE__ ;
    NetDetail *netDetail = new NetDetail(m_defaultDevice, "", "", false, true, true, this);
    netDetail->show();
}

void WlanPage::showControlCenter()
{
    QProcess process;
    process.startDetached("ukui-control-center --wlanconnect");
}
void WlanPage::onWifiEnabledChanged(bool isWifiOn)
{
//监听外部命令导致wifi状态变化，更新界面
    //应该先检测是否有无线网卡可用，才改变开关状态
    m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, m_devList);
    if (m_devList.isEmpty()) {
        qDebug() << "have no device to use "  << Q_FUNC_INFO << __LINE__;
        return;
    }
    if (m_netSwitch->getSwitchStatus() == isWifiOn) {
        return;
    } else {
        //m_wirelessConnectOpreation->setWirelessEnabled(isWifiOn);
        m_netSwitch->setSwitchStatus(isWifiOn);
        //外部命令导致连接状态发生变化，通知主界面刷新图标
        emit this->wlanConnectChanged();
    }
}

void WlanPage::onActiveFailed(QString errorMessage)
{
    qDebug() << "active failed and the message is: " << errorMessage << Q_FUNC_INFO << __LINE__;
}

void WlanPage::updateByStrength()
{
    if (m_expandedItem) {
        qDebug() << "Has expanded item and forbid refresh wifi strength" << Q_FUNC_INFO << __LINE__;
        return;
    }
    qDebug() << "Will update Wlan list by strength." << Q_FUNC_INFO << __LINE__;
    QList<KyWirelessNetItem> wlanList;
    if (!m_resource->getDeviceWifiNetwork(m_defaultDevice, wlanList)) {
        return;
    }
//    int height = 0;
    int currentRow = 0;
    for (int i = 0; i < wlanList.length(); i ++) {
        if (wlanList.at(i).m_NetSsid == this->m_activatedWlanSSid) { //排除已连接WiFi
            continue;
        }
        QString currentSsid = wlanList.at(i).m_NetSsid; //应该在第currentRow行的新的WiFi名称
        QModelIndex currentIndex = m_inactivatedNetListWidget->model()->index(currentRow, 0, QModelIndex()); //当前行的index
        WlanListItem * lastWlan = static_cast<WlanListItem *>(m_inactivatedNetListWidget->indexWidget(currentIndex)); //原来在第currentRow行的WlanListItem
        if (lastWlan && currentSsid == lastWlan->getSsid()) { //WiFi的排序未改变，不需要修改，继续遍历
            currentRow ++;
            continue;
        } else if (!lastWlan) { //已经超出原列表长度了，需要new一个放上去
            KyWirelessNetItem *data = new KyWirelessNetItem(wlanList.at(i));
            WlanListItem *wlanItemWidget = new WlanListItem(m_resource, data, m_defaultDevice);
            QListWidgetItem *wlanItem = new QListWidgetItem();
            connect(wlanItemWidget, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
            connect(wlanItemWidget, &WlanListItem::connectButtonClicked, this, &WlanPage::onConnectButtonClicked);
            QPair<QListWidgetItem*,WlanListItem*> pair (wlanItem, wlanItemWidget);
            m_itemsMap.insert(data->m_NetSsid, pair);
            wlanItem->setSizeHint(QSize(m_inactivatedNetListWidget->width(), wlanItemWidget->height()));
            m_inactivatedNetListWidget->addItem(wlanItem);
            m_inactivatedNetListWidget->setItemWidget(wlanItem, wlanItemWidget);
            int height = m_inactivatedNetListWidget->height() + wlanItemWidget->height() + NET_LIST_SPACING;
            m_inactivatedNetListWidget->setFixedHeight(height);
            m_inactivatedWlanListAreaCentralWidget->setFixedHeight(m_inactivatedNetListWidget->height() + m_hiddenWlanLabel->height());
        } else {//找到了该位置的wifi而且与现在的排序不符，需要调整
            KyWirelessNetItem *data = new KyWirelessNetItem(wlanList.at(i));
            WlanListItem * currentWlan = new WlanListItem(m_resource, data, m_defaultDevice);
            connect(currentWlan, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
            connect(currentWlan, &WlanListItem::connectButtonClicked, this, &WlanPage::onConnectButtonClicked);
            QPair<QListWidgetItem*, WlanListItem*> newPair;
            newPair.first = m_itemsMap.value(lastWlan->getSsid()).first;
            newPair.second = currentWlan;
            m_itemsMap[currentSsid] = newPair;//先把map中的当前行wlan替换掉,第currentRow行的ssid->currentRow行的控件
            m_itemsMap[lastWlan->getSsid()] = newPair;//临时使原来指向第currentRow行的的wlan依然指向这个value，防止访问空指针的情况发生，等遍历到该ssid时会赋予其正确的value
            m_inactivatedNetListWidget->setIndexWidget(currentIndex, currentWlan); //最后把新的wlan填充到currentRow行
        }
        currentRow ++;
    }
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
            qDebug() << "find " <<iter.key();
            KyWirelessNetItem data;
            QString ssid ="";
            QString devName = iter.key();
            m_resource->getSsidByUuid(actMap[iter.key()].at(0), ssid, devName);
            if (m_resource->getWifiNetwork(iter.key(), ssid, data)) {
                qDebug() << data.m_NetSsid << data.m_signalStrength << data.m_secuType << data.m_connectUuid;
                vector.append(QStringList()<<data.m_NetSsid<<QString::number(data.m_signalStrength)<<data.m_secuType<<data.m_connectUuid);
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
    QString uuid("");
    QList<KyApConnectItem *> apConnectItemList;
    m_apConnectResource->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        uuid = apConnectItemList.at(0)->m_connectUuid;
    }
    m_wirelessConnectOpreation->activeWirelessAp(uuid, apName, apPassword, apDevice);
}

//断开热点
void WlanPage::deactiveWirelessAp(const QString apName, const QString apPassword, const QString apDevice)
{
    QString uuid("");
    QList<KyApConnectItem *> apConnectItemList;
    m_apConnectResource->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        foreach (auto item, apConnectItemList) {
            if (apName == item->m_connectName && apPassword == item->m_password && apDevice == item->m_ifaceName) {
                uuid = item->m_connectUuid;
                qDebug() << "[WlanPage] deactiveWirelessAp uuid = " << uuid;
                break;
            }
        }
    }
    if (!uuid.isEmpty()) {
        m_wirelessConnectOpreation->deactiveWirelessAp(apName, uuid);
    } else {
        qDebug() << "[WlanPage] deactiveWirelessAp can not find apName " << apName;
        emit deactivateFailed("Deactivate hotspot failed.Don't exist " + apName);
    }
}

void WlanPage::getStoredApInfo(QStringList &list)
{
    list.clear();
    QList<KyApConnectItem *> apConnectItemList;
    m_apConnectResource->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        list << apConnectItemList.at(0)->m_connectName;
        list << apConnectItemList.at(0)->m_password;
        list << apConnectItemList.at(0)->m_ifaceName;
        list << (apConnectItemList.at(0)->m_isActivated? "true":"false");
    }
}

void WlanPage::activateWireless(const QString& devName, const QString& ssid)
{
    KyWirelessNetItem data;
    if (!m_resource->getWifiNetwork(devName, ssid, data)) {
        qDebug() << "no such wifi " << ssid << " in " << devName;
        return;
    }

    if (data.m_isConfigured) {
        m_wirelessConnectOpreation->activeWirelessConnect(devName, data.m_connectUuid);
    } else {
        //todo: 显示界面输入密码 （无需密码的wifi？）
    }
}

void WlanPage::deactivateWireless(const QString& devName, const QString& ssid)
{
    KyWirelessNetItem data;
    if (!m_resource->getWifiNetwork(devName, ssid, data)) {
        qDebug() << "no such wifi " << ssid << " in " << devName;
        return;
    }

    QMap<QString,QStringList> actMap;
    m_resource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);
    QMap<QString,QStringList>::iterator iter = actMap.begin();
    if (!actMap.contains(devName)) {
        qDebug() << "no such device" << devName;
        return;
    }

    QString uuid = actMap[devName].at(0);
    QString actSsid;
    QString devInterface = devName;
    m_resource->getSsidByUuid(uuid, actSsid, devInterface);

    if (actSsid != ssid) {
        qDebug() << "no such actived wifi" << ssid;
        return;
    }

    qDebug() << "deActivateWirelessConnection" << devName << ssid;
    m_wirelessConnectOpreation->deActivateWirelessConnection(data.m_connName, data.m_connectUuid);
}

void WlanPage::onMainWindowVisibleChanged(const bool &visible)
{
    qDebug() << "Received signal of mainwindow visible changed. cur_state = " << visible << Q_FUNC_INFO << __LINE__;
    if (visible) {
        //打开页面时先触发一次扫描
        requestScan();
    }

    if (!m_scanTimer) {
        qWarning() << "No QTimer!" << Q_FUNC_INFO << __LINE__;
        return;
    }
    //若页面打开，开始扫描倒计时，若关闭，停止扫描倒计时
    if (visible) {
        qWarning() << "start refresh Timer" << Q_FUNC_INFO << __LINE__;
        m_scanTimer->start(20 * 1000);
        m_refreshIconTimer->start(5*1000);
    } else {
        m_scanTimer->stop();
        m_refreshIconTimer->stop();
    }
}

void WlanPage::showDetailPage(QString devName, QString ssid)
{
    KyWirelessNetItem data;
    bool isActive;
    if (!m_resource->getWifiNetwork(devName, ssid, data)) {
        qDebug()<<"[WlanPage] " << ssid << " is missing when showDetailPage";
        return;
    }

    QMap<QString,QStringList> actMap;
    m_resource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);
    if (actMap.empty() || !actMap.contains(devName)) {
        qDebug()<<"[WlanPage] " << devName << " is missing in active device when showDetailPage";
        isActive = false;
    } else {
        QString actSsid;
        m_resource->getSsidByUuid(actMap[devName].at(0), actSsid, devName);
        isActive = !actSsid.compare(ssid);
    }

    NetDetail *netDetail = new NetDetail(devName, ssid, data.m_connectUuid, isActive, true, true, this);
    netDetail->show();
}

