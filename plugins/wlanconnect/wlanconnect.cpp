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
#include "ui_wlanconnect.h"
#include <kysdk/applications/accessinfohelper.h>

#include <QGSettings>
#include <QProcess>
#include <QTimer>
#include <QtDBus>
#include <QDir>
#include <QDebug>
#include <QtAlgorithms>

#define WIRELESS_TYPE 1

#define SCANTIMER  20 * 1000
#define UPDATETIMER 5 * 1000

#define SPACING 8

#define EXCELLENT_SIGNAL 80
#define GOOD_SIGNAL 55
#define OK_SIGNAL 30
#define LOW_SIGNAL 5
#define NONE_SIGNAL 0

#define SIGNAL_EXCELLENT 1
#define SIGNAL_GOOD      2
#define SIGNAL_OK        3
#define SIGNAL_LOW       4
#define SIGNAL_NONE      5
#define ICON_SIZE 16,16

#define KYLIN_APP_MANAGER_NAME           "com.kylin.AppManager"
#define KYLIN_APP_MANAGER_PATH           "/com/kylin/AppManager"
#define KYLIN_APP_MANAGER_INTERFACE      "com.kylin.AppManager"

#define DBUSSERVICE_UKCC        "org.ukui.ukcc.session"
#define DBUSPATH_UKCC           "/"
#define DBUSINTERFACE_UKCC      "org.ukui.ukcc.session.interface"

#define ACTIVATING   1
#define ACTIVATED    2
#define DEACTIVATING 3
#define DEACTIVATED  4

#define NO_MARGINS 0,0,0,0
#define MAIN_LAYOUT_MARGINS 0,0,0,8

const QString WIRELESS_SWITCH = "wirelessswitch";
const QByteArray GSETTINGS_SCHEMA = "org.ukui.kylin-nm.switch";

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

const QString KWifi6Symbolic        = "ukui-wifi6-full-symbolic";
const QString KWifi6PlusSymbolic    = "ukui-wifi6+-full-symbolic";
const QString KWifi7Symbolic        = "ukui-wifi7-full-symbolic";

const QString KWifi6LockSymbolic    = "ukui-wifi6-full-pwd-symbolic";
const QString KWifi6PlusLockSymbolic= "ukui-wifi6+-full-pwd-symbolic";
const QString KWifi7LockSymbolic    = "ukui-wifi7-full-pwd-symbolic";

const QString KWifi6Good            = "ukui-wifi6-high-symbolic";
const QString KWifi6PlusGood        = "ukui-wifi6+-high-symbolic";
const QString KWifi7Good            = "ukui-wifi7-high-symbolic";

const QString KWifi6LockGood        = "ukui-wifi6-high-pwd-symbolic";
const QString KWifi6PlusLockGood    = "ukui-wifi6+-high-pwd-symbolic";
const QString KWifi7LockGood        = "ukui-wifi7-high-pwd-symbolic";

const QString KWifi6OK              = "ukui-wifi6-medium-symbolic";
const QString KWifi6PlusOK          = "ukui-wifi6+-high-medium-symbolic";
const QString KWifi7OK              = "ukui-wifi7-medium-symbolic";

const QString KWifi6LockOK          = "ukui-wifi6-medium-pwd-symbolic";
const QString KWifi6PlusLockOK      = "ukui-wifi6+-medium-pwd-symbolic";
const QString KWifi7LockOK          = "ukui-wifi7-medium-pwd-symbolic";

const QString KWifi6Low             = "ukui-wifi6-low-symbolic";
const QString KWifi6PlusLow         = "ukui-wifi6+-low-symbolic";
const QString KWifi7Low             = "ukui-wifi7-low-symbolic";

const QString KWifi6LockLow         = "ukui-wifi6-low-pwd-symbolic";
const QString KWifi6PlusLockLow     = "ukui-wifi6+-low-pwd-symbolic";
const QString KWifi7LockLow         = "ukui-wifi7-low-pwd-symbolic";

const QString KWifi6None            = "ukui-wifi6-none-symbolic";
const QString KWifi6PlusNone        = "ukui-wifi6+-none-symbolic";
const QString KWifi7None            = "ukui-wifi7-none-symbolic";

const QString KWifi6LockNone        = "ukui-wifi6-none-pwd-symbolic";
const QString KWifi6PlusLockNone    = "ukui-wifi6+-none-pwd-symbolic";
const QString KWifi7LockNone        = "ukui-wifi7-none-pwd-symbolic";

const QString KLanSymbolic      = ":/img/plugins/netconnect/eth.svg";
const QString NoNetSymbolic     = ":/img/plugins/netconnect/nonet.svg";

const QString KApSymbolic       = "network-wireless-hotspot-symbolic";

const QString IsApConnection    = "1";
const QString WarningIconName   = "dialog-warning";

void WlanConnect::showDesktopNotify(const QString &message)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QList<QVariant> args;
    args<<(tr("Settings"))
         <<((unsigned int) 0)
         <<QString("gnome-dev-ethernet")
         <<tr("Settings desktop message") //显示的是什么类型的信息
         <<message //显示的具体信息
         <<QStringList()
         <<QVariantMap{{"desktop-entry","kylin-nm"}}
         <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
}

WlanConnect::WlanConnect() :  m_firstLoad(true)
{
    QTranslator* translator = new QTranslator(this);
    translator->load("/usr/share/kylin-nm/wlanconnect/" + QLocale::system().name());
    QApplication::installTranslator(translator);

    pluginName = tr("WLAN");
    pluginType = NETWORK;
}

WlanConnect::~WlanConnect()
{
    if (!m_firstLoad) {
        delete ui;
        ui = nullptr;
    }
    delete m_interface;
}

QString WlanConnect::plugini18nName()
{
    return pluginName;
}

int WlanConnect::pluginTypes()
{
    return pluginType;
}

QWidget *WlanConnect::pluginUi()
{
    if (m_firstLoad) {
        m_firstLoad = false;

        ui = new Ui::WlanConnect;
        pluginWidget = new QWidget;
        pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
        ui->setupUi(pluginWidget);
        qDBusRegisterMetaType<QVector<QStringList>>();
        qDBusRegisterMetaType<QStringList>();
        m_interface = new QDBusInterface("com.kylin.network", "/com/kylin/network",
                                         "com.kylin.network",
                                         QDBusConnection::sessionBus());
        if(!m_interface->isValid()) {
            qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        }
        initSearchText();
        initComponent();
    }
    return pluginWidget;
}

