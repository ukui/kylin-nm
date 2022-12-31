/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "wlanconnect.h"

#include <QGSettings>
#include <QProcess>
#include <QTimer>
#include <QtDBus>
#include <QDir>
#include <QDebug>
#include <QtAlgorithms>
#include <kwindowsystem.h>
#include <kwindowsystem_export.h>

#define SCANTIMER  20 * 1000
#define UPDATETIMER 10 * 1000

#define SPACING 8

const QString KWifiSymbolic     = "network-wireless-signal-excellent";
const QString KWifiLockSymbolic = "network-wireless-secure-signal-excellent";
const QString KWifiGood         = "network-wireless-signal-good";
const QString KWifiLockGood     = "network-wireless-secure-signal-good";
const QString KWifiOK           = "network-wireless-signal-ok";
const QString KWifiLockOK       = "network-wireless-secure-signal-ok";
const QString KWifiLow          = "network-wireless-signal-low";
const QString KWifiLockLow      = "network-wireless-secure-signal-low";
const QString KWifiNone         = "network-wireless-signal-none";
const QString KWifiLockNone     = "network-wireless-secure-signal-none";

const QString KApSymbolic       = "network-wireless-hotspot-symbolic";

const QString IsApConnection    = "1";

#define NO_MARGINS 0,0,0,0
#define NO_SPACING 0
#define TITLE_FRAME_HEIGHT 50     //TabWidget的tab和widget有间隙，和设计稿看起来一致就不能设为设计稿里的高度
#define TITLE_LAYOUT_MARGINS 24,0,24,0
#define WIDGET_WIDTH 420
#define WIDGET_HEIGHT 436
#define LOG_FLAG "[WlanConnect]"

bool intThan(int sign1, int sign2)
{
    return sign1 < sign2;
}

//void WlanConnect::showDesktopNotify(const QString &message)
//{
//    QDBusInterface iface("org.freedesktop.Notifications",
//                         "/org/freedesktop/Notifications",
//                         "org.freedesktop.Notifications",
//                         QDBusConnection::sessionBus());
//    QList<QVariant> args;
//    args<<(tr("ukui control center"))
//       <<((unsigned int) 0)
//      <<QString("/usr/share/icons/ukui-icon-theme-default/24x24/devices/gnome-dev-ethernet.png")
//     <<tr("ukui control center desktop message") //显示的是什么类型的信息
//    <<message //显示的具体信息
//    <<QStringList()
//    <<QVariantMap()
//    <<(int)-1;
//    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
//}

WlanConnect::WlanConnect() :  m_firstLoad(true)
{
    //dbus类型注册
    qRegisterMetaType<KyWirelessConnectSetting>("KyWirelessConnectSetting");
    qRegisterMetaType<KySecuType>("KySecuType");
    qRegisterMetaType<QList<KyActivateItem>>("QList<KyActivateItem>");
    qRegisterMetaType<QList<KyWirelessNetItem>>("QList<KyWirelessNetItem>");
    qRegisterMetaType<KyEapMethodPeapInfo>("KyEapMethodPeapInfo");
    qRegisterMetaType<KyEapMethodTtlsInfo>("KyEapMethodTtlsInfo");
    qRegisterMetaType<KyConnectState>("KyConnectState");

    QLocale local;
    QString locale = local.name();
    QTranslator* translator = new QTranslator(this);
    if (translator->load(":/translations/wlan_"+ locale + ".qm")) {
        QApplication::installTranslator(translator);
    } else {
        qWarning() << "Translations load fail";
    }
}

WlanConnect::~WlanConnect()
{
    //启动keyring进程
    QDBusInterface interface("com.kylin.network", "/com/kylin/network",
                             "com.kylin.network",
                             QDBusConnection::sessionBus());
    if (interface.isValid()) {
        interface.call("keyRingInit");
    }

    if (m_styleGsettings != nullptr) {
        delete m_styleGsettings;
        m_styleGsettings = nullptr;
    }

    thread->quit();

    if (nullptr !=pluginWidget) {
        delete pluginWidget;
        pluginWidget = nullptr;
    }
}

QWidget *WlanConnect::pluginUi() {
    if (m_firstLoad) {
        m_firstLoad = false;

        //停止keyring进程
        QDBusInterface interface("com.kylin.network", "/com/kylin/network",
                                 "com.kylin.network",
                                 QDBusConnection::sessionBus());
        if (interface.isValid()) {
            interface.call("keyRingClear");
        }

        KylinAgent* agent = new KylinAgent(this);
        agent->setParentWidget(parentWidget);
        agent->startKylinAgent();

        pluginWidget = new QWidget;
        pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
        pluginWidget->setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);
        initUi();
        initComponent();
        initConnect();
    }
    return pluginWidget;
}

void WlanConnect::setParentWidget(QWidget* widget){
    if (parentWidget != nullptr) {
        return;
    }
    parentWidget = widget;
}

void WlanConnect::setPluginType(PluginType type, bool useSwitch)
{
    Q_UNUSED(useSwitch)
    if (type == SIMPLE) {
        m_isSimpleMode = true;
    } else {
        m_isSimpleMode = false;
    }
}

