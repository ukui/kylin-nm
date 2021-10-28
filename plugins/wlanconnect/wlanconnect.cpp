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

const QString KLanSymbolic      = ":/img/plugins/netconnect/eth.svg";
const QString NoNetSymbolic     = ":/img/plugins/netconnect/nonet.svg";


#define ACTIVATING   1
#define ACTIVATED    2
#define DEACTIVATING 3
#define DEACTIVATED  4

#define MAIN_LAYOUT_MARGINS 0,0,0,8

bool intThan(int sign1, int sign2)
{
    return sign1 < sign2;
}


WlanConnect::WlanConnect() :  mFirstLoad(true) {

    QTranslator* translator = new QTranslator(this);
    translator->load("/usr/share/kylin-nm/wlanconnect/" + QLocale::system().name());
    QApplication::installTranslator(translator);

    pluginName = tr("WlanConnect");
    pluginType = NETWORK;
}

WlanConnect::~WlanConnect()
{
    if (!mFirstLoad) {
        delete ui;
        ui = nullptr;
    }
    delete m_interface;
    delete m_switchGsettings;
}

QString WlanConnect::plugini18nName() {
    return pluginName;
}

int WlanConnect::pluginTypes() {
    return pluginType;
}

QWidget *WlanConnect::pluginUi() {
    if (mFirstLoad) {
        mFirstLoad = false;

        ui = new Ui::WlanConnect;
        pluginWidget = new QWidget;
        pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
        ui->setupUi(pluginWidget);
        qDBusRegisterMetaType<QVector<QStringList>>();
        m_interface = new QDBusInterface("com.kylin.network", "/com/kylin/network",
                                         "com.kylin.network",
                                         QDBusConnection::sessionBus());
        if(!m_interface->isValid()) {
            qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        }
        initComponent();
    }
    return pluginWidget;
}

const QString WlanConnect::name() const {

    return QStringLiteral("wlanconnect");
}

bool WlanConnect::isEnable() const
{
    return true;
}


bool WlanConnect::isShowOnHomePage() const
{
    return true;
}

QIcon WlanConnect::icon() const
{
    return QIcon();
}

QString WlanConnect::translationPath() const
{
    return "/usr/share/kylin-nm/wlanconnect/%1.ts";
}

void WlanConnect::initSearchText() {
    //~ contents_path /wlanconnect/Advanced settings"
    ui->detailBtn->setText(tr("Advanced settings"));
    ui->titleLabel->setText(tr("WLAN"));
    //~ contents_path /wlanconnect/open
    ui->openLabel->setText(tr("open"));
}

bool WlanConnect::eventFilter(QObject *w, QEvent *e) {
    if (e->type() == QEvent::Enter) {
        if (w->findChild<QWidget*>())
            w->findChild<QWidget*>()->setStyleSheet("QWidget{background: palette(button);border-radius:4px;}");
    } else if (e->type() == QEvent::Leave) {
        if (w->findChild<QWidget*>())
            w->findChild<QWidget*>()->setStyleSheet("QWidget{background: palette(base);border-radius:4px;}");
    }
    return QObject::eventFilter(w,e);
}

