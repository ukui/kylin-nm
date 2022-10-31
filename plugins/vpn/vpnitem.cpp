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
#include "vpnitem.h"
#include <QPainter>
#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8
#define RADIUS 6.0

#define THEME_QT_SCHEMA  "org.ukui.style"
#define MODE_QT_KEY      "style-name"

VpnItem::VpnItem(bool bAcitve, QWidget *parent)
    : isAcitve(bAcitve), QPushButton(parent)
{
    this->setMinimumSize(550, 58);
    this->setProperty("useButtonPalette", true);
    this->setFlat(true);
    QPalette pal = this->palette();
    QColor color = pal.color(QPalette::Button);
    color.setAlphaF(0.5);
    pal.setColor(QPalette::Button, color);
    this->setPalette(pal);
    QHBoxLayout *mLanLyt = new QHBoxLayout(this);
    mLanLyt->setContentsMargins(16,0,16,0);
    mLanLyt->setSpacing(16);
    iconLabel = new QLabel(this);
    iconLabel->setProperty("useIconHighlightEffect", 0x2);
    titileLabel = new FixLabel(this);
    statusLabel = new QLabel(this);
    statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    infoLabel = new GrayInfoButton(this);

    m_moreButton = new QToolButton(this);
    m_moreButton->setProperty("useButtonPalette", true);
    m_moreButton->setPopupMode(QToolButton::InstantPopup);
    m_moreButton->setAutoRaise(true);
    m_moreButton->setIcon(QIcon::fromTheme("view-more-horizontal-symbolic"));
    m_moreMenu = new QMenu(m_moreButton);
    m_connectAction = new QAction(m_moreMenu);
    m_deleteAction = new QAction(tr("Delete"), m_moreMenu);
    setConnectActionText(isAcitve);

    m_moreMenu->addAction(m_connectAction);
    m_moreMenu->addAction(m_deleteAction);
    m_moreButton->setMenu(m_moreMenu);

    mLanLyt->addWidget(iconLabel);
    mLanLyt->addWidget(titileLabel,Qt::AlignLeft);
    mLanLyt->addStretch();
    mLanLyt->addWidget(statusLabel);
//    mLanLyt->addWidget(infoLabel);
    mLanLyt->addWidget(m_moreButton);

    loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));

    waitTimer = new QTimer(this);
    connect(waitTimer, &QTimer::timeout, this, &VpnItem::updateIcon);

    connect(m_connectAction, &QAction::triggered, this, &VpnItem::onConnectTriggered);
    connect(m_deleteAction, &QAction::triggered, this, &VpnItem::onDeletetTriggered);
    m_moreMenu->installEventFilter(this);
}

void VpnItem::updateIcon()
{
    if (currentIconIndex > 6) {
        currentIconIndex = 0;
    }
    statusLabel->setPixmap(loadIcons.at(currentIconIndex).pixmap(16,16));
    currentIconIndex ++;
}

void VpnItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    loading = true;
}

void VpnItem::stopLoading(){
    waitTimer->stop();
    loading = false;
}

void VpnItem::setConnectActionText(bool isAcitve)
{
    if (isAcitve) {
        m_connectAction->setText(tr("Disconnect"));
    } else {
        m_connectAction->setText(tr("Connect"));
    }
}

void VpnItem::onConnectTriggered()
{
    if (!m_connectAction) {
        return;
    }
    if (m_connectAction->text() == tr("Connect")) {
        Q_EMIT connectActionTriggered();
    } else if (m_connectAction->text() == tr("Disconnect")) {
        Q_EMIT disconnectActionTriggered();
    }
}

void VpnItem::onDeletetTriggered()
{
    if (!m_deleteAction) {
        return;
    }
    Q_EMIT deleteActionTriggered();
}

void VpnItem::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(pal.color(QPalette::Base));

    QRect rect = this->rect();

#if 0
    if (!useHalfFillet) {
        painter.drawRect(rect);
    } else {
        QPainterPath path;
//        path.addRoundedRect (rect, RADIUS, RADIUS);
//        QRect temp_rect(rect.left(), rect.top(), rect.width(), rect.height()/2);
//        path.addRect(temp_rect);
        //设置起点
        path.moveTo(rect.topLeft().x(), rect.topLeft().y());
        path.lineTo(rect.bottomLeft().x(), rect.bottomLeft().y() - RADIUS);
        //绘制圆角 圆弧以外切圆的270度位置为起点，逆时针画圆弧运行90度结束
        path.arcTo(QRect(QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - (RADIUS * 2)), QSize(RADIUS * 2, RADIUS * 2)), 180, 90);
        path.lineTo(rect.bottomRight().x()  - RADIUS, rect.bottomRight().y());
        //画圆弧
        path.arcTo(QRect(QPoint(rect.bottomRight().x() - (RADIUS * 2), rect.bottomRight().y() - (RADIUS * 2)), QSize(RADIUS * 2, RADIUS * 2)), 270, 90);
        path.lineTo(rect.topRight());
        path.lineTo(rect.topLeft());
        painter.drawPath(path);
    }
#endif

    painter.drawRect(rect);
    QPushButton::paintEvent(event);
}

bool VpnItem::eventFilter(QObject *watched, QEvent *event)
{
    //菜单右边界与按钮右边界对齐
    if (event->type() == QEvent::Show && watched == m_moreMenu) {
        int menuXPos = m_moreMenu->pos().x();
        int menuWidth = m_moreMenu->size().width();
        int btnWidth = m_moreButton->size().width();

        QPoint pos = QPoint (menuXPos - menuWidth + btnWidth, m_moreMenu->pos().y());
        m_moreMenu->move(pos);
        return true;
    }
    return false;
}