void WlanConnect::initUi()
{
    pluginWidget->setFixedSize(WIDGET_WIDTH,WIDGET_HEIGHT);

    thread = new QThread;
    manager = new KyNetworkManager();
    manager->moveToThread(thread);
    connect(thread, &QThread::started, manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(thread, &QThread::finished, manager, &KyNetworkManager::deleteLater);
    thread->start();

    while (!manager->isInitFinished()) {
        ::usleep(1000);
    }

    m_mainLayout = new QVBoxLayout(pluginWidget);
    m_mainLayout->setContentsMargins(NO_MARGINS);
    m_mainLayout->setSpacing(NO_SPACING);
    pluginWidget->setLayout(m_mainLayout);


    m_titleFrame = new QFrame(pluginWidget);
    m_titleFrame->setFixedHeight(53);
    m_titleLayout = new QHBoxLayout(m_titleFrame);
    m_titleLayout->setContentsMargins(TITLE_LAYOUT_MARGINS);
    m_titleLabel = new QLabel(m_titleFrame);
    m_titleLabel->setText(tr("WLAN"));
    m_wirelessSwitch = new KSwitchButton(pluginWidget);
    m_wirelessSwitch->installEventFilter(this);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_wirelessSwitch);
    m_titleDivider = new Divider(pluginWidget);

    m_scrollFrame = new QFrame(pluginWidget);
    if (m_isSimpleMode) {
        m_scrollFrame->setFixedHeight(383);
    } else {
        m_scrollFrame->setFixedHeight(330);
    }
    m_scrollLayout = new QVBoxLayout(m_scrollFrame);
    m_scrollLayout->setContentsMargins(NO_MARGINS);
    m_scrollLayout->setSpacing(NO_SPACING);
    m_scrollFrame->setLayout(m_scrollLayout);

    m_scrollArea = new QScrollArea(m_scrollFrame);
    m_scrollArea->setFrameShape(QFrame::Shape::NoFrame);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setBackgroundRole(QPalette::Base);
    m_scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setContentsMargins(NO_MARGINS);

    m_scrollLayout->addWidget(m_scrollArea);

    m_listWidget = new QWidget(pluginWidget);
    m_listWidget->setFixedWidth(WIDGET_WIDTH);
    m_scrollAreaLayout = new QVBoxLayout(m_listWidget);
    m_scrollAreaLayout->setSpacing(NO_SPACING);
    m_scrollAreaLayout->setContentsMargins(NO_MARGINS);
    m_scrollAreaLayout->setAlignment(Qt::AlignTop);
    m_listWidget->setLayout(m_scrollAreaLayout);

    m_listWidget->setMinimumWidth(MIN_ITEM_WIDTH);
    m_scrollArea->setWidget(m_listWidget);

    m_settingsDivider = new Divider(pluginWidget);
    m_settingsFrame = new QFrame(pluginWidget);
    m_settingsFrame->setFixedHeight(TITLE_FRAME_HEIGHT);

    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(TITLE_LAYOUT_MARGINS);
    m_settingsLabel = new KBorderlessButton(m_settingsFrame);
    m_settingsLabel->setCursor(Qt::PointingHandCursor);
    m_settingsLabel->setText(tr("Settings"));

    m_settingsLayout->addWidget(m_settingsLabel);
    m_settingsLayout->addStretch();
    m_settingsFrame->setLayout(m_settingsLayout);

    m_mainLayout->addWidget(m_titleFrame);
    m_mainLayout->addWidget(m_titleDivider);
    m_mainLayout->addWidget(m_scrollFrame);
    if (!m_isSimpleMode) {
        m_mainLayout->addStretch();
        m_mainLayout->addWidget(m_settingsDivider);
        m_mainLayout->addWidget(m_settingsFrame);
    } else {
        m_settingsDivider->hide();
        m_settingsFrame->hide();
    }

    //适配主题
    if (!m_isSimpleMode) {
        //主题切换
        onPaletteChanged();
        const QByteArray style_id(THEME_SCHAME);
        if (QGSettings::isSchemaInstalled(style_id)) {
            m_styleGsettings = new QGSettings(style_id);
            connect(m_styleGsettings, &QGSettings::changed, this, [=](QString key){
                if ("styleName" == key) {
                    onPaletteChanged();
                }
            });
        } else {
            qDebug() << "Gsettings interface \"org.ukui.style\" is not exist!";
        }

        //适配半透明控件
        m_wirelessSwitch->setTranslucent(true);
        m_scrollArea->setProperty("needTranslucent", true);

    } else {
        QPalette pal = m_scrollArea->palette();
        pal.setBrush(QPalette::Base, QColor(0,0,0,0));     //背景透明
        m_scrollArea->setPalette(pal);
    }
}


bool WlanConnect::eventFilter(QObject *w, QEvent *e) {
    if (w == m_wirelessSwitch && e->type() == QEvent::MouseButtonRelease) {
        if (!m_wirelessSwitch->isCheckable()) {
//            showDesktopNotify(tr("No wireless device avaliable"));
        } else {
            Q_EMIT setWirelessNetworkEnabled(!m_wirelessSwitch->isChecked());
        }
        return true;
    }
    return QObject::eventFilter(w,e);
}

