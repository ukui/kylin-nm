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
#ifndef LANITEM_H
#define LANITEM_H
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariantMap>
#include <QTimer>
#include <QGSettings>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QObject>
#include "klabel.h"
//#include "infobutton.h"
#include "../component/AddBtn/grayinfobutton.h"
//#include "kborderlessbutton.h"

using namespace kdk;

enum ConnectivityType {
    UnknownConnectivity = 0,
    NoConnectivity = 1,
    Portal = 2,
    Limited = 3,
    Full = 4
};

enum ConnectState {
    UnknownState = 0, /**< The active connection is in an unknown state */
    Activating, /**< The connection is activating */
    Activated, /**< The connection is activated */
    Deactivating, /**< The connection is being torn down and cleaned up */
    Deactivated /**< The connection is no longer active */
};

class LanItem : public QPushButton
{
    Q_OBJECT
public:
    LanItem(bool isAcitve, QWidget *parent = nullptr);
    ~LanItem();
public:
    QLabel * iconLabel = nullptr;
    GrayInfoButton * infoLabel = nullptr;
    KLabel * titileLabel = nullptr;
    QLabel * statusLabel = nullptr;

public:
    void startLoading();
    void stopLoading();
    void setNetworkCheckFrameHidden(bool state);
    void setConnectivityWarn(ConnectivityType connectivityType);

    bool loading = false;
    bool isAcitve = false;
    bool isPppoe = false;

    QString uuid;
    QString dbusPath;

protected:
    void paintEvent(QPaintEvent *);

private:
    QTimer *waitTimer = nullptr;
    QGSettings *themeGsettings = nullptr;
    QList<QIcon> loadIcons;
    int currentIconIndex=0;

    QFrame *m_networkCheckFrame = nullptr;
    QLabel * m_warnLabel = nullptr;
    QLabel * m_warnTextLabel = nullptr;
    //KBorderlessButton * m_networkCheckBtn = nullptr;

private slots:
    void updateIcon();

};

#endif // LANITEM_H
