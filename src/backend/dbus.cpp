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

#include "dbus.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

const QByteArray GSETTINGS_SCHEMA_KYLIN_NM = "org.ukui.kylin-nm.switch";
const QString    KEY_WIRELESS_SWITCH          = "wirelessswitch";
const QString    KEY_WIRED_SWITCH             = "wiredswitch";

/*
 * Implementation of adaptor class DbusAdaptor
 */

DbusAdaptor::DbusAdaptor(QString display, MainWindow *m, QObject *parent)
    : QObject(parent),
      m_display(display),
      m_mainWindow(m)
{
    // constructor
    qDBusRegisterMetaType<QMap<QString, bool> >();
    qDBusRegisterMetaType<QMap<QString, int> >();
    qDBusRegisterMetaType<QList<QStringList> >();
    qDBusRegisterMetaType<QMap<QString, QList<QStringList> >>();

    bool isServiceRegistered = QDBusConnection::sessionBus().interface()->isServiceRegistered(QStringLiteral("com.kylin.network"));
    if(!isServiceRegistered) {
        registerService();
    }

    m_watcher = new QDBusServiceWatcher(QStringLiteral("com.kylin.network"),QDBusConnection::sessionBus(), QDBusServiceWatcher::WatchForOwnerChange, this);
    connect(m_watcher, &QDBusServiceWatcher::serviceOwnerChanged, this, &DbusAdaptor::onServiceOwnerChanged);

    m_pSysBusInterfaces = new QDBusInterface(SYSTEM_DBUS_SERVICE,
                                             SYSTEM_DBUS_PATH,
                                             SYSTEM_DBUS_INTERFACE,
                                             QDBusConnection::systemBus());

    if (m_pSysBusInterfaces->isValid()) {
        QDBusConnection::systemBus().connect(SYSTEM_DBUS_SERVICE,
                                             SYSTEM_DBUS_PATH,
                                             SYSTEM_DBUS_INTERFACE,
                                             "sysWiredMainSwitchChanged",
                                             this,
                                             SLOT(onWiredMainSwitchBtnChanged(bool)));
    }
    else
    {
        qWarning() << Q_FUNC_INFO << __LINE__  ;
    }

    connectToMainwindow();
}

DbusAdaptor::~DbusAdaptor()
{
    if(mNetworkAdaptor) delete mNetworkAdaptor;

}

void DbusAdaptor::onServiceOwnerChanged(const QString &service, const QString &oldOwner, const QString &newOwner)
{
    if (newOwner.isEmpty()) {
        bool success = registerService();
        if (success) {
            m_watcher->deleteLater();
        }
        qDebug() << "try to register service:" << success;
        return;
    }

    uint newOwnerPid = QDBusConnection::sessionBus().interface()->servicePid(newOwner);
    qDebug() << "newOwnerPid:" << newOwnerPid << ", myPid:" << QCoreApplication::applicationPid() << ", display:" << m_display;
}

bool DbusAdaptor::registerService()
{
    mNetworkAdaptor=new NetworkAdaptor(this);

    QDBusConnection conn = QDBusConnection::sessionBus();
    auto reply = conn.interface()->registerService(QStringLiteral("com.kylin.network"),
                                                          QDBusConnectionInterface::ReplaceExistingService,
                                                          QDBusConnectionInterface::DontAllowReplacement);
    if (reply.value() == QDBusConnectionInterface::ServiceNotRegistered) {
        return false;
    }

    bool res = QDBusConnection::sessionBus().registerObject("/com/kylin/network", this);
    if (!res) {
        QDBusConnection::sessionBus().interface()->unregisterService(QStringLiteral("com.kylin.network"));
    }
    return res;
}


//无线列表
QVariantList DbusAdaptor::getWirelessList(QString devName)
{
    QList<QStringList> list;
    m_mainWindow->getWirelessList(devName, list);
    QVariantList vList;
    for (int i = 0; i < list.size(); ++i) {
        vList.append(QVariant::fromValue(list.at(i)));
    }
    return vList;
}

bool DbusAdaptor::getWirelessSwitchBtnState()
{
    return m_mainWindow->getWirelessSwitchBtnState();
}

bool DbusAdaptor::getWiredMainSwitchBtnState()
{
    if (m_pSysBusInterfaces && m_pSysBusInterfaces->isValid())
    {
        QDBusReply <bool> reply = m_pSysBusInterfaces->call("getWiredMainSwitch");
        if (reply.isValid()) {
            bool status = reply.value();
            return status;
        }
    }
    return false;
}

int DbusAdaptor::getDeviceConnectivity(const QString deviceName)
{
    return m_mainWindow->getDeviceConnectivity(deviceName);
}

