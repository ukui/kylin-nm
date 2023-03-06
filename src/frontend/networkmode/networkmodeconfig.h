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
#ifndef NETWORKMODECONFIG_H
#define NETWORKMODECONFIG_H

#include <QObject>
#include <QThread>
#include <QDBusInterface>
#include <QDBusReply>
#include <kylin-nm/kylinnetworkmanager.h>

enum NetworkModeType {
    DBUS_INVAILD = -2,
    NO_CONFIG = -1,
    KSC_FIREWALL_PUBLIC = 0,
    KSC_FIREWALL_PRIVATE
};

class NetworkModeConfig : public QObject
{
    Q_OBJECT
public:
    NetworkModeConfig(QObject *parent = 0);
    ~NetworkModeConfig() = default;
    void initWiredNetworkMode();
    void initWirelessNetworkMode();

private:
    KyNetworkManager* m_manager;
    QThread*        m_thread;
    QDBusInterface *m_firewallDbusIface = nullptr;

    //安全中心-获取网络模式配置
    int getNetworkModeConfig(QString uuid);
    //安全中心-设置网络模式配置
    void setNetworkModeConfig(QString uuid, QString cardName, QString ssid, int mode);
    //安全中心-解除连接（用于防火墙处从正在使用的网络中删除）
    int breakNetworkConnect(QString uuid, QString cardName, QString ssid);

    void setFirstConnectNetworkMode(QString uuid, QString deviceName, QString ssid);

private Q_SLOTS:
    void onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, KyConnectState status);
};

#endif // NETWORKMODECONFIG_H
