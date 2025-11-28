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
#include "itemframe.h"
#include <kysdk/applications/accessinfohelper.h>
#include <QPainter>
#include <QPalette>

#define LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_MARGINS 0,0,0,0
ItemFrame::ItemFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    deviceLanLayout = new QVBoxLayout(this);
    deviceLanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    lanItemFrame = new QFrame(this);
    lanItemFrame->setFrameShape(QFrame::Shape::NoFrame);

    lanItemLayout = new QVBoxLayout(lanItemFrame);
    lanItemLayout->setContentsMargins(LAYOUT_MARGINS);
    lanItemLayout->setSpacing(0);

    addLanWidget = new AddNetBtn(false, this);
    KDK_EXTEND_ALL_INFO_FORMAT(addLanWidget, "NetConnect", "", "add a wired network");

    deviceLanLayout->setSpacing(0);
    setLayout(deviceLanLayout);
    lanItemFrame->setLayout(lanItemLayout);

    deviceFrame = new DeviceFrame(devName, this);
    deviceLanLayout->addWidget(deviceFrame);
    KHLineFrame *deviceLine = new KHLineFrame(this);
    deviceLanLayout->addWidget(deviceLine);
    deviceLanLayout->addWidget(lanItemFrame);
    KHLineFrame *addLanLine = new KHLineFrame(this);
    deviceLanLayout->addWidget(addLanLine);
    deviceLanLayout->setSpacing(0);
    deviceLanLayout->addWidget(addLanWidget);

    //下拉按钮
    connect(deviceFrame->dropDownLabel, &DrownLabel::labelClicked, this, &ItemFrame::onDrownLabelClicked);
}

void ItemFrame::updateSeparators()
{
    /* 先清除所有现有的分割线 */
    for (KHLineFrame *separator : separators) {
        lanItemLayout->removeWidget(separator);
        separator->deleteLater();
    }
    separators.clear();

    /* 在网络项之间添加分割线 */
    int itemCount = 0;
    for (int i = 0; i < lanItemLayout->count(); ++i) {
        QLayoutItem *item = lanItemLayout->itemAt(i);
        if (item && item->widget() && qobject_cast<LanItem*>(item->widget())) {
            itemCount++;
            /* 如果不是第一个网络项，在前面添加分割线 */
            if (itemCount > 1) {
                KHLineFrame *separator = new KHLineFrame(lanItemFrame);
                lanItemLayout->insertWidget(i, separator);
                separators.append(separator);
                i++; /* 因为插入了一个widget，索引要增加 */
            }
        }
    }
}

void ItemFrame::onDrownLabelClicked()
{
    if (!deviceFrame->dropDownLabel->isChecked) {
        lanItemFrame->show();
        deviceFrame->dropDownLabel->setDropDownStatus(true);
    } else {
        lanItemFrame->hide();
        deviceFrame->dropDownLabel->setDropDownStatus(false);
    }
}