void WlanConnect::initComponent() {

    m_wirelessSwitch->blockSignals(true);
    m_wirelessSwitch->setChecked(manager->getWirelessEnabled());
    m_wirelessSwitch->blockSignals(false);

    getDeviceList(deviceList);
    if (deviceList.isEmpty()) {
        qDebug() << "[WlanConnect]no device exist when init, set switch disable";
        m_wirelessSwitch->setChecked(false);
        m_wirelessSwitch->setCheckable(false);
        qDebug() << "m_wirelessSwitch  setCheckable setChecked" << false;
    }

    initNet();

    if (!m_wirelessSwitch->isChecked() || deviceList.isEmpty()) {
        hideLayout(m_scrollAreaLayout);
    } else {
        if(m_scrollAreaLayout->layout()->count() > 0) {
            QLayoutItem *firstItem = m_scrollAreaLayout->layout()->itemAt(0);
            ItemFrame *firstItemFrame = qobject_cast<ItemFrame *>(firstItem->widget());
            firstItemFrame->m_divider->hide();
        }
    }


    connect(m_settingsLabel, &KBorderlessButton::clicked, this, [=]() {
        runExternalApp();
    });

    m_wlanDetailPagePtrMap.clear();
}

void WlanConnect::initConnect()
{
    connect(manager, &KyNetworkManager::wirelessStateChange, this, &WlanConnect::onActiveConnectionChanged);
    connect(manager, &KyNetworkManager::deviceStateChange, this, &WlanConnect::onDeviceStatusChanged);
    connect(manager, &KyNetworkManager::wifiEnabledChanged, this, &WlanConnect::onWifiEnabledChanged);
    connect(manager, &KyNetworkManager::secuTypeChange, this, &WlanConnect::onNetworkSecuTypeChange);
    connect(manager, &KyNetworkManager::wirelessConnectionRemove, this, &WlanConnect::onWirelessConnectionRemove);
    connect(manager, &KyNetworkManager::wirelessConnectionAdd, this, &WlanConnect::onWirelessConnectionAdd);
    connect(manager, &KyNetworkManager::wirelessConnectionUpdate, this, &WlanConnect::onWirelessConnectionUpdate);
    connect(manager, &KyNetworkManager::wirelessDeviceAdd, this, &WlanConnect::onWirelessDeviceAdd);
    connect(manager, &KyNetworkManager::deviceRemove, this, &WlanConnect::onWirelessDeviceRemove);
    connect(manager, &KyNetworkManager::wirelessDeviceNameUpdate, this, &WlanConnect::onDeviceNameChanged);
    connect(manager, &KyNetworkManager::wifiNetworkAdd, this, &WlanConnect::onNetworkAdd);
    connect(manager, &KyNetworkManager::wifiNetworkRemove, this, &WlanConnect::onNetworkRemove);
    connect(manager, &KyNetworkManager::wifiNetworkUpdate, this, &WlanConnect::onNetworkUpdate);
    connect(manager, &KyNetworkManager::updateWifiList, this, &WlanConnect::updateList);

    connect(this, &WlanConnect::requestWirelessScan, manager, &KyNetworkManager::onRequestWirelessScan);
    connect(this, &WlanConnect::activateConnection, manager, &KyNetworkManager::onActivateConnection);
    connect(this, &WlanConnect::deactivateConnection, manager, &KyNetworkManager::onDeactivateConnection);
    connect(this,&WlanConnect::addAndActivateNormalWifi, manager, &KyNetworkManager::onAddAndActivateNormalWifi);
    connect(this, &WlanConnect::setWirelessNetworkEnabled, manager, &KyNetworkManager::onSetWirelessNetworkEnabled);
    connect(this, &WlanConnect::deleteConnect, manager, &KyNetworkManager::onDeleteConnect);

    connect(manager, &KyNetworkManager::wirelessStateChange, this, &WlanConnect::updateNetworkModeState);
    connect(manager, &KyNetworkManager::wirelessStateChange, this, [=](QString deviceName, QString ssid, QString uuid, KyConnectState status) {
        Q_EMIT connectStateChanged(uuid, status);
    });

    //定时20s扫描
    m_scanTimer = new QTimer(this);
    m_scanTimer->start(SCANTIMER);
    connect(m_scanTimer, &QTimer::timeout, this, &WlanConnect::reScan);

    //10s重新排序
    //    m_updateTimer = new QTimer(this);
    //    m_updateTimer->start(UPDATETIMER);
    //    connect(m_updateTimer, &QTimer::timeout, this, &WlanConnect::updateList);

    reScan();
}

void WlanConnect::reScan()
{
    Q_EMIT requestWirelessScan();
}

//更新列表顺序 ->从so中重新拿 然后插+更新icon
void WlanConnect::updateList(QString devName, QList<KyActivateItem> connectItemList, QList<KyWirelessNetItem> list)
{
    if (!m_wirelessSwitch->isChecked()) {
        return;
    }

    if (!deviceFrameMap.keys().contains(devName)) {
        return;
    }

    resortWifiList(deviceFrameMap[devName], connectItemList, list);
}

