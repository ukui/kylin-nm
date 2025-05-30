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

#include "dbus_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class NetworkAdaptor
 */

NetworkAdaptor::NetworkAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

NetworkAdaptor::~NetworkAdaptor()
{
    // destructor
}

void NetworkAdaptor::activateConnect(int type, const QString &devName, const QString &ssid)
{
    // handle method call com.kylin.network.activateConnect
    QMetaObject::invokeMethod(parent(), "activateConnect", Q_ARG(int, type), Q_ARG(QString, devName), Q_ARG(QString, ssid));
}

void NetworkAdaptor::activeWirelessAp(const QString &apName, const QString &apPassword, const QString &band, const QString &apDevice)
{
    // handle method call com.kylin.network.activeWirelessAp
    QMetaObject::invokeMethod(parent(), "activeWirelessAp", Q_ARG(QString, apName), Q_ARG(QString, apPassword), Q_ARG(QString, band), Q_ARG(QString, apDevice));
}

void NetworkAdaptor::deActivateConnect(int type, const QString &devName, const QString &ssid)
{
    // handle method call com.kylin.network.deActivateConnect
    QMetaObject::invokeMethod(parent(), "deActivateConnect", Q_ARG(int, type), Q_ARG(QString, devName), Q_ARG(QString, ssid));
}

void NetworkAdaptor::deleteConnect(int type, const QString &ssid)
{
    // handle method call com.kylin.network.deActivateConnect
    QMetaObject::invokeMethod(parent(), "deleteConnect", Q_ARG(int, type), Q_ARG(QString, ssid));
}

void NetworkAdaptor::deactiveWirelessAp(const QString &apName, const QString &uuid)
{
    // handle method call com.kylin.network.deactiveWirelessAp
    QMetaObject::invokeMethod(parent(), "deactiveWirelessAp", Q_ARG(QString, apName), Q_ARG(QString, uuid));
}

QString NetworkAdaptor::getActiveConnectionPath(const QString &uuid)
{
    // handle method call com.kylin.network.getActiveConnectionPath
    QString out0;
    QMetaObject::invokeMethod(parent(), "getActiveConnectionPath", Q_RETURN_ARG(QString, out0), Q_ARG(QString, uuid));
    return out0;
}

QString NetworkAdaptor::getApConnectionPath(const QString &uuid)
{
    // handle method call com.kylin.network.getApConnectionPath
    QString out0;
    QMetaObject::invokeMethod(parent(), "getApConnectionPath", Q_RETURN_ARG(QString, out0), Q_ARG(QString, uuid));
    return out0;
}

QStringList NetworkAdaptor::getApInfoBySsid(const QString &devName, const QString &ssid)
{
    // handle method call com.kylin.network.getApInfoBySsid
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "getApInfoBySsid", Q_RETURN_ARG(QStringList, out0), Q_ARG(QString, devName), Q_ARG(QString, ssid));
    return out0;
}

QVariantMap NetworkAdaptor::getDeviceListAndEnabled(int devType)
{
    // handle method call com.kylin.network.getDeviceListAndEnabled
    QVariantMap out0;
    QMetaObject::invokeMethod(parent(), "getDeviceListAndEnabled", Q_RETURN_ARG(QVariantMap, out0), Q_ARG(int, devType));
    return out0;
}

QStringList NetworkAdaptor::getStoredApInfo()
{
    // handle method call com.kylin.network.getStoredApInfo
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "getStoredApInfo", Q_RETURN_ARG(QStringList, out0));
    return out0;
}

QVariantList NetworkAdaptor::getWiredList(const QString &devName)
{
    // handle method call com.kylin.network.getWiredList
    QVariantList out0;
    QMetaObject::invokeMethod(parent(), "getWiredList", Q_RETURN_ARG(QVariantList, out0), Q_ARG(QString, devName));
    return out0;
}

