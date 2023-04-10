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
#ifndef VPNLISTITEM_H
#define VPNLISTITEM_H
#include "listitem.h"
#include "kylinactiveconnectresource.h"

#include <QDBusInterface>
#include <QEvent>
#include <QAction>
#include <QProcess>

#include "vpndetails/vpndetail.h"
#include "kylinconnectresource.h"
//#include "kylinwiredconnectoperation.h"
#include "kyvpnconnectoperation.h"


#define KYLIN_APP_MANAGER_NAME "com.kylin.AppManager"
#define KYLIN_APP_MANAGER_PATH "/com/kylin/AppManager"
#define KYLIN_APP_MANAGER_INTERFACE "com.kylin.AppManager"

class VpnListItem : public ListItem
{
    Q_OBJECT

public:
    VpnListItem(const KyConnectItem *vpnConnectItem, QWidget *parent = nullptr);
    VpnListItem(QWidget *parent = nullptr);

    ~VpnListItem();

public:
    void updateConnectionState(ConnectState state);

    QString getConnectionName();
    void updateConnectionName(QString connectionName);

    QString getConnectionPath();
    void updateConnectionPath(QString connectionPath);

    void activeConnection();

protected:
    void setIcon(bool isOn);
    void onRightButtonClicked();
    bool launchApp(QString desktopFile);
    void runExternalApp();

private:
    void connectItemCopy(const KyConnectItem *vpnConnectItem);

public Q_SLOTS:
    void onInfoButtonClicked();

private Q_SLOTS:
    void onNetButtonClicked();
    void onMenuTriggered(QAction *action);

private:
    KyConnectItem m_vpnConnectItem;

    KyVpnConnectOperation *m_connectOperation = nullptr;
    KyNetworkDeviceResourse *m_deviceResource = nullptr;

    QString m_deviceName = "";

    vpnDetail *m_vpnDetail = nullptr;
};

#endif // VPNLISTITEM_H