void WlanConnect::initComponent() {
    wifiSwtch = new SwitchButton(pluginWidget, false);
    ui->openWIifLayout->addWidget(wifiSwtch);
    ui->detailLayOut_3->setContentsMargins(MAIN_LAYOUT_MARGINS);

    //开关
    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        m_switchGsettings = new QGSettings(GSETTINGS_SCHEMA);
        connect(wifiSwtch, &SwitchButton::checkedChanged, this, [=] (bool checked) {
            if (!m_interface->isValid()) {
                return;
            }
            if (wifiSwtch->getDisabledFlag()) {
                return;
            }
            qDebug() << "[WlanConnect]call setWirelessSwitchEnable " << checked << __LINE__;
            m_interface->call(QStringLiteral("setWirelessSwitchEnable"),checked);
            qDebug() << "[WlanConnect]call setWirelessSwitchEnable respond" << __LINE__;
        });
        setSwitchStatus();
        connect(m_switchGsettings, &QGSettings::changed, this, [=] (const QString &key) {
            if (key == WIRELESS_SWITCH) {
                setSwitchStatus();
            }
        });
    } else {
        wifiSwtch->blockSignals(true);
        wifiSwtch->setChecked(true);
        wifiSwtch->blockSignals(false);
        qDebug()<<"[netconnect] org.ukui.kylin-nm.switch is not installed!";
    }

    //获取设备列表
    getDeviceList(deviceList);
    if (deviceList.isEmpty()) {
        qDebug() << "[WlanConnect]no device exist when init, set switch disable";
        wifiSwtch->setDisabledFlag(true);
        wifiSwtch->setChecked(false);
    }
    initNet();

    if (deviceList.isEmpty() || !m_interface->isValid()) {
        hideLayout(ui->availableLayout);
    }

    // 有线网络断开或连接时刷新可用网络列表
    connect(m_interface, SIGNAL(wlanactiveConnectionStateChanged(QString, QString, QString, int)), this, SLOT(onActiveConnectionChanged(QString, QString, QString, int)), Qt::QueuedConnection);
    //无线网络新增时添加网络
    connect(m_interface, SIGNAL(wlanAdd(QString, QStringList)), this, SLOT(onNetworkAdd(QString, QStringList)), Qt::QueuedConnection);
    //删除无线网络
    connect(m_interface, SIGNAL(wlanRemove(QString, QString)), this, SLOT(onNetworkRemove(QString, QString)), Qt::QueuedConnection);
    //网卡插拔处理
    connect(m_interface, SIGNAL(deviceStatusChanged()), this, SLOT(onDeviceStatusChanged()), Qt::QueuedConnection);
    //信号更新处理 改为每过固定时间 主动获取
//    connect(m_interface, SIGNAL(signalStrengthChange(QString, QString, int)), this, SLOT(updateStrengthList(QString, QString, int)));
    //网卡name处理
    connect(m_interface, SIGNAL(deviceNameChanged(QString, QString, int)), this, SLOT(onDeviceNameChanged(QString, QString, int)), Qt::QueuedConnection);
    
    connect(m_interface, SIGNAL(timeToUpdate()), this, SLOT(updateList()), Qt::QueuedConnection);
    //高级设置
    connect(ui->detailBtn, &QPushButton::clicked, this, [=](bool checked) {
        Q_UNUSED(checked)
        runExternalApp();
    });

    //定时20s扫描
    m_scanTimer = new QTimer(this);
    m_scanTimer->start(SCANTIMER);
    connect(m_scanTimer, &QTimer::timeout, this, &WlanConnect::reScan, Qt::QueuedConnection);
    reScan();

//    m_updateTimer = new QTimer(this);
//    m_updateTimer->start(UPDATETIMER);

}

void WlanConnect::reScan()
{
    qDebug() << "time to rescan wifi";
    if (m_interface->isValid()) {
        qDebug() << "[WlanConnect]call reScan" << __LINE__;
        m_interface->call("reScan");
        qDebug() << "[WlanConnect]call reScan respond" << __LINE__;
    }
}

//更新列表顺序
void WlanConnect::updateList()
{
    if (!wifiSwtch->isChecked()) {
        return;
    }
    qDebug() << "update list";
    if(m_interface->isValid()) {
        qDebug() << "[WlanConnect]call getWirelessList" << __LINE__;
        QDBusMessage result = m_interface->call(QStringLiteral("getWirelessList"));
        qDebug() << "[WlanConnect]call getWirelessList respond" << __LINE__;
        if(result.type() == QDBusMessage::ErrorMessage)
        {
            qWarning() << "getWirelessList error:" << result.errorMessage();
            return;
        }
        auto dbusArg =  result.arguments().at(0).value<QDBusArgument>();
        QMap<QString, QVector<QStringList>> variantList;
        dbusArg >> variantList;

        if (variantList.size() == 0) {
            qDebug() << "[WlanConnect]updateList " << " list empty";
            return;
        }

        QMap<QString, QVector<QStringList>>::iterator iter;

        for (iter = variantList.begin(); iter != variantList.end(); iter++) {
            if (deviceFrameMap.contains(iter.key())) {
                QVector<QStringList> wifiList = iter.value();
                resortWifiList(deviceFrameMap[iter.key()], wifiList);
            }
        }
    }
}

