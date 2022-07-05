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
#include "blacklistpage.h"
#include <QDebug>

#define CONTENTS_MARGINS 0, 0, 0, 0
#define FRAME_MIN_SIZE 550, 60
#define FRAME_MAX_SIZE 16777215, 16777215
#define LINE_MAX_SIZE 16777215, 1
#define LINE_MIN_SIZE 0, 1

BlacklistPage::BlacklistPage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *Vlayout = new QVBoxLayout(this);
    Vlayout->setContentsMargins(CONTENTS_MARGINS);
    Vlayout->setSpacing(0);

    QFrame *blacklistFrame = new QFrame(this);
    blacklistFrame->setMinimumSize(FRAME_MIN_SIZE);
    blacklistFrame->setMaximumSize(FRAME_MAX_SIZE);
    blacklistFrame->setFrameShape(QFrame::Box);

    m_blacklistLayout = new QVBoxLayout(blacklistFrame);
    m_blacklistLayout->setContentsMargins(0, 0, 0, 0);
    m_blacklistLayout->setSpacing(0);

    m_titleLabel = new TitleLabel(this);
    m_titleLabel->setText(tr("Blacklist"));

    Vlayout->addWidget(m_titleLabel);
    Vlayout->addSpacing(8);
    Vlayout->addWidget(blacklistFrame);
}

QFrame* BlacklistPage::myLine()
{
    QFrame *line = new QFrame(this);
    line->setMinimumSize(QSize(LINE_MIN_SIZE));
    line->setMaximumSize(QSize(LINE_MAX_SIZE));
    line->setLineWidth(0);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    return line;
}

void BlacklistPage::getBlacklistDevice(QMap<QString, QString> &blacklistMap)
{
    if (m_settingPathInterface == nullptr || !m_settingPathInterface->isValid()) {
        qDebug() << "dbus interface m_settingPathInterface is invaild";
        return ;
    }

    QDBusMessage reply = m_settingPathInterface->call("Getblacklist");
    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "[mobilehotspot]Getstainfo error:" << reply.errorMessage();
        return;
    }
    if (reply.arguments().isEmpty()
        || reply.arguments().at(0).toString() == ""
        || reply.arguments().at(1).toString() == "") {
        qDebug() << "Dbus interface call Getblacklist return is empty!";
        return ;
    }

    QStringList macList = reply.arguments().at(0).toString().split(";");
    QStringList hostNameList = reply.arguments().at(1).toString().split(";");
    for (int index = 0; index < macList.count() && macList.at(index) != nullptr; index ++) {
        QString macTemp = macList.at(index);
        macTemp = macTemp.trimmed();
        if (!blacklistMap.contains(macTemp)) {
            blacklistMap[macTemp] = hostNameList.at(index);
        }
    }
}

void BlacklistPage::initBlacklistDev()
{
    QMap<QString, QString>::const_iterator item = m_blacklistMap.cbegin();
    while (item != m_blacklistMap.cend()) {
        addBlacklistDevFrame(item.key(), item.value());
        item ++;
        if (item != m_blacklistMap.cend()) {
            m_blacklistLayout->addWidget(myLine());
        }
    }
}

void BlacklistPage::onsetStaIntoBlacklist(QString staMac)
{
    if (m_settingPathInterface == nullptr || !m_settingPathInterface->isValid()) {
        qDebug() << "dbus interface m_settingPathInterface is invaild";
        return ;
    }

    QDBusMessage reply = m_settingPathInterface->call("Addblacklist", staMac);
    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "[mobilehotspot]Getstainfo error:" << reply.errorMessage();
        return ;
    }

    refreshBlacklist();
    return ;
}

void BlacklistPage::addBlacklistDevFrame(QString staMac, QString staName)
{
    BlacklistItem *itemFrame = new BlacklistItem(staMac, staName, m_blacklistLayout->widget());
    m_blacklistLayout->addWidget(itemFrame);
    connect(itemFrame, &BlacklistItem::onBtnClicked, this, &BlacklistPage::onRemoveFromBlacklistBtnClicked);
}

void BlacklistPage::clearBlacklistLayout()
{
    m_blacklistMap.clear();
    if (m_blacklistLayout->layout() != NULL) {
        QLayoutItem* layoutItem;
        while ((layoutItem = m_blacklistLayout->layout()->takeAt(0)) != NULL) {
            delete layoutItem->widget();
            delete layoutItem;
        }
    }
}

bool BlacklistPage::removeStaFromBlacklist(QString staMac)
{
    if (m_settingPathInterface == nullptr || !m_settingPathInterface->isValid()) {
        qDebug() << "dbus interface m_settingPathInterface is invaild";
        return false;
    }

    QDBusMessage reply = m_settingPathInterface->call("Delblacklist", staMac);
    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "[mobilehotspot]Getstainfo error:" << reply.errorMessage();
        return false;
    }

    return true;
}

void BlacklistPage::refreshBlacklist()
{
    clearBlacklistLayout();
    getBlacklistDevice(m_blacklistMap);
    initBlacklistDev();

    int height = 0;
    for (int i = 0; i < m_blacklistLayout->count(); i ++) {
        QWidget *w = m_blacklistLayout->itemAt(i)->widget();
        if (w != nullptr) {
            height += w->height();
        }
    }
    this->setFixedHeight(height + m_titleLabel->height() + 8);

    if (!m_blacklistMap.isEmpty()) {
        this->show();
    } else {
        this->hide();
    }
    this->update();
}

void BlacklistPage::onRemoveFromBlacklistBtnClicked(QString staMac)
{
    if (staMac.isNull() || staMac.isEmpty()) {
        return;
    }
    removeStaFromBlacklist(staMac);
    refreshBlacklist();
}

