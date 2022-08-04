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
#include "joinhiddenwifipage.h"

#define  WINDOW_WIDTH  480
#define  MIN_WINDOW_HEIGHT  368
#define  PEAP_WINDOW_HEIGHT  524
#define  TLS_WINDOW_HEIGHT  580
#define  LAYOUT_MARGINS  0, 0, 0, 0
#define  TOP_LAYOUT_MARGINS  24, 12, 24, 16
#define  CENTER_LAYOUT_MARGINS  24, 0, 24, 8
#define  BOTTOM_LAYOUT_MARGINS  24, 24, 24, 24
#define  LAYOUT_SPACING  16
#define  LABEL_MIN_WIDTH  146
#define  MAX_NAME_LENGTH 32
#define  PSK_SCRO_HEIGHT  182
#define  PEAP_SCRO_HEIGHT  340
#define  TLS_SCRO_HEIGHT  560
#define  MEDIUM_WEIGHT_VALUE  57

JoinHiddenWiFiPage::JoinHiddenWiFiPage(QString devName, KDialog *parent)
    :m_devName(devName),
      KDialog(parent)
{
    m_wirelessConnOpration = new KyWirelessConnectOperation(this);

    initUI();
    initComponent();

    setFixedWidth(WINDOW_WIDTH);
    setAttribute(Qt::WA_DeleteOnClose);

    setJoinBtnEnable();
}

JoinHiddenWiFiPage::~JoinHiddenWiFiPage()
{

}

void JoinHiddenWiFiPage::closeEvent(QCloseEvent *event)
{
    emit this->hiddenWiFiPageClose(m_devName);
    return QWidget::closeEvent(event);
}

void JoinHiddenWiFiPage::initUI()
{
    m_topWidget = new QWidget(this);
    m_centerWidget = new QWidget(this);
    m_bottomWidget = new QWidget(this);
    m_secuWidget = new SecurityPage(false, this);
    m_secuWidget->setSecurity(KySecuType::WPA_AND_WPA2_PERSONAL);

    m_descriptionLabel = new QLabel(this);
    m_nameLabel = new QLabel(this);
    m_nameEdit =new LineEdit(this);

    m_bottomDivider = new Divider(this);
    m_showListBtn = new KBorderlessButton(this);
    m_cancelBtn =new QPushButton(this);
    m_joinBtn =new QPushButton(this);

    m_hiddenWifiScrollArea = new QScrollArea(this);
    m_hiddenWifiScrollArea->setFrameShape(QFrame::NoFrame);
    m_hiddenWifiScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QPalette pa = m_hiddenWifiScrollArea->palette();
    pa.setBrush(QPalette::Window, Qt::transparent);
    m_hiddenWifiScrollArea->setPalette(pa);

    m_pageLayout = new QVBoxLayout(this);
    m_pageLayout->setContentsMargins(LAYOUT_MARGINS);
    m_pageLayout->setSpacing(0);
    m_pageLayout->addWidget(m_topWidget);
    m_pageLayout->addWidget(m_hiddenWifiScrollArea);
    m_pageLayout->addWidget(m_bottomDivider);
    m_pageLayout->addWidget(m_bottomWidget);
    this->mainWidget()->setLayout(m_pageLayout);

    m_topLayout = new QHBoxLayout(m_topWidget);
    m_topLayout->setContentsMargins(TOP_LAYOUT_MARGINS);
    m_topLayout->setSpacing(0);
    m_topLayout->addWidget(m_descriptionLabel);
    m_topLayout->addStretch();

    QWidget *ssidWidget = new QWidget(this);
    QHBoxLayout *ssidLayout = new QHBoxLayout(ssidWidget);
    ssidLayout->setContentsMargins(LAYOUT_MARGINS);
    ssidLayout->setSpacing(0);
    m_nameLabel->setMinimumWidth(LABEL_MIN_WIDTH);
    ssidLayout->addWidget(m_nameLabel);
    ssidLayout->addWidget(m_nameEdit);

    m_centerVBoxLayout = new QVBoxLayout(m_centerWidget);
    m_centerVBoxLayout->setContentsMargins(CENTER_LAYOUT_MARGINS);
    m_centerVBoxLayout->setSpacing(0);
    m_centerVBoxLayout->addWidget(ssidWidget);
    m_centerVBoxLayout->addSpacing(LAYOUT_SPACING);
    m_centerVBoxLayout->addWidget(m_secuWidget);
    m_centerVBoxLayout->addStretch();
    m_hiddenWifiScrollArea->setWidget(m_centerWidget);

    //底部按钮
    m_bottomLayout = new QHBoxLayout(m_bottomWidget);
    m_bottomLayout->setContentsMargins(BOTTOM_LAYOUT_MARGINS);
    m_bottomLayout->setSpacing(LAYOUT_SPACING);
    m_bottomLayout->addWidget(m_showListBtn);
    m_bottomLayout->addStretch();
    m_bottomLayout->addWidget(m_cancelBtn);
    m_bottomLayout->addWidget(m_joinBtn);

    //请输入您想要加入网络的名称和安全类型
   m_descriptionLabel->setText(tr("Please enter the network name and security type"));
   QFont font = m_descriptionLabel->font();
   font.setWeight(MEDIUM_WEIGHT_VALUE);
   m_descriptionLabel->setFont(font);
   m_nameLabel->setText(tr("Network name(SSID)")); //网络名(SSID)
   m_showListBtn->setText(tr("Show Network List")); //显示网络列表
   m_cancelBtn->setText(tr("Cancel"));
   m_joinBtn->setText(tr("Join"));

   m_nameEdit->setMaxLength(MAX_NAME_LENGTH);
   m_nameEdit->setPlaceholderText(tr("Required")); //必填

   this->setWindowTitle(tr("Find and Join Wi-Fi"));
   this->setWindowIcon(QIcon::fromTheme("kylin-network"));

   this->setFixedHeight(MIN_WINDOW_HEIGHT);
}