void WlanConnect::resortWifiList(ItemFrame *frame, QList<KyActivateItem> connectItemList, QList<KyWirelessNetItem> list)
{
    qDebug() << "resortWifiList" << frame->deviceFrame->deviceLabel->text();
    if(nullptr == frame || frame->lanItemLayout->count() <= 0) {
        return;
    }
    int frameIndex = 0;
    QString activateSsid = "";
    for (int i = 0; i < connectItemList.size(); ++i) {
        if (connectItemList.at(i).m_connStatus == CONNECT_STATE_ACTIVATED) {
            activateSsid = connectItemList.at(i).m_ssid;
            for (int i = 0; i < list.size(); ++i) {
                if (list.at(i).m_NetSsid == activateSsid) {
                    updateIcon(frame->itemMap[list.at(i).m_NetSsid],
                            list.at(i).m_signalStrength,
                            list.at(i).m_secuType,
                            list.at(i).m_isApConnection);
                    break;
                }
            }
            frameIndex++;
            qDebug() << "resortWifiList no" << frameIndex << activateSsid << list.at(i).m_signalStrength;
            break;
        }
    }


    for (int listIndex = 0; listIndex < list.size(); ++listIndex) {
        if (frameIndex > frame->lanItemLayout->count() - 1) {
            return;
        }
        if (frame->itemMap.contains(list.at(listIndex).m_NetSsid) && list.at(listIndex).m_NetSsid != activateSsid) {
            frame->lanItemLayout->removeWidget(frame->itemMap[list.at(listIndex).m_NetSsid]);
            frame->lanItemLayout->insertWidget(frameIndex, frame->itemMap[list.at(listIndex).m_NetSsid]);
            if (!frame->itemMap[list.at(listIndex).m_NetSsid]->getIsLoading()) {
                updateIcon(frame->itemMap[list.at(listIndex).m_NetSsid],
                        list.at(listIndex).m_signalStrength,
                        list.at(listIndex).m_secuType,
                        list.at(listIndex).m_isApConnection);
            }
            frameIndex++;
            qDebug() << "resortWifiList no" << frameIndex << list.at(listIndex).m_NetSsid << list.at(listIndex).m_signalStrength;
        } else {
            qDebug() << "not find " << list.at(listIndex).m_NetSsid << " in current list, ignore";
        }
    }
    qDebug() << "resort finish";
}

void WlanConnect::updateIcon(WlanItem *item, int signalStrength, QString security, bool isApConnection)
{
    qDebug() << "updateIcon" << item->getName();

    int sign = setSignal(signalStrength);
    bool isLock = true;
    if (security.isEmpty()) {
        isLock = false;
    } else {
        isLock = true;
    }

    QString iconamePath;
    if (isApConnection) {
        iconamePath = KApSymbolic;
    } else {
        iconamePath = wifiIcon(isLock, sign);
    }
    QIcon searchIcon = QIcon::fromTheme(iconamePath);

    item->setItemIcon(searchIcon);
    qDebug() << "updateIcon" << item->getName() << " finish";
}

//device status change
void WlanConnect::onDeviceStatusChanged()
{

}

void WlanConnect::onDeviceNameChanged(QString oldName, QString newName)
{
    qDebug() << "============onDeviceNameChanged" << oldName << newName;
    if (!deviceFrameMap.contains(oldName) || !deviceList.contains(oldName)) {
        qWarning() << "[WlanConnect]onDeviceNameChanged no such device " << oldName;
        return;
    }

    if (deviceFrameMap.contains(newName) && deviceList.contains(newName)) {
        qWarning() << "[WlanConnect]onDeviceNameChanged already has device " << newName;
        return;
    }
    removeDeviceFrame(oldName);

    getDeviceList(deviceList);
    if (deviceList.contains(newName)) {
        addDeviceFrame(newName);
        initNetListFromDevice(newName);
        setSwitchStatus(manager->getWirelessEnabled());
    }
}

//activeconnect status change
void WlanConnect::onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, KyConnectState status)
{
    qDebug() << "onActiveConnectionChanged" << ssid << status;
    if (!m_wirelessSwitch->isChecked()) {
        return;
    }
    if (uuid.isEmpty()) {
        return;
    }
    WlanItem * item= nullptr;
    //device ssid 有可能均为空
    if (deviceName.isEmpty() || ssid.isEmpty()) {
        if (status == CONNECT_STATE_ACTIVATING || status == CONNECT_STATE_ACTIVATED) {
            return;
        }
        QMap<QString, ItemFrame*>::iterator iter;
        for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
            if (uuid == iter.value()->uuid) {
                QMap<QString, WlanItem*>::iterator itemIter;
                for (itemIter = iter.value()->itemMap.begin(); itemIter != iter.value()->itemMap.end(); itemIter++) {
                    if (itemIter.value()->getUuid() == uuid ) {
                        item = itemIter.value();
                        break;
                    }
                }
                break;
            }
        }
    } else {
        if (!deviceFrameMap.contains(deviceName)) {
            return;
        }
        for (int i = 0; i < deviceFrameMap[deviceName]->itemMap.size(); ++i) {
            if (deviceFrameMap[deviceName]->itemMap.contains(ssid)) {
                item = deviceFrameMap[deviceName]->itemMap[ssid];
                if (status == CONNECT_STATE_ACTIVATED || status == CONNECT_STATE_ACTIVATING) {
                    deviceFrameMap[deviceName]->itemMap[ssid]->setUuid(uuid);
                    deviceFrameMap[deviceName]->uuid = uuid;
                    if (status == CONNECT_STATE_ACTIVATED) {
                        deviceFrameMap[deviceName]->lanItemLayout->removeWidget(item);
                        deviceFrameMap[deviceName]->lanItemLayout->insertWidget(0,item);
                    }
                } else if (status == CONNECT_STATE_DEACTIVATED) {
                    deviceFrameMap[deviceName]->uuid.clear();
                }
                break;
            }
        }
    }

    if (nullptr != item) {
        itemActiveConnectionStatusChanged(item, status);
        if (status == CONNECT_STATE_ACTIVATED ||
                status == CONNECT_STATE_DEACTIVATED) {
            QList<KyWirelessNetItem> list;
            manager->getWifiNetworkList(deviceName, list);
            for (int i = 0; i < list.size(); ++i) {
                if (list.at(i).m_NetSsid == ssid) {
                    updateIcon(item,list.at(i).m_signalStrength,list.at(i).m_secuType,list.at(i).m_isApConnection);
                    break;
                }
            }
        }
    }
}

