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
#include "fixlabel.h"
//#include "infobutton.h"
#include "../component/AddBtn/grayinfobutton.h"

class LanItem : public QPushButton
{
public:
    LanItem(bool isAcitve, QWidget *parent = nullptr);
    ~LanItem();
public:
    QLabel * iconLabel = nullptr;
    GrayInfoButton * infoLabel = nullptr;
    FixLabel * titileLabel = nullptr;
    QLabel * statusLabel = nullptr;

public:
    void startLoading();
    void stopLoading();

    bool loading = false;
    bool isAcitve = false;

    QString uuid;
    QString dbusPath;

protected:
    void paintEvent(QPaintEvent *);

private:
    QTimer *waitTimer = nullptr;
    QGSettings *themeGsettings = nullptr;
    QList<QIcon> loadIcons;
        int currentIconIndex=0;

private slots:
        void updateIcon();

};

#endif // LANITEM_H
