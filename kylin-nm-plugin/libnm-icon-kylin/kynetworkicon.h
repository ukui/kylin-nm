/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
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

#ifndef KYNETWORKICON_H
#define KYNETWORKICON_H

#include "libnm-base-kylin_global.h"

#include <kylinnetworkmanager.h>

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QThread>

class LIBNMBASEKYLIN_EXPORT KyNetworkIcon : public QPushButton
{
    Q_OBJECT
public:
    KyNetworkIcon(QWidget *parent = nullptr);
    ~KyNetworkIcon();

private:
    KyNetworkManager* manager;
    QThread*        thread;

    QTimer *loadingTimer;
    QTimer *refreshTimer;

    int currentIconIndex=0;
    QList<QIcon> loadIcons;

    KyConnectStatus iconStatus;

    void initConnect();

    void startLoading();
    void stopLoading();

protected:
    void paintEvent(QPaintEvent *);

private Q_SLOTS:
    void onSetTrayIconLoading();
    void updateIcon();
    void onWiredStateChange(QString deviceName, QString uuid, KyConnectState state);
    void onWirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state);
};

#endif // KYNETWORKICON_H
