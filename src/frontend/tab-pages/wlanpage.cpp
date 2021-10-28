#include "wlanpage.h"
#include "kywirelessnetitem.h"
#include <QEvent>
#include <QDateTime>
#include <QDebug>
#include <QSettings>
#include <QScrollBar>

#define AP_SCAN_INTERVAL (20*1000)
#define ICON_REFRESH_INTERVAL (5*1000)
#define EMPTY_SSID "emptyssid"
#define LOG_FLAG  "[WlanPage]"

WlanPage::WlanPage(QWidget *parent) : TabPage(parent)
{
    m_wirelessNetResource = new KyWirelessNetResource(this);
    m_activatedConnectResource = new KyActiveConnectResourse(this);
    m_netDeviceResource=new KyNetworkDeviceResourse(this);
    m_connectResource = new KyConnectResourse(this);
    m_wirelessConnectOpreation = new KyWirelessConnectOperation(this);

    initDevice();
    initWlanUI();
    initWlanSwitchState();
    initDeviceCombox();
    initWlanArea();

    initTimer();

    connect(m_wirelessNetResource, &KyWirelessNetResource::wifiNetworkAdd, this, &WlanPage::onWlanAdded);
    connect(m_wirelessNetResource, &KyWirelessNetResource::wifiNetworkRemove, this, &WlanPage::onWlanRemoved);
    connect(m_wirelessNetResource, &KyWirelessNetResource::signalStrengthChange, this, &WlanPage::signalStrengthChange);
    connect(m_wirelessNetResource, &KyWirelessNetResource::secuTypeChange, this, &WlanPage::onSecurityTypeChange);
    connect(m_wirelessNetResource, &KyWirelessNetResource::connectionAdd, this, &WlanPage::onConnectionAdd);
    connect(m_wirelessNetResource, &KyWirelessNetResource::connectionRemove, this, &WlanPage::onConnectionRemove);

    connect(m_activatedConnectResource, &KyActiveConnectResourse::stateChangeReason,
                                                    this, &WlanPage::onConnectionStateChanged);

    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceAdd, this, &WlanPage::onDeviceAdd);
    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceRemove, this, &WlanPage::onDeviceRemove);
    connect(m_netDeviceResource, &KyNetworkDeviceResourse::deviceNameUpdate, this, &WlanPage::onDeviceNameUpdate);

    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::activateConnectionError, this, &WlanPage::activateFailed);
    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::addAndActivateConnectionError, this, &WlanPage::activateFailed);
    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::deactivateConnectionError, this, &WlanPage::deactivateFailed);

    connect(m_wirelessConnectOpreation, &KyWirelessConnectOperation::wifiEnabledChanged, this, &WlanPage::onWifiEnabledChanged);
}

bool WlanPage::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        if (w == m_settingsLabel) {
            //ZJP_TODO 打开控制面板
            qDebug() << LOG_FLAG <<"recive event show control center";
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
    m_inactivatedNetListArea->setBackgroundRole(QPalette::Base);
    m_inactivatedNetListArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_inactivatedWlanListAreaLayout = new QVBoxLayout(m_inactivatedNetListArea);
    m_inactivatedWlanListAreaLayout->setSpacing(MAIN_LAYOUT_SPACING);
    m_inactivatedWlanListAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListArea->setLayout(m_inactivatedWlanListAreaLayout);

    m_inactivatedNetListWidget = new QListWidget(m_inactivatedNetListArea);
    m_inactivatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_inactivatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_inactivatedNetListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    addWlanMoreItem();

    m_inactivatedWlanListAreaLayout->addWidget(m_inactivatedNetListWidget);

    m_activatedNetListWidget = new QListWidget(m_activatedNetFrame);
    m_activatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_activatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_activatedNetListWidget->setFixedHeight(NORMAL_HEIGHT);
    m_activatedNetListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_activatedNetLayout->addWidget(m_activatedNetListWidget);

    m_settingsLabel->installEventFilter(this);
}


void WlanPage::onWlanSwithGsettingsChanged(const QString &key)
{
    if (key == WIRELESS_SWITCH) {
        m_wlanSwitchEnable = m_switchGsettings->get(WIRELESS_SWITCH).toBool();
        qDebug() << LOG_FLAG << "wlan switch state" << m_wlanSwitchEnable;
        m_netSwitch->setSwitchStatus(m_wlanSwitchEnable);
        initDeviceCombox();
        initWlanArea();
    }

    return;
}

/**
 * @brief WlanPage::onWlanSwitchClicked 点击无线网开关
 */
void WlanPage::onWlanSwitchClicked()
{
    qDebug() <<"[WlanPage] On wlan switch button clicked! old state = "
            << !m_netSwitch->getSwitchStatus() << Q_FUNC_INFO << __LINE__;

    if (m_devList.isEmpty()) {
        qDebug() << "[WlanPage] have no device to use "  << Q_FUNC_INFO << __LINE__;
        showDesktopNotify(tr("No wireless network card detected"));
        //检测不到无线网卡不再触发click信号
        m_netSwitch->setSwitchStatus(false);
        //m_netSwitch->setEnabled(false);
    } else {
        bool switchEnable = m_netSwitch->getSwitchStatus();
        if (m_wlanSwitchEnable != switchEnable) {
            qDebug()<< LOG_FLAG << "wlan switch state " << switchEnable;
            m_wirelessConnectOpreation->setWirelessEnabled(switchEnable);
            //m_switchGsettings->set(WIRELESS_SWITCH, switchEnable);
        }
    }

    return;
}

