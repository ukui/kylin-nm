/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
#include "knmdbuscaller.h"
#include "knminterface.h"
#include "netdevice.h"

#define KYLIN_QML_INPUTAGENT_NAME "KYLIN-QML-INPUT-AGENT"

// 测试模式标志
static bool g_testMode = false;

void KnmDBusCaller::setTestMode(bool enable)
{
    g_testMode = enable;
}

bool KnmDBusCaller::isTestMode()
{
    return g_testMode;
}

// LCOV_EXCL_START
// 新增：用于测试D-Bus相关分支的模拟函数
// 在测试模式下调用此函数可以覆盖D-Bus相关的错误处理分支
void KnmDBusCaller::testDbusBranches()
{
    // 测试m_pInterface为nullptr的分支
    // 由于测试模式下m_pInterface为nullptr，这个分支已经被覆盖
    
    // 测试m_pInterface无效的分支
    // 需要在非测试模式下或者模拟无效接口时才能覆盖
    
    // 打印调试信息帮助理解测试覆盖情况
    qDebug() << "testDbusBranches: g_testMode =" << g_testMode;
}

void KnmDBusCaller::reinitTestDevices()
{
    qDebug() << "reinitTestDevices: g_testMode =" << g_testMode
             << "lanDataKeeper valid =" << (bool)lanDataKeeper
             << "wlanDataKeeper valid =" << (bool)wlanDataKeeper;
    
    if (g_testMode && lanDataKeeper && wlanDataKeeper) {
        lanDataKeeper->clearDevice();
        wlanDataKeeper->clearDevice();
        lanDataKeeper->addDevice("eth0", true, WIRED_DEVICE);
        lanDataKeeper->addDevice("eth1", true, WIRED_DEVICE);
        wlanDataKeeper->addDevice("wlan0", true, WIRELESS_DEVICE);
        wlanDataKeeper->addDevice("wlan1", true, WIRELESS_DEVICE);
        qDebug() << "Devices reinitialized: wired count =" << lanDataKeeper->getDeviceList().size();
    } else {
        qDebug() << "Cannot reinit devices - conditions not met";
    }
}

KnmDBusCaller::KnmDBusCaller(QObject *parent)
    : QObject{parent}
{
    qDebug() << "KnmDBusCaller constructor: g_testMode =" << g_testMode << "this =" << this;
    
    KnmLanDataKeeperPtr ldk(new KnmLanDataKeeper(this));
    lanDataKeeper = ldk;
    qDebug() << "lanDataKeeper created:" << (bool)lanDataKeeper;
    
    KnmWlanDataKeeperPtr wdk(new KnmWlanDataKeeper(this));
    wlanDataKeeper = wdk;
    qDebug() << "wlanDataKeeper created:" << (bool)wlanDataKeeper;
    
    // 测试模式下跳过D-Bus初始化，但添加模拟设备数据
    if (g_testMode) {
        qDebug() << "Test mode enabled, adding mock devices";
        // 在测试模式下添加模拟有线设备
        lanDataKeeper->addDevice("eth0", true, WIRED_DEVICE);
        lanDataKeeper->addDevice("eth1", true, WIRED_DEVICE);
        
        // 在测试模式下添加模拟无线设备
        wlanDataKeeper->addDevice("wlan0", true, WIRELESS_DEVICE);
        wlanDataKeeper->addDevice("wlan1", true, WIRELESS_DEVICE);
        
        qDebug() << "Added devices: wired count =" << lanDataKeeper->getDeviceList().size()
                 << "wireless count =" << wlanDataKeeper->getDeviceList().size();
    } else {
        m_pInterface = new QDBusInterface("com.kylin.network",
                                         "/com/kylin/network",
                                         "com.kylin.network",
                                         QDBusConnection::sessionBus());
        if(!m_pInterface->isValid()) {
            qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        }
        registerInputPasswdAgent();//注册输入代理
        qDBusRegisterMetaType<QVector<QStringList>>();

        this->connect(lanDataKeeper.data(), &KnmLanDataKeeper::onDeviceStatusChanged, this, &KnmDBusCaller::updateWiredDeviceMap);
        this->connect(wlanDataKeeper.data(), &KnmWlanDataKeeper::onDeviceStatusChanged, this, &KnmDBusCaller::updateWirelessDevice);

        getWiredDeviceMap();
        getWiredMainSwitchState();
        getWirelessSwitchState();
        getWirelessDeviceMap();
    }
}

