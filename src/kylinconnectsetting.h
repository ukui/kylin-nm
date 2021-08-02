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

#ifndef KYLINCONNECTSETTING_H
#define KYLINCONNECTSETTING_H

#include <QStringList>
#include <QString>
#include <QObject>

#include <NetworkManagerQt/AdslDevice>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>
#include <NetworkManagerQt/WiredSetting>

typedef enum {
    CONFIG_IP_MANUAL,
    CONFIG_IP_DHCP,
}KyIpConfigType;

typedef enum {
    IPADDRESS_V4,
    IPADDRESS_V6,
}KyIpAddressType;

class KyConnectSetting : public QObject
{
    Q_OBJECT
public:
    KyConnectSetting();
    ~KyConnectSetting();

public:
    void setIfaceName(QString &ifaceName);
    void setConnectName(QString &connectName);
    int  setIpConfigType(KyIpAddressType ipType, KyIpConfigType configType);
    void ipv4AddressConstruct(QString &ipv4Address, QString &ipv4NetMask, QString &ipv4GateWay, QStringList &ipv4Dns);
    void ipv6AddressConstruct(QString &ipv6Address, QString &ipv6NetMask, QString &ipv6GateWay, QStringList &ipv6Dns);
    void dumpInfo();

public:
    QString  m_connectName;
    QString  m_ifaceName;

    KyIpConfigType  m_ipv4ConfigIpType;
    QList<NetworkManager::IpAddress> m_ipv4Address;
    QList<QHostAddress> m_ipv4Dns;

    KyIpConfigType  m_ipv6ConfigIpType;
    QList<NetworkManager::IpAddress> m_ipv6Address;
    QList<QHostAddress> m_ipv6Dns;
};

#endif // KYLINCONNECTSETTING_H
