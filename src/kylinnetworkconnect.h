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
#include "kylinconnectsetting.h"

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
    void  createConnect(KyConnectType connectType, const KyConnectSetting &connectInfo);
    void  updateConnect(const QString &connectUuid, const KyConnectSetting &connectInfo);
    void  deleteConnect(const QString &connectUuid);
    int  addAndActivateConnect(const QString );
    int  activateConnection(const QString connectUuid);
    void activateWirelessConnection(NetworkManager::WirelessNetwork::Ptr wirelessNet);
    void addAndActivateWirelessConnection(NetworkManager::WirelessNetwork::Ptr wirelessNet, const QString &psk);
    int  deactivateConnection(const QString connectName, const QString &uuid);
    void requestScan(const QString ifaceName);
    void requestAllWifiScan();

private:
    void connectSettingInit(
                        NetworkManager::ConnectionSettings::Ptr connectionSettings,
                        const KyConnectSetting &connectInfo);
    void ipv4SettingInit(NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                         const KyConnectSetting &connectInfo);
    void ipv6SettingInit(NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                         const KyConnectSetting &connectInfo);

    void createWiredConnect(const KyConnectSetting &connectInfo);

signals:
    void activateConnectionFinished(NetworkManager::Connection::Ptr &conn);
    void deactivateConnectionFinished(NetworkManager::Connection::Ptr &conn);

    void noConnection();
    void notSavedConnection();

signals:
    void checkActiveonnection(const QString &activeConn);
    void connResult(int);
    void starWaiting();

public slots:
    void onActivateWirelessConnection(const QString &connectSsid, const QString &connectUuid);
    void onActivateWirelessConnectionWithPWD(const QString &connectSsid, const QString &psk, bool isNotSaved, const QString &connectUuid);
    void onCheckActiveonnection(const QString &activeConn);
    void onStateChangedReason(NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};

#endif // KYLINNETWORKCONNECT_H
