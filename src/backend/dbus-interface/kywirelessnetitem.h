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
#ifndef KYWIRELESSNETITEM_H
#define KYWIRELESSNETITEM_H

#include <QString>

#include "kylinnetworkresourcemanager.h"
#include "kywirelessconnectoperation.h"

QString enumToQstring(NetworkManager::AccessPoint::Capabilities, NetworkManager::AccessPoint::WpaFlags, NetworkManager::AccessPoint::WpaFlags);

class KyWirelessNetItem
{
public:
    KyWirelessNetItem(NetworkManager::WirelessNetwork::Ptr net);
    KyWirelessNetItem() {;}
    ~KyWirelessNetItem();

private:
    void init(NetworkManager::WirelessNetwork::Ptr net);
    void initInfoBySsid();

public:
    QString                                 m_NetSsid;
    QString                                 m_connectUuid;
    QString                                 m_bssid;
    int                                     m_signalStrength;
    uint                                    m_frequency;
    QString                                 m_secuType;
    KySecuType                              m_kySecuType;
    QString                                 m_uni;

    //only for m_isConfiged = true
    bool                                    m_isConfigured;
    QString                                 m_connName;
    QString                                 m_connDbusPath;
    uint                                    m_channel;

    int getCategory(QString uni);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    QString                                 m_device;

};

#endif // KYWIRELESSNETITEM_H
