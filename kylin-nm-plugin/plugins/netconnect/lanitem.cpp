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

#include "lanitem.h"
#include <QPainterPath>
#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8
#define RADIUS 6.0

LanItem::LanItem(bool isSimple, QWidget *parent)
    : QFrame(parent),isSimple(isSimple)
{
    this->setFixedSize(404, 48);
    QHBoxLayout *mLanLyt = new QHBoxLayout(this);
    mLanLyt->setContentsMargins(16,0,0,0);
    mLanLyt->setSpacing(0);
    radioBtn = new RadioItemButton(this);
    titileLabel = new FixLabel(this);
    titileLabel->setMinimumWidth(282);

    mLanLyt->addWidget(radioBtn);
    mLanLyt->addSpacing(10);
    mLanLyt->addWidget(titileLabel,Qt::AlignLeft);
    if (!isSimple) {
        infoLabel = new InfoButton(this);
        mLanLyt->addSpacing(8);
        mLanLyt->addWidget(infoLabel);
        connect(infoLabel, &InfoButton::released, this, &LanItem::infoButtonClick);
    }

    loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    waitTimer = new QTimer(this);
    connect(waitTimer, &QTimer::timeout, this, &LanItem::updateIcon);

    m_menu = new QMenu(this);//右键菜单
    m_menu->setWindowFlag(Qt::X11BypassWindowManagerHint);
    connect(m_menu, &QMenu::triggered, this, &LanItem::itemClick);
}

void LanItem::updateIcon()
{
    if (currentIconIndex > 6) {
        currentIconIndex = 0;
    }
    radioBtn->setButtonIcon(loadIcons.at(currentIconIndex));
    currentIconIndex ++;
}

void LanItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    m_loading = true;
}

void LanItem::stopLoading(){
    waitTimer->stop();
    m_loading = false;
}

void LanItem::mousePressEvent(QMouseEvent *event)
{
    return QFrame::mousePressEvent(event);
}
void LanItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        if (!m_menu) {
            return QFrame::mouseReleaseEvent(event);
        }

        m_menu->clear();
        if (m_isAcitve || m_loading) {
            m_menu->addAction(new QAction(tr("Disconnect"), this));
        } else if (!m_isAcitve && !m_loading) {
            m_menu->addAction(new QAction(tr("Connect"), this));
        }
        m_menu->move(cursor().pos());
        m_menu->show();

    } else {
        if (!m_isAcitve && !m_loading) {
           Q_EMIT itemClick();
        }
    }
    return QFrame::mouseReleaseEvent(event);
}
void LanItem::enterEvent(QEvent *event)
{
    m_isIn = true;
    update();
    return QFrame::enterEvent(event);
}
void LanItem::leaveEvent(QEvent *event)
{
    m_isIn = false;
    update();
    return QFrame::leaveEvent(event);
}
void LanItem::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    if (m_isIn) {
        QColor color(240, 240, 240);
        color.setAlphaF(0.39);
        painter.setBrush(color);
    }
    else
        painter.setBrush(pal.color(QPalette::Base));

    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect, RADIUS, RADIUS);
    painter.drawPath(path);
    return QFrame::paintEvent(event);
}