void WlanPage::initWlanSwitchState()
{
    m_wlanSwitchEnable = m_wirelessConnectOpreation->getWirelessEnabled();

    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        m_switchGsettings = new QGSettings(GSETTINGS_SCHEMA);
        if (m_switchGsettings->keys().contains(WIRELESS_SWITCH)) {
            if (m_devList.isEmpty()) {
                m_wlanSwitchEnable = false;
            } else {
                m_wlanSwitchEnable = m_switchGsettings->get(WIRELESS_SWITCH).toBool();
            }
            connect(m_switchGsettings, &QGSettings::changed, this, &WlanPage::onWlanSwithGsettingsChanged);
        }
    }

    m_netSwitch->setSwitchStatus(m_wlanSwitchEnable);
    //m_netSwitch->setEnabled(m_wlanSwitchEnable);

    connect(m_netSwitch, &SwitchButton::clicked, this, &WlanPage::onWlanSwitchClicked);

    return;
}

void WlanPage::initTimer()
{
    m_scanTimer = new QTimer(this);
    connect(m_scanTimer, &QTimer::timeout, this, &WlanPage::requestScan);

    m_refreshIconTimer = new QTimer(this);
    connect(m_refreshIconTimer, &QTimer::timeout, this, &WlanPage::onRefreshIconTimer);
    m_refreshIconTimer->start(ICON_REFRESH_INTERVAL);
}

/**
 * @brief WlanPage::initDevice 初始化默认网卡
 */
void WlanPage::initDevice()
{
    m_devList.clear();
    m_netDeviceResource->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, m_devList);

    m_currentDevice = getDefaultDeviceName(WIRELESS);

    return;
}

void WlanPage::initDeviceCombox()
{
    //TODO 获取设备列表，单设备时隐藏下拉框，多设备时添加到下拉框
    disconnect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                this, &WlanPage::onDeviceComboxIndexChanged);
    m_deviceComboBox->clear();

    if (m_wlanSwitchEnable) {
        if (0 == m_devList.count()) {
            m_deviceFrame->show();
            m_tipsLabel->show();
            m_deviceComboBox->hide();
            m_currentDevice = "";
            setDefaultDevice(WIRELESS, m_currentDevice);
        } else if (1 == m_devList.count()) {
            m_deviceFrame->hide();
            if (m_currentDevice != m_devList.at(0)) {
                m_currentDevice = m_devList.at(0);
                setDefaultDevice(WIRELESS, m_currentDevice);
            }
        } else {
            m_deviceFrame->show();
            m_deviceComboBox->show();
            m_tipsLabel->hide();
            for (int index = 0; index < m_devList.count(); ++index) {
                m_deviceComboBox->addItem(m_devList.at(index));
            }

            if (m_devList.contains(m_currentDevice)) {
                m_deviceComboBox->setCurrentText(m_currentDevice);
            } else {
                m_currentDevice = m_deviceComboBox->currentText();
                setDefaultDevice(WIRELESS, m_currentDevice);
            }
        }
    } else {
        m_deviceFrame->hide();
        //m_currentDevice = "";
        //setDefaultDevice(WIRELESS, m_currentDevice);
    }

    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                            this, &WlanPage::onDeviceComboxIndexChanged, Qt::DirectConnection);
    return;
}

QListWidgetItem *WlanPage::addEmptyItem(QListWidget *wirelessListWidget)
{
    WlanListItem *p_wlanItem = new WlanListItem();
    QListWidgetItem *p_listWidgetItem = new QListWidgetItem(wirelessListWidget);
    p_listWidgetItem->setSizeHint(QSize(wirelessListWidget->width(), p_wlanItem->height()));
    wirelessListWidget->addItem(p_listWidgetItem);
    wirelessListWidget->setItemWidget(p_listWidgetItem, p_wlanItem);

    return p_listWidgetItem;
}

QListWidgetItem *WlanPage::addNewItem(KyWirelessNetItem &wirelessNetItem,
                                      QListWidget *wirelessListWidget)
{
    WlanListItem *p_wlanItem = new WlanListItem(wirelessNetItem, m_currentDevice);
    connect(p_wlanItem, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);

    QListWidgetItem *p_listWidgetItem = new QListWidgetItem(wirelessListWidget);
    p_listWidgetItem->setSizeHint(QSize(wirelessListWidget->width(), p_wlanItem->height()));
    wirelessListWidget->addItem(p_listWidgetItem);
    wirelessListWidget->setItemWidget(p_listWidgetItem, p_wlanItem);

    return p_listWidgetItem;
}

QListWidgetItem *WlanPage::insertNewItem(KyWirelessNetItem &wirelessNetItem,
                                         QListWidget *wirelessListWidget,
                                         int row)
{
    WlanListItem *p_wlanItem = new WlanListItem(wirelessNetItem, m_currentDevice);
    connect(p_wlanItem, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);

    QListWidgetItem *p_listWidgetItem = new QListWidgetItem(wirelessListWidget);
    p_listWidgetItem->setSizeHint(QSize(wirelessListWidget->width(), p_wlanItem->height()));
    wirelessListWidget->insertItem(row, p_listWidgetItem);
    wirelessListWidget->setItemWidget(p_listWidgetItem, p_wlanItem);

    return p_listWidgetItem;
}

void WlanPage::clearWirelessNetItemMap(QMap<QString, QListWidgetItem*> &wirelessNetItem,
                                       QListWidget *wirelessListWidget)
{
    QMap<QString, QListWidgetItem*>::iterator iter;

    iter = wirelessNetItem.begin();
    while (iter != wirelessNetItem.end()) {
        QListWidgetItem *p_listWidgetItem = iter.value();
        if (p_listWidgetItem == m_hiddenItem) {
            continue;
        }

        WlanListItem *p_wlanItem = (WlanListItem *)wirelessListWidget->itemWidget(p_listWidgetItem);
        wirelessListWidget->removeItemWidget(p_listWidgetItem);

        delete p_wlanItem;
        p_wlanItem = nullptr;

        delete p_listWidgetItem;
        p_listWidgetItem = nullptr;

        iter = wirelessNetItem.erase(iter);
    }

    return;
}