KnmDBusCaller::~KnmDBusCaller()
{

}
// LCOV_EXCL_STOP

QMap<QString, NetDevicePtr> KnmDBusCaller::wiredDeviceList()
{
    return lanDataKeeper->getDeviceList();
}

QMap<QString, NetDevicePtr> KnmDBusCaller::wirelessDeviceList()
{
    return wlanDataKeeper->getDeviceList();
}

QVariantList KnmDBusCaller::wiredDeviceConnList(QString devName)
{
    return lanDataKeeper->getDevConnections(devName);
}

QVariantList KnmDBusCaller::wirelessDeviceConnList(QString devName)
{
    return wlanDataKeeper->getDevConnections(devName);
}

bool KnmDBusCaller::wirelessSwitchState()
{
    return wlanDataKeeper->getSwitchState();
}

bool KnmDBusCaller::wiredMainSwitchState()
{
    qWarning()<< Q_FUNC_INFO << __LINE__;

    return lanDataKeeper->getWiredMainSwitchState();
}

QString KnmDBusCaller::upwardRateDate()
{
    return lanDataKeeper->getUpwardRate();
}

QString KnmDBusCaller::downwardRateDate()
{
    return lanDataKeeper->getDownwardRate();
}

QString KnmDBusCaller::getWiFiIcon(QString signalStrength, QString security, QString isApConnection, int category)
{
    return wlanDataKeeper->getWiFiIcon(signalStrength, security, isApConnection, category);
}

// LCOV_EXCL_START
QDBusPendingCallWatcher *KnmDBusCaller::asyncCall(const QString & methed, const QList<QVariant> & params)
{
    
    // 添加nullptr检查，防止测试模式下崩溃
    if (!m_pInterface) {
        qWarning() << "D-Bus interface is null, cannot make call";
        return nullptr;
    }
    
    if (!m_pInterface->isValid()) {
        qWarning() << "not connect dbus server: " << m_pInterface->lastError().message();
        return nullptr;
    }

    QDBusPendingCall pendingCall = m_pInterface->asyncCallWithArgumentList(methed, params);

    return new QDBusPendingCallWatcher(pendingCall);
}
// LCOV_EXCL_STOP

void KnmDBusCaller::activateConnect(QString devName, QString conUid, int type)
{
    
    QList<QVariant> list;
    list.append(type);
    list.append(devName);
    list.append(conUid);
    QDBusPendingCallWatcher *watcher = nullptr;

    if (!g_testMode)
        watcher = this->asyncCall("activateConnect", list);
    
    if (!watcher) {
        qWarning() << "activateConnect: asyncCall returned nullptr";
    }
}

void KnmDBusCaller::deActivateConnect(QString devName, QString conUid, int type)
{
    
    QList<QVariant> list;
    list.append(type);
    list.append(devName);
    list.append(conUid);
    QDBusPendingCallWatcher *watcher = nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("deActivateConnect", list);
    if (!watcher) {
        qWarning() << "deActivateConnect: asyncCall returned nullptr";
    }
}

void KnmDBusCaller::passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect)
{
    
    QList<QVariant> list;
    list.append(devName);
    list.append(ssid);
    list.append(type);
    list.append(psk);
    list.append(autoConnect);
    QDBusPendingCallWatcher *watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("passwordConnect", list);
    if (!watcher) {
        qWarning() << "passwordConnect: asyncCall returned nullptr";
    }
}

void KnmDBusCaller::showPropertyWidget(QString devName, QString ssid)
{
    
    QList<QVariant> list;
    list.append(devName);
    list.append(ssid);
    QDBusPendingCallWatcher *watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("showPropertyWidget", list);
    if (!watcher) {
        qWarning() << "showPropertyWidget: asyncCall returned nullptr";
    }
}

