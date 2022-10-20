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
#include "singlepage.h"
#include <qsettings.h>
#include <QDBusInterface>
#include <QLabel>
#include <QApplication>
#include <QDBusReply>

SinglePage::SinglePage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

SinglePage::~SinglePage()
{
    delete m_netDivider;
}

void SinglePage::initUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);
    this->setLayout(m_mainLayout);

    m_netFrame = new QFrame(this);
    m_netFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT);
    m_netLayout = new QVBoxLayout(m_netFrame);
    m_netLayout->setContentsMargins(NET_LAYOUT_MARGINS);
    m_netFrame->setLayout(m_netLayout);

    m_netLabel = new QLabel(m_netFrame);
    m_netLabel->setContentsMargins(TEXT_MARGINS);
    m_netLabel->setFixedHeight(TEXT_HEIGHT);

    m_netListArea = new QWidget(m_netFrame);
    m_netAreaLayout = new QVBoxLayout(m_netListArea);
    m_netAreaLayout->setSpacing(MAIN_LAYOUT_SPACING);
    m_netAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);

    m_netLayout->addWidget(m_netLabel);
    m_netLayout->addWidget(m_netListArea);

    m_netDivider = new Divider(this);
    m_settingsFrame = new QFrame(this);
    m_settingsFrame->setFixedHeight(TITLE_FRAME_HEIGHT);

    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);

    m_settingsLabel = new KyLable(m_settingsFrame);
    m_settingsLabel->setCursor(Qt::PointingHandCursor);
    m_settingsLabel->setText(tr("Settings"));
    m_settingsLabel->setScaledContents(true);

    m_settingsLayout->addWidget(m_settingsLabel);
    m_settingsLayout->addStretch();
    m_settingsFrame->setLayout(m_settingsLayout);

    m_mainLayout->addWidget(m_netFrame);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_netDivider);
    m_mainLayout->addWidget(m_settingsFrame);

}

void SinglePage::showDesktopNotify(const QString &message, QString soundName)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QStringList actions;  //跳转动作
    actions.append("kylin-nm");
    actions.append("default");          //默认动作：点击消息体时打开麒麟录音
    QMap<QString, QVariant> hints;
    if (!soundName.isEmpty()) {
        hints.insert("sound-name",soundName); //添加声音
    }
    QList<QVariant> args;
    args<<(tr("Kylin NM"))
       <<((unsigned int) 0)
       <<QString("gnome-dev-ethernet")
       <<tr("kylin network applet desktop message") //显示的是什么类型的信息
       <<message //显示的具体信息
       <<actions
       <<hints
       <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
}