const QString WlanConnect::name() const {

    return QStringLiteral("wlanconnect");
}

bool WlanConnect::isEnable() const
{
    //get isEnable
    QDBusInterface dbus("com.kylin.network",
                        "/com/kylin/network",
                        "com.kylin.network",
                        QDBusConnection::sessionBus());
    if (!dbus.isValid()) {
        return false;
    }

    QMap<QString,bool> map;
    QDBusReply<QVariantMap> reply = dbus.call(QStringLiteral("getDeviceListAndEnabled"),1);
    if (!reply.isValid())
    {
        qWarning() << "[wlanConnect]getWirelessDeviceList error:" << reply.error().message();
        return false;
    }

    QVariantMap::const_iterator item = reply.value().cbegin();
    while (item != reply.value().cend()) {
        map.insert(item.key(), item.value().toBool());
        item ++;
    }

    bool isEnabled = !map.isEmpty();

    qWarning() << Q_FUNC_INFO << __LINE__ << isEnabled;

    const QByteArray schema("org.ukui.control-center.plugins");
    if (QGSettings::isSchemaInstalled(schema)) {
        qWarning() << Q_FUNC_INFO << __LINE__;
        //get gsettings
        QGSettings *showSettings;
        QString path("/org/ukui/control-center/plugins/wlanconnect/");
        showSettings = new QGSettings(schema, path.toUtf8());

        QVariant enabledState = showSettings->get("show");

        qWarning() << Q_FUNC_INFO << __LINE__ << enabledState.toBool();

        //set gsettings
        if (!enabledState.isValid() || enabledState.isNull()) {
            qWarning() << Q_FUNC_INFO << __LINE__ << "QGSettins get plugin show status error";
        } else {
            if (enabledState.toBool() != isEnabled) {
                showSettings->set("show", isEnabled);
            }
        }
        delete showSettings;
        showSettings = nullptr;
    }

    return isEnabled;
}

bool WlanConnect::isShowOnHomePage() const
{
    return true;
}

QIcon WlanConnect::icon() const
{
    return QIcon::fromTheme("network-wireless-signal-excellent-symbolic");
}

QString WlanConnect::translationPath() const
{
    return "/usr/share/kylin-nm/wlanconnect/%1.ts";
}

void WlanConnect::initSearchText() {
    //~ contents_path /wlanconnect/Add Others"
    tr("Add Others");
    //~ contents_path /wlanconnect/Advanced settings"
    ui->detailBtn->setText(tr("Advanced settings"));
    KDK_EXTEND_ALL_INFO_FORMAT(ui->detailBtn, "WlanConnect", "", "advanced settings of wlanconnect");

    ui->titleLabel->setText(tr("WLAN"));
    //~ contents_path /wlanconnect/WLAN
    ui->openLabel->setText(tr("WLAN"));
}

bool WlanConnect::eventFilter(QObject *w, QEvent *e) {
    if (e->type() == QEvent::Enter) {
        if (w->findChild<QWidget*>())
            w->findChild<QWidget*>()->setStyleSheet("QWidget{background: palette(button);border-radius:4px;}");
    } else if (e->type() == QEvent::Leave) {
        if (w->findChild<QWidget*>())
            w->findChild<QWidget*>()->setStyleSheet("QWidget{background: palette(base);border-radius:4px;}");
    }

    if (w == m_wifiSwitch && m_wifiSwitch->isEnabled()) {
        if (e->type() == QMouseEvent::MouseButtonRelease) {
            m_wifiSwitch->clearFocus();//放if外会崩溃，如果按住鼠标不松，按空格依旧存在问题
            if (!getSwitchBtnEnable()) {
                showDesktopNotify(tr("No wireless network card detected"));
            } else {
                UkccCommon::buriedSettings(QString("wlanconnect"), QString("Open"), QString("settings"),!getSwitchBtnState()?"true":"false");
                if (m_interface != nullptr && m_interface->isValid()) {
                    m_interface->call(QStringLiteral("setWirelessSwitchEnable"), !getSwitchBtnState());
                }
                return true;
            }
        }
    }

    return QObject::eventFilter(w,e);
}

