/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
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
#ifndef DBUS_ADAPTOR_H
#define DBUS_ADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.kylin.network
 */
class NetworkAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.network")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.kylin.network\">\n"
"    <signal name=\"lanAdd\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"as\" name=\"info\"/>\n"
"    </signal>\n"
"    <signal name=\"lanRemove\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"dbusPath\"/>\n"
"    </signal>\n"
"    <signal name=\"lanUpdate\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"as\" name=\"info\"/>\n"
"    </signal>\n"
"    <signal name=\"wlanAdd\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"as\" name=\"info\"/>\n"
"    </signal>\n"
"    <signal name=\"wlanRemove\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"ssid\"/>\n"
"    </signal>\n"
"    <signal name=\"wlanactiveConnectionStateChanged\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"ssid\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"uuid\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"status\"/>\n"
"    </signal>\n"
"    <signal name=\"lanActiveConnectionStateChanged\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"uuid\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"status\"/>\n"
"    </signal>\n"
"    <signal name=\"activateFailed\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"errorMessage\"/>\n"
"    </signal>\n"
"    <signal name=\"deactivateFailed\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"errorMessage\"/>\n"
"    </signal>\n"
"    <signal name=\"deviceStatusChanged\"/>\n"
"    <signal name=\"wirelessDeviceStatusChanged\"/>\n"
"    <signal name=\"deviceNameChanged\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"oldName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"newName\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"type\"/>\n"
"    </signal>\n"
"    <signal name=\"wirelessSwitchBtnChanged\">\n"
"      <arg direction=\"out\" type=\"b\" name=\"state\"/>\n"
"    </signal>\n"
"    <signal name=\"wiredMainSwitchBtnChanged\">\n"
"      <arg direction=\"out\" type=\"b\" name=\"state\"/>\n"
"    </signal>\n"
"    <signal name=\"hotspotDeactivated\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"ssid\"/>\n"
"    </signal>\n"
"    <signal name=\"hotspotActivated\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"ssid\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"uuid\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"activePath\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"settingPath\"/>\n"
"    </signal>\n"
"    <signal name=\"signalStrengthChange\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"ssid\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"strength\"/>\n"
"    </signal>\n"
"    <signal name=\"secuTypeChange\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"ssid\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"secuType\"/>\n"
"    </signal>\n"
"    <signal name=\"timeToUpdate\"/>\n"
"    <signal name=\"showKylinNMSignal\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"display\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"type\"/>\n"
"    </signal>\n"
"    <signal name=\"showPropertyWidgetSignal\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"display\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"ssid\"/>\n"
"    </signal>\n"
"    <signal name=\"showCreateWiredConnectWidgetSignal\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"display\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"    </signal>\n"
"    <signal name=\"showAddOtherWlanWidgetSignal\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"display\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"devName\"/>\n"
"    </signal>\n"
"    <method name=\"getWirelessList\">\n"
"      <arg direction=\"out\" type=\"av\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"    </method>\n"
"    <method name=\"getWiredList\">\n"
"      <arg direction=\"out\" type=\"av\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"    </method>\n"
"    <method name=\"setWiredSwitchEnable\">\n"
"      <arg direction=\"in\" type=\"b\" name=\"enable\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"setWirelessSwitchEnable\">\n"
"      <arg direction=\"in\" type=\"b\" name=\"enable\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"setDeviceEnable\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"enable\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"activateConnect\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"type\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"ssid\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"deleteConnect\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"type\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"ssid\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"deActivateConnect\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"type\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"ssid\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"setDeviceAutoConnectState\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"state\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"deActivateConnectConcise\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"type\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"concise\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"ssid\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"getDeviceListAndEnabled\">\n"
"      <arg direction=\"out\" type=\"a{sv}\"/>\n"
"      <annotation value=\"QVariantMap\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"      <arg direction=\"in\" type=\"i\" name=\"devType\"/>\n"
"    </method>\n"
"    <method name=\"getWirelessDeviceCap\">\n"
"      <arg direction=\"out\" type=\"a{sv}\"/>\n"
"      <annotation value=\"QVariantMap\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"    <method name=\"showPropertyWidget\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"ssid\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"showCreateWiredConnectWidget\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"showAddOtherWlanWidget\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"activeWirelessAp\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"apName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"apPassword\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"band\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"apDevice\"/>\n"
"    </method>\n"
"    <method name=\"deactiveWirelessAp\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"apName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"uuid\"/>\n"
"    </method>\n"
"    <method name=\"getStoredApInfo\">\n"
"      <arg direction=\"out\" type=\"as\"/>\n"
"    </method>\n"
"    <method name=\"getApInfoBySsid\">\n"
"      <arg direction=\"out\" type=\"as\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"ssid\"/>\n"
"    </method>\n"
"    <method name=\"passwordConnect\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"devName\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"ssid\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"type\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"psk\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"autoConnect\"/>\n"
"      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
"    </method>\n"
"    <method name=\"getApConnectionPath\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"uuid\"/>\n"
"    </method>\n"
"    <method name=\"getActiveConnectionPath\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"uuid\"/>\n"
"    </method>\n"
"    <method name=\"reScan\"/>\n"
"    <method name=\"keyRingInit\"/>\n"
"    <method name=\"keyRingClear\"/>\n"
"    <method name=\"showKylinNM\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"type\"/>\n"
"    </method>\n"
"    <method name=\"getWirelessSwitchBtnState\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"getWiredMainSwitchBtnState\">\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"getDeviceConnectivity\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"deviceName\"/>\n"
"      <arg direction=\"out\" type=\"i\"/>\n"
"    </method>\n"
"    <method name=\"getCableStateByDevice\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"deviceName\"/>\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"

