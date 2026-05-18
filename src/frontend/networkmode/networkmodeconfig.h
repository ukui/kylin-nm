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
#include <QDBusInterface>
#include <QDBusReply>
#include <QDir>
#include <QDBusPendingCallWatcher>

#include "kylinactiveconnectresource.h"
#include "kylinconnectresource.h"
#include "kywirelessnetresource.h"

#define CONFIG_FILE_PATH QString(QDir::homePath() + "/.config/ukui/kylin-nm.conf")
#define AUTO_FIRE_WALL_PERMIITTED "conf/autoFireWallPermitted"
#define FIRE_WALL_PERMISSION_SHOW "conf/firewallPermissionShow"

enum NetworkModeType {
    DBUS_INVALID = -2,
    NO_CONFIG = -1,
    KSC_FIREWALL_PUBLIC = 0,
    KSC_FIREWALL_PRIVATE
};

class NetworkModeConfig : public QObject
{
    Q_OBJECT
public:
    static NetworkModeConfig *getInstance();
    //异步安全中心-获取网络模式配置
    void getNetworkModeConfigAsync(const QString &uuid);
    //安全中心-获取网络模式配置
    int getNetworkModeConfig(QString uuid);
    //安全中心-设置网络模式配置
    void setNetworkModeConfig(QString uuid, QString cardName, QString ssid, int mode);
    //安全中心-解除连接（用于防火墙处从正在使用的网络中删除）
    void breakNetworkConnect(QString uuid, QString cardName, QString ssid);

    static NetworkModeConfig *m_netModeInstance;

Q_SIGNALS:
    // 异步结果信号（核心新增）
    void networkModeConfigReady(const QString &uuid, int configType, bool success);

private:
    explicit NetworkModeConfig(QObject *parent = nullptr);
    QDBusInterface *m_dbusInterface = nullptr;
};


// 定义待处理的配置数据结构体
struct PendingConfigData {
    QString uuid;
    QString deviceName;
    QString ssid;
    bool isWired;
};

struct PendingStateData {
    QString uuid;
    QString deviceName;
    QString ssid;
    bool isWired;
    bool isWireless;
};


class NetworkMode : public QObject
{
    Q_OBJECT
public:
    NetworkMode(QObject *parent = 0);
    ~NetworkMode() = default;
    void initWiredNetworkMode();
    void initWirelessNetworkMode();

private:
    KyActiveConnectResourse *m_activatedConnectResource = nullptr;
    // 保存待处理的初始化配置
    QMap<QString, PendingConfigData> m_pendingConfigMap;
    // 保存待处理的连接状态变化配置
    QMap<QString, PendingStateData> m_pendingStateMap;

    void setFirstConnectNetworkMode(QString uuid, QString deviceName, QString ssid);

private Q_SLOTS:
    void onConnectionStateChanged(QString uuid,
                                  NetworkManager::ActiveConnection::State state,
                                  NetworkManager::ActiveConnection::Reason reason);
    // 新增：异步查询结果回调
    void onGetNetworkModeConfigReady(const QString &uuid, int configType, bool success);
};

#endif // NETWORKMODECONFIG_H
