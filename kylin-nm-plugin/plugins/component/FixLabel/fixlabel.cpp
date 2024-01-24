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

#include "fixlabel.h"
#include <QGSettings>


FixLabel::FixLabel(QWidget *parent) :
    QLabel(parent)
{
    const QByteArray id("org.ukui.style");
    QGSettings * fontSetting = new QGSettings(id, QByteArray(), this);
    connect(fontSetting, &QGSettings::changed,[=](QString key) {
        if ("systemFont" == key || "systemFontSize" ==key) {
            changedLabelSlot();
        }
    });
}


void FixLabel::setLabelText(QString text) {

    mStr = text;
    changedLabelSlot();
}

QString FixLabel::getText(){
    return mStr;
}

void FixLabel::changedLabelSlot() {
    QFontMetrics  fontMetrics(this->font());
    int fontSize = fontMetrics.width(mStr);
    if (fontSize > this->width()) {
        setText(fontMetrics.elidedText(mStr, Qt::ElideRight, this->width()));
        setToolTip(mStr);
    } else {
        setText(mStr);
        setToolTip("");
    }
}