void WlanConnect::resortWifiList(ItemFrame *frame, QVector<QStringList> list)
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
            updateIcon(frame->itemMap[list.at(0).at(0)], list.at(0).at(1), list.at(0).at(2));
            frameIndex ++;
        }
    } else {
        qDebug() << " no active connection when resort";
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
            updateIcon(frame->itemMap[list.at(listIndex).at(0)], list.at(listIndex).at(1), list.at(listIndex).at(2));
            frameIndex++;
        } else {
            qDebug() << "not find " << list.at(listIndex).at(0) << " in current list, ignore";
        }
    }
    qDebug() << "resort finish";
}

void WlanConnect::updateIcon(WlanItem *item, QString signalStrength, QString security)
{
    qDebug() << "updateIcon" << item->titileLabel->text();
    int sign = setSignal(signalStrength);
    bool isLock = true;
    if (security.isEmpty()) {
        isLock = false;
    } else {
        isLock = true;
    }

    QString iconamePath = wifiIcon(isLock, sign);
    QIcon searchIcon = QIcon::fromTheme(iconamePath);
    if (iconamePath != KLanSymbolic && iconamePath != NoNetSymbolic) {
        item->iconLabel->setProperty("useIconHighlightEffect", 0x10);
    }
    item->iconLabel->setPixmap(searchIcon.pixmap(searchIcon.actualSize(QSize(24, 24))));
    qDebug() << "updateIcon" << item->titileLabel->text() << " finish";
}

//wifi strength update
//void WlanConnect::updateStrengthList(QString deviceName, QString ssid, int strength)
//{
//    return;
//    if(!wifiSwtch->isChecked()) {
//        return;
//    }
//    qDebug()<<"[WlanConnect]Update wireless network signal strength：" << deviceName <<ssid << strength;
//    bool isLock = false;
//    QMap<QString, ItemFrame *>::iterator iters;
//    for (iters = deviceWlanlistInfo.deviceLayoutMap.begin(); iters != deviceWlanlistInfo.deviceLayoutMap.end(); iters++) {
//        if (iters.key() == deviceName) {
//            qDebug() << "[WlanConnect] updateStrengthList find " << deviceName << " in deviceWlanlistInfo.deviceLayoutMap";
//            if (iters.value()->lanItemLayout->layout() != NULL) {
//                WlanItem* item = nullptr;
//                QMap<QString, WlanItem*>::iterator iter;
//                for (iter = deviceWlanlistInfo.wlanItemMap.begin(); iter != deviceWlanlistInfo.wlanItemMap.end(); iter++) {
//                    if (iter.key() == ssid) {
//                        item = iter.value();
//                        break;
//                    }
//                }
//                isLock = item->isLock;
//                //remove the item from layout
//                iters.value()->lanItemLayout->removeWidget(item);
//                //get position
//                int index = sortWlanNet(deviceName,ssid, QString::number(strength));
//                //add the item to new position
//                qDebug()<<"更新后位置:"<<index;
//                int sign = setSignal(QString::number(strength));
//                QString iconamePath = wifiIcon(isLock, sign);
//                QIcon searchIcon = QIcon::fromTheme(iconamePath);
//                if (iconamePath != KLanSymbolic && iconamePath != NoNetSymbolic) {
//                    item->iconLabel->setProperty("useIconHighlightEffect", 0x10);
//                }
//                item->iconLabel->setPixmap(searchIcon.pixmap(searchIcon.actualSize(QSize(24, 24))));

//                iters.value()->lanItemLayout->insertWidget(index, item);
//            }
//        }
//    }
//}

