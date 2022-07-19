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
#include "infobutton.h"
#include <QEvent>
#include <QPainter>
#include <QApplication>

#define BUTTON_SIZE 36,36
#define ICON_SIZE 16,16
#define BACKGROUND_COLOR QColor(0,0,0,0)
#define FOREGROUND_COLOR_NORMAL qApp->palette().text().color()
//#define FOREGROUND_COLOR_HOVER QColor(55,144,250,255)
//#define FOREGROUND_COLOR_PRESS QColor(36,109,212,255)
#define FOREGROUND_COLOR_BRIGHTTEXT qApp->palette().brightText().color()
#define FOREGROUND_COLOR_HIGHLIGHT qApp->palette().highlight().color()
#define OUTER_PATH 8,8,16,16
#define INNER_PATH 9,9,14,14
#define TEXT_POS 14,5,16,16,0

#define BUTTON_SIZE 36,36

InfoButton::InfoButton(QWidget *parent) : QPushButton(parent)
{
    this->setFixedSize(BUTTON_SIZE);
    initUI();
    connect(qApp, &QApplication::paletteChanged, this, &InfoButton::onPaletteChanged);
}

void InfoButton::initUI()
{
    this->setFixedSize(BUTTON_SIZE);
    m_backgroundColor = BACKGROUND_COLOR;
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
}

void InfoButton::onPaletteChanged()
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
    this->repaint();
}

QColor InfoButton::mixColor(const QColor &c1, const QColor &c2, qreal bias)
{
    if (bias <= 0.0) {
         return c1;
     }
     if (bias >= 1.0) {
         return c2;
     }
     if (qIsNaN(bias)) {
         return c1;
     }

     qreal r = mixQreal(c1.redF(),   c2.redF(),   bias);
     qreal g = mixQreal(c1.greenF(), c2.greenF(), bias);
     qreal b = mixQreal(c1.blueF(),  c2.blueF(),  bias);
     qreal a = mixQreal(c1.alphaF(), c2.alphaF(), bias);

     return QColor::fromRgbF(r, g, b, a);
}

void InfoButton::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, m_backgroundColor);
    pal.setColor(QPalette::Text, m_foregroundColor);

    QPainterPath cPath;
    cPath.addRect(0, 0, ICON_SIZE);
    cPath.addEllipse(0, 0, ICON_SIZE);

    QPainterPath outerPath;
    outerPath.addEllipse(OUTER_PATH);

    QPainterPath innerPath;
    innerPath.addEllipse(INNER_PATH);
    outerPath -= innerPath;

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);

    painter.setBrush(pal.color(QPalette::Base));
    painter.drawPath(cPath);

    painter.fillPath(outerPath, pal.color(QPalette::Text));
    painter.setPen(m_foregroundColor);
    QFont font("Noto Sans CJK SC", 11, QFont::Normal, false);
    painter.setFont(font);
    painter.drawText(TEXT_POS, "i");
}

void InfoButton::enterEvent(QEvent *event)
{
//    m_foregroundColor = FOREGROUND_COLOR_HOVER;
    m_foregroundColor = FOREGROUND_COLOR_HIGHLIGHT;
    this->update();
}

void InfoButton::leaveEvent(QEvent *event)
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
    this->update();
}

void InfoButton::mousePressEvent(QMouseEvent *event)
{
//    m_foregroundColor = FOREGROUND_COLOR_PRESS;
    m_foregroundColor = mixColor(FOREGROUND_COLOR_HIGHLIGHT, FOREGROUND_COLOR_BRIGHTTEXT, 0.2);
    this->update();
    return QPushButton::mousePressEvent(event);
}

void InfoButton::mouseReleaseEvent(QMouseEvent *event)
{
//    m_foregroundColor = FOREGROUND_COLOR_HOVER;
    m_foregroundColor = mixColor(FOREGROUND_COLOR_HIGHLIGHT, FOREGROUND_COLOR_BRIGHTTEXT, 0.2);
    this->update();
    return QPushButton::mouseReleaseEvent(event);
}