QVariantMap NetworkAdaptor::getWirelessDeviceCap()
{
    // handle method call com.kylin.network.getWirelessDeviceCap
    QVariantMap out0;
    QMetaObject::invokeMethod(parent(), "getWirelessDeviceCap", Q_RETURN_ARG(QVariantMap, out0));
    return out0;
}

QVariantList NetworkAdaptor::getWirelessList(const QString &devName)
{
    // handle method call com.kylin.network.getWirelessList
    QVariantList out0;
    QMetaObject::invokeMethod(parent(), "getWirelessList", Q_RETURN_ARG(QVariantList, out0), Q_ARG(QString, devName));
    return out0;
}

bool NetworkAdaptor::getWirelessSwitchBtnState()
{
    // handle method call com.kylin.network.getWirelessSwitchBtnState
    bool out0;
    QMetaObject::invokeMethod(parent(), "getWirelessSwitchBtnState", Q_RETURN_ARG(bool, out0));
    return out0;
}
bool NetworkAdaptor::getWiredMainSwitchBtnState()
{
    // handle method call com.kylin.network.getWiredMainSwitchBtnState
    bool out0;
    QMetaObject::invokeMethod(parent(), "getWiredMainSwitchBtnState", Q_RETURN_ARG(bool, out0));
    return out0;
}
void NetworkAdaptor::keyRingClear()
{
    // handle method call com.kylin.network.keyRingClear
    QMetaObject::invokeMethod(parent(), "keyRingClear");
}

void NetworkAdaptor::keyRingInit()
{
    // handle method call com.kylin.network.keyRingInit
    QMetaObject::invokeMethod(parent(), "keyRingInit");
}

void NetworkAdaptor::reScan()
{
    // handle method call com.kylin.network.reScan
    QMetaObject::invokeMethod(parent(), "reScan");
}

void NetworkAdaptor::passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect)
{
    // handle method call com.kylin.network.passwordConnect
    QMetaObject::invokeMethod(parent(), "passwordConnect",
                              Q_ARG(QString, devName),
                              Q_ARG(QString, ssid),
                              Q_ARG(QString, type),
                              Q_ARG(QString, psk),
                              Q_ARG(bool, autoConnect));
}

void NetworkAdaptor::setDeviceEnable(const QString &devName, bool enable)
{
    // handle method call com.kylin.network.setDeviceEnable
    QMetaObject::invokeMethod(parent(), "setDeviceEnable", Q_ARG(QString, devName), Q_ARG(bool, enable));
}

void NetworkAdaptor::setWiredSwitchEnable(bool enable)
{
    // handle method call com.kylin.network.setWiredSwitchEnable
    QMetaObject::invokeMethod(parent(), "setWiredSwitchEnable", Q_ARG(bool, enable));
}

void NetworkAdaptor::setWirelessSwitchEnable(bool enable)
{
    // handle method call com.kylin.network.setWirelessSwitchEnable
    QMetaObject::invokeMethod(parent(), "setWirelessSwitchEnable", Q_ARG(bool, enable));
}

void NetworkAdaptor::showAddOtherWlanWidget(const QString &devName)
{
    // handle method call com.kylin.network.showAddOtherWlanWidget
    QMetaObject::invokeMethod(parent(), "showAddOtherWlanWidget", Q_ARG(QString, devName));
}

void NetworkAdaptor::showCreateWiredConnectWidget(const QString &devName)
{
    // handle method call com.kylin.network.showCreateWiredConnectWidget
    QMetaObject::invokeMethod(parent(), "showCreateWiredConnectWidget", Q_ARG(QString, devName));
}

void NetworkAdaptor::showKylinNM(int type)
{
    // handle method call com.kylin.network.showKylinNM
    QMetaObject::invokeMethod(parent(), "showKylinNM", Q_ARG(int, type));
}

void NetworkAdaptor::showPropertyWidget(const QString &devName, const QString &ssid)
{
    // handle method call com.kylin.network.showPropertyWidget
    QMetaObject::invokeMethod(parent(), "showPropertyWidget", Q_ARG(QString, devName), Q_ARG(QString, ssid));
}

