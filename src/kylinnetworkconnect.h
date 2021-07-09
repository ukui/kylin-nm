/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#ifndef KYLINNETWORKCONNECT_H
#define KYLINNETWORKCONNECT_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectinfo.h"

typedef enum{
    WIRED_CONNECT,
    WIFI_CONNECT,
}KyConnectType;

class KyNetworkConnect : public QObject
{
    Q_OBJECT
public:
    KyNetworkConnect();
    ~KyNetworkConnect();

public:
    void  createConnect(KyConnectType connectType, const KyConnectInfo &connectInfo);
    void  updateConnect(const QString &connectUuid, const KyConnectInfo &connectInfo);
    void  deleteConnect(const QString &connectUuid);
    int  addAndActivateConnect(const QString );
    int  activateConnection(const QString connectUuid);
    int  deactivateConnection(const QString connectName, const QString &uuid);
    void requestScan(const QString ifaceName);
    void requestAllWifiScan();

private:
    void connectSettingInit(
                        NetworkManager::ConnectionSettings::Ptr connectionSettings,
                        const KyConnectInfo &connectInfo);
    void ipv4SettingInit(NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                         const KyConnectInfo &connectInfo);
    void ipv6SettingInit(NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                         const KyConnectInfo &connectInfo);

    void createWiredConnect(const KyConnectInfo &connectInfo);

signals:
    void activateConnectionFinished(NetworkManager::Connection::Ptr &conn);
    void deactivateConnectionFinished(NetworkManager::Connection::Ptr &conn);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};

#endif // KYLINNETWORKCONNECT_H