void WlanPage::deleteWirelessItemFormMap(QMap<QString, QListWidgetItem*> &wirelessNetItemMap,
                                         QListWidget *wirelessListWidget, QString ssid)
{
    QListWidgetItem *p_listWidgetItem = wirelessNetItemMap.value(ssid);
    if (nullptr == p_listWidgetItem) {
        qWarning()<< LOG_FLAG <<"wireless item is not exsit, it's ssid is " << ssid;
        return;
    }

    WlanListItem *p_wlanItem = (WlanListItem *)wirelessListWidget->itemWidget(p_listWidgetItem);
    if (nullptr == p_wlanItem) {
        qWarning() << LOG_FLAG << "p_wlanItem is null";
        return;
    }

    wirelessNetItemMap.remove(ssid);

    wirelessListWidget->takeItem(wirelessListWidget->row(p_listWidgetItem));

    delete p_wlanItem;
    p_wlanItem = nullptr;

    delete p_listWidgetItem;
    p_listWidgetItem = nullptr;

    return;
}

void WlanPage::updateWlanItemState(QListWidget *p_wirelessListWidget,
                                   QListWidgetItem *p_listWidgetItem,
                                   ConnectState state)
{
    WlanListItem *p_wlanItem = nullptr;
    p_wlanItem = (WlanListItem *)p_wirelessListWidget->itemWidget(p_listWidgetItem);
    if (nullptr != p_wlanItem) {
        p_wlanItem->updateConnectState(state);
    }

    return;
}

void WlanPage::constructActivateConnectionArea()
{
    int height = 0;
    clearWirelessNetItemMap(m_activateConnectionItemMap, m_activatedNetListWidget);

    if (!m_currentDevice.isEmpty()) {
        KyWirelessNetItem wirelessNetItem;
        bool ret = m_wirelessNetResource->getActiveWirelessNetItem(m_currentDevice, wirelessNetItem);
        if (ret == true) {
            QListWidgetItem *p_listWidgetItem = addNewItem(wirelessNetItem, m_activatedNetListWidget);
            m_activateConnectionItemMap.insert(wirelessNetItem.m_NetSsid, p_listWidgetItem);
            updateWlanItemState(m_activatedNetListWidget, p_listWidgetItem, Activated);

            height += p_listWidgetItem->sizeHint().height();
        }
    }

    if (height == 0) {  
        QListWidgetItem *p_listWidgetItem = addEmptyItem(m_activatedNetListWidget);
        m_activateConnectionItemMap.insert(EMPTY_SSID, p_listWidgetItem);

        height += p_listWidgetItem->sizeHint().height();
    }

    m_activatedNetListWidget->setFixedHeight(height);

    return;
}

void WlanPage::constructWirelessNetArea()
{
    qDebug() << "[WlanPage] Started loading wireless net list!"
             << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");

    clearWirelessNetItemMap(m_wirelessNetItemMap, m_inactivatedNetListWidget);
    m_expandedItem = nullptr;

    QList<KyWirelessNetItem> wirelessNetItemList;
    if (!m_wirelessNetResource->getDeviceWifiNetwork(m_currentDevice, wirelessNetItemList)) {
        qWarning()<<"[WlanPage] get wireless net item list failed.";
        return;
    }

    QString activateSsid = m_activateConnectionItemMap.firstKey();

    int height = 0;
    foreach (auto wirelessNetItem, wirelessNetItemList) {
        if (wirelessNetItem.m_NetSsid == activateSsid) {
            continue;
        }

        QListWidgetItem *p_listWidgetItem = addNewItem(wirelessNetItem, m_inactivatedNetListWidget);
        m_wirelessNetItemMap.insert(wirelessNetItem.m_NetSsid, p_listWidgetItem);
        updateWlanItemState(m_inactivatedNetListWidget, p_listWidgetItem, Deactivated);

        if (height == 0) {
            height += p_listWidgetItem->sizeHint().height();
        }
        height += p_listWidgetItem->sizeHint().height() + NET_LIST_SPACING;
    }

    addWlanMoreItem();

    qDebug() << "[WlanPage] Stopped loading wireless net list! time="
             << QDateTime::currentDateTime().toString("hh:mm:ss.zzzz");

    return;
}

void WlanPage::initWlanArea()
{
    if (m_wlanSwitchEnable) {
        m_activatedNetFrame->show();
        m_activatedNetDivider->show();
        constructActivateConnectionArea();

        m_inactivatedNetFrame->show();
        constructWirelessNetArea();
    } else {
        m_activatedNetFrame->hide();
        m_activatedNetDivider->hide();

        m_inactivatedNetFrame->hide();
    }

    return;
}

void WlanPage::onWlanAdded(QString interface, KyWirelessNetItem &item)
{
    //for dbus
    QStringList info;
    info <<item.m_NetSsid<<QString::number(item.m_signalStrength)<<item.m_secuType;
    emit wlanAdd(interface, info);

    if (interface != m_currentDevice) {
        qDebug() << "[WlanPage] wlan add interface not equal defaultdevice";
        return;
    }

    if (m_wirelessNetItemMap.contains(item.m_NetSsid)) {
        return;
    }

    qDebug() << "[WlanPage] A Wlan Added! interface = "
             << interface << "; ssid = " << item.m_NetSsid << Q_FUNC_INFO <<__LINE__;

    QListWidgetItem *p_listWidgetItem = addNewItem(item, m_inactivatedNetListWidget);
    m_wirelessNetItemMap.insert(item.m_NetSsid, p_listWidgetItem);
    updateWlanItemState(m_inactivatedNetListWidget, p_listWidgetItem, Deactivated);

    addWlanMoreItem();

    return;
}

void WlanPage::onWlanRemoved(QString interface, QString ssid)
{
    emit wlanRemove(interface, ssid);

    if (interface != m_currentDevice) {
        qDebug()<<"[WlanPage] the device is not current device,"
                <<"current device" << m_currentDevice
                << "remove wlan device" << interface;
        return;
    }

    if (!m_wirelessNetItemMap.contains(ssid)) {
        return;
    }

    if (m_expandedItem == m_wirelessNetItemMap.value(ssid)) {
        m_expandedItem = nullptr;
    }

    qDebug() << "[WlanPage] A Wlan Removed! interface = " << interface
             << "; ssid = " << ssid << Q_FUNC_INFO <<__LINE__;



    deleteWirelessItemFormMap(m_wirelessNetItemMap,
                                      m_inactivatedNetListWidget, ssid);

    return;
}