void WlanConnect::initComponent() {
    m_wifiSwitch = new KSwitchButton(pluginWidget);
    KDK_EXTEND_ALL_INFO_FORMAT(m_wifiSwitch, "WlanConnect", "", "wife switch");
    ui->openWIifLayout->setSpacing(5);
    m_wirelessNoDeviceIconLabel=new QLabel(ui->openWifiFrame);
    ui->openWIifLayout->addWidget(m_wirelessNoDeviceIconLabel);
    m_wirelessNoDeviceLabel=new QLabel(ui->openWifiFrame);
    ui->openWIifLayout->addWidget(m_wirelessNoDeviceLabel);
    ui->openWIifLayout->addWidget(m_wifiSwitch);
    ui->openWIifLayout->setContentsMargins(0,0,8,0);
    ui->detailLayOut_3->setContentsMargins(MAIN_LAYOUT_MARGINS);
    ui->verticalLayout_3->setContentsMargins(NO_MARGINS);
    ui->verticalLayout_3->setSpacing(8);
    ui->availableLayout->setSpacing(SPACING);

    m_wifiSwitch->installEventFilter(this);

    //开关
    initSwtichState();

    //获取设备列表
    getDeviceList(deviceList);
    setSwitchStatus();

    initNet();
    wlanComponnetSettings();

    if (!getSwitchBtnState() || deviceList.isEmpty() || !m_interface->isValid()) {
        hideLayout(ui->availableLayout);
    }

    // 有线网络断开或连接时刷新可用网络列表
    connect(m_interface, SIGNAL(wlanactiveConnectionStateChanged(QString, QString, QString, int)), this, SLOT(onActiveConnectionChanged(QString, QString, QString, int)), Qt::QueuedConnection);
    // 无线网络新增时添加网络
    connect(m_interface, SIGNAL(wlanAdd(QString, QStringList)), this, SLOT(onNetworkAdd(QString, QStringList)), Qt::QueuedConnection);
    // 删除无线网络
    connect(m_interface, SIGNAL(wlanRemove(QString, QString)), this, SLOT(onNetworkRemove(QString, QString)), Qt::QueuedConnection);
    // 网卡插拔处理
    connect(m_interface, SIGNAL(wirelessDeviceStatusChanged()), this, SLOT(onDeviceStatusChanged()), Qt::QueuedConnection);
    // 网卡name处理
    connect(m_interface, SIGNAL(deviceNameChanged(QString, QString, int)), this, SLOT(onDeviceNameChanged(QString, QString, int)), Qt::QueuedConnection);
    connect(m_interface, SIGNAL(wirelessSwitchBtnChanged(bool)), this, SLOT(onSwitchBtnChanged(bool)), Qt::QueuedConnection);
    connect(m_interface, SIGNAL(timeToUpdate()), this, SLOT(updateList()), Qt::QueuedConnection);
    // 无线网络开关(使用空格)
    connect(m_wifiSwitch, SIGNAL(stateChanged(bool)), this, SLOT(switchStatusChanged(bool)),Qt::QueuedConnection);
    // 高级设置
    connect(ui->detailBtn, &QPushButton::clicked, this, [=](bool checked) {
        Q_UNUSED(checked)
        UkccCommon::buriedSettings(QString("wlanconnect"), QString("Advanced settings"), QString("clicked"));
        runExternalApp();
    });

    //定时20s扫描
    m_scanTimer = new QTimer(this);
    m_scanTimer->start(SCANTIMER);
    connect(m_scanTimer, &QTimer::timeout, this, &WlanConnect::reScan, Qt::QueuedConnection);
    reScan();
}

void WlanConnect::switchStatusChanged(bool status)
{
    m_interface->call(QStringLiteral("setWirelessSwitchEnable"), status);
}

void WlanConnect::reScan()
{
    qDebug() << "time to rescan wifi";
    if (m_interface != nullptr && m_interface->isValid()) {
        qDebug() << "[WlanConnect]call reScan" << __LINE__;
        m_interface->call("reScan");
        qDebug() << "[WlanConnect]call reScan respond" << __LINE__;
    }
}

//更新列表顺序
void WlanConnect::updateList()
{
    if (!getSwitchBtnState()) {
        return;
    }
    qDebug() << "update list";
    if(m_interface != nullptr && m_interface->isValid()) {
        QMap<QString, QList<QStringList>> variantList = getWirelessList();

        if (variantList.size() == 0) {
            qDebug() << "[WlanConnect]updateList " << " list empty";
            return;
        }

        QMap<QString, QList<QStringList>>::iterator iter;

        for (iter = variantList.begin(); iter != variantList.end(); iter++) {
            if (deviceFrameMap.contains(iter.key())) {
                QList<QStringList> wifiList = iter.value();
                resortWifiList(deviceFrameMap[iter.key()], wifiList);
                deviceFrameMap[iter.key()]->filletStyleChange();
            }
        }
    }
}

void WlanConnect::resortWifiList(ItemFrame *frame, QList<QStringList> list)
{
    if(nullptr == frame || frame->lanItemLayout->count() <= 0 || list.isEmpty()) {
        return;
    }
    qDebug() << "begin resort" << frame->deviceFrame->deviceLabel->text();

    int frameIndex = 0;
    int listIndex = 1;
    if (list.at(0).size() > 1) {
        if (frame->itemMap.contains(list.at(0).at(0))) {
            frame->lanItemLayout->removeWidget(frame->itemMap[list.at(0).at(0)]);
            frame->lanItemLayout->insertWidget(0, frame->itemMap[list.at(0).at(0)]);
            qDebug() << "active resort insert position 0" << list.at(0).at(0);
            frame->itemMap[list.at(0).at(0)]->isAcitve = true;
            frame->itemMap[list.at(0).at(0)]->uuid = list.at(0).at(3);
            frame->uuid = list.at(0).at(3);
            frame->itemMap[list.at(0).at(0)]->statusLabel->setText(tr("connected"));
            if (list.at(0).size() > 5) {
                updateIcon(frame->itemMap[list.at(0).at(0)], list.at(0).at(1), list.at(0).at(2), list.at(0).at(4), list.at(0).at(5).toInt());
            }
            frameIndex ++;
        }
    } else {
        qDebug() << " no active connection when resort";
        if (!frame->uuid.isEmpty()) {
            QMap<QString, WlanItem*>::iterator itemIter;
            for (itemIter = frame->itemMap.begin(); itemIter != frame->itemMap.end(); itemIter++) {
                if (itemIter.value()->uuid == frame->uuid ) {
                    WlanItem * item= nullptr;
                    item = itemIter.value();
                    qDebug() << "a active connect missing when resort";
                    itemIter.value()->uuid.clear();
                    itemActiveConnectionStatusChanged(item, DEACTIVATED);
                    break;
                }
            }
        }
        frame->uuid.clear();
    }

    for ( ; listIndex < list.size(); listIndex++) {
        if (frameIndex > frame->lanItemLayout->count() - 1) {
            return;
        }
        if (frame->itemMap.contains(list.at(listIndex).at(0))) {
            frame->lanItemLayout->removeWidget(frame->itemMap[list.at(listIndex).at(0)]);
            frame->lanItemLayout->insertWidget(frameIndex, frame->itemMap[list.at(listIndex).at(0)]);
            qDebug() << "custom resort " << list.at(listIndex).at(0) <<" insert position" << frameIndex;
            if (frame->itemMap[list.at(listIndex).at(0)]->isAcitve) {
                frame->itemMap[list.at(listIndex).at(0)]->isAcitve = false;
                frame->itemMap[list.at(listIndex).at(0)]->uuid.clear();
                frame->itemMap[list.at(listIndex).at(0)]->statusLabel->setText("");
            }
            if (list.at(listIndex).size() > 4) {
                updateIcon(frame->itemMap[list.at(listIndex).at(0)], list.at(listIndex).at(1), list.at(listIndex).at(2), list.at(listIndex).at(3), list.at(listIndex).at(4).toInt());
            }
            frameIndex++;
        } else {
            qDebug() << "not find " << list.at(listIndex).at(0) << " in current list, ignore";
        }
    }
    // 更新样式和分割线
    frame->filletStyleChange();

    qDebug() << "resort finish";
}

