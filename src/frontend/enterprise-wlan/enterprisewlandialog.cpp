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
#include "enterprisewlandialog.h"
#include <QApplication>
#include <QDesktopWidget>
#include "xatom-helper.h"
#define MAIN_SIZE_EXPAND 480,580
#define MAIN_SIZE_NARROW 480,484
#define PEAP_SCRO_HEIGHT  390
#define TLS_SCRO_HEIGHT  590
#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define CENTER_LAYOUT_MARGINS 24, 16, 24, 8
#define BUTTON_LAYOUT_MARGINS 24, 24, 24, 24
#define MAIN_LAYOUT_SPACING 0
#define BUTTON_SPACING 16
#define LABEL_MIN_WIDTH  146
#define MEDIUM_WEIGHT_VALUE 57

#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

EnterpriseWlanDialog::EnterpriseWlanDialog(KyWirelessNetItem &wirelessNetItem, QString device, QWidget *parent) : QWidget(parent)
{
//    //设置窗口无边框，阴影
//#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
//    MotifWmHints window_hints;
//    window_hints.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
//    window_hints.functions = MWM_FUNC_ALL;
//    window_hints.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), window_hints);
//#else
//    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
//#endif
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlag(Qt::Window);
//    this->setWindowTitle(tr("Connect Enterprise WLAN"));
    this->setWindowIcon(QIcon::fromTheme("kylin-network"));

    m_wirelessNetItem = wirelessNetItem;
    m_deviceName = device;
    m_connectOperation = new KyWirelessConnectOperation();
    m_resource = new KyWirelessNetResource();

    initUI();
    initData();
    centerToScreen();
}

EnterpriseWlanDialog::~EnterpriseWlanDialog() {
    if (m_securityPage) {
        delete m_securityPage;
        m_securityPage = nullptr;
    }
    if (m_connectOperation) {
        delete m_connectOperation;
        m_connectOperation = nullptr;
    }
}

void EnterpriseWlanDialog::closeEvent(QCloseEvent *event)
{
    Q_EMIT this->enterpriseWlanDialogClose(false);
    return QWidget::closeEvent(event);
}

void EnterpriseWlanDialog::paintEvent(QPaintEvent *event)
{
    QPalette pal = qApp->palette();
    QPainter painter(this);
    painter.setBrush(pal.color(QPalette::Base));
    painter.drawRect(this->rect());
    painter.fillRect(rect(), QBrush(pal.color(QPalette::Base)));

    return QWidget::paintEvent(event);
}

void EnterpriseWlanDialog::initUI()
{
    m_mainLayout = new QVBoxLayout(this);
    this->setLayout(m_mainLayout);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);

    m_centerWidget = new QWidget(this);
    QVBoxLayout *centerLayout = new QVBoxLayout(m_centerWidget);
    centerLayout->setContentsMargins(CENTER_LAYOUT_MARGINS);
    centerLayout->setSpacing(MAIN_LAYOUT_SPACING);

    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setText(tr("Wi-Fi network requires authentication")); //Wi-Fi网络要求认证
    QFont font = m_descriptionLabel->font();
    font.setWeight(MEDIUM_WEIGHT_VALUE);
    m_descriptionLabel->setFont(font);
    m_ssidLabel = new QLabel(this);
    QString str = tr("Access to Wi-Fi network \""); //访问Wi-Fi网络
    str.append(m_wirelessNetItem.m_NetSsid);
    str.append(tr("\" requires a password or encryption key.")); //需要密码或加密秘钥
    m_ssidLabel->setText(str);
    m_ssidLabel->setWordWrap(true);
    m_securityPage = new SecurityPage(false, this);
    m_securityPage->setSecurity(KySecuType::WPA_AND_WPA2_ENTERPRISE);
//    m_securityPage->setSecurityVisible(false);

    centerLayout->addWidget(m_descriptionLabel);
    centerLayout->addSpacing(8);
    centerLayout->addWidget(m_ssidLabel);
    centerLayout->addSpacing(BUTTON_SPACING);
    centerLayout->addWidget(m_securityPage);
    centerLayout->addStretch();
    m_enterWlanScrollArea = new QScrollArea(this);
    m_enterWlanScrollArea->setFrameShape(QFrame::NoFrame);
    m_enterWlanScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_enterWlanScrollArea->setWidget(m_centerWidget);
    QPalette pal = m_enterWlanScrollArea->palette();
    pal.setBrush(QPalette::Window, Qt::transparent);
    m_enterWlanScrollArea->setPalette(pal);

    m_bottomDivider = new Divider(this);

    QWidget *bottomWidget = new QWidget(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(bottomWidget);
    btnLayout->setContentsMargins(BUTTON_LAYOUT_MARGINS);
    btnLayout->setSpacing(BUTTON_SPACING);
    m_cancelBtn = new QPushButton(this);
    m_connectBtn = new QPushButton(this);
    m_cancelBtn->setText(tr("Cancel"));
    m_connectBtn->setText(tr("Connect"));
    m_connectBtn->setEnabled(false);
    btnLayout->addStretch();
    btnLayout->addWidget(m_cancelBtn);
    btnLayout->addWidget(m_connectBtn);

    m_mainLayout->addWidget(m_enterWlanScrollArea);
    m_mainLayout->addWidget(m_bottomDivider);
    m_mainLayout->addWidget(bottomWidget);

    this->setFixedSize(MAIN_SIZE_EXPAND);
    this->setWindowTitle(m_wirelessNetItem.m_NetSsid);
    initConnections();
    onPaletteChanged();
}