void WlanPage::updateWlanListItem(QString ssid)
{
    KyWirelessNetItem wirelessNetItem;

    qDebug() << LOG_FLAG << "update wlan list item " << ssid;

    bool ret = m_wirelessNetResource->getWifiNetwork(m_currentDevice, ssid, wirelessNetItem);
    if (ret) {
        QListWidgetItem *p_listWidgetItem = m_wirelessNetItemMap.value(ssid);
        if (p_listWidgetItem) {
            WlanListItem *p_wlanItem = (WlanListItem *)m_inactivatedNetListWidget->itemWidget(p_listWidgetItem);
            p_wlanItem->updateWirelessNetItem(wirelessNetItem);
        }
    }

    return;
}

void WlanPage::onConnectionAdd(QString deviceName, QString ssid)
{
    qDebug() << LOG_FLAG << "one connection is added, it's ssid " << ssid << "device name"<< deviceName;

    if (deviceName == m_currentDevice) {
        updateWlanListItem(ssid);
    }

    return;
}
void WlanPage::onConnectionRemove(QString deviceName, QString ssid)
{
    qDebug() << LOG_FLAG << "one connection is removed, it's ssid " << ssid << "device name"<< deviceName;
    if (deviceName == m_currentDevice) {
        if (m_activateConnectionItemMap.contains(ssid)) {
            updateWirelessNetArea(nullptr, ssid, deviceName);
        }
        updateWlanListItem(ssid);
    }

    return;
}

void WlanPage::onSecurityTypeChange(QString devName, QString ssid, QString secuType)
{
    QListWidgetItem *p_listWidgetItem = nullptr;
    WlanListItem *p_wlanItem = nullptr;

    qDebug()<< LOG_FLAG << "security type is chenged";

    if (m_wirelessNetItemMap.contains(ssid)) {
        p_listWidgetItem = m_wirelessNetItemMap.value(ssid);
        p_wlanItem = (WlanListItem*)m_inactivatedNetListWidget->itemWidget(p_listWidgetItem);
    } else if (m_activateConnectionItemMap.contains(ssid)) {
        p_listWidgetItem = m_activateConnectionItemMap.value(ssid);
        p_wlanItem = (WlanListItem*)m_activatedNetListWidget->itemWidget(p_listWidgetItem);
    }

    if (nullptr != p_wlanItem) {
        p_wlanItem->updateWirelessNetSecurity(ssid, secuType);
    }

    emit secuTypeChange(devName, ssid, secuType);

    return;
}


void WlanPage::addDeviceToCombox(QString deviceName)
{
    disconnect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                this, &WlanPage::onDeviceComboxIndexChanged);
    if (m_wlanSwitchEnable) {
        if (m_currentDevice.isEmpty()){
            m_deviceFrame->hide();
            m_currentDevice = deviceName;
            setDefaultDevice(WIRELESS, m_currentDevice);
        } else if (m_deviceComboBox->count() == 0) {
            m_deviceComboBox->addItem(m_currentDevice);
            m_deviceComboBox->addItem(deviceName);
            m_deviceFrame->show();
            m_deviceComboBox->show();
            m_tipsLabel->hide();
        } else {
            m_deviceComboBox->addItem(deviceName);
        }
    }

    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                            this, &WlanPage::onDeviceComboxIndexChanged);
    return;
}

void WlanPage::onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType)
{
    qDebug() << LOG_FLAG << "deviceAdd" << deviceName;
    if (deviceType !=  NetworkManager::Device::Type::Wifi) {
        return;
    }
    
    if (m_devList.contains(deviceName)) {
        return;
    }

    if (m_devList.isEmpty()) {
        m_wlanSwitchEnable = m_switchGsettings->get(WIRELESS_SWITCH).toBool();
        //m_netSwitch->setEnabled(m_wlanSwitchEnable);
        m_netSwitch->setSwitchStatus(m_wlanSwitchEnable);
    }

    m_devList << deviceName;
    addDeviceToCombox(deviceName);
    if (m_currentDevice == deviceName) {
        initWlanArea();
    }

    emit deviceStatusChanged();

    return;
}

void WlanPage::deleteDeviceFromCombox(QString deviceName)
{
    disconnect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                this, &WlanPage::onDeviceComboxIndexChanged);

    if (m_wlanSwitchEnable) {
        if (0 == m_devList.count()) {
            m_deviceFrame->hide();
            //m_tipsLabel->show();
            //m_deviceComboBox->hide();
            m_currentDevice = "";
            setDefaultDevice(WIRELESS, m_currentDevice);
        } else if (1 == m_devList.count()) {
            m_deviceFrame->hide();
            m_deviceComboBox->clear();
            m_currentDevice = m_devList.at(0);
            setDefaultDevice(WIRELESS, m_currentDevice);
        } else {
            int index = m_deviceComboBox->findData(deviceName);
            if (-1 != index) {
                m_deviceComboBox->removeItem(index);
                m_currentDevice = m_deviceComboBox->currentText();
                setDefaultDevice(WIRELESS, m_currentDevice);
            }
        }
    }

    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                                            this, &WlanPage::onDeviceComboxIndexChanged);
    return;
}

void WlanPage::onDeviceRemove(QString deviceName)
{
    if (!m_devList.contains(deviceName)) {
        return;
    }

    qDebug() << "deviceRemove" << deviceName;

    QString originalDeviceName = m_currentDevice;

    m_devList.removeOne(deviceName);
    deleteDeviceFromCombox(deviceName);

    if (m_devList.isEmpty()) {
        m_wlanSwitchEnable = false;
        //m_netSwitch->setEnabled(m_wlanSwitchEnable);
        m_netSwitch->setSwitchStatus(m_wlanSwitchEnable);
    }

    if (originalDeviceName == deviceName) {
        initWlanArea();
    }

    emit deviceStatusChanged();
}