"  </interface>\n"
        "")
public:
    NetworkAdaptor(QObject *parent);
    virtual ~NetworkAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    Q_NOREPLY void activateConnect(int type, const QString &devName, const QString &ssid);
    void activeWirelessAp(const QString &apName, const QString &apPassword, const QString &band, const QString &apDevice);
    Q_NOREPLY void deActivateConnect(int type, const QString &devName, const QString &ssid);
    Q_NOREPLY void deActivateConnectConcise(int type, bool concise, const QString &devName, const QString &ssid);
    Q_NOREPLY void deleteConnect(int type, const QString &ssid);
    Q_NOREPLY void setDeviceAutoConnectState(const QString &deviceName, bool state);
    void deactiveWirelessAp(const QString &apName, const QString &uuid);
    QString getActiveConnectionPath(const QString &uuid);
    QString getApConnectionPath(const QString &uuid);
    QStringList getApInfoBySsid(const QString &devName, const QString &ssid);
    QVariantMap getDeviceListAndEnabled(int devType);
    QStringList getStoredApInfo();
    QVariantList getWiredList(const QString &devName);
    QVariantMap getWirelessDeviceCap();
    QVariantList getWirelessList(const QString &devName);
    bool getWirelessSwitchBtnState();
    bool getWiredMainSwitchBtnState();
    void keyRingClear();
    void keyRingInit();
    void reScan();
    Q_NOREPLY void passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect);
    Q_NOREPLY void setDeviceEnable(const QString &devName, bool enable);
    Q_NOREPLY void setWiredSwitchEnable(bool enable);
    Q_NOREPLY void setWirelessSwitchEnable(bool enable);
    Q_NOREPLY void showAddOtherWlanWidget(const QString &devName);
    Q_NOREPLY void showCreateWiredConnectWidget(const QString &devName);
    void showKylinNM(int type);
    Q_NOREPLY void showPropertyWidget(const QString &devName, const QString &ssid);
    int getDeviceConnectivity(const QString deviceName);

    //获取网卡是否插入了网线
    bool getCableStateByDevice(const QString deviceName);
Q_SIGNALS: // SIGNALS
    void activateFailed(const QString &errorMessage);
    void deactivateFailed(const QString &errorMessage);
    void deviceNameChanged(const QString &oldName, const QString &newName, int type);
    void deviceStatusChanged();
    void hotspotActivated(const QString &devName, const QString &ssid, const QString &uuid, const QString &activePath, const QString &settingPath);
    void hotspotDeactivated(const QString &devName, const QString &ssid);
    void lanActiveConnectionStateChanged(const QString &devName, const QString &uuid, int status);
    void lanAdd(const QString &devName, const QStringList &info);
    void lanRemove(const QString &dbusPath);
    void lanUpdate(const QString &devName, const QStringList &info);
    void secuTypeChange(const QString &devName, const QString &ssid, const QString &secuType);
    void showAddOtherWlanWidgetSignal(const QString &display, const QString &devName);
    void showCreateWiredConnectWidgetSignal(const QString &display, const QString &devName);
    void showKylinNMSignal(const QString &display, int type);
    void showPropertyWidgetSignal(const QString &display, const QString &devName, const QString &ssid);
    void signalStrengthChange(const QString &devName, const QString &ssid, int strength);
    void timeToUpdate();
    void wirelessDeviceStatusChanged();
    void wirelessSwitchBtnChanged(bool state);
    void wiredMainSwitchBtnChanged(bool state);
    void wlanAdd(const QString &devName, const QStringList &info);
    void wlanRemove(const QString &devName, const QString &ssid);
    void wlanactiveConnectionStateChanged(const QString &devName, const QString &ssid, const QString &uuid, int status);
};

#endif