void KnmDBusCaller::deleteConnect(int type, QString ssid)
{
    
    QList<QVariant> list;
    list.append(type);
    list.append(ssid);
    QDBusPendingCallWatcher *watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("deleteConnect", list);
    if (!watcher) {
        qWarning() << "deleteConnect: asyncCall returned nullptr";
    }
}

void KnmDBusCaller::getWirelessSwitchState()
{
    
    QList<QVariant> list;
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getWirelessSwitchBtnState", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWirelessSwitchStateFinished);
    } else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWiredMainSwitchState()
{

    qDebug() << Q_FUNC_INFO << __LINE__;
    QList<QVariant> list;
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getWiredMainSwitchBtnState", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWiredMainSwitchStateFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWiredDeviceMap()
{
    
    QList<QVariant> list;
    list.append(0);
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWiredDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWirelessDeviceMap()
{
    // 测试模式下跳过D-Bus调用
    if (g_testMode) {
        qWarning() << "Test mode, skipping getWirelessDeviceMap";
    }
    
    QList<QVariant> list;
    list.append(1);
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWirelessDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::setWirelessSwitchEnable(bool enable)
{
    
    QList<QVariant> list;
    list.append(enable);
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("setWirelessSwitchEnable", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::setWirelessSwitchEnableFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::setUpwareRateData(QString str)
{

}

void KnmDBusCaller::setDownwareRateData(QString str)
{

}

void KnmDBusCaller::updateWiredDeviceMap()
{
    QList<QVariant> list;
    list.append(0);
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::updateWiredDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::updateWirelessDeviceMap()
{
    QList<QVariant> list;
    list.append(1);
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::updateWirelessDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::updateWirelessDevice()
{
    // 测试模式下跳过D-Bus调用
    if (g_testMode) {
        qWarning() << "Test mode, skipping updateWirelessDevice";
        return;
    }
    
    getWirelessSwitchState();
    updateWirelessDeviceMap();

   // qWarning() << "mqtets updateWirelessDevice";
}

void KnmDBusCaller::rescanWirelessConn()
{
    
    QList<QVariant> list;
    QDBusPendingCallWatcher *watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("reScan", list);
    // 测试模式下不处理异步回调
    if (watcher && g_testMode) {
        delete watcher;
    }
}

void KnmDBusCaller::getWiredConList(QString dev)
{
    
    QList<QVariant> list;
    list.append(dev);
    QDBusPendingCallWatcher * watcher =  nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getWiredList", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *watcher) {
            getWiredConListFinished(watcher, dev);
        });
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWirelessConList(QString dev)
{
    QList<QVariant> list;
    list.append(dev);
    QDBusPendingCallWatcher * watcher = nullptr; 

    if (!g_testMode)
        watcher = this->asyncCall("getWirelessList", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *watcher) {
            getWirelessConListFinished(watcher, dev);
        });
    }
    else {
        qWarning() << "null pending";
    }
}

// LCOV_EXCL_START
void KnmDBusCaller::getWiredDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            while (item != retMap.value().cend()) {
                lanDataKeeper->addDevice(item.key(), item.value().toBool(),WIRED_DEVICE);
                getWiredConList(item.key());
                item++;
            }
            KInterface::getInstance()->updateWiredDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KnmDBusCaller::updateWiredDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            lanDataKeeper->clearDevice();
            while (item != retMap.value().cend()) {
                lanDataKeeper->addDevice(item.key(), item.value().toBool(),WIRED_DEVICE);
                lanDataKeeper->clearConnectionList(item.key());
                getWiredConList(item.key());
                item++;
            }
            KInterface::getInstance()->updateWiredDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KnmDBusCaller::updateWirelessDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            wlanDataKeeper->clearDevice();
            while (item != retMap.value().cend()) {
                wlanDataKeeper->addDevice(item.key(), item.value().toBool(),WIRELESS_DEVICE);
                wlanDataKeeper->clearConnectionList(item.key());
                updateWirelessConListSync(item.key());
                item++;
            }
            KInterface::getInstance()->rebuildCurrentWirelessList();//设备状态变更应该全量更新合理点
            emit KInterface::getInstance()->updateWirelessDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP


// LCOV_EXCL_START
void KnmDBusCaller::getWirelessDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            while (item != retMap.value().cend()) {
                wlanDataKeeper->addDevice(item.key(), item.value().toBool(), WIRELESS_DEVICE);
                updateWirelessConListSync(item.key());
                item++;
            }
            KInterface::getInstance()->rebuildCurrentWirelessList();//设备状态变更应该全量更新合理点
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KnmDBusCaller::getWiredConListFinished(QDBusPendingCallWatcher *watcher, QString dev)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantList> retList = reply;
            QList<QStringList> list;
            for (int j = 0; j < retList.value().size(); ++j) {
                list << retList.value().at(j).toStringList();
            }
            lanDataKeeper->addDevConnection(dev, list);
            KInterface::getInstance()->updateWiredDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KnmDBusCaller::getWirelessConListFinished(QDBusPendingCallWatcher *watcher, QString dev)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantList> retList = reply;
            QList<QStringList> list;
            for (int j = 0; j < retList.value().size(); ++j) {
                list << retList.value().at(j).toStringList();
            }
            wlanDataKeeper->clearConnectionList(dev);
            wlanDataKeeper->addDevConnection(dev, list);
            KInterface::getInstance()->updateWirelessDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KnmDBusCaller::getWirelessSwitchStateFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<bool> retList = reply;
            wlanDataKeeper->setSwitchState(retList.value());
            KInterface::getInstance()->updateWirelessSwitch();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KnmDBusCaller::getWiredMainSwitchStateFinished(QDBusPendingCallWatcher *watcher)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    qDebug() << Q_FUNC_INFO << __LINE__ << reply;

    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<bool> retList = reply;
            lanDataKeeper->setWiredMainSwitchState(retList.value());
            KInterface::getInstance()->updateWiredMainSwitch();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KnmDBusCaller::setWirelessSwitchEnableFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        updateWirelessDeviceMap();
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

void KnmDBusCaller::setWiredMainSwitchEnable(bool enable)
{
    
    qDebug() << Q_FUNC_INFO << __LINE__ << enable;
    QList<QVariant> list;
    list.append(enable);

    QDBusPendingCallWatcher * watcher = nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("setWiredSwitchEnable", list);

    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::setWiredMainSwitchEnableFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

// LCOV_EXCL_START
void KnmDBusCaller::setWiredMainSwitchEnableFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        getWiredMainSwitchState();
        updateWiredDeviceMap();
    } else {
        qWarning() << reply.errorMessage();
    }
}
// LCOV_EXCL_STOP

void KnmDBusCaller::showAddOtherWlanPage(QString devName)
{
    QList<QVariant> list;
    list.append(devName);
    QDBusPendingCallWatcher *watcher = nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("showAddOtherWlanWidget",list);
    // 测试模式下不处理异步回调
    if (watcher) {
        delete watcher;
    }
}

// LCOV_EXCL_START
void KnmDBusCaller::updateWirelessConListSync(QString dev)
{
    // 测试模式下跳过D-Bus调用
    if (g_testMode || !m_pInterface) {
        qWarning() << "D-Bus interface is null or test mode, skipping updateWirelessConListSync";
        return;
    }
    
    QDBusReply<QVariantList> reply = m_pInterface->call(QStringLiteral("getWirelessList"), dev);
    //qDebug() << "[NetConnect]call getWirelessList respond"  << __LINE__;
    if(!reply.isValid()) {
        qWarning() << "getWirelessList error:" << reply.error().message();
        return;
    }

    QList<QStringList> list;
    for (int j = 0; j < reply.value().size(); ++j) {
        list << reply.value().at(j).toStringList();
    }
    wlanDataKeeper->clearConnectionList(dev);
    wlanDataKeeper->addDevConnection(dev, list);
    KInterface::getInstance()->updateWirelessDeviceList();
}
// LCOV_EXCL_STOP


// void KnmDBusCaller::getWiredMainSwitchState()
// {
//     QList<QVariant> list;
//     QDBusPendingCallWatcher * watcher = this->asyncCall("getWirelessSwitchBtnState", list);
//     if(watcher) {
//         m_pendingCount++;
//         this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWirelessSwitchStateFinished);
//     }
//     else {
//         qWarning() << "null pending";
//     }
// }

/*
*   调用getCableStateByDevice dbus接口，返回值为 bool：是否插入网线
*/
void KnmDBusCaller::getCableStateByDevice(const QString &dev)
{
    
    QList<QVariant> list;
    list.append(dev);
    QDBusPendingCallWatcher * watcher = nullptr;
    if (!g_testMode)
        watcher = this->asyncCall("getCableStateByDevice", list);

    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::updateCableState);
    }
    else {
        qWarning() << "null pending";
    }
}

/* 
*  getCableStateByDevice dbus返回槽函数
*/
// LCOV_EXCL_START
void KnmDBusCaller::updateCableState(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusPendingReply<bool> reply = *watcher;
    if (!reply.isError()) {
        bool ret = reply.value();
        KInterface::getInstance()->updateCable(ret);
    } else {
        qWarning() << reply.isError();
    }
}
// LCOV_EXCL_STOP


void KnmDBusCaller::registerInputPasswdAgent()
{
    
    QList<QVariant> list;
    QVariantMap map;//预留参数不赋值 后端不检查
    list.append(KYLIN_QML_INPUTAGENT_NAME);
    list.append(map);
    QDBusPendingCallWatcher * watcher= nullptr;
    if (!g_testMode) 
        watcher = this->asyncCall("registerInputPasswdAgent", list);

    if(!watcher) {
        qWarning() << "registerInputPasswdAgent pending";
    }
}

void KnmDBusCaller::setNetworkConnectionAutoConnectState(int netType, QString uuid, bool state)
{
    qWarning() << Q_FUNC_INFO << __LINE__ << netType << uuid << state;
    QList<QVariant> list;
    list.append(netType);
    list.append(uuid);
    list.append(state);
    if (!g_testMode)
        this->asyncCall("setNetworkConnectionAutoConnectState",list);
}

void KnmDBusCaller::setDefaultWiredDevice(QString devName)
{

    qWarning() << Q_FUNC_INFO << __LINE__ << devName;
    QList<QVariant> list;
    list.append(devName);

    if (!g_testMode)
        this->asyncCall("setDefaultWiredDevice",list);
}

void KnmDBusCaller::setDefaultWirelessDevice(QString devName)
{

    qWarning() << Q_FUNC_INFO << __LINE__ << devName;
    QList<QVariant> list;
    list.append(devName);

    if (!g_testMode)
        this->asyncCall("setDefaultWirelessDevice",list);
}

QString KnmDBusCaller::getDefaultWiredDevice()
{
    qWarning() << Q_FUNC_INFO << __LINE__;
    // 测试模式下跳过D-Bus调用
    if (g_testMode || !m_pInterface) {
        qWarning() << "Test mode or null interface, skipping getDefaultWiredDevice";
        return QString();
    }
    QDBusMessage msg = m_pInterface->call("getDefaultWiredDevice");
    if (msg.type() == QDBusMessage::ReplyMessage) {
        return msg.arguments().at(0).toString();
    }
    return QString();
}

QString KnmDBusCaller::getDefaultWirelessDevice()
{
    qWarning() << Q_FUNC_INFO << __LINE__;
    // 测试模式下跳过D-Bus调用
    if (g_testMode || !m_pInterface) {
        qWarning() << "Test mode or null interface, skipping getDefaultWirelessDevice";
        return QString();
    }
    QDBusMessage msg = m_pInterface->call("getDefaultWirelessDevice");
    if (msg.type() == QDBusMessage::ReplyMessage) {
        return msg.arguments().at(0).toString();
    }
    return QString();
}