void WlanPage::updateDeviceForCombox(QString oldDeviceName, QString newDeviceName)
{
    if (m_currentDevice == oldDeviceName) {
        m_currentDevice = newDeviceName;
        setDefaultDevice(WIRELESS, m_currentDevice);
    }

    int index = m_deviceComboBox->findText(oldDeviceName);
    if (index != -1) {
        m_deviceComboBox->setItemText(index, newDeviceName);
    }

    return;
}

void WlanPage::onDeviceNameUpdate(QString oldName, QString newName)
{
    if (!m_devList.contains(oldName)) {
        return;
    }

    qDebug()<<"[WlanPage] update device name, old name"
           << oldName << "new Name" << newName;

    m_devList.removeOne(oldName);
    m_devList.append(newName);

    QString originalDeviceName = m_currentDevice;

    updateDeviceForCombox(oldName, newName);
    if (originalDeviceName != m_currentDevice) {
        initWlanArea();
    }

    emit deviceNameChanged(oldName, newName, WIRELESS);
}

void WlanPage::sendApStateChangeSignal(QString uuid,
                                       QString ssid,
                                       QString deviceName,
                                       NetworkManager::ActiveConnection::State state)
{
    bool ret = false;
    ret = m_connectResource->isApConnection(uuid);
    if (!ret) {
        return;
    }

    if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        qDebug() << "[WlanPage] hotspot Deactivated";
        emit hotspotDeactivated(deviceName, ssid);
    } else if (state == NetworkManager::ActiveConnection::State::Activated) {
        qDebug() << "[WlanPage] hotspot activated";
        emit hotspotActivated(deviceName, ssid, uuid);
    }

    return;
}

void WlanPage::wlanShowNotify(QString ssid, NetworkManager::ActiveConnection::State state,
                              NetworkManager::ActiveConnection::Reason reason)
{
    if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        showDesktopNotify(tr("WLAN Disconnected Successfully"));
        qDebug() << "[WlanPage] wlan disconnected successfully"  << Q_FUNC_INFO <<__LINE__;
    } else if (state == NetworkManager::ActiveConnection::State::Activated) {
        qDebug() << "[WlanPage] wlan connected successfully" << Q_FUNC_INFO <<__LINE__;
        this->showDesktopNotify(tr("WLAN Connected Successfully"));
    }

    return;
}

void WlanPage::updateActivatedArea(QString uuid, QString ssid, QString devName)
{
    if (m_activateConnectionItemMap.contains(ssid)) {
        return;
    }

    if (m_expandedItem == m_wirelessNetItemMap.value(ssid)) {
        m_expandedItem = nullptr;
    }
    deleteWirelessItemFormMap(m_wirelessNetItemMap,
                                      m_inactivatedNetListWidget, ssid);

    KyWirelessNetItem wirelessNetItem;
    bool ret = m_wirelessNetResource->getWifiNetwork(devName, ssid, wirelessNetItem);
    if (!ret) {
        qWarning()<<"[WlanPage] get wireless item failed, when update activated connection area.";
        return;
    }

    deleteWirelessItemFormMap(m_activateConnectionItemMap, m_activatedNetListWidget, EMPTY_SSID);
    QListWidgetItem *p_listWidgetItem = addNewItem(wirelessNetItem, m_activatedNetListWidget);
    m_activateConnectionItemMap.insert(wirelessNetItem.m_NetSsid, p_listWidgetItem);

    m_activatedNetListWidget->setFixedHeight(p_listWidgetItem->sizeHint().height());

    return;
}

void WlanPage::updateWirelessNetArea(QString uuid, QString ssid, QString devName)
{
    if(m_wirelessNetItemMap.contains(ssid)) {
        return;
    }

    deleteWirelessItemFormMap(m_activateConnectionItemMap, m_activatedNetListWidget, ssid);
    QListWidgetItem *p_activeListWidgetItem = addEmptyItem(m_activatedNetListWidget);
    m_activateConnectionItemMap.insert(EMPTY_SSID, p_activeListWidgetItem);
    m_activatedNetListWidget->setFixedHeight(p_activeListWidgetItem->sizeHint().height());

    KyWirelessNetItem wirelessNetItem;
    bool ret = m_wirelessNetResource->getWifiNetwork(devName, ssid, wirelessNetItem);
    if (!ret) {
        qWarning()<<"[WlanPage] get wireless item failed, when update wireless area.";
        return;
    }

    QListWidgetItem *p_listWidgetItem = addNewItem(wirelessNetItem, m_inactivatedNetListWidget);
    m_wirelessNetItemMap.insert(wirelessNetItem.m_NetSsid, p_listWidgetItem);

    // 更新‘更多’条目，以保证其处于listwidget的最底部
    addWlanMoreItem();

    return;
}

