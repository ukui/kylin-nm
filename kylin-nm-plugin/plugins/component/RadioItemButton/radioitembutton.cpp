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

#include "radioitembutton.h"
#include <QPainter>
//#include <QSvgRenderer>
#include <QApplication>
#include <QStyle>
#include <QDebug>
#include <QPainterPath>
#define BUTTON_SIZE 36,36
#define ICON_SIZE 16,16
#define BACKGROUND_COLOR QColor(0,0,0,0)

RadioItemButton::RadioItemButton(QWidget *parent) : QPushButton(parent)
{
    this->setAutoFillBackground(false);
    m_iconLabel = new QLabel(this);

    this->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setAlignment(Qt::AlignCenter);

    setActive(false);
    //JXJ_TODO loading动画
    connect(qApp, &QApplication::paletteChanged, this, &RadioItemButton::onPaletteChanged);
}


//设置图标
void RadioItemButton::setButtonIcon(const QIcon &icon)
{
    if (icon.isNull()) {
        return;
    }
    m_pixmap = icon.pixmap(ICON_SIZE);
    refreshButtonIcon();
}
//显示默认图标
void RadioItemButton::setDefaultPixmap()
{
    m_iconLabel->setPixmap(m_pixmap);
}
//根据连接状态更改图标颜色
void RadioItemButton::setActive(const bool &isActive)
{
    m_isActivated = isActive;
    refreshButtonIcon();
}


void RadioItemButton::onPaletteChanged()
{
    refreshButtonIcon();
}

void RadioItemButton::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, BACKGROUND_COLOR);
    pal.setColor(QPalette::Text, m_backgroundColor);

    QPainterPath cPath;
    cPath.addRect(0, 0, this->width(), this->height());
    cPath.addEllipse(0, 0, this->width(), this->width());

    QPainterPath innerPath;
    innerPath.addEllipse(0, 0, this->width(), this->width());

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);

    painter.setBrush(pal.color(QPalette::Base));
    painter.drawPath(cPath);

    painter.fillPath(innerPath, pal.color(QPalette::Text));
}

void RadioItemButton::mousePressEvent(QMouseEvent *event)
{
    if (m_isActivated) {
        m_backgroundColor = qApp->palette().highlight().color();
    } else {
        m_backgroundColor = qApp->palette().brightText().color();
        m_backgroundColor.setAlphaF(0.21);
    }
    this->update();
    return QPushButton::mousePressEvent(event);
}

void RadioItemButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isActivated) {
        m_backgroundColor = qApp->palette().highlight().color();
    } else {
        m_backgroundColor = qApp->palette().brightText().color();
        m_backgroundColor.setAlphaF(0.18);
    }
    this->update();
    return QPushButton::mouseReleaseEvent(event);
}

void RadioItemButton::enterEvent(QEvent *event)
{
    if (m_isActivated) {
        m_backgroundColor = qApp->palette().highlight().color();
    } else {
        m_backgroundColor = qApp->palette().brightText().color();
        m_backgroundColor.setAlphaF(0.32);
    }
    this->update();
    return QPushButton::enterEvent(event);
}

void RadioItemButton::leaveEvent(QEvent *event)
{
    if (m_isActivated) {
        m_backgroundColor = qApp->palette().highlight().color();
    } else {
        m_backgroundColor = qApp->palette().brightText().color();
        m_backgroundColor.setAlphaF(0.18);
    }
    this->update();
    return QPushButton::leaveEvent(event);
}

void RadioItemButton::refreshButtonIcon()
{
    if (m_isActivated) {
        m_backgroundColor = qApp->palette().highlight().color();
        m_iconLabel->setPixmap(loadSvg(m_pixmap, PixmapColor::WHITE));
    } else {
        m_backgroundColor = qApp->palette().brightText().color();
        m_backgroundColor.setAlphaF(0.18);
//        if (qApp->palette().base().color().red() > MIDDLE_COLOR) {
            m_iconLabel->setPixmap(m_pixmap);
//        } else {
//            m_iconLabel->setPixmap(loadSvg(m_pixmap, PixmapColor::WHITE));
//        }
    }

    return;
}

const QPixmap RadioItemButton::loadSvg(const QPixmap &source, const PixmapColor &cgColor)
{
    QImage img = source.toImage();
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                switch (cgColor) {
                case PixmapColor::WHITE:
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                    break;
                case PixmapColor::BLACK:
                    color.setRed(0);
                    color.setGreen(0);
                    color.setBlue(0);
                    img.setPixelColor(x, y, color);
                    break;
                case PixmapColor::GRAY:
                    color.setRed(152);
                    color.setGreen(163);
                    color.setBlue(164);
                    img.setPixelColor(x, y, color);
                    break;
                case PixmapColor::BLUE:
                    color.setRed(61);
                    color.setGreen(107);
                    color.setBlue(229);
                    img.setPixelColor(x, y, color);
                    break;
                default:
                    return source;
                    break;
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}