void WlanConnect::updateIcon(WlanItem *item, QString signalStrength, QString security, QString isApConnection, int category)
{
    qDebug() << "updateIcon" << item->titileLabel->text()<<" security:"<<security;

    int sign = setSignal(signalStrength);
    bool isLock = true;
    if (security.isEmpty()) {
        isLock = false;
    } else {
        isLock = true;
    }

    QString iconamePath;
    if (isApConnection == IsApConnection && item->isAcitve) {
        iconamePath = KApSymbolic;
    } else {
        iconamePath = wifiIcon(isLock, sign, category);
    }
    QIcon searchIcon = QIcon::fromTheme(iconamePath);
    item->iconLabel->setPixmap(searchIcon.pixmap(searchIcon.actualSize(QSize(ICON_SIZE))));
    qDebug() << "updateIcon" << item->titileLabel->text() << " finish";
}

//device add or remove
void WlanConnect::onDeviceStatusChanged()
{
    qDebug()<<"[WlanConnect]onDeviceStatusChanged";
    QEventLoop eventloop;
    QTimer::singleShot(300, &eventloop, SLOT(quit()));
    eventloop.exec();
    QStringList list;
    getDeviceList(list);

    QStringList removeList,addList;

    //remove的设备
    for (int i = 0; i< deviceList.size(); ++i) {
        if (!list.contains(deviceList.at(i))) {
            qDebug() << "[WlanConnect]onDeviceStatusChanged " << deviceList.at(i) << "was removed";
            removeList << deviceList.at(i);
        }
    }

    //add的设备
    for (int i = 0; i< list.size(); ++i) {
        if (!deviceList.contains(list.at(i))) {
            qDebug() << "[WlanConnect]onDeviceStatusChanged " << list.at(i) << "was add";
            addList << list.at(i);
        }
    }

    for (int i = 0; i < removeList.size(); ++i) {
        removeDeviceFrame(removeList.at(i));
    }

    deviceList = list;
    setSwitchStatus();


    for (int i = 0; i < addList.size(); ++i) {
        addDeviceFrame(addList.at(i));
        initNetListFromDevice(addList.at(i));
    }
}

void WlanConnect::onDeviceNameChanged(QString oldName, QString newName, int type)
{
    if (WIRELESS_TYPE != type || !deviceFrameMap.contains(oldName) || !deviceList.contains(oldName)) {
        qDebug() << "[WlanConnect]onDeviceNameChanged no such device " << oldName;
        return;
    }

    if (deviceFrameMap.contains(newName) && deviceList.contains(newName)) {
        qDebug() << "[WlanConnect]onDeviceNameChanged already has device " << newName;
        return;
    }

    qDebug() << "[WlanConnect]onDeviceNameChanged " << oldName << "change to" << newName;

    removeDeviceFrame(oldName);
    removeDeviceFrame(newName);

    getDeviceList(deviceList);
    if (deviceList.contains(newName)) {
        addDeviceFrame(newName);
        initNetListFromDevice(newName);
    }
}

void WlanConnect::onSwitchBtnChanged(bool state)
{
    if (getSwitchBtnState() == state) {
        return;
    }
    setSwitchBtnEnable(true);
    setSwitchBtnState(state);
    if (!getSwitchBtnState()) {
        hideLayout(ui->availableLayout);
    } else {
        showLayout(ui->availableLayout);
    }

    setSwitchStatus();
}

//activeconnect status change
void WlanConnect::onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, int status)
{
    if (!getSwitchBtnState()) {
        return;
    }
    if (uuid.isEmpty()) {
        return;
    }
    WlanItem * item= nullptr;
    //device ssid 有可能均为空
    if (deviceName.isEmpty() || ssid.isEmpty()) {
        if (status == ACTIVATING || status == ACTIVATED) {
            return;
        }
        QMap<QString, ItemFrame*>::iterator iter;
        for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
            if (uuid == iter.value()->uuid) {
                QMap<QString, WlanItem*>::iterator itemIter;
                for (itemIter = iter.value()->itemMap.begin(); itemIter != iter.value()->itemMap.end(); itemIter++) {
                    if (itemIter.value()->uuid == uuid) {
                        item = itemIter.value();
                        if (status == DEACTIVATED) {
                            itemIter.value()->uuid.clear();
                        }
                        break;
                    }
                }
                break;
            } else if (uuid != iter.value()->uuid && status == DEACTIVATED) {
                if (!ssid.isEmpty()) {
                    QMap<QString, WlanItem*>::iterator itemIter;
                    for (itemIter = iter.value()->itemMap.begin(); itemIter != iter.value()->itemMap.end(); itemIter++) {
                        if (itemIter.value()->titileLabel->text() == ssid && itemIter.value()->uuid == uuid) {
                           itemActiveConnectionStatusChanged(itemIter.value(), status);
                            break;
                        }
                    }
                }
            }
        }
    } else {
        if (!deviceFrameMap.contains(deviceName)) {
            return;
        }
        if (deviceFrameMap[deviceName]->itemMap.contains(ssid)) {
            item = deviceFrameMap[deviceName]->itemMap[ssid];
            if (status == ACTIVATED || status == ACTIVATING) {
                deviceFrameMap[deviceName]->itemMap[ssid]->uuid = uuid;
                deviceFrameMap[deviceName]->uuid = uuid;
                if (status == ACTIVATED) {
                    deviceFrameMap[deviceName]->lanItemLayout->removeWidget(item);
                    deviceFrameMap[deviceName]->lanItemLayout->insertWidget(0,item);
                    deviceFrameMap[deviceName]->filletStyleChange();
                }
            } else if (status == DEACTIVATED) {
                deviceFrameMap[deviceName]->itemMap[ssid]->uuid.clear();
                deviceFrameMap[deviceName]->uuid.clear();
                //todo 断开后排序 现在等下次更新列表 自动排序
            }
        } else {
            if (uuid == deviceFrameMap[deviceName]->uuid) {
                QMap<QString, WlanItem*>::iterator itemIter;
                for (itemIter = deviceFrameMap[deviceName]->itemMap.begin(); itemIter != deviceFrameMap[deviceName]->itemMap.end(); itemIter++) {
                    if (itemIter.value()->uuid == uuid) {
                        item = itemIter.value();
                        if (status == DEACTIVATED) {
                            itemIter.value()->uuid.clear();
                            deviceFrameMap[deviceName]->uuid.clear();
                        }
                        break;
                    }
                }
            }
        }
    }

    if (nullptr != item) {
        itemActiveConnectionStatusChanged(item, status);
    }
}

