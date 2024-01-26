/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
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

#ifndef NETBUTTON_H
#define NETBUTTON_H
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QVariantAnimation>

#define MIDDLE_COLOR 178

class RadioItemButton : public QPushButton
{
    Q_OBJECT

public:
    RadioItemButton(QWidget * parent = nullptr);
//    ~RadioItemButton();
    void setButtonIcon(const QIcon &icon);
    void setDefaultPixmap();
    void setActive(const bool &isActive);
    enum PixmapColor {
        WHITE = 0,
        BLACK,
        GRAY,
        BLUE,
    };
    const QPixmap loadSvg(const QPixmap &source, const PixmapColor &color);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    bool m_isActivated = false;
    QLabel * m_iconLabel = nullptr;
    QColor m_backgroundColor;

    QPixmap m_pixmap;

    void refreshButtonIcon();

private Q_SLOTS:

    void onPaletteChanged();
};

#endif // NETBUTTON_H

