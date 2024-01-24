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

#include "enterprisewlanpage.h"

#include <QPainter>

#define  MAIN_SIZE_EXPAND  480,550
#define  MAIN_SIZE_NARROW  480,340
#define  PAGE_LAYOUT_MARGINS  24,0,24,0
#define  TOP_LAYOUT_MARGINS  0,14,0,0
#define  BOTTOM_LAYOUT_MARGINS  0,24,0,24
#define  LABEL_MIN_WIDTH  150

EnterpriseWlanPage::EnterpriseWlanPage(QString ssid, QString device, bool isLockScreen, QWidget *parent)
    : m_ssid(ssid), m_deviceName(device), QWidget(parent)
{
    initUI();
    initConnections();

    setAttribute(Qt::WA_DeleteOnClose);

    if (isLockScreen) {
        setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
        setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
        setWindowFlags(Qt::Popup);
    }

    centerToScreen();
}

void EnterpriseWlanPage::paintEvent(QPaintEvent *event)
{
    QPalette pal = qApp->palette();
    QColor colorPal = pal.color(QPalette::Background);

    //设置窗体为圆角
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    painter.setBrush(colorPal);
    painter.setPen(Qt::transparent);
    auto rect = this->rect();
    painter.drawRoundedRect(rect, 12, 12);      //窗口圆角

    return QWidget::paintEvent(event);
}

void EnterpriseWlanPage::initUI()
{
    m_ssidTitleLabel = new QLabel(this);
    m_ssidTitleLabel->setMinimumWidth(LABEL_MIN_WIDTH);
    m_ssidLabel = new QLabel(this);
    m_entSecurityWidget = new EntSecurityWidget(this);
    m_cancelBtn = new QPushButton(this);
    m_connectBtn = new QPushButton(this);
    m_connectBtn->setEnabled(false);
    m_ssidWidget = new QWidget(this);
    m_btnWidget = new QWidget(this);

    m_mainLayout = new QVBoxLayout(this);
    this->setLayout(m_mainLayout);
    m_mainLayout->setContentsMargins(PAGE_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_ssidWidget);
    m_mainLayout->addWidget(m_entSecurityWidget);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_btnWidget);

    QHBoxLayout *ssidLayout = new QHBoxLayout(m_ssidWidget);
    ssidLayout->setContentsMargins(TOP_LAYOUT_MARGINS);
    ssidLayout->addWidget(m_ssidTitleLabel);
    ssidLayout->addWidget(m_ssidLabel);
    ssidLayout->addStretch();

    QHBoxLayout *btnLayout = new QHBoxLayout(m_btnWidget);
    btnLayout->setContentsMargins(BOTTOM_LAYOUT_MARGINS);
    btnLayout->setSpacing(16);
    btnLayout->addStretch();
    btnLayout->addWidget(m_cancelBtn);
    btnLayout->addWidget(m_connectBtn);

    m_ssidTitleLabel->setText(tr("Network name(SSID)")); //网络名(SSID)
    m_ssidLabel->setText(m_ssid);
    m_cancelBtn->setText(tr("Cancel"));
    m_connectBtn->setText(tr("Connect"));

    this->setWindowTitle(tr("Connect Enterprise WLAN"));
    this->setWindowIcon(QIcon::fromTheme("kylin-network"));
    this->setFixedSize(MAIN_SIZE_EXPAND);
}

void EnterpriseWlanPage::initConnections()
{
    connect(m_cancelBtn, &QPushButton::clicked, this, &EnterpriseWlanPage::close);
    connect(m_connectBtn, &QPushButton::clicked, this, &EnterpriseWlanPage::onBtnConnectClicked);
    connect(m_entSecurityWidget, &EntSecurityWidget::eapTypeChanged, this, &EnterpriseWlanPage::onEapTypeChanged);
    connect(m_entSecurityWidget, &EntSecurityWidget::setSecuPageState, this, [ = ](bool status) {
       m_connectBtn->setEnabled(status);
    });

    connect(m_entSecurityWidget, &EntSecurityWidget::setSecuPageState, this, [=](bool status) {
       m_connectBtn->setEnabled(status);
    });
}

void EnterpriseWlanPage::centerToScreen()
{
    QDesktopWidget* m = QApplication::desktop();
    QRect desk_rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    int desk_x = desk_rect.width();
    int desk_y = desk_rect.height();
    int x = this->width();
    int y = this->height();
    this->move((desk_x - x)/ 2 , (desk_y - y)/ 2);
}

void EnterpriseWlanPage::onBtnConnectClicked()
{
    KyWirelessConnectSetting connSettingInfo;
    //基本信息
    connSettingInfo.m_ssid = m_ssid;
    connSettingInfo.setConnectName(m_ssid);
    connSettingInfo.setIfaceName(m_deviceName);
    connSettingInfo.m_isAutoConnect = true;

    //ipv4 ipv6
    connSettingInfo.setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
    connSettingInfo.setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);

    KyEapMethodType eapType;
    m_entSecurityWidget->getEnterpriseType(eapType);
    if (eapType == PEAP) {
        KyEapMethodPeapInfo info = m_entSecurityWidget->assemblePeapInfo();
        Q_EMIT connectPeapConnect(info, connSettingInfo);
    } else if (eapType = TTLS) {
        KyEapMethodTtlsInfo info = m_entSecurityWidget->assembleTtlsInfo();
        Q_EMIT connectTtlsConnect(info, connSettingInfo);
    } else {
        qWarning() << "unsupport now!!!";
    }

    close();
}

void EnterpriseWlanPage::onEapTypeChanged(const KyEapMethodType &type)
{
    switch (type) {
    case KyEapMethodType::TLS:
        this->setFixedSize(MAIN_SIZE_EXPAND);
        centerToScreen();
        break;
    case KyEapMethodType::PEAP:
        this->setFixedSize(MAIN_SIZE_NARROW);
        centerToScreen();
        break;
    case KyEapMethodType::TTLS:
        this->setFixedSize(MAIN_SIZE_NARROW);
        centerToScreen();
        break;
    default:
        break;
    }
}
