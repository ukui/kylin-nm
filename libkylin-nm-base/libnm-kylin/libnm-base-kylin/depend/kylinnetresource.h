/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef KYNETRESOURCE_H
#define KYNETRESOURCE_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"
#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>

class KyDetailInfo
{
public:
    QString strSecType;
    QString strChan;
    QString strMac;
    QString strHz;
    QString strBandWidth;
    QString strDynamicIpv4;
    QString strDynamicIpv6;
    QString strDynamicIpv4Dns;
    bool    isAutoConnect = false;
};

class KyNetResource : public QObject
{
    Q_OBJECT
public:
    explicit KyNetResource(QObject *parent = nullptr);
    ~KyNetResource();

    void getConnectionSetting(QString connectUuid, KyConnectSetting &connectSetting);

protected:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;

    void getBaseInfo(QString devName,QString ssid, bool isWlan, bool isActive, KyDetailInfo &conInfo);
    void getDynamicIpInfo(QString uuid, KyDetailInfo &conInfo);

private:
    void getIpv4ConnectSetting(NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                                                    KyConnectSetting &connectSetting);
    void getIpv6ConnectSetting(NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                                                    KyConnectSetting &connectSetting);
};

#endif // KYNETRESOURCE_H
