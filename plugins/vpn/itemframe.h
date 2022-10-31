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
#ifndef ITEMFRAME_H
#define ITEMFRAME_H
#include <QFrame>
#include <QVBoxLayout>
#include "../component/AddBtn/addnetbtn.h"
#include "vpnitem.h"

class ItemFrame : public QFrame
{
    Q_OBJECT
public:
    ItemFrame(QWidget *parent = nullptr);
    //单设备整体layout
    QVBoxLayout * deviceLanLayout = nullptr;
    //单设备列表Frame
    QFrame * lanItemFrame = nullptr;
    //单设备列表layout
    QVBoxLayout * lanItemLayout = nullptr;
    //item列表
    QMap<QString, VpnItem *> itemMap;
//    //已激活uuid
//    QString uuid = "";
    //新建无线连接
    AddNetBtn * addWlanWidget = nullptr;
    void filletStyleChange();
};

#endif // ITEMFRAME_H