// wifi add
void WlanConnect::onNetworkAdd(QString deviceName, QStringList wlanInfo)
{
    qDebug()<<"[WlanConnect]onNetworkAdd "<< deviceName << " " << wlanInfo;
    if(!getSwitchBtnState() || deviceName.isEmpty()) {
        return;
    }

    if (!deviceList.contains(deviceName)) {
        qDebug() << "[WlanConnect]onNetworkAdd not contain " << deviceName << "then add";
        deviceList.append(deviceName);
        addDeviceFrame(deviceName);
        onNetworkAdd(deviceName, wlanInfo);
        return;
    }

    bool isLock = true;
    if (wlanInfo.at(2) == "") {
        isLock = false;
    } else {
        isLock = true;
    }

    QMap<QString, ItemFrame *>::iterator iter;
    for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
        if (deviceName == iter.key()) {
            addOneWlanFrame(iter.value(), deviceName, wlanInfo.at(0), wlanInfo.at(1), "", isLock, false, WIRELESS_TYPE, wlanInfo.at(3), wlanInfo.at(3).toInt(), DEACTIVATED);
        }
    }

}

// wifi remove
void WlanConnect::onNetworkRemove(QString deviceName, QString wlannName)
{
    //当前无此设备 忽略
    if (deviceName.isEmpty() || !deviceFrameMap.contains(deviceName)) {
        qDebug() << "[WlanConnect]recieve network remove,but no such device:" << deviceName;
        return;
    }
    qDebug()<<"[WlanConnect]Wifi remove device:" << deviceName << ",wlan name:" << wlannName;
    QMap<QString, ItemFrame *>::iterator iter;
    for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
        if (deviceName == iter.key()) {
            removeOneWlanFrame(iter.value(), deviceName, wlannName);
        }
    }
}

// 获取设备列表
void WlanConnect::getDeviceList(QStringList &list)
{
    if (m_interface == nullptr || !m_interface->isValid()) {
        return;
    }
    qDebug() << "[WlanConnect]call getDeviceListAndEnabled"  << __LINE__;
    QDBusReply<QVariantMap> reply = m_interface->call(QStringLiteral("getDeviceListAndEnabled"),1);
    qDebug() << "[WlanConnect]call getDeviceListAndEnabled respond"  << __LINE__;
    if(!reply.isValid())
    {
        qWarning() << "[WlanConnect]getWirelessDeviceList error:" << reply.error().message();
        return;
    }

    QMap<QString,bool> map;
    QVariantMap::const_iterator item = reply.value().cbegin();
    while (item != reply.value().cend()) {
        map.insert(item.key(), item.value().toBool());
        item ++;
    }

    //筛选已托管(managed)网卡
    QMap<QString, bool>::iterator iters;
    for (iters = map.begin(); iters != map.end(); ++iters) {
        if (iters.value() == true) {
            list << iters.key();
        }
    }

}

void WlanConnect::initSwtichState()
{
    if (m_interface == nullptr || !m_interface->isValid()) {
        return;
    }

    QDBusMessage result = m_interface->call("getWirelessSwitchBtnState");
    qDebug() << "[WlanConnect]call getWirelessSwitchBtnState respond" << __LINE__;
    if(result.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "[WlanConnect]getWirelessSwitchBtnState error:" << result.errorMessage();
        return;
    }

    bool state = result.arguments().at(0).toBool();
    setSwitchBtnEnable(true);
    setSwitchBtnState(state);
}

void WlanConnect::setSwitchBtnEnable(bool state)
{
    if (m_wifiSwitch != nullptr) {
        m_wifiSwitch->setEnabled(state);
    }
}

bool WlanConnect::getSwitchBtnEnable()
{
    if (m_wifiSwitch != nullptr) {
        return m_wifiSwitch->isEnabled();
    }
}

void WlanConnect::setSwitchBtnState(bool state)
{
    if (m_wifiSwitch != nullptr) {
        m_wifiSwitch->blockSignals(true);
        m_wifiSwitch->setChecked(state);
        m_wifiSwitch->blockSignals(false);
    }
}

bool WlanConnect::getSwitchBtnState()
{
    if (m_wifiSwitch != nullptr) {
        return m_wifiSwitch->isChecked();
    }
}

bool WlanConnect::LaunchApp(QString desktopFile)
{
    QDBusInterface m_appManagerDbusInterface(KYLIN_APP_MANAGER_NAME,
                                             KYLIN_APP_MANAGER_PATH,
                                             KYLIN_APP_MANAGER_INTERFACE,
                                             QDBusConnection::sessionBus());//局部变量

    if (!m_appManagerDbusInterface.isValid()) {
        qWarning()<<"m_appManagerDbusInterface init error";
        return false;
    } else {
        QDBusReply<bool> reply =m_appManagerDbusInterface.call("LaunchApp",desktopFile);
        return reply;
    }
}

//初始化整体列表和单设备列表
void WlanConnect::initNet()
{
    //先构建每个设备的列表头
    for (int i = 0; i < deviceList.size(); ++i) {
        addDeviceFrame(deviceList.at(i));
    }
    //再填充每个设备的列表
    for (int i = 0; i < deviceList.size(); ++i) {
        initNetListFromDevice(deviceList.at(i));
    }
}