void WlanPage::onConnectionStateChanged(QString uuid,
                                      NetworkManager::ActiveConnection::State state,
                                      NetworkManager::ActiveConnection::Reason reason)
{
    QString devName, ssid;
    m_wirelessNetResource->getSsidByUuid(uuid, ssid);
    m_wirelessNetResource->getDeviceByUuid(uuid, devName);

    qDebug()<< LOG_FLAG << "emit wlanActiveConnectionStateChanged" << devName << ssid << state;
    emit wlanActiveConnectionStateChanged(devName, ssid, uuid, state);

    if (ssid.isEmpty() || devName.isEmpty()) {
        qDebug()<< LOG_FLAG << "ssid or devicename is empty"
                << "devicename"<< devName <<"ssid"<<ssid;
        return;
    }

    if (!m_connectResource->isWirelessConnection(uuid)) {
         qDebug()<< LOG_FLAG << "it is not wireless connection" << uuid;
        return;
    }

    if (m_connectResource->isApConnection(uuid)) {
        sendApStateChangeSignal(uuid, ssid, devName, state);
    } else {
        wlanShowNotify(ssid, state, reason);
    }

    if (devName != m_currentDevice) {
        return;
    }

    qDebug()<< "[WlanPage] wlan state changed, ssid = " << ssid
            << "; state = " << state << "; reason = " << reason << Q_FUNC_INFO <<__LINE__;
    if (state == NetworkManager::ActiveConnection::State::Activated) {
        updateActivatedArea(uuid, ssid, devName);
        if (m_activateConnectionItemMap.contains(ssid)) {
            QListWidgetItem *p_listWidgetItem = m_activateConnectionItemMap.value(ssid);
            updateWlanItemState(m_activatedNetListWidget, p_listWidgetItem, Activated);
        }
    } else if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        updateWirelessNetArea(uuid, ssid, devName);
        if (m_wirelessNetItemMap.contains(ssid)) {
            QListWidgetItem *p_listWidgetItem = m_wirelessNetItemMap.value(ssid);
            updateWlanItemState(m_inactivatedNetListWidget, p_listWidgetItem, Deactivated);
        }
    } else if (state == NetworkManager::ActiveConnection::State::Deactivating){
        if (m_activateConnectionItemMap.contains(ssid)) {
            QListWidgetItem *p_listWidgetItem = m_activateConnectionItemMap.value(ssid);
            updateWlanItemState(m_activatedNetListWidget, p_listWidgetItem, Deactivating);
        }
    } else if (state == NetworkManager::ActiveConnection::State::Activating) {
        if (m_wirelessNetItemMap.contains(ssid)) {
            QListWidgetItem *p_listWidgetItem = m_wirelessNetItemMap.value(ssid);
            updateWlanItemState(m_inactivatedNetListWidget, p_listWidgetItem, Activating);
        }
    }
    emit this->wlanConnectChanged(state);
    return;
}

void WlanPage::onItemHeightChanged(const bool isExpanded, const QString &ssid)
{
    if (!m_wirelessNetItemMap.contains(ssid)) {
        return;
    }

    QListWidgetItem *p_listWidgetItem = m_wirelessNetItemMap.value(ssid);

    qDebug()<<LOG_FLAG << "expanded changed" << isExpanded << ssid;

    int height = p_listWidgetItem->sizeHint().height();
    int width = p_listWidgetItem->sizeHint().width();

    if (isExpanded) {
        if (m_expandedItem != p_listWidgetItem) {
            qDebug()<<LOG_FLAG << "expanded wlan item";
            m_expandedItem = p_listWidgetItem;
            QSize expandedSize(width, EXPANDED_HEIGHT);
            p_listWidgetItem->setSizeHint(expandedSize);
        }
        m_inactivatedNetListWidget->scrollToItem(p_listWidgetItem, QAbstractItemView::QAbstractItemView::EnsureVisible);
    } else {
        m_expandedItem = nullptr;

        if (height > NORMAL_HEIGHT) {
            qDebug()<<LOG_FLAG<<"do not expanded";
            QSize normalSize(width, NORMAL_HEIGHT);
            p_listWidgetItem->setSizeHint(normalSize);
        }
    }

    return;
}

void WlanPage::onDeviceComboxIndexChanged(int currentIndex)
{
    qDebug() << "onDeviceComboxIndexChanged";
    if (!m_deviceComboBox || currentIndex < 0) {
        return;
    }

    m_currentDevice = m_deviceComboBox->itemText(currentIndex);
    setDefaultDevice(WIRELESS, m_currentDevice);

    initWlanArea();

    return;
}

//申请触发扫描，初始化执行&定时执行
void WlanPage::requestScan()
{
    if (!m_wirelessConnectOpreation) {
        qWarning() << "[WlanPage]Scan failed! m_wirelessConnectOpreation is nullptr!" << Q_FUNC_INFO << __LINE__;
        return;
    }

    m_wirelessConnectOpreation->requestWirelessScan();

    return;
}

void WlanPage::onHiddenWlanClicked()
{
    qDebug() << "[wlanPage] AddHideWifi Clicked! " << Q_FUNC_INFO << __LINE__ ;
    NetDetail *netDetail = new NetDetail(m_currentDevice, "", "", false, true, true, this);
    netDetail->show();
}

void WlanPage::showControlCenter()
{
    qDebug() << LOG_FLAG <<"show control center";

    QProcess process;
    process.startDetached("ukui-control-center -m wlanconnect");

    return;
}

void WlanPage::onWifiEnabledChanged(bool isWifiOn)
{
    //监听外部命令导致wifi状态变化，更新界面
    qDebug() << "[WlanPage] onWifiEnabledChanged wifi state" << isWifiOn;

    //应该先检测是否有无线网卡可用，才改变开关状态
    if (m_devList.isEmpty()) {
        qDebug() << "[WLanPage] have no device to use "  << Q_FUNC_INFO << __LINE__;
        return;
    }

    if (m_wlanSwitchEnable == isWifiOn) {
        return;
    } else {
        //m_netSwitch->setSwitchStatus(isWifiOn);
        //m_netSwitch->setEnabled(isWifiOn);
        m_switchGsettings->set(WIRELESS_SWITCH, isWifiOn);
//        emit this->wlanConnectChanged();
    }

    return;
}

