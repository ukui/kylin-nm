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
#include "netconnect.h"

#include <QGSettings>
#include <QProcess>
#include <QTimer>
#include <QtDBus>
#include <QDir>
#include <QDebug>
#include <QtAlgorithms>
#include <kwindowsystem.h>
#include <kwindowsystem_export.h>

const QString KLanSymbolic      = "network-wired-connected-symbolic";
const QString NoNetSymbolic     = "network-wired-disconnected-symbolic";


#define NO_MARGINS 0,0,0,0
#define NO_SPACING 0
#define TITLE_FRAME_HEIGHT 50     //TabWidget的tab和widget有间隙，和设计稿看起来一致就不能设为设计稿里的高度
#define TITLE_LAYOUT_MARGINS 24,0,24,0
#define WIDGET_WIDTH 420
#define WIDGET_HEIGHT 436
#define LOG_FLAG "[NetConnect]"

const QString    KEY_WIRED_SWITCH             = "wiredswitch";

const QString CONFIG_FILE_PATH   =  QDir::homePath() + "/.config/ukui/kylin-nm.conf";


bool checkMapValveContainsTrue(QMap<QString, bool> map) {
    if (map.isEmpty()) {
        return false;
    }
    QMap<QString, bool>::iterator iter;
    for (iter = map.begin(); iter != map.end(); iter++) {
        if (iter.value()) {
            return true;
        }
    }
    return false;
}

NetConnect::NetConnect() :  mFirstLoad(true) {
    QLocale local;
    QString locale = local.name();
    QTranslator* translator = new QTranslator(this);
    if (translator->load(":/translations/"+ locale + ".qm")) {
        QApplication::installTranslator(translator);
    } else {
        qWarning() << "Translations load fail";
    }
}

NetConnect::~NetConnect() {
    if (m_switchGsettings != nullptr) {
        delete m_switchGsettings;
        m_switchGsettings = nullptr;
    }

    if (m_styleGsettings != nullptr) {
        delete m_styleGsettings;
        m_styleGsettings = nullptr;
    }

    if (m_interfaceUi != nullptr) {
        delete m_interfaceUi;
        m_interfaceUi = nullptr;
    }

    thread->quit();
    if (nullptr !=pluginWidget) {
        delete pluginWidget;
        pluginWidget = nullptr;
    }
}

QWidget *NetConnect::pluginUi() {
    if (mFirstLoad) {
        mFirstLoad = false;

        pluginWidget = new QWidget;
        pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
        pluginWidget->setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);
        initUi();
        initComponent();
        initConnect();
        if (QApplication::applicationName() == "kylin-nm") {
            initDbus();
        }
    }
    return pluginWidget;
}

void NetConnect::setPluginType(PluginType type, bool useSwitch)
{
    if (type == SIMPLE) {
        m_isSimpleMode = true;
    } else {
        m_isSimpleMode = false;
    }
    m_useSwitch = useSwitch;
}

void NetConnect::initUi()
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
    m_titleLabel->setText(tr("LAN"));
    m_wiredSwitch = new KSwitchButton(pluginWidget);
    m_wiredSwitch->installEventFilter(this);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_wiredSwitch);
    m_titleDivider = new Divider(pluginWidget);

    m_scrollFrame = new QFrame(pluginWidget);
    if (!m_isSimpleMode) {
        m_scrollFrame->setMinimumHeight(330);
    } else {
        m_scrollFrame->setMinimumHeight(383);
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
    m_settingsLabel = new KyLable(m_settingsFrame);
    m_settingsLabel->setCursor(Qt::PointingHandCursor);
    m_settingsLabel->setText(tr("Settings"));
    m_settingsLabel->setScaledContents(true);
    m_settingsLabel->installEventFilter(this);


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
        //适配半透明控件
        m_wiredSwitch->setTranslucent(true);
        m_scrollArea->setProperty("needTranslucent", true);
    }
    QPalette pal = m_scrollArea->palette();
    pal.setBrush(QPalette::Base, QColor(0,0,0,0));     //背景透明
    m_scrollArea->setPalette(pal);
}