//wifi add==============================================================
void WlanConnect::onNetworkAdd(QString deviceName, KyWirelessNetItem wlanInfo)
{
    qDebug()<<"[WlanConnect]onNetworkAdd "<< deviceName << " " << wlanInfo.m_NetSsid;
    if(!m_wirelessSwitch->isChecked() || deviceName.isEmpty()) {
        return;
    }

    if (!deviceList.contains(deviceName)) {
        qDebug() << "[WlanConnect]onNetworkAdd not contain " << deviceName << "then add";
        deviceList.append(deviceName);
        addDeviceFrame(deviceName);
        onNetworkAdd(deviceName, wlanInfo);
        return;
    }

    insertOneWlanFrame(deviceFrameMap[deviceName], deviceName, wlanInfo);
}

//wifi remove =============================================================
void WlanConnect::onNetworkRemove(QString deviceName, QString wlannName)
{
    //当前无此设备 忽略
    if (deviceName.isEmpty() || !deviceFrameMap.contains(deviceName)) {
        qWarning() << "[WlanConnect]recieve network remove,but no such device:" << deviceName;
        return;
    }
    qDebug()<<"[WlanConnect]Wifi remove device:" << deviceName << ",wlan name:" << wlannName;
    removeOneWlanFrame(deviceFrameMap[deviceName], deviceName, wlannName);

    //删除对应的详情页
    if (m_wlanDetailPagePtrMap.contains(deviceName) && m_wlanDetailPagePtrMap[deviceName].contains(wlannName)) {
        if (m_wlanDetailPagePtrMap[deviceName][wlannName] != nullptr) {
            delete m_wlanDetailPagePtrMap[deviceName][wlannName];
            m_wlanDetailPagePtrMap[deviceName][wlannName] = nullptr;
        }
    }
}

//wifi secu Update =============================================================
void WlanConnect::onNetworkUpdate(QString deviceName, QString wlannName, KyWirelessNetItem item)
{
    //当前无此设备 忽略
    if (deviceName.isEmpty() || !deviceFrameMap.contains(deviceName)) {
        qWarning() << "[WlanConnect]recieve network update,but no such device:" << deviceName;
        return;
    }
    qDebug()<<"[WlanConnect]Wifi update device:" << deviceName << ",wlan name:" << wlannName;

    bool isFind = false;
    QMap<QString, WlanItem*>::iterator itemIter;
    for (itemIter = deviceFrameMap[deviceName]->itemMap.begin(); itemIter != deviceFrameMap[deviceName]->itemMap.end(); ++itemIter) {
        if (itemIter.value()->getName() == wlannName) {
            itemIter.value()->setSecuType(item.m_secuType);
            isFind = true;
            break;
        }
    }

    if (!isFind) {
        insertOneWlanFrame(deviceFrameMap[deviceName], deviceName, item);
    }
}

//获取设备列表=======================================================
void WlanConnect::getDeviceList(QStringList &list)
{
    manager->getNetworkDeviceList(DEVICE_TYPE_WIFI, list);
}

//设置开关
void WlanConnect::setSwitchStatus(bool status)
{
    m_wirelessSwitch->setCheckable(true);
    m_wirelessSwitch->blockSignals(true);
    m_wirelessSwitch->setChecked(status);
    m_wirelessSwitch->blockSignals(false);
    if (!status) {
        hideLayout(m_scrollAreaLayout);
    } else {
        showLayout(m_scrollAreaLayout);
    }
}

//初始化整体列表和单设备列表
void WlanConnect::initNet() {
    //先构建每个设备的列表头
    qDebug() << deviceList <<deviceList.size();
    for (int i = 0; i < deviceList.size(); ++i) {
        addDeviceFrame(deviceList.at(i));
    }
    //再填充每个设备的列表
    for (int i = 0; i < deviceList.size(); ++i) {
        initNetListFromDevice(deviceList.at(i));
    }
}

//初始化设备列表
void WlanConnect::initNetListFromDevice(QString deviceName)
{
    qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName;
    if (!m_wirelessSwitch->isChecked()) {
        qWarning() << "[WlanConnect]initNetListFromDevice " << deviceName << " switch off";
        return;
    }
    if (!deviceFrameMap.contains(deviceName)) {
        qWarning() << "[WlanConnect]initNetListFromDevice " << deviceName << " not exist";
        return;
    }
    QList<KyWirelessNetItem> list;
    manager->getWifiNetworkList(deviceName, list);

    for (int i = 0; i < list.size(); ++i) {
        KyWirelessNetItem item = list.at(i);
        addOneWlanFrame(deviceFrameMap[deviceName], deviceName, item);
    }
    QList<KyActivateItem> activateList;
    manager->getActiveConnectionList(deviceName, CONNECT_TYPE_WIRELESS, activateList);
    if (activateList.size() != 0) {
        onActiveConnectionChanged(deviceName,activateList.at(0).m_ssid, activateList.at(0).m_uuid, activateList.at(0).m_connStatus);

        for (KyActivateItem item : activateList) {
            initActiveNetworkMode(deviceName, item);
        }
    }
}

//高级设置
void WlanConnect::runExternalApp() {
    QProcess process;
    process.startDetached("ukui-control-center -m wlanconnect");
}

