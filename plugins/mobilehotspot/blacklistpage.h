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
#ifndef BLACKLISTPAGE_H
#define BLACKLISTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>

#include "titlelabel.h"
#include "blacklistitem.h"

using namespace kdk;

class BlacklistPage : public QWidget
{
    Q_OBJECT
public:
    explicit BlacklistPage(QWidget *parent = nullptr);
    ~BlacklistPage();

    void refreshBlacklist();
    bool setStaIntoBlacklist(QString staMac);

    inline void setInterface(QDBusInterface *settingInterface) {
            m_settingPathInterface = settingInterface;
    }

private:

    QFrame* myLine();

    TitleLabel *m_titleLabel = nullptr;
    QVBoxLayout *m_blacklistLayout = nullptr;
    QMap<QString, QString> m_blacklistMap;

    QDBusInterface  *m_settingPathInterface = nullptr;

    void getBlacklistDevice(QMap<QString, QString> &blacklistMap);
    bool removeStaFromBlacklist(QString staMac);
    void initBlacklistDev();
    void addBlacklistDevFrame(QString staMac, QString staName);
    void clearBlacklistLayout();

private slots:
    void onsetStaIntoBlacklist(QString staMac);
    void onRemoveFromBlacklistBtnClicked(QString staMac);
};

#endif // MOBILEHOTSPOTWIDGET_H
