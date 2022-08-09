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
#include "firewalldialog.h"

#define ICON_SIZE 16,16

FirewallDialog::FirewallDialog(KDialog *parent)
{
    initUI();
    this->setWindowIcon(QIcon::fromTheme("kylin-network"));
    this->setFixedSize(480, 204);
    setAttribute(Qt::WA_DeleteOnClose);
}

FirewallDialog::~FirewallDialog()
{

}

void FirewallDialog::initUI()
{
    m_iconLabel = new QLabel(this);
    m_contentLabel = new QLabel(this);
    m_suggestLabel = new QLabel(this);
    m_YesBtn = new QPushButton(this);
    m_NoBtn = new QPushButton(this);
    m_dialogLayout = new QVBoxLayout(this);

    QWidget *contentWidget = new QWidget(this);
    QGridLayout *contentLayout = new QGridLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(m_iconLabel, 0, 0, Qt::AlignVCenter);
    contentLayout->addWidget(m_contentLabel, 0, 1);
    contentLayout->addWidget(m_suggestLabel, 1, 1);
    m_iconLabel->setFixedWidth(16);

    QWidget *btnWidget = new QWidget(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(16);
    btnLayout->addStretch();
    btnLayout->addWidget(m_YesBtn);
    btnLayout->addWidget(m_NoBtn);

    m_dialogLayout->setContentsMargins(24, 16, 24, 24);
    m_dialogLayout->setSpacing(0);
    m_dialogLayout->addWidget(contentWidget);
    m_dialogLayout->addStretch();
    m_dialogLayout->addWidget(btnWidget);

    QIcon icon = QIcon::fromTheme("dialog-info");
    m_iconLabel->setPixmap(icon.pixmap(ICON_SIZE));

    QFont font = m_contentLabel->font();
    font.setWeight(57);
    m_contentLabel->setFont(font);
    //是否允许此网络上的其他设备发现这台电脑？
    m_contentLabel->setText(tr("Allow other devices on this network to discover this computer?"));
    m_contentLabel->setWordWrap(true);
    //不建议在公共网络上开启此功能
    m_suggestLabel->setText(tr("It is not recommended to enable this feature on public networks"));
    m_suggestLabel->setWordWrap(true);

    m_YesBtn->setText(tr("Not allowed (recommended)"));
    m_NoBtn->setText(tr("Allowed"));

    this->closeButton();
    this->mainWidget()->setLayout(m_dialogLayout);

    connect(m_YesBtn, &QPushButton::clicked, this, &FirewallDialog::setPrivateNetMode);
    connect(m_NoBtn, &QPushButton::clicked, this, &FirewallDialog::setPublicNetMode);
}