//有线列表
QVariantList DbusAdaptor::getWiredList(QString devName)
{
    QList<QStringList> list;
    m_mainWindow->getWiredList(devName, list);
    QVariantList vList;
    for (int i = 0; i < list.size(); ++i) {
        vList.append(QVariant::fromValue(list.at(i)));
    }
    return vList;
}

//有线开关
void DbusAdaptor::setWiredSwitchEnable(bool enable)
{
    //todo

    if (m_pSysBusInterfaces->isValid())
    {
        m_pSysBusInterfaces->call(QStringLiteral("setWiredMainSwitch"), enable);
    }
    else
    {
        qWarning()<< Q_FUNC_INFO << __LINE__ <<"m_pSysBusInterfaces is isValid!";
    }

    if (!enable) {
        // int devType 0:lan 1:wlan  
        int devType = 0;
        const auto devList = mNetworkAdaptor->getDeviceListAndEnabled(devType);
        for (auto it = devList.cbegin(); it != devList.cend(); ++it) {
            const QString &devName = it.key();
            const auto connections = getWiredList(devName);
            for (const QVariant &conn : connections) {
                // 网卡名称,uuid,对应DBUS路径
                const auto connInfo = conn.toList();
                if (connInfo.size() >= 2) {  // 只需确保有UUID即可
                    deActivateConnect(0, devName, connInfo.at(1).toString());
                }
            }
        }
    }

#if 0
    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA_KYLIN_NM)) {
        QGSettings *gsetting = new QGSettings(GSETTINGS_SCHEMA_KYLIN_NM);
        if (gsetting->get(KEY_WIRED_SWITCH).toBool() != enable) {
            gsetting->set(KEY_WIRED_SWITCH, enable);
        }
        delete gsetting;
        gsetting = nullptr;
    } else {
        qDebug()<<"isSchemaInstalled false";
    }
#endif

}

//无线开关
void DbusAdaptor::setWirelessSwitchEnable(bool enable)
{
    //todo mainwindow调用backend 对开关 打开/关闭
//    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA_KYLIN_NM)) {
//        QGSettings *gsetting = new QGSettings(GSETTINGS_SCHEMA_KYLIN_NM);
//        if (gsetting->get(KEY_WIRELESS_SWITCH).toBool() != enable) {
//            gsetting->set(KEY_WIRELESS_SWITCH, enable);
//        }
//        delete gsetting;
//        gsetting = nullptr;
//    } else {
//        qDebug()<<"isSchemaInstalled false";
//    }
    m_mainWindow->setWirelessSwitchEnable(enable);
}

//启用/禁用网卡
void DbusAdaptor::setDeviceEnable(QString devName, bool enable)
{
    m_mainWindow->setWiredDeviceEnable(devName, enable);
}

//设置默认网卡
//void DbusAdaptor::setDefaultWiredDevice(QString deviceName)
//{
//    if (!checkDeviceExist(WIRED, deviceName)) {
//        return;
//    }
//    setDefaultDevice(WIRED, deviceName);
//    parent()->setWiredDefaultDevice(deviceName);
//    return;
//}

//QString DbusAdaptor::getDefaultWiredDevice()
//{
//    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
//    m_settings->beginGroup("DEFAULTCARD");
//    QString key("wired");
//    QString deviceName = m_settings->value(key, "").toString();
//    m_settings->endGroup();
//    delete m_settings;
//    m_settings = nullptr;
//    return deviceName;
//}

//void DbusAdaptor::setDefaultWirelessDevice(QString deviceName)
//{
//    if (!checkDeviceExist(WIRED, deviceName)) {
//        return;
//    }
//    setDefaultDevice(WIRELESS, deviceName);
//    parent()->setWirelessDefaultDevice(deviceName);
//    return;
//}

//QString  DbusAdaptor::getDefaultWirelessDevice()
//{
//    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
//    m_settings->beginGroup("DEFAULTCARD");
//    QString key("wireless");
//    QString deviceName = m_settings->value(key, "").toString();
//    m_settings->endGroup();
//    delete m_settings;
//    m_settings = nullptr;
//    return deviceName;
//}

//连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
void DbusAdaptor::activateConnect(int type, QString devName, QString ssid)
{
    if (type == WIRED) {
        m_mainWindow->activateWired(devName,ssid);
    } else if (type == WIRELESS) {
        m_mainWindow->activateWireless(devName,ssid);
    } else {
        qDebug() << "[DbusAdaptor] activateConnect type is invalid";
    }
}