//初始化设备列表 网卡标号问题？
void WlanConnect::initNetListFromDevice(QString deviceName)
{
    qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName;
    if (!getSwitchBtnState()) {
         qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " switch off";
        return;
    }
    if (!deviceFrameMap.contains(deviceName)) {
        qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " not exist";
        return;
    }
    if (m_interface == nullptr || !m_interface->isValid()) {
        return;
    }

    QMap<QString, QList<QStringList>> variantList = getWirelessList();
    if (variantList.size() == 0) {
        qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " list empty";
        return;
    }

    QMap<QString, QList<QStringList>>::iterator iter;
    for (iter = variantList.begin(); iter != variantList.end(); iter++) {
        if (deviceName == iter.key()) {
            QList<QStringList> wlanListInfo = iter.value();
            if (wlanListInfo.size() <= 0) {
                break;
            }
            //处理列表 已连接
            qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " acitved wifi " << wlanListInfo.at(0);
            addActiveItem(deviceFrameMap[deviceName], deviceName,  wlanListInfo.at(0));
            //处理列表 未连接
            for (int i = 1; i < wlanListInfo.length(); i++) {
                qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " deacitved wifi " << wlanListInfo.at(i);
                addCustomItem(deviceFrameMap[deviceName], deviceName, wlanListInfo.at(i));
            }
        }
    }
    return;
}

//高级设置
void WlanConnect::runExternalApp() {
    if (!LaunchApp("nm-connection-editor.desktop")){
        QString cmd = "nm-connection-editor";
        QProcess process(this);
        process.startDetached(cmd);
    }
}

//根据信号强度分级+安全性分图标
QString WlanConnect::wifiIcon(bool isLock, int strength, int category) {

    if (category == 0) {
        switch (strength)
        {
        case SIGNAL_EXCELLENT:
            return isLock ? KWifiLockSymbolic : KWifiSymbolic;
        case SIGNAL_GOOD:
            return isLock ? KWifiLockGood : KWifiGood;
        case SIGNAL_OK:
            return isLock ? KWifiLockOK : KWifiOK;
        case SIGNAL_LOW:
            return isLock ? KWifiLockLow : KWifiLow;
        case SIGNAL_NONE:
            return isLock ? KWifiLockNone : KWifiNone;
        default:
            return "";
        }
    } else if (category == 1) {
        switch (strength)
        {
        case SIGNAL_EXCELLENT:
            return isLock ? KWifi6LockSymbolic : KWifi6Symbolic;
        case SIGNAL_GOOD:
            return isLock ? KWifi6LockGood : KWifi6Good;
        case SIGNAL_OK:
            return isLock ? KWifi6LockOK : KWifi6OK;
        case SIGNAL_LOW:
            return isLock ? KWifi6LockLow : KWifi6Low;
        case SIGNAL_NONE:
            return isLock ? KWifi6LockNone : KWifi6None;
        default:
            return "";
        }
    } else if (category == 3) {//显示wifi7图标
        switch (strength)
        {
        case SIGNAL_EXCELLENT:
            return isLock ? KWifi7LockSymbolic : KWifi7Symbolic;
        case SIGNAL_GOOD:
            return isLock ? KWifi7LockGood : KWifi7Good;
        case SIGNAL_OK:
            return isLock ? KWifi7LockOK : KWifi7OK;
        case SIGNAL_LOW:
            return isLock ? KWifi7LockLow : KWifi7Low;
        case SIGNAL_NONE:
            return isLock ? KWifi7LockNone : KWifi7None;
        default:
            return "";
        }
    } else {
        switch (strength)
        {
        case SIGNAL_EXCELLENT:
            return isLock ? KWifi6PlusLockSymbolic : KWifi6PlusSymbolic;
        case SIGNAL_GOOD:
            return isLock ? KWifi6PlusLockGood : KWifi6PlusGood;
        case SIGNAL_OK:
            return isLock ? KWifi6PlusLockOK : KWifi6PlusOK;
        case SIGNAL_LOW:
            return isLock ? KWifi6PlusLockLow : KWifi6PlusLow;
        case SIGNAL_NONE:
            return isLock ? KWifi6PlusLockNone : KWifi6PlusNone;
        default:
            return "";
        }
    }
}

//根据信号强度分级
int WlanConnect::setSignal(QString lv) {
    int signal = lv.toInt();
    int signalLv = 0;

    if (signal > EXCELLENT_SIGNAL) {
        signalLv = 1;
    } else if (signal > GOOD_SIGNAL) {
        signalLv = 2;
    } else if (signal > OK_SIGNAL) {
        signalLv = 3;
    } else if (signal > LOW_SIGNAL) {
        signalLv = 4;
    } else {
        signalLv = 5;
    }
    return signalLv;
}

//隐藏
void WlanConnect::hideLayout(QVBoxLayout * layout)
{
    for (int i = layout->layout()->count()-1; i >= 0; --i) {
        QLayoutItem *it = layout->layout()->itemAt(i);
        ItemFrame *itemFrame = qobject_cast<ItemFrame *>(it->widget());
        itemFrame->hide();
    }
}

//显示
void WlanConnect::showLayout(QVBoxLayout * layout)
{
    for (int i = layout->layout()->count()-1; i >= 0; --i) {
        QLayoutItem *it = layout->layout()->itemAt(i);
        ItemFrame *itemFrame = qobject_cast<ItemFrame *>(it->widget());
        itemFrame->show();
    }
}

//获取应该插入哪个位置
int WlanConnect::sortWlanNet(QString deviceName, QString name, QString signal)
{
    if (m_interface == nullptr || !m_interface->isValid()) {
        return 0;
    }

    QMap<QString, QList<QStringList>> variantList = getWirelessList();

    QMap<QString, QList<QStringList>>::iterator iter;
    for (iter = variantList.begin(); iter != variantList.end(); iter++) {
        if (deviceName == iter.key()) {
            QList<QStringList> wlanListInfo = iter.value();
            for (int i = 0; i < wlanListInfo.size(); i++) {
                if (name == wlanListInfo.at(i).at(0)) {
                    return i;
                }
            }
        }
    }
    return 0;
}

void WlanConnect::activeConnect(QString netName, QString deviceName, int type)
{
    if (m_interface == nullptr || !m_interface->isValid()) {
        return;
    }
    QVariantMap value;
    value.insert("ssid",netName);
    value.insert("device",deviceName);
    value.insert("type",type);
    qDebug() << "[WlanConnect]call activateConnect" << __LINE__;
    m_interface->call("requestInputPasswdAgent",value);
    qDebug() << "[WlanConnect]call activateConnect respond" << __LINE__;
}