void JoinHiddenWiFiPage::initComponent()
{
    connect(m_cancelBtn, &QPushButton::clicked, this, [=] {
        close();
    });

    connect(m_joinBtn, SIGNAL(clicked()), this, SLOT(onBtnJoinClicked()));
    connect(m_showListBtn, SIGNAL(clicked()), this, SLOT(onBtnShowListClicked()));

    connect(m_secuWidget, &SecurityPage::secuTypeChanged, this, &JoinHiddenWiFiPage::onSecuTypeChanged);
    connect(m_secuWidget, &SecurityPage::eapTypeChanged, this, &JoinHiddenWiFiPage::onEapTypeChanged);
    connect(m_secuWidget, &SecurityPage::setSecuPageState, this, [ = ](bool status) {
       m_isSecuOk = status;
       setJoinBtnEnable();
    });
    connect(m_nameEdit, &LineEdit::textChanged, this, &JoinHiddenWiFiPage::setJoinBtnEnable);
}

void JoinHiddenWiFiPage::setJoinBtnEnable()
{
    if (!m_nameEdit->text().isEmpty() && m_isSecuOk) {
        m_isJoinBtnEnable = true;
    } else {
        m_isJoinBtnEnable = false;
    }
    m_joinBtn->setEnabled(m_isJoinBtnEnable);
}

void JoinHiddenWiFiPage::onBtnJoinClicked()
{
    KyWirelessConnectSetting connSettingInfo;
    //基本信息
    connSettingInfo.m_ssid = m_nameEdit->text();
    connSettingInfo.setConnectName(connSettingInfo.m_ssid);
    connSettingInfo.setIfaceName(m_devName);
    connSettingInfo.m_secretFlag = 0;

    KySecuType secuType;
    KyEapMethodType eapType;
    m_secuWidget->getSecuType(secuType, eapType);

    if (secuType == WPA_AND_WPA2_ENTERPRISE) {
        if (eapType == TLS) {
            m_info.tlsInfo.devIfaceName = m_devName;
            m_secuWidget->updateTlsChange(m_info.tlsInfo);
            m_wirelessConnOpration->addAndActiveWirelessEnterPriseTlsConnect(m_info.tlsInfo, connSettingInfo, m_devName, true);
        } else if (eapType == PEAP) {
            m_secuWidget->updatePeapChange(m_info.peapInfo);
            m_wirelessConnOpration->addAndActiveWirelessEnterPrisePeapConnect(m_info.peapInfo, connSettingInfo, m_devName, true);
        } else if (eapType = TTLS) {
            m_secuWidget->updateTtlsChange(m_info.ttlsInfo);
            m_wirelessConnOpration->addAndActiveWirelessEnterPriseTtlsConnect(m_info.ttlsInfo, connSettingInfo, m_devName, true);
        }
    } else {
        m_secuWidget->updateSecurityChange(connSettingInfo);
        m_wirelessConnOpration->addAndActiveWirelessConnect(m_devName, connSettingInfo, true);
    }

    close();
}

void JoinHiddenWiFiPage::onBtnShowListClicked()
{
    emit showWlanList(1); //WLAN_PAGE_INDEX
}

void JoinHiddenWiFiPage::onSecuTypeChanged(const KySecuType &type)
{
    if (type != KySecuType::WPA_AND_WPA2_ENTERPRISE) {
        this->setFixedHeight(MIN_WINDOW_HEIGHT);
        m_centerWidget->setFixedSize(WINDOW_WIDTH, PSK_SCRO_HEIGHT);
    }
}

void JoinHiddenWiFiPage::onEapTypeChanged(const KyEapMethodType &type)
{
    if (type == KyEapMethodType::TLS) {
        this->setFixedHeight(TLS_WINDOW_HEIGHT);
        m_centerWidget->setFixedSize(WINDOW_WIDTH, TLS_SCRO_HEIGHT);
    } else if (type == KyEapMethodType::PEAP || type == KyEapMethodType::TTLS) {
        this->setFixedHeight(PEAP_WINDOW_HEIGHT);
        m_centerWidget->setFixedSize(WINDOW_WIDTH, PEAP_SCRO_HEIGHT);
    }
}