bool NetConnect::eventFilter(QObject *w, QEvent *e) {
    if (w == m_wiredSwitch && e->type() == QEvent::MouseButtonRelease) {
        if (!m_wiredSwitch->isCheckable()) {
//            showDesktopNotify(tr("No ethernet device avaliable"));
            qDebug() << "No ethernet device avaliable";
        } else {
            Q_EMIT setWiredEnabled(!m_wiredSwitch->isChecked());
        }
        return true;
    }  else if (w == m_settingsLabel) {
        if (e->type() == QEvent::MouseButtonRelease) {
            //ZJP_TODO 打开控制面板
            runExternalApp();
        }
    }
    return QObject::eventFilter(w,e);
}

void NetConnect::initDbus()
{
    if (m_initDbusCount < 5) {
        m_interfaceUi = new QDBusInterface("com.kylin.network", "/com/kylin/network/interface",
                                           "com.kylin.network.interface",
                                           QDBusConnection::sessionBus());
        if (m_interfaceUi->isValid()) {
            qDebug() << QApplication::applicationName() << "[LAN PLUGIN WIDGET]interface dbus valid";
        } else {
            m_initDbusCount++;
            QTimer::singleShot(1000, [this] {
                initDbus();
            });
            qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
            return;
        }
        connect(m_interfaceUi, SIGNAL(signalShowPropertyWidget(QString, QString)), this, SLOT(showDetailPage(QString, QString)));
        connect(m_interfaceUi, SIGNAL(signalShowCreateWiredConnectWidget(QString)), this, SLOT(showAddNetworkPage(QString)));
    } else {
        qWarning()<< "com.kylin.network.interface 不可用";
    }
}

void NetConnect::initComponent() {

    getDeviceStatusMap(deviceStatusMap);
    initNet();
    setSwitchStatus();

//    connect(m_settingsLabel, &KBorderlessButton::clicked, this, [=](bool checked) {
//        Q_UNUSED(checked)
//        runExternalApp();
//    });

    m_lanDetailPagePtrMap.clear();
    m_createPagePtrMap.clear();
}

void NetConnect::initConnect()
{
    connect(manager, &KyNetworkManager::wiredStateChange, this, &NetConnect::onActiveConnectionChanged);

    //connect(manager, &KyNetworkManager::deviceStateChange, this, &NetConnect::onDeviceStatusChanged);
    connect(manager, &KyNetworkManager::carrierChange, this, &NetConnect::onDeviceCarrierChange);

    connect(manager, &KyNetworkManager::wiredConnectionAdd, this, &NetConnect::onLanAdd);
    connect(manager, &KyNetworkManager::wiredConnectionUpdate, this, &NetConnect::updateLanInfo);
    connect(manager, &KyNetworkManager::connectionRemove, this, &NetConnect::onLanRemove);

    connect(manager, &KyNetworkManager::wiredDeviceAdd, this, &NetConnect::onDeviceAdd);
    connect(manager, &KyNetworkManager::deviceRemove, this, &NetConnect::onDeviceRemove);
    connect(manager, &KyNetworkManager::wiredDeviceUpdate, this, &NetConnect::onDeviceNameChanged);

    connect(manager, &KyNetworkManager::deviceManagedChange, this, &NetConnect::onDeviceManagedChange);
    connect(manager, &KyNetworkManager::wiredEnabledChanged, this, &NetConnect::onWiredEnabledChanged);


    connect(this, &NetConnect::setWiredDeviceEnable, manager, &KyNetworkManager::onSetWiredDeviceEnable);
    connect(this, &NetConnect::setWiredEnabled, manager, &KyNetworkManager::onSetWiredEnabled);
    connect(this, &NetConnect::activateConnection, manager, &KyNetworkManager::onActivateConnection);
    connect(this, &NetConnect::deactivateConnection, manager, &KyNetworkManager::onDeactivateConnection);
    connect(this, &NetConnect::deleteConnect, manager, &KyNetworkManager::onDeleteConnect);
    connect(this, &NetConnect::updateIpv4AndIpv6SettingInfo, manager, &KyNetworkManager::onUpdateIpv4AndIpv6SettingInfo);
    connect(this, &NetConnect::createWiredConnect, manager, &KyNetworkManager::onCreateWiredConnect);

    connect(manager, &KyNetworkManager::wiredStateChange, this, [=](QString deviceName, QString uuid, KyConnectState status) {
        KyConnectSetting connectSetting;
        manager->getConnectIpInfo(uuid, connectSetting);
        updateNetworkModeState(deviceName, connectSetting.m_connectName, uuid, status);
    });
    connect(manager, &KyNetworkManager::wiredStateChange, this, [=](QString deviceName, QString uuid, KyConnectState status) {
        Q_EMIT connectStateChanged(uuid, status);
    });


}
//获取网卡列表
void NetConnect::getDeviceStatusMap(QMap<QString, bool> &map)
{
    map.clear();
    QStringList wiredDevList;
    wiredDevList.clear();
    manager->getNetworkDeviceList(DEVICE_TYPE_ETHERNET, wiredDevList);

    qDebug() << wiredDevList;
    if (!wiredDevList.isEmpty()) {
        for (int i = 0; i < wiredDevList.size(); ++i) {

            QString devName = wiredDevList.at(i);qDebug() << devName;
            map.insert(devName, manager->getDeviceManaged(devName));
        }
    }
}

