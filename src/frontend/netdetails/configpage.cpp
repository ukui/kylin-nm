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
#include "configpage.h"

#include <QProcess>

#define VERTICAL_SPACING  24
#define KSC_FIREWALL_PUBLIC  0
#define KSC_FIREWALL_PRIVATE  1

ConfigPage::ConfigPage(QWidget *parent)
{
    initUi();
    initComponent();
}

void ConfigPage::initUi()
{
    m_descriptionLabel = new QLabel(this);
    m_publicButton = new QRadioButton(this);
    m_privateButton = new QRadioButton(this);
    m_publicLabel = new QLabel(this);
    m_privateLabel = new QLabel(this);
    m_congigBtn = new KBorderlessButton(this);

    QWidget *centerWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(centerWidget);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setVerticalSpacing(VERTICAL_SPACING);
    gridLayout->addWidget(m_publicButton, 0, 0, Qt::AlignTop);
    gridLayout->addWidget(m_publicLabel, 0, 1);
    gridLayout->addWidget(m_privateButton, 1, 0, Qt::AlignTop);
    gridLayout->addWidget(m_privateLabel, 1, 1);

    m_vBoxLayout = new QVBoxLayout(this);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->setSpacing(VERTICAL_SPACING);
    m_vBoxLayout->addWidget(m_descriptionLabel);
    m_vBoxLayout->addWidget(centerWidget);
    m_vBoxLayout->addWidget(m_congigBtn);
    m_vBoxLayout->addStretch();

    //网络配置文件类型
    m_descriptionLabel->setText(tr("Network profile type"));
    m_descriptionLabel->setAlignment(Qt::AlignLeft);
    //公用（推荐）无法在网络上发现你的设备。在大多数情况下，在家庭、工作或公共位置连接到网络时使用此功能。
    m_publicLabel->setText(tr("Public(recommended)  Your device can not be discovered on the network. In most cases, "
                               "use this feature when connected to a network at home, work, or a public location."));
    m_publicLabel->setWordWrap(true);
    //专用 可在网络上发现你的设备。如果需要文件共享或使用通过此网络通信的应用，请选择此项。你应该了解并信任网络上的人员和设备。
    m_privateLabel->setText(tr("Private  Your device can be discovered on the network. Select this if you require file "
                                "sharing or use applications that communicate over this network. "
                                "You should know and trust the people and devices on the network."));
    m_privateLabel->setWordWrap(true);
    //配置防火墙和安全设置
    m_congigBtn->setText(tr("Config firewall and security settings"));

    QPalette btnPal;
    QColor btnColor = palette().highlight().color();
    btnPal.setColor(QPalette::ButtonText, btnColor);
    m_congigBtn->setPalette(btnPal);
}

void ConfigPage::initComponent()
{
    connect(m_congigBtn, &KBorderlessButton::clicked, this, &ConfigPage::onConfigButtonClicked);
}

void ConfigPage::setConfigState(int type)
{
    if (type == KSC_FIREWALL_PUBLIC) {
        m_publicButton->setChecked(true);

    } else if (type == KSC_FIREWALL_PRIVATE) {
        m_privateButton->setChecked(true);
    }

}

bool ConfigPage::checkIsChanged(int type)
{
    if (type == KSC_FIREWALL_PUBLIC && m_publicButton->isChecked()) {
        return false;
    } else if (type == KSC_FIREWALL_PRIVATE && m_privateButton->isChecked()) {
        return false;
    } else {
        return true;
    }
}

int ConfigPage::getConfigState()
{
    if (m_publicButton->isChecked()) {
        return KSC_FIREWALL_PUBLIC;
    } else if (m_privateButton->isChecked()) {
        return KSC_FIREWALL_PRIVATE;
    }
}


void ConfigPage::onConfigButtonClicked()
{
    qDebug() << "show ksc defender net protect";

    QProcess process;
    process.startDetached("/usr/sbin/ksc-defender --net-protect");

}
