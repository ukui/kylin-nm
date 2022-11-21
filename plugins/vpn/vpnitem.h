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
#ifndef VPNITEM_H
#define VPNITEM_H
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariantMap>
#include <QTimer>
#include <QDebug>
#include <QGSettings>
#include <QImage>
#include <QPainter>
#include <QToolButton>
#include <QMenu>
#include <QEvent>
#include "fixlabel.h"
#include "infobutton.h"

class VpnItem : public QPushButton
{
    Q_OBJECT
public:
    VpnItem(bool bAcitve, QWidget *parent = nullptr);
public:
    QLabel * iconLabel = nullptr;
    InfoButton * infoLabel = nullptr;
    FixLabel * titileLabel = nullptr;
    QLabel * statusLabel = nullptr;
    QToolButton* m_moreButton = nullptr;
    QMenu* m_moreMenu = nullptr;
    QAction* m_connectAction = nullptr;
    QAction* m_deleteAction = nullptr;

    QString uuid = "";
    QString dbusPath = "";

    void setHalfFillet(bool flag) {useHalfFillet = flag; repaint();}
public:
    void startLoading();
    void stopLoading();
    void setConnectActionText(bool isAcitve);

    bool isAcitve = false;
    bool loading = false;

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QTimer *waitTimer = nullptr;
    QGSettings *themeGsettings = nullptr;
    bool useHalfFillet = false;
    QList<QIcon> loadIcons;
    int currentIconIndex=0;

private slots:
    void updateIcon();
    void onConnectTriggered();
    void onDeletetTriggered();

Q_SIGNALS:
    void connectActionTriggered();
    void disconnectActionTriggered();
    void deleteActionTriggered();

};

#endif // VPNITEM_H