//根据信号强度分级+安全性分图标
QString WlanConnect::wifiIcon(bool isLock, int strength) {
    switch (strength) {
    case 1:
        return isLock ? KWifiLockSymbolic : KWifiSymbolic;
    case 2:
        return isLock ? KWifiLockGood : KWifiGood;
    case 3:
        return isLock ? KWifiLockOK : KWifiOK;
    case 4:
        return isLock ? KWifiLockLow : KWifiLow;
    case 5:
        return isLock ? KWifiLockNone : KWifiNone;
    default:
        return "";
    }
}

//根据信号强度分级
int WlanConnect::setSignal(int signal) {
    int signalLv = 0;

    if (signal > 75) {
        signalLv = 1;
    } else if (signal > 55 && signal <= 75) {
        signalLv = 2;
    } else if (signal > 35 && signal <= 55) {
        signalLv = 3;
    } else if (signal > 15 && signal  <= 35) {
        signalLv = 4;
    } else if (signal <= 15) {
        signalLv = 5;
    }
    return signalLv;
}

//隐藏
void WlanConnect::hideLayout(QVBoxLayout * layout) {
    for (int i = layout->layout()->count()-1; i >= 0; --i) {
        QLayoutItem *it = layout->layout()->itemAt(i);
        ItemFrame *itemFrame = qobject_cast<ItemFrame *>(it->widget());
        itemFrame->hide();
    }
}

//显示
void WlanConnect::showLayout(QVBoxLayout * layout) {
    for (int i = layout->layout()->count()-1; i >= 0; --i) {
        QLayoutItem *it = layout->layout()->itemAt(i);
        ItemFrame *itemFrame = qobject_cast<ItemFrame *>(it->widget());
        itemFrame->show();
    }

    if(layout->layout()->count() > 0) {
        QLayoutItem *firstItem = layout->layout()->itemAt(0);
        ItemFrame *firstItemFrame = qobject_cast<ItemFrame *>(firstItem->widget());
        firstItemFrame->m_divider->hide();
    }
}

//增加设备
void WlanConnect::addDeviceFrame(QString devName)
{
    qDebug() << "[WlanConnect]addDeviceFrame " << devName;
    ItemFrame *itemFrame = new ItemFrame(devName, pluginWidget);
    m_scrollAreaLayout->addWidget(itemFrame);
    itemFrame->deviceFrame->deviceLabel->setText(devName);
    deviceFrameMap.insert(devName, itemFrame);
    if (deviceList.indexOf(devName) < 0) {
        deviceList << devName;
    }

    connect(itemFrame, &ItemFrame::addNetItemClick , [=](){
        //加入网络
        itemFrame->showJoinPage(m_isSimpleMode, parentWidget);
    });

    connect(itemFrame, &ItemFrame::connectHideNormalConnect, manager, &KyNetworkManager::onAddAndActivateNormalWifi);
    connect(itemFrame, &ItemFrame::connectHideTtlsConnect, manager, &KyNetworkManager::onAddAndActiveWirelessEnterPriseTtlsConnect);
    connect(itemFrame, &ItemFrame::connectHidePeapConnect, manager, &KyNetworkManager::onAddAndActiveWirelessEnterPrisePeapConnect);
}

//减少设备
void WlanConnect::removeDeviceFrame(QString devName)
{
    qDebug() << "[WlanConnect]removeDeviceFrame " << devName;
    if (deviceFrameMap.contains(devName)) {
        ItemFrame *item = deviceFrameMap[devName];
        if (item->lanItemFrame->layout() != NULL) {
            QLayoutItem* layoutItem;
            while ((layoutItem = item->lanItemFrame->layout()->takeAt(0)) != NULL) {
                delete layoutItem->widget();
                delete layoutItem;
            }
            item->itemMap.clear();
        }
        delete item;
        item = nullptr;
        item->disconnect(this);
        deviceFrameMap.remove(devName);
    }
//    getDeviceList(deviceList);
    deviceList.removeOne(devName);
}