void EnterpriseWlanDialog::centerToScreen()
{
    QDesktopWidget* m = QApplication::desktop();
    QRect desk_rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    int desk_x = desk_rect.width();
    int desk_y = desk_rect.height();
    int x = this->width();
    int y = this->height();
    this->move(desk_x / 2 - x / 2 + desk_rect.left(), desk_y / 2 - y / 2 + desk_rect.top());
}

void EnterpriseWlanDialog::initConnections()
{
    connect(m_closeBtn, &QPushButton::clicked, this, &EnterpriseWlanDialog::close);
    connect(m_cancelBtn, &QPushButton::clicked, this, &EnterpriseWlanDialog::close);
    connect(m_connectBtn, &QPushButton::clicked, this, &EnterpriseWlanDialog::onBtnConnectClicked);
    connect(m_securityPage, &SecurityPage::eapTypeChanged, this, &EnterpriseWlanDialog::onEapTypeChanged);
    connect(m_securityPage, &SecurityPage::setSecuPageState, this, [ = ](bool status) {
       m_connectBtn->setEnabled(status);
    });

    connect(qApp, &QApplication::paletteChanged, this, &EnterpriseWlanDialog::onPaletteChanged);
}

void EnterpriseWlanDialog::onPaletteChanged()
{
    QPalette pal = qApp->palette();

//    QGSettings * styleGsettings = nullptr;
//    const QByteArray style_id(THEME_SCHAME);
//    if (QGSettings::isSchemaInstalled(style_id)) {
//       styleGsettings = new QGSettings(style_id);
//       QString currentTheme = styleGsettings->get(COLOR_THEME).toString();
//       if(currentTheme == "ukui-default"){
//           pal = lightPalette(this);
//       }
//    }

    this->setPalette(pal);

    setFramePalette(m_securityPage, pal);
}

void EnterpriseWlanDialog::initData()
{
    if (m_wirelessNetItem.m_isConfigured) {
        KyEapMethodType type;
        m_connectOperation->getEnterpiseEapMethod(m_wirelessNetItem.m_connectUuid, type);
        if (type) {
            onEapTypeChanged(type);
        } else {
            qWarning() << "Get eap type failed!" << Q_FUNC_INFO << __LINE__;
        }
    }
}

void EnterpriseWlanDialog::onBtnConnectClicked()
{
    qDebug() << "Clicked connectBtn, will connect enterprise WLAN, ssid = "
             << m_wirelessNetItem.m_NetSsid << "." << Q_FUNC_INFO << __LINE__;
    KySecuType secuType;
    KyEapMethodType eapType;
    m_securityPage->getSecuType(secuType, eapType);

    KyWirelessConnectSetting connetSetting;
    connetSetting.setConnectName(m_wirelessNetItem.m_NetSsid);
    connetSetting.setIfaceName(m_deviceName);
//    connetSetting.isAutoConnect = true; //ZJP_TODO 自动连接选项
    connetSetting.m_type = KyKeyMgmt::WpaEap;
    connetSetting.m_ssid = m_wirelessNetItem.m_NetSsid;
    connetSetting.m_secretFlag = 0;
    connetSetting.dumpInfo();

    if (eapType  == KyEapMethodType::TLS) {
        m_info.tlsInfo.devIfaceName = m_deviceName;
         m_securityPage->updateTlsChange(m_info.tlsInfo);
        m_connectOperation->addAndActiveWirelessEnterPriseTlsConnect(m_info.tlsInfo, connetSetting, m_deviceName, false);
    } else if (eapType == KyEapMethodType::PEAP) {
        m_securityPage->updatePeapChange(m_info.peapInfo);
        m_connectOperation->addAndActiveWirelessEnterPrisePeapConnect(m_info.peapInfo, connetSetting, m_deviceName, false);
    } else if (eapType == KyEapMethodType::TTLS) {
        m_securityPage->updateTtlsChange(m_info.ttlsInfo);
        m_connectOperation->addAndActiveWirelessEnterPriseTtlsConnect(m_info.ttlsInfo, connetSetting, m_deviceName, false);
    } else {
        qWarning() << "Connect enterprise wlan failed!(Unknown eap type)" << Q_FUNC_INFO << __LINE__;
    }
    close();
}

void EnterpriseWlanDialog::onEapTypeChanged(const KyEapMethodType &type)
{
    switch (type) {
    case KyEapMethodType::TLS:
        if (!m_wirelessNetItem.m_connectUuid.isEmpty()) {
            m_resource->getEnterPriseInfoTls(m_wirelessNetItem.m_connectUuid, m_info.tlsInfo);
        }
        this->setFixedSize(MAIN_SIZE_EXPAND);
        m_centerWidget->setFixedHeight(TLS_SCRO_HEIGHT);
        break;
    case KyEapMethodType::PEAP:
        if (m_wirelessNetItem.m_connectUuid.isEmpty()) {
            m_resource->getEnterPriseInfoPeap(m_wirelessNetItem.m_connectUuid, m_info.peapInfo);
            }
        this->setFixedSize(MAIN_SIZE_NARROW);
        m_centerWidget->setFixedHeight(PEAP_SCRO_HEIGHT);
        break;
    case KyEapMethodType::TTLS:
        if (!m_wirelessNetItem.m_connectUuid.isEmpty()) {
            m_resource->getEnterPriseInfoTtls(m_wirelessNetItem.m_connectUuid, m_info.ttlsInfo);
        }
        this->setFixedSize(MAIN_SIZE_NARROW);
        m_centerWidget->setFixedHeight(PEAP_SCRO_HEIGHT);
        break;
    default:
        break;
    }
}