void WlanPage::onRefreshIconTimer()
{  
    emit timeToUpdate();

    if(!this->isVisible()) {
        return;
    }
    qDebug() << "onRefreshIconTimer";

    if (m_expandedItem) {
        qDebug()<< LOG_FLAG << "Has expanded item and forbid refresh wifi strength" << Q_FUNC_INFO << __LINE__;
        return;
    }

    //qDebug()<< LOG_FLAG << "Will update Wlan list by strength." << Q_FUNC_INFO << __LINE__;

    QList<KyWirelessNetItem> wlanList;
    if (!m_wirelessNetResource->getDeviceWifiNetwork(m_currentDevice, wlanList)) {
        return;
    }

    QString activateSsid = m_activateConnectionItemMap.firstKey();
    int sortRow = 0;
    for (int index = 0; index < wlanList.length(); index++) {
        KyWirelessNetItem sortItem = wlanList.at(index);
        QString sortSsid = sortItem.m_NetSsid; //应该在第currentRow行的新的WiFi名称
        qDebug()<< LOG_FLAG << "sort ssid"<< sortSsid << "active ssid" << activateSsid << "sort row"<< sortRow;
        if (sortSsid == activateSsid) { //排除已连接WiFi
            continue;
        }

        QListWidgetItem *p_listWidgetItem = m_inactivatedNetListWidget->item(sortRow);
        if (p_listWidgetItem) {
            WlanListItem *p_wlanItem = (WlanListItem *)m_inactivatedNetListWidget->itemWidget(p_listWidgetItem);

            if (nullptr == p_wlanItem) {
                qDebug() << LOG_FLAG << "p_wlanItem is null continue";
                continue;//暂时先保持continue，后续讨论是否使用break直接跳出循环
            }

            // 该item是‘更多’条目，不需要更新
            if (WMI_OB_NAME == p_wlanItem->objectName()) {
                qDebug() << LOG_FLAG << "p_wlanItem is WlanMoreItem";
                continue;
            }

            qDebug()<< LOG_FLAG << "row" << sortRow << "item ssid" << p_wlanItem->getSsid();
            if (sortSsid == p_wlanItem->getSsid()) {
                qDebug()<< LOG_FLAG << "sort wlan set signal strength." << Q_FUNC_INFO << __LINE__;
                p_wlanItem->setWlanSignal(sortItem.m_signalStrength);
                sortRow++;
                continue;
            }

            QListWidgetItem *p_sortListWidgetItem = m_wirelessNetItemMap.value(sortSsid);
            if (p_sortListWidgetItem) {
                WlanListItem *p_wlanItem = (WlanListItem *)m_inactivatedNetListWidget->itemWidget(p_listWidgetItem);
                if (p_wlanItem == nullptr) {
                    continue;
                }

                if (Deactivated != p_wlanItem->getConnectionState()) {
                    continue;
                }
                qDebug()<< LOG_FLAG << "sort wlan update position." << sortSsid << Q_FUNC_INFO << __LINE__;
                int takeRow = m_inactivatedNetListWidget->row(p_sortListWidgetItem);
                m_inactivatedNetListWidget->takeItem(takeRow);

                WlanListItem *p_sortWlanItem = new WlanListItem(sortItem, m_currentDevice);
                connect(p_sortWlanItem, &WlanListItem::itemHeightChanged, this, &WlanPage::onItemHeightChanged);
                m_inactivatedNetListWidget->insertItem(sortRow, p_sortListWidgetItem);
                m_inactivatedNetListWidget->setItemWidget(p_sortListWidgetItem, p_sortWlanItem);
                updateWlanItemState(m_inactivatedNetListWidget, p_sortListWidgetItem, Deactivated);
                sortRow++;
                continue;
            }
        }

        qDebug()<< LOG_FLAG << "sort wlan add new item." << Q_FUNC_INFO << __LINE__;
        QListWidgetItem *p_newListWidgetItem = insertNewItem(sortItem, m_inactivatedNetListWidget, sortRow);
        m_wirelessNetItemMap.insert(sortItem.m_NetSsid, p_newListWidgetItem);
        updateWlanItemState(m_inactivatedNetListWidget, p_listWidgetItem, Deactivated);

        sortRow++;
    }

    qDebug()<< LOG_FLAG << "sort wlan finished." << Q_FUNC_INFO << __LINE__;

    return;
}