//断开连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
void DbusAdaptor::deActivateConnect(int type, QString devName, QString ssid)
{
    if (type == WIRED) {
        qDebug()  << "deactivateWired";
        m_mainWindow->deactivateWired(devName,ssid);
    } else if (type == WIRELESS) {
        m_mainWindow->deactivateWireless(devName,ssid);
    } else {
        qDebug() << "[DbusAdaptor] deactivateConnect type is invalid";
    }
}
//delete连接
void DbusAdaptor::deleteConnect(int type,QString Uuid)
{
    qWarning() << Q_FUNC_INFO << __LINE__  << type << Uuid;
    if (type == WIRED) {
        qWarning() << Q_FUNC_INFO << __LINE__;
        m_mainWindow->deleteWiredConnect(type,Uuid);
    } else if (type == WIRELESS) {
        qWarning() << Q_FUNC_INFO << __LINE__;

        m_mainWindow->deleteWireleeConnect(type,Uuid);
    } else {
        qWarning() << Q_FUNC_INFO << __LINE__  << "deleteConnect type is invalid";
    }
}

//获取设备列表和启用/禁用状态
QVariantMap DbusAdaptor::getDeviceListAndEnabled(int devType)
{
    QMap<QString, bool> map;
    map.clear();
    getDeviceEnableState(devType, map);
    QVariantMap vMap;
    QMap<QString, bool>::const_iterator item = map.cbegin();
    QString switchSettingFile = "/etc/kylin-nm/switch.conf";
    QSettings switchSetting(switchSettingFile, QSettings::IniFormat, this);
    while (item != map.cend()) {
        if (switchSetting.contains(item.key()))
            vMap.insert(item.key(), QVariant::fromValue(switchSetting.value(item.key())));
        else
            vMap.insert(item.key(), QVariant::fromValue(item.value()));
        item ++;
    }
    return vMap;
}

//获取无线设备能力
QVariantMap DbusAdaptor::getWirelessDeviceCap()
{
    QMap<QString, int> map;
    m_mainWindow->getWirelessDeviceCap(map);
    QVariantMap vMap;
    QMap<QString, int>::const_iterator item = map.cbegin();
    while (item != map.cend()) {
        vMap.insert(item.key(), QVariant::fromValue(item.value()));
        item ++;
    }
    return vMap;
}

//唤起属性页 根据网卡类型 参数2 为ssid/uuid
void DbusAdaptor::showPropertyWidget(QString devName, QString ssid)
{
    QString display = checkDisplay();
    if (m_display == display) {
        m_mainWindow->showPropertyWidget(devName,ssid);
    } else {
        Q_EMIT showPropertyWidgetSignal(display, devName, ssid);
    }
}

//唤起新建有线连接界面
void DbusAdaptor::showCreateWiredConnectWidget(QString devName)
{
    QString display = checkDisplay();
    if (m_display == display) {
        m_mainWindow->showCreateWiredConnectWidget(devName);
    } else {
        Q_EMIT showCreateWiredConnectWidgetSignal(display, devName);
    }
}

//唤起加入其他无线网络界面
void DbusAdaptor::showAddOtherWlanWidget(QString devName)
{
    QString display = checkDisplay();
    if (m_display == display) {
        qDebug() << "showAddOtherWlanWidget";
        m_mainWindow->showAddOtherWlanWidget(devName);
    } else {
        qDebug() << display;
        Q_EMIT showAddOtherWlanWidgetSignal(display, devName);
    }
}

//开启热点
void DbusAdaptor::activeWirelessAp(const QString apName, const QString apPassword, const QString band, const QString apDevice)
{
    m_mainWindow->activeWirelessAp(apName, apPassword, band, apDevice);
}

//断开热点
void DbusAdaptor::deactiveWirelessAp(const QString apName, const QString uuid)
{
    m_mainWindow->deactiveWirelessAp(apName, uuid);
}
void DbusAdaptor::passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect)
{
    m_mainWindow->passwordConnect(devName, ssid, type, psk, autoConnect);
}
//获取热点
QStringList DbusAdaptor::getStoredApInfo()
{
    QStringList list;
    list.clear();
    m_mainWindow->getStoredApInfo(list);
    return list;
}

//获取热点path
QString DbusAdaptor::getApConnectionPath(QString uuid)
{
    QString path;
    path.clear();
    m_mainWindow->getApConnectionPath(path, uuid);
    return path;
}

//获取热点path
QString DbusAdaptor::getActiveConnectionPath(QString uuid)
{
    QString path;
    path.clear();
    m_mainWindow->getActiveConnectionPath(path, uuid);
    return path;
}

QStringList DbusAdaptor::getApInfoBySsid(QString devName, QString ssid)
{
    QStringList list;
    list.clear();
    m_mainWindow->getApInfoBySsid(devName, ssid, list);
    return list;
}