//lanUpdate
void NetConnect::updateLanInfo(QString deviceName, QString connectUuid, QString connectName, QString connectPath)
{
    //设备归属变化 || 有线名称变化
    QMap<QString, ItemFrame *>::iterator iter;
    for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
        if (deviceName.isEmpty()) {
            //变为无指定网卡，所有列表都要添加
            if (!iter.value()->itemMap.contains(connectUuid)) {
                QStringList lanInfo;
                lanInfo << connectName << connectUuid << connectPath;
                addOneLanFrame(iter.value(), iter.key(), lanInfo);
            } else {
                if (iter.value()->itemMap[connectUuid]->getName() != connectName) {
                    qDebug() << "[NetConnect]" << iter.key()
                             << iter.value()->itemMap[connectUuid]->getName() << "change to" << connectName;
                    iter.value()->itemMap[connectUuid]->setName(connectName);
                    deviceFrameMap[iter.key()]->lanItemLayout->removeWidget(iter.value()->itemMap[connectUuid]);
                    int index = getInsertPos(connectName, deviceFrameMap[iter.key()]->lanItemLayout);
                    deviceFrameMap[iter.key()]->lanItemLayout->insertWidget(index,iter.value()->itemMap[connectUuid]);
                }
            }
        } else {
            if (iter.key() != deviceName) {
                qDebug() << "[NetConnect]" << connectUuid << " not belongs to " << iter.key();
                removeOneLanFrame(iter.value(), deviceName, connectUuid);
            } else {
                if (!iter.value()->itemMap.contains(connectUuid)) {
                    qDebug() << "[NetConnect]" << connectUuid << " now belongs to " << deviceName;
                    QStringList lanInfo;
                    lanInfo << connectName << connectUuid << connectPath;
                    addOneLanFrame(iter.value(), deviceName, lanInfo);
                } else {
                    qDebug() << "[NetConnect]" << deviceName
                             << iter.value()->itemMap[connectUuid]->getName() << "change to" << connectName;
                    if (iter.value()->itemMap[connectUuid]->getName() != connectName) {
                        iter.value()->itemMap[connectUuid]->setName(connectName);
                        deviceFrameMap[iter.key()]->lanItemLayout->removeWidget(iter.value()->itemMap[connectUuid]);
                        int index = getInsertPos(connectName, deviceFrameMap[iter.key()]->lanItemLayout);
                        deviceFrameMap[iter.key()]->lanItemLayout->insertWidget(index,iter.value()->itemMap[connectUuid]);
                    }
                }
            }
        }
    }
}


//总开关
void NetConnect::setSwitchStatus()
{
    getDeviceStatusMap(deviceStatusMap);
    bool status = checkMapValveContainsTrue(deviceStatusMap);

    if (!status) {
        if (manager->getWiredEnabled()) {
            m_wiredSwitch->setCheckable(true);
            m_wiredSwitch->setChecked(true);
        } else {
            m_wiredSwitch->setChecked(false);
            m_wiredSwitch->setCheckable(false);
        }
        hideLayout(m_scrollAreaLayout);
    } else {
        m_wiredSwitch->setCheckable(true);
        if (manager->getWiredEnabled()) {
            m_wiredSwitch->setChecked(true);
            showLayout(m_scrollAreaLayout);
        } else {
            m_wiredSwitch->setChecked(false);
            hideLayout(m_scrollAreaLayout);
        }
    }
}

//总体隐藏
void NetConnect::hideLayout(QVBoxLayout * layout) {
    for (int i = layout->layout()->count()-1; i >= 0; --i)
    {
        QLayoutItem *it = layout->layout()->itemAt(i);
        ItemFrame *itemFrame = qobject_cast<ItemFrame *>(it->widget());
        itemFrame->hide();
    }
}