//for dbus
void WlanPage::getWirelessList(QMap<QString, QVector<QStringList> > &map)
{
    QMap<QString,QStringList> actMap;
    m_wirelessNetResource->getWirelessActiveConnection(NetworkManager::ActiveConnection::State::Activated, actMap);

    QMap<QString, QList<KyWirelessNetItem> > wlanMap;
    if (!m_wirelessNetResource->getAllDeviceWifiNetwork(wlanMap)) {
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
            m_wirelessNetResource->getSsidByUuid(actMap[iter.key()].at(0), ssid);
            if (m_wirelessNetResource->getWifiNetwork(iter.key(), ssid, data)) {
                vector.append(QStringList()<<data.m_NetSsid
                              <<QString::number(data.m_signalStrength)<<data.m_secuType<<data.m_connectUuid);
                activeSsid = data.m_NetSsid;
            } else {
                vector.append(QStringList("--"));
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
void WlanPage::activeWirelessAp(const QString apName, const QString apPassword, const QString wirelessBand, const QString apDevice)
{
    QString uuid("");
    QList<KyApConnectItem *> apConnectItemList;
    m_connectResource->getApConnections(apConnectItemList);
    if (!apConnectItemList.isEmpty()) {
        uuid = apConnectItemList.at(0)->m_connectUuid;
    }

    m_wirelessConnectOpreation->activeWirelessAp(uuid, apName, apPassword, apDevice, wirelessBand);
}

//断开热点
void WlanPage::deactiveWirelessAp(const QString apName, const QString uuid)
{
    if (!uuid.isEmpty()) {
        m_wirelessConnectOpreation->deactiveWirelessAp(apName, uuid);
    }
}

void WlanPage::getStoredApInfo(QStringList &list)
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
}

void WlanPage::getApInfoBySsid(QString devName, QString ssid, QStringList &list)
{
    list.clear();
    QList<KyApConnectItem *> apConnectItemList;
    m_connectResource->getApConnections(apConnectItemList);
    for (int i = 0; i < apConnectItemList.size(); i++) {
        if (apConnectItemList.at(i)->m_connectSsid == ssid
                && apConnectItemList.at(i)->m_ifaceName == devName) {
            list << apConnectItemList.at(i)->m_password;
            list << apConnectItemList.at(i)->m_band;
        }
    }

    return;
}

void WlanPage::activateWirelessConnection(const QString& devName, const QString& ssid)
{
    KyWirelessNetItem wirelessNetItem;
    if (!m_wirelessNetResource->getWifiNetwork(devName, ssid, wirelessNetItem)) {
        qDebug() << "[WlanPage] no such wifi " << ssid << " in " << devName;
        return;
    }

    if (devName != m_currentDevice) {
        int index = m_deviceComboBox->findText(devName);
        if (index >= 0) {
            m_deviceComboBox->setCurrentIndex(index);
        } else {
            qDebug() << "[WlanPage]activateWirelessConnection no such " << devName;
            return;
        }
    }

    QListWidgetItem *p_listWidgetItem = nullptr;
    WlanListItem *p_wlanItem = nullptr;

    if (m_wirelessNetItemMap.contains(ssid)) {
        p_listWidgetItem = m_wirelessNetItemMap.value(ssid);
        p_wlanItem = (WlanListItem*)m_inactivatedNetListWidget->itemWidget(p_listWidgetItem);

        m_inactivatedNetListWidget->scrollToItem(p_listWidgetItem, QAbstractItemView::EnsureVisible);


        QMouseEvent *event = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::postEvent(p_wlanItem, event);
        emit showMainWindow();
    } else {
        qDebug() << "[WlanPage]activateWirelessConnection no such " << ssid << "in" << devName;
    }
    return;
}

void WlanPage::deactivateWirelessConnection(const QString& devName, const QString& ssid)
{
    KyWirelessNetItem wirelessNetItem;
    if (!m_wirelessNetResource->getWifiNetwork(devName, ssid, wirelessNetItem)) {
        qDebug() << "[WlanPage] no such wifi " << ssid << " in " << devName;
        return;
    }

    if (!m_connectResource->isActivatedConnection(wirelessNetItem.m_connectUuid)) {
        qDebug()<<"[WlanPage]"<<wirelessNetItem.m_connName << "is not activated, so not deactivated.";
        return;
    }

    qDebug() << "[WlanPage] deActivate Wireless Connection" << devName << ssid;

    m_wirelessConnectOpreation->deActivateWirelessConnection(wirelessNetItem.m_connName, wirelessNetItem.m_connectUuid);

    return;
}

void WlanPage::onMainWindowVisibleChanged(const bool &visible)
{
    qDebug() << "[WlanPage] Received signal of mainwindow visible changed. cur_state = " << visible << Q_FUNC_INFO << __LINE__;
    if (visible) {
        //打开页面时先触发一次扫描，然后定时扫描wifi热点和刷新icon
        requestScan();
        m_scanTimer->start(AP_SCAN_INTERVAL);
//        m_refreshIconTimer->start(ICON_REFRESH_INTERVAL);
    } else {
        //界面关闭的时候，停止wifi扫描和刷新
        m_scanTimer->stop();
//        m_refreshIconTimer->stop();
    }

    return;
}

void WlanPage::showDetailPage(QString devName, QString ssid)
{
    KyWirelessNetItem wirelessNetItem;
    if (!m_wirelessNetResource->getWifiNetwork(devName, ssid, wirelessNetItem)) {
        qDebug()<<"[WlanPage] " << ssid << " is missing when showDetailPage";
        return;
    }

    bool isActive = m_connectResource->isActivatedConnection(wirelessNetItem.m_connectUuid);

    NetDetail *netDetail = new NetDetail(devName, ssid, wirelessNetItem.m_connectUuid, isActive, true, !wirelessNetItem.m_isConfigured, this);
    netDetail->show();

    return;
}

bool WlanPage::wlanIsConnected()
{
    if (m_activateConnectionItemMap.isEmpty()) {
        return false;
    } else if (m_activateConnectionItemMap.contains(EMPTY_SSID)) {
        return false;
    }
    return true;
}

void WlanPage::setWirelessSwitchEnable(bool enable)
{
    qDebug() << "dbus setWirelessSwitchEnable = " << enable << __LINE__;
    //应该先检测是否有无线网卡可用，才改变开关状态
    if (m_devList.isEmpty()) {
        qDebug() << "have no device to use "  << Q_FUNC_INFO << __LINE__;
        //检测不到无线网卡不再触发click信号
        m_netSwitch->setSwitchStatus(false);
        //m_netSwitch->setEnabled(false);
    }else{
        m_wirelessConnectOpreation->setWirelessEnabled(enable);
    }
}

void WlanPage::getWirelessDeviceCap(QMap<QString, int> &map)
{
    for (int i = 0; i < m_devList.size(); ++i) {
        QString devName = m_devList.at(i);
        map.insert(devName, m_netDeviceResource->getWirelessDeviceCapability(devName));
    }
}

/**
 * @brief WlanPage::addWlanMoreItem
 * 添加‘更多网络’的条目
 */
void WlanPage::addWlanMoreItem()
{
    if (m_hiddenItem) {
        m_inactivatedNetListWidget->takeItem(m_inactivatedNetListWidget->row(m_hiddenItem));
        delete m_hiddenItem;
        m_hiddenItem = nullptr;
    }

    if (m_hiddenWlanWidget) {
//        disconnect(m_hiddenWlanWidget, &WlanMoreItem::hiddenWlanClicked, this, &WlanPage::onHiddenWlanClicked);
        m_hiddenWlanWidget->setParent(nullptr);
        delete m_hiddenWlanWidget;
        m_hiddenWlanWidget = nullptr;
    }

    m_hiddenWlanWidget = new WlanMoreItem(this);
    connect(m_hiddenWlanWidget, &WlanMoreItem::hiddenWlanClicked, this, &WlanPage::onHiddenWlanClicked);
    m_hiddenWlanWidget->setFixedHeight(NORMAL_HEIGHT);

    m_hiddenItem = new QListWidgetItem(m_inactivatedNetListWidget);
    m_inactivatedNetListWidget->addItem(m_hiddenItem);
    m_inactivatedNetListWidget->setItemWidget(m_hiddenItem, m_hiddenWlanWidget);
    return;
}
