/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
#include "addnetbtn.h"
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QPainter>
#include <QPainterPath>

#define RADIUS 6.0

AddNetBtn::AddNetBtn(bool isWlan, QWidget *parent) : QPushButton(parent)
{
    this->setObjectName("this");
    this->setMinimumSize(QSize(580, 60));
    this->setMaximumSize(QSize(16777215, 60));
    this->setProperty("useButtonPalette", true);
    this->setFlat(true);

    QHBoxLayout *addLyt = new QHBoxLayout;

    QLabel *iconLabel = new QLabel(this);
    m_textLabel = new QLabel(this);

    if (isWlan) {
        m_textLabel->setText(tr("Add Others"));
        addLyt->addSpacing(8);
        addLyt->addWidget(m_textLabel);
    } else {
        m_textLabel->setText(tr("Add WiredNetork"));
        QIcon mAddIcon = QIcon::fromTheme("list-add-symbolic");
        iconLabel->setPixmap(mAddIcon.pixmap(mAddIcon.actualSize(QSize(16, 16))));
        iconLabel->setProperty("useIconHighlightEffect", 0x2);
//        iconLabel->setProperty("iconHighlightEffectMode", 1);

        addLyt->addStretch();
        addLyt->addWidget(iconLabel);
        addLyt->addWidget(m_textLabel);
    }

    addLyt->addStretch();
    this->setLayout(addLyt);
}

AddNetBtn::~AddNetBtn()
{

}

void AddNetBtn::enterEvent(QEvent *event){
    Q_EMIT enterWidget();

    QPushButton::enterEvent(event);
}

void AddNetBtn::leaveEvent(QEvent *event){
    Q_EMIT leaveWidget();

    QPushButton::leaveEvent(event);
}

void AddNetBtn::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(this->palette().base().color());

    QRect rect = this->rect();
    QPainterPath path;
    const qreal radius = 6.0;

    if (m_cornerType == All) {
        path.addRoundedRect(rect, radius, radius);
    } else if (m_cornerType == BottomRight) {
        // 只绘制右下两个圆角
        path.moveTo(rect.topLeft());
        path.lineTo(rect.topRight());
        path.lineTo(rect.bottomRight() - QPointF(0, radius));
        path.quadTo(rect.bottomRight(), rect.bottomRight() - QPointF(radius, 0));
        path.lineTo(rect.bottomLeft() + QPointF(radius, 0));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() - QPointF(0, radius));
        path.lineTo(rect.topLeft());
    } else {
        path.addRect(rect);
    }

    painter.fillPath(path, this->palette().base().color());
    QPushButton::paintEvent(event);
}

void AddNetBtn::setUseRoundedCorners(bool useRounded) {
    m_useRoundedCorners = useRounded;
    update();
}

void AddNetBtn::setCornerType(CornerType type) { 
    m_cornerType = type; update(); 
}

void AddNetBtn::setTextLabel(const QString str) {
    m_textLabel->setText(str);
}