void WlanConnect::deActiveConnect(QString netName, QString deviceName, int type)
{
    if (m_interface == nullptr || !m_interface->isValid()) {
        return;
    }
    qDebug() << "[WlanConnect]call deActivateConnect" << __LINE__;
    m_interface->call("deActivateConnect",type, deviceName, netName);
    qDebug() << "[WlanConnect]call deActivateConnect respond" << __LINE__;
}


//处理列表 已连接
void WlanConnect::addActiveItem(ItemFrame *frame, QString devName, QStringList infoList)
{
    if (frame == nullptr) {
        return;
    }
    if (infoList.size() == 1) {
        return;
    }

    bool isLock = true;
    if (infoList.at(2) == "") {
        isLock = false;
    } else {
        isLock = true;
    }
    addOneWlanFrame(frame, devName, infoList.at(0), infoList.at(1), infoList.at(3), isLock, true, WIRELESS_TYPE, infoList.at(4), infoList.at(5).toInt(), ACTIVATED);
}

//处理列表 未连接
void WlanConnect::addCustomItem(ItemFrame *frame, QString devName, QStringList infoList)
{
    if (frame == nullptr) {
        return;
    }
    bool isLock = true;
    if (infoList.at(2) == "") {
        isLock = false;
    } else {
        isLock = true;
    }
    addOneWlanFrame(frame, devName, infoList.at(0), infoList.at(1), "", isLock, false, WIRELESS_TYPE, infoList.at(3), infoList.at(4).toInt(), DEACTIVATED);
}

//增加设备
void WlanConnect::addDeviceFrame(QString devName)
{
    qDebug() << "[WlanConnect]addDeviceFrame " << devName;
    ItemFrame *itemFrame = new ItemFrame(devName, pluginWidget);
    ui->availableLayout->addWidget(itemFrame);
    itemFrame->deviceFrame->deviceLabel->setText(tr("card")+/*QString("%1").arg(count)+*/"："+devName);
    deviceFrameMap.insert(devName, itemFrame);

    connect(itemFrame->addWlanWidget, &AddNetBtn::clicked, this, [=](){
        UkccCommon::buriedSettings(QString("wlanconnect"), QString("Add wlan"), QString("clicked"));
        if (m_interface != nullptr && m_interface->isValid()) {
            qDebug() << "[NetConnect]call showAddOtherWlanWidget" << devName  << __LINE__;
            m_interface->call(QStringLiteral("showAddOtherWlanWidget"), devName);
            qDebug() << "[NetConnect]call setDeviceEnable Respond"  << __LINE__;
        }
    });

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
                layoutItem = nullptr;
            }
            item->itemMap.clear();
        }
        delete item;
        item = nullptr;
        deviceFrameMap.remove(devName);
    }
}

//增加ap
void WlanConnect::addOneWlanFrame(ItemFrame *frame, QString deviceName, QString name, QString signal, QString uuid, bool isLock, bool status, int type, QString isApConnection, int category, int activeStatus)
{
    if (nullptr == frame) {
        return;
    }

    bool bApConnection = (isApConnection == IsApConnection);

    if (frame->itemMap.contains(name)) {
            qDebug() << "[WlanConnect]Already exist a wifi " << name << " in " << deviceName;
            return;
    }
    //设置单项的信息
    int sign = setSignal(signal);
    WlanItem * wlanItem = new WlanItem(status, isLock, pluginWidget);
    KDK_EXTEND_ALL_INFO_FORMAT(wlanItem, "WlanConnect", "", "wlan item of wlanconnect");
    QString iconamePath;
    if (bApConnection) {
        iconamePath = KApSymbolic;
    } else {
        iconamePath = wifiIcon(isLock, sign, category);
    }
    QIcon searchIcon = QIcon::fromTheme(iconamePath);
    wlanItem->iconLabel->setPixmap(searchIcon.pixmap(searchIcon.actualSize(QSize(ICON_SIZE))));
    wlanItem->titileLabel->setText(name);
    if (status) {
        wlanItem->statusLabel->setText(tr("connected"));
        frame->uuid = uuid;
        wlanItem->uuid = uuid;
    } else {
        wlanItem->statusLabel->setText("");
    }

    connect(wlanItem->infoLabel, &GrayInfoButton::clicked, this, [=]{
        if (m_interface == nullptr || !m_interface->isValid()) {
            return;
        }
        UkccCommon::buriedSettings(QString("wlanconnect"), QString("info"), QString("clicked"));
        qDebug() << "[WlanConnect]call showPropertyWidget" << __LINE__;
        m_interface->call(QStringLiteral("showPropertyWidget"), deviceName, name);
        qDebug() << "[WlanConnect]call showPropertyWidget respond" << __LINE__;
    });

    connect(wlanItem, &QPushButton::clicked, this, [=] {
        openKylinm();
        QTimer::singleShot(100,this,[=](){
            if (wlanItem->isAcitve) {
                deActiveConnect(name, deviceName, type);
            } else {
                activeConnect(name, deviceName, type);
            }
        });
    });
    //记录到deviceFrame的itemMap中
    deviceFrameMap[deviceName]->itemMap.insert(name, wlanItem);
    int index;
    if (status) {
        index = 0;
    } else {
        index = sortWlanNet(deviceName, name, signal);
    }
    qDebug()<<"insert " << name << " to " << deviceName << " list, postion " << index;
    frame->lanItemLayout->insertWidget(index, wlanItem);
    frame->filletStyleChange();

    if (activeStatus == ACTIVATING || activeStatus == DEACTIVATING) {
        itemActiveConnectionStatusChanged(wlanItem, activeStatus);
    }
}

