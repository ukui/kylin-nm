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
    m_deviceSeparator = new KHLineFrame(this);
    deviceLanLayout->addWidget(m_deviceSeparator);

    deviceLanLayout->addWidget(lanItemFrame);

    m_addSeparator = new KHLineFrame(this);
    m_addSeparator->setVisible(false);
    deviceLanLayout->setSpacing(0);
    deviceLanLayout->addWidget(m_addSeparator);
    deviceLanLayout->addWidget(addLanWidget);

    //下拉按钮
    connect(deviceFrame->dropDownLabel, &DrownLabel::labelClicked, this, &ItemFrame::onDrownLabelClicked);
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
    updateCornerStyle();
    filletStyleChange();
}

void ItemFrame::updateCornerStyle()
{
    bool hasItems = (lanItemLayout->count() > 0);
    bool deviceEnabled = true;
    if (deviceFrame && deviceFrame->deviceSwitch) {
        deviceEnabled = deviceFrame->deviceSwitch->isChecked();
    }

    if (!deviceEnabled) {
        if (m_deviceSeparator) {
            m_deviceSeparator->hide();
        }
        if (m_addSeparator) {
            m_addSeparator->hide();
        }
        for (int i = 0; i < lanItemLayout->count(); ++i) {
            QLayoutItem *it = lanItemLayout->itemAt(i);
            LanItem *item = qobject_cast<LanItem *>(it ? it->widget() : nullptr);
            if (!item) {
                continue;
            }
            item->setTopSeparatorVisible(false);
            item->setBottomSeparatorVisible(false);
        }
        addLanWidget->setCornerType(AddNetBtn::All);
        return;
    }

    if (hasItems) {
        if (m_deviceSeparator) {
            m_deviceSeparator->hide();
        }
        if (m_addSeparator) {
            m_addSeparator->setVisible(true);
        }
        addLanWidget->setCornerType(AddNetBtn::BottomRight);
    } else {
        if (m_deviceSeparator) {
            m_deviceSeparator->show();
        }
        if (m_addSeparator) {
            m_addSeparator->setVisible(false);
        }
        addLanWidget->setCornerType(AddNetBtn::All);
    }

    int cnt = lanItemLayout->count();
    for (int i = 0; i < cnt; ++i) {
        QLayoutItem *it = lanItemLayout->itemAt(i);
        LanItem *item = qobject_cast<LanItem *>(it ? it->widget() : nullptr);
        if (!item) {
            continue;
        }
        item->setTopSeparatorVisible(i == 0 ? hasItems : false);
        item->setBottomSeparatorVisible(i < cnt - 1);
    }
}

void ItemFrame::filletStyleChange()
{
    int cnt = lanItemLayout->count();
    for (int i = 0; i < cnt; ++i) {
        QLayoutItem *it = lanItemLayout->itemAt(i);
        LanItem *item = qobject_cast<LanItem *>(it ? it->widget() : nullptr);
        if (!item) {
            continue;
        }
        item->setBottomSeparatorVisible(i < cnt - 1);
    }
}