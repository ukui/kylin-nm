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
#include "deviceframe.h"
#include <QPainter>
#include <QPainterPath>

#define LAYOUT_MARGINS 18,0,8,0
#define FRAME_HEIGHT 58
#define LAYOUT_SPACING 16
#define RADIUS 6.0

DeviceFrame::DeviceFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    this->setFrameShape(QFrame::Box);
    this->setFixedHeight(FRAME_HEIGHT);
    QHBoxLayout *deviceLayout = new QHBoxLayout(this);
    deviceLayout->setContentsMargins(LAYOUT_MARGINS);
    setLayout(deviceLayout);
    deviceLayout->setSpacing(LAYOUT_SPACING);

    deviceLabel = new QLabel(this);
    dropDownLabel = new DrownLabel(devName, this);

    deviceLayout->addWidget(deviceLabel);
    deviceLayout->addStretch();
    deviceLayout->addWidget(dropDownLabel);
}

DeviceFrame::~DeviceFrame()
{

}

void DeviceFrame::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    QRect rect = this->rect();
    const int radius = RADIUS;
    
    // 创建单一路径实现上圆角下直角
    QPainterPath path;
    path.moveTo(rect.topLeft() + QPointF(0, radius));
    path.arcTo(rect.left(), rect.top(), radius * 2, radius * 2, 180, -90);
    path.lineTo(rect.right() - radius, rect.top());
    path.arcTo(rect.right() - radius * 2, rect.top(), radius * 2, radius * 2, 90, -90);
    path.lineTo(rect.right(), rect.bottom());
    path.lineTo(rect.left(), rect.bottom());
    path.lineTo(rect.left(), rect.top() + radius);
    
    // 绘制背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(pal.color(QPalette::Base));
    painter.drawPath(path);
    
    // 保持基类绘制
    QFrame::paintEvent(event);
}