//总体显示
void NetConnect::showLayout(QVBoxLayout * layout) {
    for (int i = layout->layout()->count()-1; i >= 0; --i)
    {
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

//初始化
void NetConnect::initNet()
{
    //先构建每个设备的列表头
    QStringList deviceList = deviceStatusMap.keys();
    for (int i = 0; i < deviceList.size(); ++i) {
        addDeviceFrame(deviceList.at(i), deviceStatusMap[deviceList.at(i)]);
    }
    //再填充每个设备的列表
    for (int i = 0; i < deviceList.size(); ++i) {
        initNetListFromDevice(deviceList.at(i));
    }
}

void NetConnect::runExternalApp()
{
    QProcess process;
    process.startDetached("ukui-control-center -m netconnect");
}

//激活
void NetConnect::activeConnect(QString uuid, QString deviceName)
{
    Q_EMIT activateConnection(uuid, deviceName);
}

//断开
void NetConnect::deActiveConnect(QString uuid)
{
    Q_EMIT deactivateConnection(uuid);
}

//初始化设备列表
void NetConnect::initNetListFromDevice(QString deviceName)
{
    qDebug() << "[NetConnect]initNetListFromDevice " << deviceName;
    if (!deviceFrameMap.contains(deviceName)) {
        qDebug() << "[NetConnect]initNetListFromDevice " << deviceName << " not exist";
        return;
    }
    QMap<QString,QList<KyWiredItem>> wiredMap;
    manager->getWiredList(wiredMap);

    if (wiredMap.size() == 0) {
        qDebug() << "[NetConnect]initNetListFromDevice " << deviceName << " list empty";
        return;
    }

    QMap<QString,QList<KyWiredItem>>::iterator iter;
    for (iter = wiredMap.begin(); iter != wiredMap.end(); iter++) {
        if (deviceName == iter.key()) {
            QList<KyWiredItem> lanListInfo = iter.value();
            //处理列表
            for (int i = 0; i < lanListInfo.length(); i++) {
                addLanItem(deviceFrameMap[deviceName], deviceName, lanListInfo.at(i));
            }
        }
    }
    QList<KyActivateItem> activateList;
    manager->getActiveConnectionList(deviceName, CONNECT_TYPE_WIRED, activateList);
    if (activateList.size() != 0) {
        onActiveConnectionChanged(deviceName, activateList.at(0).m_uuid, activateList.at(0).m_connStatus);

        for (KyActivateItem item : activateList) {
            initActiveNetworkMode(deviceName, item);
        }
    }
}

//初始化时添加一个项
void NetConnect::addLanItem(ItemFrame *frame, QString devName, KyWiredItem item)
{
    if (frame == nullptr) {
        return;
    }

    LanItem * lanItem = new LanItem(m_isSimpleMode, pluginWidget);

    QIcon searchIcon = QIcon::fromTheme(NoNetSymbolic);
    lanItem->setItemIcon(searchIcon);
    lanItem->setName(item.m_connectName);

    lanItem->setUuid(item.m_connectUuid);
    lanItem->setPath(item.m_connectPath);

    //todo show detail page
    connect(lanItem, &LanItem::infoButtonClick, this, [=]{
        showLanDetailPage(devName, lanItem);
    });

    lanItem->setStatus(false);

    connect(lanItem, &LanItem::itemClick, this, [=] {
        if (lanItem->getStatus() || lanItem->getIsLoading()) {
            deActiveConnect(lanItem->getUuid());
        } else {
            activeConnect(lanItem->getUuid(), devName);
        }
    });

    //记录到deviceFrame的itemMap中
    deviceFrameMap[devName]->itemMap.insert(item.m_connectUuid, lanItem);
    frame->lanItemLayout->addWidget(lanItem);
}

//增加设备
void NetConnect::addDeviceFrame(QString devName, bool isEnable)
{
    ItemFrame *itemFrame = new ItemFrame(devName, pluginWidget);
    m_scrollAreaLayout->addWidget(itemFrame);
    itemFrame->deviceFrame->deviceLabel->setText(devName);
    if (isEnable) {
        itemFrame->show();
    } else {
        itemFrame->hide();
    }
    deviceFrameMap.insert(devName, itemFrame);
    deviceStatusMap.insert(devName, true);
}

//减少设备
void NetConnect::removeDeviceFrame(QString devName)
{
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

//device add or remove=================================
#if 0
void NetConnect::onDeviceStatusChanged()
{
    qDebug()<<"[NetConnect]onDeviceStatusChanged";
    QEventLoop eventloop;
    QTimer::singleShot(300, &eventloop, SLOT(quit()));
    eventloop.exec();
    QStringList list;
    QMap<QString, bool> map;
    getDeviceStatusMap(map);
    list = map.keys();

    QStringList removeList;
    QMap<QString, bool> addMap;

    //remove的设备
    for (int i = 0; i< deviceStatusMap.keys().size(); ++i) {
        if (!list.contains(deviceStatusMap.keys().at(i))) {
            qDebug() << "[NetConnect]onDeviceStatusChanged " << deviceStatusMap.keys().at(i) << "was removed";
            removeList << deviceStatusMap.keys().at(i);
        }
    }

    //add的设备
    for (int i = 0; i< list.size(); ++i) {
        if (!deviceStatusMap.keys().contains(list.at(i))) {
            qDebug() << "[NetConnect]onDeviceStatusChanged " << list.at(i) << "was add, init status" << map[list.at(i)];
            addMap.insert(list.at(i),map[list.at(i)]);
        }
    }

    for (int i = 0; i < removeList.size(); ++i) {
        removeDeviceFrame(removeList.at(i));
    }

    QStringList addList = addMap.keys();
    for (int i = 0; i < addList.size(); ++i) {
        qDebug() << "add a device " << addList.at(i) << "status" << map[addList.at(i)];
        addDeviceFrame(addList.at(i));
        initNetListFromDevice(addList.at(i));
    }
    deviceStatusMap = map;
    if (deviceStatusMap.isEmpty()) {
        wiredSwitch->setDisabledFlag(true);
        wiredSwitch->setChecked(false);
    } else {
        wiredSwitch->setDisabledFlag(false);
        setSwitchStatus();
    }

}
#endif
void NetConnect::onDeviceNameChanged(QString oldName, QString newName)
{
    if (!deviceFrameMap.contains(oldName) || !deviceStatusMap.contains(oldName)) {
        qDebug() << "[NetConnect]onDeviceNameChanged no such device " << oldName;
        return;
    }

    if (deviceFrameMap.contains(newName) && deviceStatusMap.contains(newName)) {
        qDebug() << "[NetConnect]onDeviceNameChanged already has device " << newName;
        return;
    }

    qDebug() << "[NetConnect]onDeviceNameChanged " << oldName << "change to" << newName;

    //shan chu chong jian
    removeDeviceFrame(oldName);
    removeDeviceFrame(newName);

    getDeviceStatusMap(deviceStatusMap);

    addDeviceFrame(newName, manager->getDeviceManaged(newName));
    initNetListFromDevice(newName);
}

//lan add===============================================================
void NetConnect::onLanAdd(QString devName, QString connectUuid, QString connectName, QString connectPath)
{
    qDebug() << "onLanAdd";

    if (!devName.isEmpty() && !deviceStatusMap.contains(devName)) {
        return;
    }

    QStringList lanInfo;
    lanInfo << connectName << connectUuid << connectPath;

    QMap<QString, ItemFrame *>::iterator iter;
    for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
        if (devName.isEmpty()) {
            addOneLanFrame(iter.value(), iter.key(), lanInfo);
        } else if (devName == iter.key()) {
            addOneLanFrame(iter.value(), devName, lanInfo);
            break;
        }
    }
}

//wifi remove =============================================================
void NetConnect::onLanRemove(QString lanPath)
{
    qDebug() << "onLanRemove";
    QMap<QString, ItemFrame *>::iterator iter;
    for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
        QMap<QString, LanItem *>::iterator itemIter;
        for (itemIter = iter.value()->itemMap.begin(); itemIter != iter.value()->itemMap.end(); itemIter++) {
            if (itemIter.value()->getPath() == lanPath) {
               qDebug()<<"[NetConnect]lan remove " << lanPath << " find in " << itemIter.value()->getName();
               QString key = itemIter.key();
               iter.value()->lanItemLayout->removeWidget(itemIter.value());
               delete itemIter.value();
               iter.value()->itemMap.remove(key);
               break;
            }
        }
    }
    //删除对应的详情页
    if (m_lanDetailPagePtrMap.contains(lanPath)) {
        if (m_lanDetailPagePtrMap[lanPath] != nullptr) {
            delete m_lanDetailPagePtrMap[lanPath];
            m_lanDetailPagePtrMap[lanPath] = nullptr;
        }
    }
}

//增加一项
void NetConnect::addOneLanFrame(ItemFrame *frame, QString deviceName, QStringList infoList)
{
    if (nullptr == frame) {
        return;
    }

    if (frame->itemMap.contains(infoList.at(1))) {
            qDebug() << "[NetConnect]Already exist a lan " << infoList.at(1) << " in " << deviceName;
            return;
    }

    qDebug() << "[NetConnect]addOneLanFrame" << deviceName << infoList.at(0);
    QString connName = infoList.at(0);
    QString connUuid = infoList.at(1);
    QString connDbusPath = infoList.at(2);
    LanItem * lanItem = new LanItem(m_isSimpleMode, pluginWidget);

    QIcon searchIcon = QIcon::fromTheme(NoNetSymbolic);
    lanItem->setItemIcon(searchIcon);
    lanItem->setName(connName);

    lanItem->setUuid(connUuid);
    lanItem->setPath(connDbusPath);

    // todo open landetail page
    if (!m_isSimpleMode) {
        connect(lanItem, &LanItem::infoButtonClick, this, [=]{
            showLanDetailPage(deviceName, lanItem);
        });
    }

    lanItem->setStatus(false);

    connect(lanItem, &LanItem::itemClick, this, [=] {
        if (lanItem->getStatus() || lanItem->getIsLoading()) {
            deActiveConnect(lanItem->getUuid());
        } else {
            activeConnect(lanItem->getUuid(), deviceName);
        }
    });

    //记录到deviceFrame的itemMap中
    deviceFrameMap[deviceName]->itemMap.insert(connUuid, lanItem);
    int index = getInsertPos(connName, deviceFrameMap[deviceName]->lanItemLayout);
    qDebug()<<"[NetConnect]addOneLanFrame " << connName << " to " << deviceName << " list at pos:" << index;
    frame->lanItemLayout->insertWidget(index, lanItem);
}

void NetConnect::removeOneLanFrame(ItemFrame *frame, QString deviceName, QString uuid)
{
    if (nullptr == frame) {
        return;
    }

    if (!frame->itemMap.contains(uuid)) {
            qDebug() << "[NetConnect]not exist a lan " << uuid << " in " << deviceName;
            return;
    }

   qDebug()<<"[NetConnect]removeOneLanFrame " << uuid << " find in " << deviceName;

   frame->lanItemLayout->removeWidget(frame->itemMap[uuid]);
   delete frame->itemMap[uuid];
   frame->itemMap.remove(uuid);
}

//activeconnect status change
void NetConnect::onActiveConnectionChanged(QString deviceName, QString uuid, KyConnectState status)
{
    if (uuid.isEmpty()) {
        qDebug() << "[NetConnect]onActiveConnectionChanged but uuid is empty";
        return;
    }
    qDebug() << "[NetConnect]onActiveConnectionChanged " << deviceName << uuid << status;
    LanItem * item= nullptr;
    if (deviceName.isEmpty()) {
        if (status != CONNECT_STATE_DEACTIVATED) {
            return;
        }
        //断开时 设备为空 说明此有线未指定设备 添加到所有列表中
        QStringList infoList;
        QMap<QString, ItemFrame *>::iterator iters;
        for (iters = deviceFrameMap.begin(); iters != deviceFrameMap.end(); iters++) {
            if (iters.value()->itemMap.contains(uuid)) {
                item = iters.value()->itemMap[uuid];
                //为断开则重新插入
                deviceFrameMap[iters.key()]->lanItemLayout->removeWidget(item);
                int index = getInsertPos(item->getName(), deviceFrameMap[iters.key()]->lanItemLayout);
                qDebug() << "[NetConnect]reinsert" << item->getName() << "pos" << index << "in" << iters.key() << "because status changes to deactive";
                deviceFrameMap[iters.key()]->lanItemLayout->insertWidget(index,item);
                itemActiveConnectionStatusChanged(item, status);
            }
        }
        //添加到所有列表中
        if (!infoList.isEmpty()) {
            QMap<QString, ItemFrame *>::iterator iter;
            for (iter = deviceFrameMap.begin(); iter != deviceFrameMap.end(); iter++) {
                if (!iter.value()->itemMap.contains(uuid)) {
                    addOneLanFrame(iter.value(), iter.key(), infoList);
                }
            }
        }
    } else {
        if (deviceFrameMap.contains(deviceName)) {
            if (deviceFrameMap[deviceName]->itemMap.contains(uuid)) {
                item = deviceFrameMap[deviceName]->itemMap[uuid];
                if (status == CONNECT_STATE_ACTIVATED) {
                    //为已连接则放到第一个
                    deviceFrameMap[deviceName]->lanItemLayout->removeWidget(item);
                    deviceFrameMap[deviceName]->lanItemLayout->insertWidget(0,item);
                } else if (status == CONNECT_STATE_DEACTIVATED) {
                    //为断开则重新插入
                    deviceFrameMap[deviceName]->lanItemLayout->removeWidget(item);
                    int index = getInsertPos(item->getName(), deviceFrameMap[deviceName]->lanItemLayout);
                    qDebug() << "[NetConnect]reinsert" << item->getName() << "pos" << index << "in" << deviceName << "because status changes to deactive";
                    deviceFrameMap[deviceName]->lanItemLayout->insertWidget(index,item);
                }
                itemActiveConnectionStatusChanged(item, status);
            }
        } else {
            if (status == CONNECT_STATE_ACTIVATED || status == CONNECT_STATE_DEACTIVATED) {
                //虚拟网卡处理
                QMap<QString, ItemFrame *>::iterator iters;
                for (iters = deviceFrameMap.begin(); iters != deviceFrameMap.end(); iters++) {
                    if (iters.value()->itemMap.contains(uuid)) {
                        removeOneLanFrame(iters.value(), iters.key(), uuid);
                     }
                }
            }
        }
    }
}

void NetConnect::itemActiveConnectionStatusChanged(LanItem *item, KyConnectState status)
{
    if (status == CONNECT_STATE_ACTIVATING) {
        item->startLoading();
    } else if (status == CONNECT_STATE_ACTIVATED) {
        item->stopLoading();
        QIcon searchIcon = QIcon::fromTheme(KLanSymbolic);
        item->setItemIcon(searchIcon);
        item->setStatus(true);
    } else if (status == CONNECT_STATE_DEACTIVATING) {
        item->startLoading();
    } else {
        item->stopLoading();
        QIcon searchIcon = QIcon::fromTheme(NoNetSymbolic);
        item->setItemIcon(searchIcon);
        item->setStatus(false);
    }
}

void NetConnect::showLanDetailPage(QString deviceName, LanItem *item)
{
    if (item == nullptr) {
        return;
    }
    QString connName = item->getName();
    QString connUuid = item->getUuid();
    QString connDbusPath = item->getPath();
    bool isActivated = item->getStatus();

    if (connUuid.isEmpty()) {
        qDebug() << LOG_FLAG << "connect is empty, so can not show detail info.";
        return;
    }

    qDebug()<< LOG_FLAG << "the info button of lan is clicked! uuid = "
            << connUuid << "; name = " << connName
            << "." <<Q_FUNC_INFO << __LINE__;

    if (m_lanDetailPagePtrMap.contains(connDbusPath)) {
        if (m_lanDetailPagePtrMap[connDbusPath] != nullptr) {
            KWindowSystem::raiseWindow(m_lanDetailPagePtrMap[connDbusPath]->winId());
            return;
        }
    }
    NetDetail *netDetail = new NetDetail(deviceName, connName, connUuid, isActivated, false, false, pluginWidget);
    m_lanDetailPagePtrMap.insert(connDbusPath, netDetail);
    netDetail->show();

    connect(netDetail, &NetDetail::detailPageClose, [&](QString devName, QString lanName, QString lanUuid){
        if (lanUuid.isEmpty()) {
            return;
        }
        QString itemPath;
        if (deviceFrameMap.contains(devName) && deviceFrameMap[devName]->itemMap.contains(lanUuid)) {
            itemPath = deviceFrameMap[devName]->itemMap[lanUuid]->getPath();
        }
        if (m_lanDetailPagePtrMap.contains(itemPath)) {
            m_lanDetailPagePtrMap[itemPath] = nullptr;
        }
    });
}

int NetConnect::getInsertPos(QString connName, QVBoxLayout* layout)
{
//    qDebug() << "[NetConnect]getInsertPos" << connName << deviceName;
//    int index = 0;
//    QMap<QString,QList<KyWiredItem>> wiredMap;
//    manager->getWiredList(wiredMap);

//    if (!wiredMap.contains(deviceName)) {
//        return 0;
//    }

//    QList<KyActivateItem> activateList;
//    QString name;
//    manager->getActiveConnectionList(deviceName, CONNECT_TYPE_WIRED, activateList);
//    if (activateList.size() != 0) {
//        name = activateList.at(0).m_connName;
//    }

//    bool isFind = false;
//    for (;index < wiredMap[deviceName].size(); ++index) {
//        if (wiredMap[deviceName].at(index).m_connectName == connName) {
//            break;
//        } else if (wiredMap[deviceName].at(index).m_connectName == name) {
//            isFind = true;
//        }
//    }
//    if (!isFind) {
//        index++;
//    }
//    return index;
    int index = 0;
    for (; index < layout->count(); ++index)
    {
        QLayoutItem *it = layout->layout()->itemAt(index);
        LanItem *item = qobject_cast<LanItem *>(it->widget());
        if (QString::compare(connName, item->getName(), Qt::CaseInsensitive) > 0) {
            qDebug() << "compare" << connName << item->getName() << index;
            continue;
        } else {
            if (item->getStatus()) {
                continue;
            }
            break;
        }
    }
    return index;
}

void NetConnect::onDeviceCarrierChange(QString deviceName, bool pluged)
{

}

void NetConnect::onDeviceManagedChange(QString deviceName, bool managed)
{
    getDeviceStatusMap(deviceStatusMap);
    if (deviceFrameMap.contains(deviceName)) {
        if (managed) {
            deviceFrameMap[deviceName]->show();
        } else {
            deviceFrameMap[deviceName]->hide();
        }
    }
}

void NetConnect::onWiredEnabledChanged(bool enabled)
{
    qDebug() << "[NetConnect] wiredEnabledChanged" <<enabled;
    if (m_wiredSwitch->isChecked() == enabled) {
        return;
    }
    setSwitchStatus();
}

void NetConnect::onDeviceAdd(QString deviceName)
{
    addDeviceFrame(deviceName, manager->getDeviceManaged(deviceName));
    initNetListFromDevice(deviceName);
    setSwitchStatus();
}

void NetConnect::onDeviceRemove(QString deviceName)
{
    if (!deviceFrameMap.contains(deviceName) || !deviceStatusMap.contains(deviceName)) {
        return;
    }
    removeDeviceFrame(deviceName);
    setSwitchStatus();

    //删除对应的新建网络弹窗
    if (m_createPagePtrMap.contains(deviceName)) {
        if (m_createPagePtrMap[deviceName] != nullptr) {
            delete m_createPagePtrMap[deviceName];
            m_createPagePtrMap[deviceName] = nullptr;
        }
    }
}

void NetConnect::initActiveNetworkMode(QString deviceName, KyActivateItem activeItem)
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


void NetConnect::updateNetworkModeState(QString deviceName, QString ssid, QString uuid, KyConnectState status)
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

          connect(this, &NetConnect::connectStateChanged, fireWallDialog, &FirewallDialog::closeMyself);

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

void NetConnect::showDetailPage(QString deviceName, QString ssid)
{
    if (deviceName.isEmpty() || ssid.isEmpty()) {
        qWarning() << "deviceName or ssid is empty, could not show detail page";
        return;
    }

    LanItem *item = nullptr;
    if (deviceFrameMap.contains(deviceName) &&
            deviceFrameMap[deviceName]->itemMap.contains(ssid)) {
        item = deviceFrameMap[deviceName]->itemMap[ssid];
        showLanDetailPage(deviceName, item);
    }
}

void NetConnect::showAddNetworkPage(const QString deviceName)
{
    if (!deviceFrameMap.contains(deviceName)) {
        qWarning() << "Not find lan device " << deviceName << ", could not show add network page";
        return;
    }
    if (m_createPagePtrMap.contains(deviceName)) {
        if (m_createPagePtrMap[deviceName] != nullptr) {
            qDebug() << "showCreateWiredConnectWidget" << deviceName << "already create,just raise";
            KWindowSystem::raiseWindow(m_createPagePtrMap[deviceName]->winId());
            return;
        }
    }
    NetDetail *netDetail = new NetDetail(deviceName, "", "", false, false, true, pluginWidget);
    connect(netDetail, &NetDetail::createPageClose, [&](QString interfaceName){
        if (m_createPagePtrMap.contains(interfaceName)) {
            m_createPagePtrMap[interfaceName] = nullptr;
        }
    });
    m_createPagePtrMap.insert(deviceName, netDetail);
    netDetail->show();
}