//device add or remove=================================
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

    for (int i = 0; i < addList.size(); ++i) {
        addDeviceFrame(addList.at(i));
    }
    deviceList = list;
    if (deviceList.isEmpty()) {
        wifiSwtch->setDisabledFlag(true);
        wifiSwtch->setChecked(false);
    } else {
        wifiSwtch->setDisabledFlag(false);
        setSwitchStatus();
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

    //shan chu chong jian
    removeDeviceFrame(oldName);
    removeDeviceFrame(newName);

    getDeviceList(deviceList);
    if (deviceList.contains(newName)) {
        addDeviceFrame(newName);
        initNetListFromDevice(newName);
    }
}

//activeconnect status change
void WlanConnect::onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, int status)
{
    if (!wifiSwtch->isChecked()) {
        return;
    }
    if (uuid.isEmpty()) {
        return;
    }
    WlanItem * item= nullptr;
    //device ssid 有可能均为空
    if (deviceName.isEmpty() && ssid.isEmpty()) {
        if (status == ACTIVATING || status == ACTIVATED) {
            return;
        }
        QMap<QString, ItemFrame*>::iterator iter;
        for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
            if (uuid == iter.value()->uuid) {
                QMap<QString, WlanItem*>::iterator itemIter;
                for (itemIter = iter.value()->itemMap.begin(); itemIter != iter.value()->itemMap.end(); itemIter++) {
                    if (itemIter.value()->uuid == uuid ) {
                        item = itemIter.value();
                        if (status == 4) {
                            itemIter.value()->uuid.clear();
                        }
                        break;
                    }
                }
                break;
            }
        }
    } else {
        if (deviceName.isEmpty() || ssid.isEmpty()) {
            return;
        }
        if (!deviceFrameMap.contains(deviceName)) {
            return;
        }
        for (int i = 0; i < deviceFrameMap[deviceName]->itemMap.size(); ++i) {
            if (deviceFrameMap[deviceName]->itemMap.contains(ssid)) {
                item = deviceFrameMap[deviceName]->itemMap[ssid];
                if (status == 2) {
                    deviceFrameMap[deviceName]->itemMap[ssid]->uuid = uuid;
                    deviceFrameMap[deviceName]->uuid = uuid;
                    deviceFrameMap[deviceName]->lanItemLayout->removeWidget(item);
                    deviceFrameMap[deviceName]->lanItemLayout->insertWidget(0,item);
                } else if (status == 4) {
                    deviceFrameMap[deviceName]->itemMap[ssid]->uuid.clear();
                    deviceFrameMap[deviceName]->uuid.clear();
                    //todo 断开后排序 现在等下次更新列表 自动排序
                }
                break;
            }
        }
    }

    if (nullptr != item) {
        itemActiveConnectionStatusChanged(item, status);
    }
}

//wifi add===============================================================
void WlanConnect::onNetworkAdd(QString deviceName, QStringList wlanInfo)
{
    qDebug()<<"[WlanConnect]onNetworkAdd "<< deviceName << " " << wlanInfo;
    if(!wifiSwtch->isChecked() || deviceName.isEmpty()) {
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
            addOneWlanFrame(iter.value(), deviceName, wlanInfo.at(0), wlanInfo.at(1), "", isLock, false, WIRELESS_TYPE);
        }
    }

}

