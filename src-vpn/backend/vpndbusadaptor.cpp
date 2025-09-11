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

#include "vpndbusadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class VpnDbusAdaptor
 */

VpnDbusAdaptor::VpnDbusAdaptor(vpnObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    qDBusRegisterMetaType<QMap<QString, bool> >();
    qDBusRegisterMetaType<QMap<QString, int> >();
    qDBusRegisterMetaType<QVector<QStringList> >();
    setAutoRelaySignals(true);
}

//虚拟连接列表
QVector<QStringList> VpnDbusAdaptor::getVirtualList()
{
    QVector<QStringList> vector;
    parent()->getVirtualList(vector);
    return vector;
}

//删除
void VpnDbusAdaptor::deleteVpn(QString uuid)
{
    qDebug()  << "delete vpn" << uuid;
    parent()->deleteVpn(uuid);
}

//连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
void VpnDbusAdaptor::activateVpn(const QString& connUuid)
{
    qDebug()  << "activate vpn" << connUuid;
    parent()->activateVpn(connUuid);
}

//断开连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
void VpnDbusAdaptor::deactivateVpn(const QString& connUuid)
{
    qDebug()  << "deactivate vpn" << connUuid;
    parent()->deactivateVpn(connUuid);
}

void VpnDbusAdaptor::showKylinVpn()
{
    parent()->onShowMainWindow();
}

void VpnDbusAdaptor::showVpnAddWidget()
{
    qDebug() << "showVpnAddWidget";
    parent()->showVpnAddWidget();
}

void VpnDbusAdaptor::showDetailPage(const QString& connUuid)
{
    qDebug()  << "showDetailPage vpn" << connUuid;
    parent()->showDetailPage(connUuid);
}