void DbusAdaptor::showKylinNM(int type)
{
    qDebug() << "display" << checkDisplay();
    QString display = checkDisplay();
    if (m_display == display) {
        m_mainWindow->onShowMainWindow(type);
    } else {

        Q_EMIT showKylinNMSignal(display, type);
    }
}

//扫描
void DbusAdaptor::reScan()
{
    m_mainWindow->rescan();
}

void DbusAdaptor::keyRingInit()
{
    m_mainWindow->keyRingInit();
}

void DbusAdaptor::keyRingClear()
{
    m_mainWindow->keyRingClear();
}

void DbusAdaptor::connectToMainwindow()
{
    connect(m_mainWindow, &MainWindow::lanAdd, this, &DbusAdaptor::lanAdd);
    connect(m_mainWindow, &MainWindow::lanRemove, this, &DbusAdaptor::lanRemove);
    connect(m_mainWindow, &MainWindow::lanUpdate, this, &DbusAdaptor::lanUpdate);
    connect(m_mainWindow, &MainWindow::wlanAdd, this, &DbusAdaptor::wlanAdd);
    connect(m_mainWindow, &MainWindow::wlanRemove, this, &DbusAdaptor::wlanRemove);
    connect(m_mainWindow, &MainWindow::wlanactiveConnectionStateChanged, this, &DbusAdaptor::wlanactiveConnectionStateChanged);
    connect(m_mainWindow, &MainWindow::lanActiveConnectionStateChanged, this, &DbusAdaptor::lanActiveConnectionStateChanged);
    connect(m_mainWindow, &MainWindow::activateFailed, this, &DbusAdaptor::activateFailed);
    connect(m_mainWindow, &MainWindow::deactivateFailed, this, &DbusAdaptor::deactivateFailed);
    connect(m_mainWindow, &MainWindow::deviceStatusChanged, this, &DbusAdaptor::deviceStatusChanged);
    connect(m_mainWindow, &MainWindow::wirelessDeviceStatusChanged, this, &DbusAdaptor::wirelessDeviceStatusChanged);
    connect(m_mainWindow, &MainWindow::deviceNameChanged, this, &DbusAdaptor::deviceNameChanged);
    connect(m_mainWindow, &MainWindow::wirelessSwitchBtnChanged, this, &DbusAdaptor::wirelessSwitchBtnChanged);
    connect(m_mainWindow, &MainWindow::wiredMainSwitchBtnChanged, this, &DbusAdaptor::wiredMainSwitchBtnChanged);
    connect(m_mainWindow, &MainWindow::hotspotDeactivated, this, &DbusAdaptor::hotspotDeactivated);
    connect(m_mainWindow, &MainWindow::hotspotActivated, this, &DbusAdaptor::hotspotActivated);
    connect(m_mainWindow, &MainWindow::signalStrengthChange, this, &DbusAdaptor::signalStrengthChange);
    connect(m_mainWindow, &MainWindow::secuTypeChange, this, &DbusAdaptor::secuTypeChange);
    connect(m_mainWindow, &MainWindow::timeToUpdate, this, &DbusAdaptor::timeToUpdate);
}

QString DbusAdaptor::checkDisplay()
{
    uint pid = 0;
    QDBusReply<uint> pidReply = connection().interface()->servicePid(message().service());
    qDebug() << "caller pid: " << pidReply.value();
    if(pidReply.isValid()) {
        pid = pidReply.value();
    } else {
        return {};
    }
    return displayFromPid(pid);;
}

QString DbusAdaptor::displayFromPid(uint pid)
{
    QFile environFile(QStringLiteral("/proc/%1/environ").arg(QString::number(pid)));
    if (environFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QByteArray DISPLAY = qApp->property("sessionType").toString() == "wayland" ? QByteArrayLiteral("WAYLAND_DISPLAY")
                                                                      : QByteArrayLiteral("DISPLAY");
        const auto lines = environFile.readAll().split('\0');
        for (const QByteArray &line : lines) {
            const int equalsIdx = line.indexOf('=');
            if (equalsIdx <= 0) {
                continue;
            }
            const QByteArray key = line.left(equalsIdx);
            if (key == DISPLAY) {
                const QByteArray value = line.mid(equalsIdx + 1);
                return value;
            }
        }
    }
    return {};
}

void DbusAdaptor::onWiredMainSwitchBtnChanged(bool state)
{
    qDebug() << Q_FUNC_INFO << __LINE__  << state;
    Q_EMIT DbusAdaptor::wiredMainSwitchBtnChanged(state);
}

