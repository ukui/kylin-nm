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
#include "klineframe.h"
#include <QPainter>

#define LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_MARGINS 0,0,0,0
ItemFrame::ItemFrame(QString devName, QWidget *parent)
{
    deviceLanLayout = new QVBoxLayout(this);
    deviceLanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    lanItemFrame = new QFrame(this);
    lanItemFrame->setFrameShape(QFrame::Shape::NoFrame);
    lanItemFrame->setContentsMargins(LAYOUT_MARGINS);

    lanItemLayout = new QVBoxLayout(lanItemFrame);
    lanItemLayout->setContentsMargins(LAYOUT_MARGINS);
    lanItemLayout->setSpacing(0);
    addWlanWidget = new AddNetBtn(true, this);
    KDK_EXTEND_ALL_INFO_FORMAT(addWlanWidget, "WlanConnect", "", "add others of wlanconnect");

    deviceLanLayout->setSpacing(0);
    setLayout(deviceLanLayout);
    lanItemFrame->setLayout(lanItemLayout);

    deviceFrame = new DeviceFrame(devName, this);
    deviceLanLayout->addWidget(deviceFrame);
    KHLineFrame *deviceLine = new KHLineFrame(this);
    deviceLanLayout->addWidget(deviceLine);
    deviceLanLayout->addWidget(lanItemFrame);
    KHLineFrame *addWlanLine = new KHLineFrame(this);
    deviceLanLayout->addWidget(addWlanLine);
    deviceLanLayout->addSpacing(0);
    deviceLanLayout->addWidget(addWlanWidget);

    //下拉按钮
    connect(deviceFrame->dropDownLabel, &DrownLabel::labelClicked, this, &ItemFrame::onDrownLabelClicked);
}

ItemFrame::~ItemFrame()
{

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

void ItemFrame::updateSeparators()
{
    // 清除所有现有的分割线
    for (KHLineFrame* line : m_separatorLines) {
        if (line) {
            lanItemLayout->removeWidget(line);
            line->deleteLater();
        }
    }
    m_separatorLines.clear();

    // 重新添加分割线
    int widgetCount = 0;
    for (int i = 0; i < lanItemLayout->count(); ++i) {
        QLayoutItem* item = lanItemLayout->itemAt(i);
        if (item && item->widget() && qobject_cast<WlanItem*>(item->widget())) {
            widgetCount++;
        }
    }

    // 在每个WlanItem之间插入分割线（最后一个item后不加）
    for (int i = 0; i < widgetCount - 1; ++i) {
        KHLineFrame* separator = new KHLineFrame(lanItemFrame); // 设置父对象为lanItemFrame
        m_separatorLines.append(separator);

        // 找到第i个WlanItem的位置
        int insertPosition = -1;
        int currentItemIndex = 0;
        for (int j = 0; j < lanItemLayout->count(); ++j) {
            QLayoutItem* item = lanItemLayout->itemAt(j);
            if (item && item->widget() && qobject_cast<WlanItem*>(item->widget())) {
                if (currentItemIndex == i) {
                    insertPosition = j + 1; // 在WlanItem后面插入
                    break;
                }
                currentItemIndex++;
            }
        }

        if (insertPosition != -1) {
            lanItemLayout->insertWidget(insertPosition, separator);
        }
    }
}

void ItemFrame::filletStyleChange()
{
    if (lanItemLayout->isEmpty()) {
        return;
    }

    // 更新网络项的圆角样式
    int itemIndex = 0;
    int totalItems = 0;

    // 先计算WlanItem的数量
    for (int i = 0; i < lanItemLayout->count(); ++i) {
        QLayoutItem *it = lanItemLayout->itemAt(i);
        if (WlanItem* itemFrame = qobject_cast<WlanItem*>(it->widget())) {
            totalItems++;
        }
    }

    // 设置圆角样式
    int currentItem = 0;
    for (int i = 0; i < lanItemLayout->count(); ++i) {
        QLayoutItem *it = lanItemLayout->itemAt(i);
        if (WlanItem* itemFrame = qobject_cast<WlanItem*>(it->widget())) {
            if (currentItem != totalItems - 1) {
                itemFrame->setHalfFillet(false);
            } else {
                itemFrame->setHalfFillet(true);
            }
            currentItem++;
        }
    }

    // 更新分割线
    updateSeparators();
}
