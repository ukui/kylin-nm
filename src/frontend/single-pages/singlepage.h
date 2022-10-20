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
#ifndef SINGLEPAGE_H
#define SINGLEPAGE_H

#include "divider.h"
#include "kylable.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>
#include <QDir>
#include <QComboBox>
#include <QEvent>
#include <QProcess>
#include <QDebug>
#include "kylinnetworkdeviceresource.h"
#include "firewalldialog.h"
#include "kwidget.h"
#include "kswitchbutton.h"
//#include "kborderlessbutton.h"

using namespace kdk;

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 50     //TabWidget的tab和widget有间隙，和设计稿看起来一致就不能设为设计稿里的高度
#define TITLE_LAYOUT_MARGINS 24,0,24,0
#define DEVICE_LAYOUT_MARGINS 24,0,24,8
#define DEVICE_COMBOBOX_WIDTH 180
#define ACTIVE_NET_LAYOUT_MARGINS 8,8,8,8
#define NET_LAYOUT_MARGINS 8,8,0,1
#define NET_LAYOUT_SPACING 8
#define NET_LIST_SPACING 0
#define TEXT_MARGINS 16,0,0,0
#define TEXT_HEIGHT 20
//#define SCROLL_AREA_HEIGHT 200
#define SETTINGS_LAYOUT_MARGINS 23,0,24,0
#define TRANSPARENT_COLOR QColor(0,0,0,0)
#define INACTIVE_AREA_MIN_HEIGHT 170
#define ACTIVE_AREA_MAX_HEIGHT 92

#define MAX_ITEMS 4
#define MAX_WIDTH 412
#define MIN_WIDTH 404

#define SCROLL_STEP 4

class SinglePage : public QWidget
{
    Q_OBJECT
public:
    explicit SinglePage(QWidget *parent = nullptr);
    ~SinglePage();

    static void showDesktopNotify(const QString &message, QString soundName);

    void hideSetting() {
        if (nullptr != m_settingsFrame) {
            m_settingsFrame->hide();
            m_netDivider->hide();
            m_netFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT + 100);
        }
    }
    void showSetting() {
        if (nullptr != m_settingsFrame) {
            m_netFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT);
            m_settingsFrame->show();
            m_netDivider->show();
        }
    }

Q_SIGNALS:
    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);

protected:
    void initUI();
    QVBoxLayout * m_mainLayout = nullptr;

    QFrame * m_netFrame = nullptr;
    QVBoxLayout * m_netLayout = nullptr;
    QLabel * m_netLabel = nullptr;
    QWidget * m_netListArea = nullptr;
    QVBoxLayout * m_netAreaLayout = nullptr;

    Divider * m_netDivider = nullptr;

    QFrame * m_settingsFrame = nullptr;
    QHBoxLayout * m_settingsLayout = nullptr;
    KyLable * m_settingsLabel = nullptr;

};

#endif // TABPAGE_H