//增加ap
void WlanConnect::addOneWlanFrame(ItemFrame *frame, QString deviceName, KyWirelessNetItem &wlanInfo)
{
    if (nullptr == frame) {
        return;
    }

    if (frame->itemMap.contains(wlanInfo.m_NetSsid)) {
        qDebug() << "[WlanConnect]Already exist a wifi " << wlanInfo.m_NetSsid << " in " << deviceName;
        return;
    }
    //设置单项的信息
    int sign = setSignal(wlanInfo.m_signalStrength);
    WlanItem * wlanItem = new WlanItem(m_isSimpleMode, pluginWidget);
    QString iconamePath;
    if (wlanInfo.m_isApConnection) {
        iconamePath = KApSymbolic;
    } else {
        iconamePath = wifiIcon(!wlanInfo.m_secuType.isEmpty(), sign);
    }

    QIcon searchIcon = QIcon::fromTheme(iconamePath);
    wlanItem->setItemIcon(searchIcon);
    wlanItem->setName(wlanInfo.m_NetSsid);
    wlanItem->setSecuType(wlanInfo.m_secuType);
    wlanItem->setUuid(wlanInfo.m_connectUuid);
    wlanItem->setPath(wlanInfo.m_dbusPath);
    wlanItem->setInterface(deviceName);

    connect(wlanItem, &WlanItem::infoButtonClick, this, [=]{
        //todo 详情页
        showWlanDetailPage(deviceName, wlanItem);
    });

    connect(wlanItem, &WlanItem::itemClick, this, [=] {
        if (wlanItem->getStatus() || wlanItem->getIsLoading()) {
            Q_EMIT deactivateConnection(wlanItem->getUuid());
        } else {
            if (wlanItem->getExpend()) {
                wlanItem->setExpend(false);
            } else {
                //先看加密类型 再看是否已有配置
                if (!wlanItem->getUuid().isEmpty()) {
                    activateConnection(wlanItem->getUuid(), wlanItem->getInterface());
                } else {
                    if (wlanItem->getSecuType().isEmpty()) {
                        KyWirelessConnectSetting sett;
                        KySecuType secuType = KySecuType::NONE;
                        sett.m_ssid = wlanItem->getName();
                        sett.m_ifaceName = wlanItem->getInterface();
                        Q_EMIT addAndActivateNormalWifi(sett, secuType);
                    } else {
                        if (wlanItem->getSecuType().indexOf("802.1X") != -1) {
                            //todo 企业网页面
                            wlanItem->showEnterPricePage(deviceName, parentWidget);
                        } else {
                            wlanItem->setExpend(true);
                            setOtherItemExpandedFalse(deviceName, wlanInfo.m_NetSsid);
                        }
                    }
                }
            }
        }
    });
    connect(wlanItem, &WlanItem::connectButtonClick, this, [=](QString password, bool isAutoConnect) {
        KySecuType secuType;
        if (wlanItem->getSecuType().indexOf("WPA3")) {
            secuType = KySecuType::WPA3_PERSONAL;
        } else {
            secuType = KySecuType::WPA_AND_WPA2_PERSONAL;
        }
        KyWirelessConnectSetting sett;
        sett.m_ssid = wlanItem->getName();
        sett.m_psk = password;
        sett.m_isAutoConnect = isAutoConnect;
        sett.m_ifaceName = wlanItem->getInterface();
        Q_EMIT addAndActivateNormalWifi(sett, secuType);
        wlanItem->setExpend(false);
    });

    connect(wlanItem, &WlanItem::connectPeapConnect, manager, &KyNetworkManager::onAddAndActiveWirelessEnterPrisePeapConnect);
    connect(wlanItem, &WlanItem::connectTtlsConnect, manager, &KyNetworkManager::onAddAndActiveWirelessEnterPriseTtlsConnect);
    //记录到deviceFrame的itemMap中
    deviceFrameMap[deviceName]->itemMap.insert(wlanInfo.m_NetSsid, wlanItem);

    qDebug()<<"add " << wlanInfo.m_NetSsid << " to " << deviceName << " list";
    frame->lanItemLayout->addWidget(wlanItem);
}

//减少ap
void WlanConnect::removeOneWlanFrame(ItemFrame *frame, QString deviceName, QString ssid)
{
    if (nullptr == frame) {
        return;
    }

    if (frame->itemMap.contains(ssid)) {
        qDebug() << "[WlanConnect]removeOneWlanFrame " << deviceName << ssid;
        frame->lanItemLayout->removeWidget(frame->itemMap[ssid]);
        delete frame->itemMap[ssid];
        frame->itemMap.remove(ssid);
    }
}

void WlanConnect::insertOneWlanFrame(ItemFrame *frame, QString devName, KyWirelessNetItem &wlanInfo)
{
    addOneWlanFrame(frame, devName, wlanInfo);
}

void WlanConnect::itemActiveConnectionStatusChanged(WlanItem *item, int status)
{
    if (status == CONNECT_STATE_ACTIVATING) {
        item->startLoading();
    } else if (status == CONNECT_STATE_ACTIVATED) {
        item->stopLoading();
        item->setStatus(true);
    } else if (status == CONNECT_STATE_DEACTIVATING) {
        item->startLoading();
    } else if (status == CONNECT_STATE_DEACTIVATED) {
        item->stopLoading();
        item->setStatus(false);
    }
}

void WlanConnect::onNetworkSecuTypeChange(QString deviceName, QString ssid, QString securityType)
{
    Q_UNUSED(deviceName)
    Q_UNUSED(ssid)
    Q_UNUSED(securityType)
}

void WlanConnect::onWifiEnabledChanged(bool status)
{
    //    if (m_wirelessSwitch->isCheckable()) {
    setSwitchStatus(status);
    //    }
}

void WlanConnect::onWirelessConnectionRemove(QString deviceName, QString ssid)
{
    qDebug() << "onWirelessConnectionRemove" << deviceName << ssid;
    WlanItem *item = findItem(deviceName,ssid);
    if (nullptr == item) {
        return;
    }

    if (item->getStatus()) {
        onActiveConnectionChanged(deviceName, ssid, item->getUuid(), CONNECT_STATE_DEACTIVATED);
    }
    item->setUuid("");
    item->setPath("");
}

void WlanConnect::onWirelessConnectionAdd(QString deviceName, QString ssid, QString uuid, QString dbusPath)
{
    qDebug() << "onWirelessConnectionAdd" << deviceName << ssid;
    WlanItem *item = findItem(deviceName,ssid);
    if (nullptr == item) {
        return;
    }
    if (!item->getUuid().isEmpty()
            && !item->getPath().isEmpty())
        item->setUuid(uuid);
    item->setPath(dbusPath);
}

void WlanConnect::onWirelessConnectionUpdate(QString deviceName, QString ssid, QString uuid, QString dbusPath, KySecuType connectSecuType)
{

}