//减少ap
void WlanConnect::removeOneWlanFrame(ItemFrame *frame, QString deviceName, QString ssid)
{
    if (nullptr == frame) {
        return;
    }

    if (frame->itemMap.contains(ssid)) {
        qDebug() << "[WlanConnect]removeOneWlanFrame " << deviceName << ssid;

        // 找到要移除的item在布局中的位置
        int itemIndex = -1;
        for (int i = 0; i < frame->lanItemLayout->count(); ++i) {
            QLayoutItem* item = frame->lanItemLayout->itemAt(i);
            if (item && item->widget() == frame->itemMap[ssid]) {
                itemIndex = i;
                break;
            }
        }

        frame->lanItemLayout->removeWidget(frame->itemMap[ssid]);
        delete frame->itemMap[ssid];
        frame->itemMap.remove(ssid);
        frame->filletStyleChange();
    }
}

void WlanConnect::itemActiveConnectionStatusChanged(WlanItem *item, int status)
{
    if (status == ACTIVATING) {
        item->startLoading();
    } else if (status == ACTIVATED) {
        item->stopLoading();
        item->statusLabel->clear();
        item->statusLabel->setMinimumSize(36,36);
        item->statusLabel->setMaximumSize(16777215,16777215);
        item->statusLabel->setText(tr("connected"));
        item->isAcitve = true;
    } else if (status == DEACTIVATING) {
        item->startLoading();
    } else if (status == DEACTIVATED) {
        item->stopLoading();
        item->statusLabel->clear();
        item->statusLabel->setMinimumSize(36,36);
        item->statusLabel->setMaximumSize(16777215,16777215);
        item->isAcitve = false;
    }
}

void WlanConnect::openKylinm()
{
    QDBusInterface sidebarIfc("org.ukui.Sidebar",
                              "/org/ukui/Sidebar",
                              "org.ukui.Sidebar",
                              QDBusConnection::sessionBus());
    sidebarIfc.call("shortcutWidgetActive", "org.ukui.shortcut.network", false);
}

QMap<QString, QList<QStringList>> WlanConnect::getWirelessList()
{
    QMap<QString, QList<QStringList>> map;
    QStringList list;
    getDeviceList(list);

    for (int i = 0; i < list.size(); ++i) {
        qDebug() << "[NetConnect]call getWirelessList"  << __LINE__;
        QDBusReply<QVariantList> reply = m_interface->call(QStringLiteral("getWirelessList"), list.at(i));
        qDebug() << "[NetConnect]call getWirelessList respond"  << __LINE__;
        if(!reply.isValid())
        {
            qWarning() << "getWirelessList error:" << reply.error().message();
            break;
        }

        QList<QStringList> llist;
        for (int j = 0; j < reply.value().size(); ++j) {
            llist << reply.value().at(j).toStringList();
        }
        map.insert(list.at(i), llist);
    }
    return map;
}

void WlanConnect::showNoDeiceInfo(bool visible,QString text)
{
    m_wirelessNoDeviceLabel->setText(text);
    if (visible) {
        QIcon warningIcon = QIcon::fromTheme(WarningIconName);
        m_wirelessNoDeviceIconLabel->setPixmap(warningIcon.pixmap(warningIcon.actualSize(QSize(ICON_SIZE))));
        m_wirelessNoDeviceLabel->resize(m_wirelessNoDeviceLabel->sizeHint()); // 设置标签大小以适应内容
        m_wirelessNoDeviceLabel->show();
        m_wirelessNoDeviceIconLabel->show();
    } else {
        m_wirelessNoDeviceLabel->hide();
        m_wirelessNoDeviceIconLabel->hide();
    }
}

bool WlanConnect::getWirelessSwitchBtnState()
{

    if (m_interface == nullptr || !m_interface->isValid()) {
        return false;
    }
    QDBusReply<bool> reply = m_interface->call(QStringLiteral("getWirelessSwitchBtnState"));

    qDebug() << "[WlanConnect]call getWirelessSwitchBtnState respond" << __LINE__;
    if (!reply.isValid()) {
        qWarning() << "[WlanConnect]getWirelessSwitchBtnState error:" << reply.error().message();
        return false;
    }

    return reply.value();
}


void WlanConnect::setSwitchStatus()
{
    setSwitchBtnEnable(true);
    bool status = getWirelessSwitchBtnState();

    setSwitchBtnState(status);
    if (deviceList.isEmpty()) {
        setSwitchBtnState(false);
        setSwitchBtnEnable(false);
    }
    showNoDeiceInfo(false,tr(""));
    if (!getSwitchBtnState()) {
        hideLayout(ui->availableLayout);
        if (deviceList.isEmpty()) showNoDeiceInfo(true,tr("No wireless network card detected"));
    } else {
        showLayout(ui->availableLayout);
    }
}

QMap<QString, QVariant> WlanConnect::getModuleHideStatus()
{
    QDBusReply<QMap<QString,QVariant>> reply_res;

    QDBusInterface iface(DBUSSERVICE_UKCC,
                         DBUSPATH_UKCC,
                         DBUSINTERFACE_UKCC,
                         QDBusConnection::sessionBus());

    if (iface.isValid()) {
        QDBusPendingCall pcall = iface.asyncCall("getModuleHideStatus");
        pcall.waitForFinished();

        QDBusMessage res = pcall.reply();

        if (res.type() == QDBusMessage::ReplyMessage) {
            if (res.arguments().size() > 0) {
                reply_res = res;
                qInfo() << reply_res.value();
            }
        } else {
            qWarning()<< res.errorName() << ": "<< res.errorMessage();
        }
    } else {
        qWarning()<< "dbus isValid";
    }
    return reply_res.value();
}

void WlanConnect::wlanComponnetSettings()
{
    QMap<QString,QVariant> configData = getModuleHideStatus();
    QString moduleSettings = configData.value("wlanconnectSettings").toString();
    QStringList setItems = moduleSettings.split(",");

    foreach (QString setItem, setItems) {
        QStringList item = setItem.split(":");
        if (item.at(0) == "wlanAdvanced") {
            ui->detailBtn->setVisible(item.at(1) == "true");
        }
    }

    QString moduleEnable = configData.value("wlanconnectEnable").toString();
    QStringList enableItems = moduleEnable.split(",");
    foreach (QString setItem, enableItems) {
        QStringList item = setItem.split(":");
        if (item.at(0) == "wlanAdvanced") {
            ui->detailBtn->setEnabled(item.at(1) == "true");
        }
    }
}
