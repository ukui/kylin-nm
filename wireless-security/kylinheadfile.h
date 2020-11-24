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

#ifndef KYLINHEADFILE_H
#define KYLINHEADFILE_H

#include "dlghidewifi.h"
#include "dlghidewifileap.h"
#include "dlghidewifieapfast.h"
#include "dlghidewifieapleap.h"
#include "dlghidewifieappeap.h"
#include "dlghidewifieappwd.h"
#include "dlghidewifieaptls.h"
#include "dlghidewifieapttls.h"
#include "dlghidewifiwep.h"
#include "dlghidewifiwpa.h"

#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QListView>
#include <QString>
#include <KWindowEffects>

class MyQss
{
public:
    MyQss();

    QString labelQss, cbxQss, leQss, btnConnQss, btnCancelQss, lineQss, checkBoxQss, checkBoxCAQss;

};

#endif // KYLINHEADFILE_H