void WlanConnect::onWirelessDeviceAdd(QString deviceName)
{
    addDeviceFrame(deviceName);
    setSwitchStatus(manager->getWirelessEnabled());
}

void WlanConnect::onWirelessDeviceRemove(QString deviceName)
{
    if (!deviceFrameMap.contains(deviceName) || deviceList.indexOf(deviceName) < 0) {
        return;
    }
    removeDeviceFrame(deviceName);
    if (deviceList.isEmpty()) {
        setSwitchStatus(false);
        m_wirelessSwitch->setCheckable(false);
    }
}

WlanItem* WlanConnect::findItem(QString devName, QString ssid)
{
    if (!deviceFrameMap.contains(devName)) {
        return nullptr;
    }

    if (!deviceFrameMap[devName]->itemMap.contains(ssid)) {
        return nullptr;
    }

    return deviceFrameMap[devName]->itemMap[ssid];
}

void WlanConnect::setOtherItemExpandedFalse(QString devName, QString ssid)
{
    QMap<QString, ItemFrame *>::iterator iter;
    for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
        if (iter.key() != devName) {
            QMap<QString, WlanItem *>::iterator itemIter;
            for (itemIter = iter.value()->itemMap.begin(); itemIter != iter.value()->itemMap.end(); itemIter++) {
                itemIter.value()->setExpend(false);
            }
        } else {
            QMap<QString, WlanItem *>::iterator itemIter;
            for (itemIter = iter.value()->itemMap.begin(); itemIter != iter.value()->itemMap.end(); itemIter++) {
                if (itemIter.value()->getName() != ssid) {
                    itemIter.value()->setExpend(false);
                }
            }
        }
    }
}

void WlanConnect::showWlanDetailPage(QString deviceName, WlanItem *item)
{
    if (item == nullptr) {
        return;
    }
    QString connName = item->getName();
    QString connUuid = item->getUuid();
    bool isActivated = item->getStatus();

    qDebug()<< LOG_FLAG << "the info button of lan is clicked! uuid = "
            << connUuid << "; name = " << connName
            << "." <<Q_FUNC_INFO << __LINE__;

    if (m_wlanDetailPagePtrMap.contains(deviceName) && m_wlanDetailPagePtrMap[deviceName].contains(connName)) {
        if (m_wlanDetailPagePtrMap[deviceName][connName] != nullptr) {
            qDebug() << LOG_FLAG << "ShowWlanDetailPage" << connName << "already create,just raise";
            KWindowSystem::raiseWindow(m_wlanDetailPagePtrMap[deviceName][connName]->winId());
            return;
        }
    }

    NetDetail *netDetail = new NetDetail(deviceName, connName, connUuid, isActivated, true, false);
    QMap<QString, NetDetail*> page;
    page.insert(connName, netDetail);
    m_wlanDetailPagePtrMap.insert(deviceName, page);
    netDetail->show();

    connect(netDetail, &NetDetail::detailPageClose, [&](QString devName, QString wlanSsid, QString wlanUuid){
        if (m_wlanDetailPagePtrMap.contains(devName) && m_wlanDetailPagePtrMap[devName].contains(wlanSsid)) {
            m_wlanDetailPagePtrMap[devName][wlanSsid] = nullptr;
        }
    });
}

void WlanConnect::initActiveNetworkMode(QString deviceName, KyActivateItem activeItem)
{
    int configType = NetworkModeConfig::getInstance()->getNetworkModeConfig(activeItem.m_uuid);
    if (configType == -1) {
        NetworkModeConfig::getInstance()->setNetworkModeConfig(activeItem.m_uuid,
                                                               deviceName,
                                                               activeItem.m_connName,
                                                               KSC_FIREWALL_PUBLIC);
    } else {
        NetworkModeConfig::getInstance()->setNetworkModeConfig(activeItem.m_uuid,
                                                               deviceName,
                                                               activeItem.m_connName,
                                                               configType);
    }
}

void WlanConnect::updateNetworkModeState(QString deviceName, QString ssid, QString uuid, KyConnectState status)
{
      if (status == CONNECT_STATE_ACTIVATED) {
        int configType = NetworkModeConfig::getInstance()->getNetworkModeConfig(uuid);
        if (configType == -1) {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC); //默认公有配置
            FirewallDialog *fireWallDialog = new FirewallDialog(); //弹窗 供用户配置
            fireWallDialog->setUuid(uuid);
            fireWallDialog->setWindowTitle(ssid);

            connect(fireWallDialog, &FirewallDialog::setPrivateNetMode, this, [=](){
                fireWallDialog->hide();
                NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
            });

            connect(fireWallDialog, &FirewallDialog::setPublicNetMode, this, [=](){
                fireWallDialog->hide();
                NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
            });

            connect(this, &WlanConnect::connectStateChanged, fireWallDialog, &FirewallDialog::closeMyself);

            fireWallDialog->show();
            fireWallDialog->centerToScreen();

        } else if (configType == KSC_FIREWALL_PUBLIC) {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
        } else if (configType == KSC_FIREWALL_PRIVATE) {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
        }
    } else if (status == CONNECT_STATE_DEACTIVATED) {
        NetworkModeConfig::getInstance()->breakNetworkConnect(uuid, deviceName, ssid);
      }
}

void WlanConnect::onPaletteChanged()
{
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, pal.base().color());
    pluginWidget->setAutoFillBackground(true);
    pluginWidget->setPalette(pal);
}