//wifi remove =============================================================
void WlanConnect::onNetworkRemove(QString deviceName, QString wlannName)
{
    //开关已关闭 忽略
//    if (!wifiSwtch->isChecked()) {
//        qDebug() << "[WlanConnect]recieve network remove,but wireless switch is off";
//        return;
//    }
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

//获取设备列表=======================================================
void WlanConnect::getDeviceList(QStringList &list)
{
    if (!m_interface->isValid()) {
        return;
    }
    qDebug() << "[WlanConnect]call getDeviceListAndEnabled"  << __LINE__;
    QDBusMessage result = m_interface->call(QStringLiteral("getDeviceListAndEnabled"),1);
    qDebug() << "[WlanConnect]call getDeviceListAndEnabled respond"  << __LINE__;
    if(result.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "[WlanConnect]getWirelessDeviceList error:" << result.errorMessage();
        return;
    }
    auto dbusArg =  result.arguments().at(0).value<QDBusArgument>();
    QMap<QString,bool> map;
    dbusArg >> map;
    list = map.keys();
}

//设置开关
void WlanConnect::setSwitchStatus()
{
//    if (deviceList.size() == 0) {
//        wifiSwtch->blockSignals(true);
//        wifiSwtch->setChecked(false);
//        wifiSwtch->blockSignals(false);
//        return;
//    }
    bool status = m_switchGsettings->get(WIRELESS_SWITCH).toBool();
    qDebug() << "[WlanConnect]setSwitchStatus" << status;
    wifiSwtch->blockSignals(true);
    wifiSwtch->setChecked(status);
    wifiSwtch->blockSignals(false);
    if (!wifiSwtch->isChecked()) {
        hideLayout(ui->availableLayout);
    } else {
        showLayout(ui->availableLayout);
    }
}

//初始化整体列表和单设备列表
void WlanConnect::initNet() {
//    int count = 1;
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
    if (!wifiSwtch->isChecked()) {
         qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " switch off";
        return;
    }
    if (!deviceFrameMap.contains(deviceName)) {
        qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " not exist";
        return;
    }
    if (!m_interface->isValid()) {
        return;
    }
    qDebug() << "[WlanConnect]call getWirelessList"  << __LINE__;
    QDBusMessage result = m_interface->call(QStringLiteral("getWirelessList"));
    qDebug() << "[WlanConnect]call getWirelessList respond"  << __LINE__;
    if(result.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "getWirelessList error:" << result.errorMessage();
        return;
    }
    auto dbusArg =  result.arguments().at(0).value<QDBusArgument>();
    QMap<QString, QVector<QStringList>> variantList;
    dbusArg >> variantList;
    if (variantList.size() == 0) {
        qDebug() << "[WlanConnect]initNetListFromDevice " << deviceName << " list empty";
        return;
    }
    QMap<QString, QVector<QStringList>>::iterator iter;

    for (iter = variantList.begin(); iter != variantList.end(); iter++) {
        if (deviceName == iter.key()) {
            QVector<QStringList> wlanListInfo = iter.value();
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
    QString cmd = "nm-connection-editor";
    QProcess process(this);
    process.startDetached(cmd);
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
int WlanConnect::setSignal(QString lv) {
    int signal = lv.toInt();
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
}

//获取应该插入哪个位置
int WlanConnect::sortWlanNet(QString deviceName, QString name, QString signal)
{
    qDebug() << "[WlanConnect]call getWirelessList"  << __LINE__;
    QDBusMessage result = m_interface->call(QStringLiteral("getWirelessList"));
    qDebug() << "[WlanConnect]call getWirelessList respond"  << __LINE__;
    if(result.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "getWirelessList error:" << result.errorMessage();
        return 0;
    }
    auto dbusArg =  result.arguments().at(0).value<QDBusArgument>();
    QMap<QString, QVector<QStringList>> variantList;
    dbusArg >> variantList;
    QMap<QString, QVector<QStringList>>::iterator iter;
    for (iter = variantList.begin(); iter != variantList.end(); iter++) {
        if (deviceName == iter.key()) {
            QVector<QStringList> wlanListInfo = iter.value();
            for (int i = 0; i < wlanListInfo.size(); i++) {
                if (name == wlanListInfo.at(i).at(0)) {
                    return i;
                }
            }
        }
    }
    return 0;
}

void WlanConnect::activeConnect(QString netName, QString deviceName, int type) {
    if (!m_interface->isValid()) {
        return;
    }
    qDebug() << "[WlanConnect]call activateConnect" << __LINE__;
    m_interface->call("activateConnect",type, deviceName, netName);
    qDebug() << "[WlanConnect]call activateConnect respond" << __LINE__;
}

void WlanConnect::deActiveConnect(QString netName, QString deviceName, int type) {
    if (!m_interface->isValid()) {
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
    addOneWlanFrame(frame, devName, infoList.at(0), infoList.at(1), infoList.at(3), isLock, true, WIRELESS_TYPE);
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
    addOneWlanFrame(frame, devName, infoList.at(0), infoList.at(1), "", isLock, false, WIRELESS_TYPE);
}

//增加设备
void WlanConnect::addDeviceFrame(QString devName)
{
    qDebug() << "[WlanConnect]addDeviceFrame " << devName;
    ItemFrame *itemFrame = new ItemFrame(devName, pluginWidget);
    ui->availableLayout->addWidget(itemFrame);
    itemFrame->deviceFrame->deviceLabel->setText(tr("card")+/*QString("%1").arg(count)+*/"："+devName);
    deviceFrameMap.insert(devName, itemFrame);
}

//减少设备
void WlanConnect::removeDeviceFrame(QString devName)
{
    qDebug() << "[WlanConnect]removeDeviceFrame " << devName;
    if (deviceFrameMap.contains(devName)) {
        ItemFrame *item = deviceFrameMap[devName];
        delete item;
        item = nullptr;
        deviceFrameMap.remove(devName);
    }
}

//增加ap
void WlanConnect::addOneWlanFrame(ItemFrame *frame, QString deviceName, QString name, QString signal, QString uuid, bool isLock, bool status, int type)
{
    if (nullptr == frame) {
        return;
    }

    if (frame->itemMap.contains(name)) {
            qDebug() << "[WlanConnect]Already exist a wifi " << name << " in " << deviceName;
            return;
    }
    //设置单项的信息
    int sign = setSignal(signal);
    QString iconamePath = wifiIcon(isLock, sign);
    WlanItem * wlanItem = new WlanItem(status, isLock, pluginWidget);
    QIcon searchIcon = QIcon::fromTheme(iconamePath);
    if (iconamePath != KLanSymbolic && iconamePath != NoNetSymbolic) {
        wlanItem->iconLabel->setProperty("useIconHighlightEffect", 0x10);
    }
    wlanItem->iconLabel->setPixmap(searchIcon.pixmap(searchIcon.actualSize(QSize(24, 24))));
    wlanItem->titileLabel->setText(name);
    if (status) {
        wlanItem->statusLabel->setText(tr("connected"));
        frame->uuid = uuid;
        wlanItem->uuid = uuid;
    } else {
        wlanItem->statusLabel->setText("");
    }

    connect(wlanItem->infoLabel, &InfoButton::clicked, this, [=]{
        // open detail page
        if (!m_interface->isValid()) {
            return;
        }
        qDebug() << "[WlanConnect]call showPropertyWidget" << __LINE__;
        m_interface->call(QStringLiteral("showPropertyWidget"), deviceName, name);
        qDebug() << "[WlanConnect]call showPropertyWidget respond" << __LINE__;
    });

    connect(wlanItem, &QPushButton::clicked, this, [=] {
        if (wlanItem->isAcitve) {
            deActiveConnect(name, deviceName, type);
        } else {
            activeConnect(name, deviceName, type);
        }
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

void WlanConnect::itemActiveConnectionStatusChanged(WlanItem *item, int status)
{
    if (status == ACTIVATING) {
        item->setCountCurrentTime(0);
        item->setWaitPage(1);
        item->startLoading();
    } else if (status == ACTIVATED) {
        item->stopLoading();
        item->statusLabel->setStyleSheet("");
        item->statusLabel->setMinimumSize(36,36);
        item->statusLabel->setMaximumSize(16777215,16777215);
        item->statusLabel->setText(tr("connected"));
        item->isAcitve = true;
    } else if (status == DEACTIVATING) {
        item->setCountCurrentTime(0);
        item->setWaitPage(1);
        item->startLoading();
    } else if (status == DEACTIVATED) {
        item->stopLoading();
        item->statusLabel->setStyleSheet("");
        item->statusLabel->setMinimumSize(36,36);
        item->statusLabel->setMaximumSize(16777215,16777215);
        item->statusLabel->setText("");
        item->isAcitve = false;
    }
}

