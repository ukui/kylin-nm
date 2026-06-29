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

#include "../../backend/dbus-interface/agentutils.h"

#include <QApplication>
#include "kwindowsystem.h"
#include "kwindowsystem_export.h"

#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"
#define  WINDOW_WIDTH  480
#define  MIN_WINDOW_HEIGHT  368
#define  EAPMIN_WINDOW_HEIGHT  524
#define  TLS_WINDOW_HEIGHT  580
#define  LAYOUT_MARGINS  0, 0, 0, 0
#define  TOP_LAYOUT_MARGINS  24, 12, 24, 16
#define  CENTER_LAYOUT_MARGINS  24, 0, 24, 8
#define  BOTTOM_LAYOUT_MARGINS  24, 24, 24, 24
#define  LAYOUT_SPACING  16
#define  LABEL_MIN_WIDTH  146
#define  LABEL_MAX_WIDTH  434
#define  MAX_NAME_LENGTH 32
#define  SCROAREA_WIDTH  480
#define  MEDIUM_WEIGHT_VALUE  57

namespace {

QString fileUrlVariantToLocalPath(const QVariant &v)
{
    QByteArray raw;
    if (v.type() == QVariant::ByteArray) {
        raw = v.toByteArray();
    } else {
        raw = v.toString().toUtf8();
    }
    QString s = QString::fromUtf8(raw).trimmed();
    while (s.endsWith(QLatin1Char('\0'))) {
        s.chop(1);
    }
    if (s.startsWith(QLatin1String("file://"))) {
        s = s.mid(7);
    }
    return s;
}

KyNoEapMethodAuth phase2VariantToPeapInner(const QVariant &v)
{
    if (v.type() == QVariant::Int || v.type() == QVariant::UInt) {
        return static_cast<KyNoEapMethodAuth>(v.toInt());
    }
    const QString s = v.toString().trimmed().toLower();
    if (s == QLatin1String("md5")) {
        return KyAuthMethodMd5;
    }
    if (s == QLatin1String("gtc")) {
        return KyAuthMethodGtc;
    }
    return KyAuthMethodMschapv2;
}

void phase2VariantToTtlsInfo(KyEapMethodTtlsInfo &info, const QVariant &phase2Auth, const QVariant &phase2Autheap)
{
    info.authEapMethod = KyAuthEapMethodUnknown;
    info.authNoEapMethod = KyAuthMethodMschapv2;
    info.authType = AUTH_NO_EAP;

    if (phase2Auth.type() == QVariant::Int || phase2Auth.type() == QVariant::UInt) {
        info.authNoEapMethod = static_cast<KyNoEapMethodAuth>(phase2Auth.toInt());
        info.authType = AUTH_NO_EAP;
    } else {
        const QString s = phase2Auth.toString().trimmed().toLower();
        if (s == QLatin1String("eap-md5") || s == QLatin1String("eap_md5")) {
            info.authType = AUTH_EAP;
            info.authEapMethod = KyAuthEapMethodMd5;
            info.authNoEapMethod = KyAuthMethodUnknown;
        } else if (s == QLatin1String("eap-gtc")) {
            info.authType = AUTH_EAP;
            info.authEapMethod = KyAuthEapMethodGtc;
            info.authNoEapMethod = KyAuthMethodUnknown;
        } else if (s == QLatin1String("eap-mschapv2") || s == QLatin1String("eap_mschapv2")) {
            info.authType = AUTH_EAP;
            info.authEapMethod = KyAuthEapMethodMschapv2;
            info.authNoEapMethod = KyAuthMethodUnknown;
        } else if (!s.isEmpty()) {
            if (s == QLatin1String("pap")) {
                info.authNoEapMethod = KyAuthMethodPap;
            } else if (s == QLatin1String("chap")) {
                info.authNoEapMethod = KyAuthMethodChap;
            } else if (s == QLatin1String("mschap")) {
                info.authNoEapMethod = KyAuthMethodMschap;
            } else if (s == QLatin1String("mschapv2")) {
                info.authNoEapMethod = KyAuthMethodMschapv2;
            } else if (s == QLatin1String("gtc")) {
                info.authNoEapMethod = KyAuthMethodGtc;
            } else if (s == QLatin1String("md5")) {
                info.authNoEapMethod = KyAuthMethodMd5;
            } else if (s == QLatin1String("otp")) {
                info.authNoEapMethod = KyAuthMethodOtp;
            } else if (s == QLatin1String("tls")) {
                info.authNoEapMethod = KyAuthMethodTls;
            }
            info.authType = AUTH_NO_EAP;
        }
    }

    if (phase2Autheap.isValid() && !phase2Autheap.toString().isEmpty()) {
        info.authType = AUTH_EAP;
        const QString a = phase2Autheap.toString().trimmed().toLower();
        if (a.contains(QLatin1String("md5"))) {
            info.authEapMethod = KyAuthEapMethodMd5;
        } else if (a.contains(QLatin1String("gtc"))) {
            info.authEapMethod = KyAuthEapMethodGtc;
        } else {
            info.authEapMethod = KyAuthEapMethodMschapv2;
        }
        info.authNoEapMethod = KyAuthMethodUnknown;
    }
}

KyNoEapMethodAuth phase2VariantToFastInner(const QVariant &v)
{
    if (v.type() == QVariant::Int || v.type() == QVariant::UInt) {
        return static_cast<KyNoEapMethodAuth>(v.toInt());
    }
    const QString s = v.toString().trimmed().toLower();
    if (s == QLatin1String("gtc")) {
        return KyAuthMethodGtc;
    }
    return KyAuthMethodMschapv2;
}

KyFastProvisioning parseFastProvisioning(const QVariant &v, bool *okOut)
{
    bool ok = false;
    const int i = v.toInt(&ok);
    if (okOut) {
        *okOut = ok;
    }
    if (!ok) {
        return KyFastProvisioningUnknown;
    }
    return static_cast<KyFastProvisioning>(i);
}

} // namespace

JoinHiddenWiFiPage::JoinHiddenWiFiPage(QString devName, KDialog *parent)
    :m_devName(devName),
      KDialog(parent)
{
    m_wirelessConnOpration = new KyWirelessConnectOperation(this);

    initUI();
    initComponent();

    setAttribute(Qt::WA_DeleteOnClose);
    KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager);

    setJoinBtnEnable();

    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &JoinHiddenWiFiPage::close);
    m_timeoutTimer->start(120000);  // 120秒 = 120000毫秒
}

JoinHiddenWiFiPage::~JoinHiddenWiFiPage()
{

}

void JoinHiddenWiFiPage::setEnterpriseInfo(const QVariantMap &info)
{
    m_bReConnectNet = true;
    m_reConnectNetSecretSubmitted = false;
    // 1. 设置SSID
    QString ssid = info.value(SSID_KEY).toString();
    m_nameEdit->setText(ssid);

    // 2. 设置安全类型为企业WiFi
    m_secuWidget->setSecurity(KySecuType::WPA_AND_WPA2_ENTERPRISE);

    // 3. 解析EAP类型（NM 为 QStringList，重试信息里也可能是单个字符串）
    QString eapMethod;
    const QVariant eapVar = info.value(EAP_KEY);
    if (eapVar.type() == QVariant::StringList) {
        const QStringList eapList = eapVar.toStringList();
        if (eapList.isEmpty()) {
            qWarning() << "未找到EAP类型";
            return;
        }
        eapMethod = eapList.first().trimmed().toLower();
    } else {
        eapMethod = eapVar.toString().trimmed().toLower();
        if (eapMethod.isEmpty()) {
            qWarning() << "未找到EAP类型";
            return;
        }
    }

    // 4. 根据EAP类型设置对应的子页面并填充字段
    KyEapMethodType eapType = KyEapMethodType::PEAP; // 默认
    if (eapMethod == QLatin1String("peap")) {
        eapType = KyEapMethodType::PEAP;
        m_info.peapInfo.userName = info.value(IDENTITY_KEY).toString();
        m_info.peapInfo.userPWD = info.value(PASSWORD_KEY).toString();
        m_info.peapInfo.phase2AuthMethod = phase2VariantToPeapInner(info.value(QStringLiteral("phase2-auth")));
        m_secuWidget->setPeapInfo(m_info.peapInfo);
    } else if (eapMethod == QLatin1String("ttls")) {
        eapType = KyEapMethodType::TTLS;
        m_info.ttlsInfo.userName = info.value(IDENTITY_KEY).toString();
        m_info.ttlsInfo.userPWD = info.value(PASSWORD_KEY).toString();
        phase2VariantToTtlsInfo(m_info.ttlsInfo,
                                info.value(QStringLiteral("phase2-auth")),
                                info.value(QStringLiteral("phase2-autheap-method")));
        m_secuWidget->setTtlsInfo(m_info.ttlsInfo);
    } else if (eapMethod == QLatin1String("tls")) {
        eapType = KyEapMethodType::TLS;
        m_info.tlsInfo.identity = info.value(IDENTITY_KEY).toString();
        m_info.tlsInfo.clientPrivateKeyPWD = info.value(PRIVATE_KEY).toString();
        m_info.tlsInfo.domain = info.value(DOMAIN_KEY).toString();
        if (m_info.tlsInfo.domain.isEmpty()) {
            m_info.tlsInfo.domain = info.value(QStringLiteral("domain")).toString();
        }
        m_info.tlsInfo.caCertPath = fileUrlVariantToLocalPath(info.value(QStringLiteral("ca-cert")));
        m_info.tlsInfo.clientCertPath = fileUrlVariantToLocalPath(info.value(QStringLiteral("client-cert")));
        m_info.tlsInfo.clientPrivateKey = fileUrlVariantToLocalPath(info.value(QStringLiteral("private-key")));
        m_info.tlsInfo.bNeedCa = m_info.tlsInfo.caCertPath.isEmpty();
        m_secuWidget->setTlsInfo(m_info.tlsInfo);
    } else if (eapMethod == QLatin1String("leap")) {
        eapType = KyEapMethodType::LEAP;
        m_info.leapInfo.m_userName = info.value(IDENTITY_KEY).toString();
        m_info.leapInfo.m_userPwd = info.value(PASSWORD_KEY).toString();
        m_secuWidget->setLeapInfo(m_info.leapInfo);
    } else if (eapMethod == QLatin1String("pwd")) {
        eapType = KyEapMethodType::PWD;
        m_info.pwdInfo.m_userName = info.value(IDENTITY_KEY).toString();
        m_info.pwdInfo.m_userPwd = info.value(PASSWORD_KEY).toString();
        m_secuWidget->setPwdInfo(m_info.pwdInfo);
    } else if (eapMethod == QLatin1String("fast")) {
        eapType = KyEapMethodType::FAST;
        m_info.fastInfo.m_userName = info.value(IDENTITY_KEY).toString();
        m_info.fastInfo.m_userPwd = info.value(PASSWORD_KEY).toString();
        m_info.fastInfo.m_anonIdentity = info.value(QStringLiteral("anonymous-identity")).toString();
        m_info.fastInfo.m_authMethod = phase2VariantToFastInner(info.value(QStringLiteral("phase2-auth")));
        bool provOk = false;
        const KyFastProvisioning prov =
            parseFastProvisioning(info.value(QStringLiteral("phase1-fast-provisioning")), &provOk);
        if (provOk) {
            m_info.fastInfo.m_pacProvisioning = prov;
        } else {
            m_info.fastInfo.m_pacProvisioning = KyFastProvisioningUnknown;
        }
        m_info.fastInfo.m_pacFilePath = fileUrlVariantToLocalPath(info.value(QStringLiteral("pac-file")));
        m_info.fastInfo.m_allowAutoPacFlag =
            provOk && prov != KyFastProvisioningDisabled && prov != KyFastProvisioningUnknown;
        m_secuWidget->setFastInfo(m_info.fastInfo);
    }

    // 5. 更新对话框高度（根据EAP类型调整）
    onEapTypeChanged(eapType);

    // 7. 启用“加入”按钮（如果信息完整）
    setJoinBtnEnable();
    this->setWindowTitle(tr("Connect WIFI"));
    m_descriptionLabel->setLabelText(tr("Please check network config"));
    m_joinBtn->setText(tr("Connect"));
    m_showListBtn->hide();
    m_secuWidget->setAutoConnectVisible(false);
    m_nameEdit->setEnabled(false);
    m_secuWidget->setOnlyPasswordEditable(true);
}

void JoinHiddenWiFiPage::closeEvent(QCloseEvent *event)
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();  // 停止计时器
    }
    if (m_bReConnectNet && !m_reConnectNetSecretSubmitted) {
        Q_EMIT cancelReConnectEnterprise(m_devName);
    }
    Q_EMIT this->hiddenWiFiPageClose(m_devName);
    return QWidget::closeEvent(event);
}

void JoinHiddenWiFiPage::initUI()
{
    m_topWidget = new QWidget(this);
    m_centerWidget = new QWidget(this);
    m_bottomWidget = new QWidget(this);
    m_secuWidget = new SecurityPage(false, this);
    m_secuWidget->setSecurity(KySecuType::WPA_AND_WPA2_PERSONAL);

    m_descriptionLabel = new FixLabel(this);
    m_descriptionLabel->setFixedWidth(LABEL_MAX_WIDTH);
    m_nameLabel = new FixLabel(this);
    m_nameLabel->setFixedWidth(LABEL_MIN_WIDTH);
    m_nameEdit =new LineEdit(this);

    m_bottomDivider = new Divider(false, this);
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

    m_centerWidget->setFixedWidth(SCROAREA_WIDTH);
    m_hiddenWifiScrollArea->setFixedWidth(SCROAREA_WIDTH);
    m_hiddenWifiScrollArea->setWidget(m_centerWidget);
    m_hiddenWifiScrollArea->setWidgetResizable(true);

    //底部按钮
    m_bottomLayout = new QHBoxLayout(m_bottomWidget);
    m_bottomLayout->setContentsMargins(BOTTOM_LAYOUT_MARGINS);
    m_bottomLayout->setSpacing(LAYOUT_SPACING);
    m_bottomLayout->addWidget(m_showListBtn);
    m_bottomLayout->addStretch();
    m_bottomLayout->addWidget(m_cancelBtn);
    m_bottomLayout->addWidget(m_joinBtn);

    //请输入您想要加入的网络信息
   m_descriptionLabel->setLabelText(tr("Please enter the network information"));
   QFont font = m_descriptionLabel->font();
   font.setWeight(MEDIUM_WEIGHT_VALUE);
   m_descriptionLabel->setFont(font);

   m_nameLabel->setLabelText(tr("Network name(SSID)")); //网络名(SSID)
   m_showListBtn->setText(tr("Show Network List")); //显示网络列表
   m_cancelBtn->setText(tr("Cancel"));
   m_joinBtn->setText(tr("Join"));
   m_joinBtn->setProperty("useButtonPalette", false);
   m_joinBtn->setProperty("isImportant", true);
   m_cancelBtn->setProperty("useButtonPalette", true);
   m_cancelBtn->setProperty("isImportant", false);

   QRegExp nameRx("^.{0,32}$");
   QValidator *validator = new QRegExpValidator(nameRx, this);
   m_nameEdit->setValidator(validator);
   m_nameEdit->setPlaceholderText(tr("Required")); //必填

   this->setWindowTitle(tr("Find and Join WLAN"));
   this->setWindowIcon(QIcon::fromTheme("kylin-network"));
   this->setFixedWidth(WINDOW_WIDTH);
   this->setFixedHeight(MIN_WINDOW_HEIGHT);
//   onPaletteChanged();
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

    connect(m_nameEdit, &LineEdit::textChanged, this, &JoinHiddenWiFiPage::resetTimeout);
    connect(m_cancelBtn, &QPushButton::clicked, this, &JoinHiddenWiFiPage::resetTimeout);
    connect(m_joinBtn, &QPushButton::clicked, this, &JoinHiddenWiFiPage::resetTimeout);
    connect(m_showListBtn, &KBorderlessButton::clicked, this, &JoinHiddenWiFiPage::resetTimeout);
    connect(m_secuWidget, &SecurityPage::secuTypeChanged, this, &JoinHiddenWiFiPage::resetTimeout);
    connect(m_secuWidget, &SecurityPage::eapTypeChanged, this, &JoinHiddenWiFiPage::resetTimeout);
    connect(m_secuWidget, &SecurityPage::setSecuPageState, this, &JoinHiddenWiFiPage::resetTimeout);
#if 0
    connect(qApp, &QApplication::paletteChanged, this, &JoinHiddenWiFiPage::onPaletteChanged);

    const QByteArray id(THEME_SCHAME);
    if(QGSettings::isSchemaInstalled(id)){
        QGSettings * fontSetting = new QGSettings(id, QByteArray(), this);
        connect(fontSetting, &QGSettings::changed,[=](QString key) {
            if ("themeColor" == key) {
                onPaletteChanged();
            }
        });
    }
#endif
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

#include <QDebug>
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
            if(m_bReConnectNet){
                Q_EMIT reConnectTlsConnect(m_info.tlsInfo, connSettingInfo);
                m_reConnectNetSecretSubmitted = true;
            }else{
                m_wirelessConnOpration->addAndActiveWirelessEnterPriseTlsConnect(m_info.tlsInfo, connSettingInfo, m_devName, true);
            }
        } else if (eapType == PEAP) {
            m_secuWidget->updatePeapChange(m_info.peapInfo);
            if(m_bReConnectNet){
                Q_EMIT reConnectPeapConnect(m_info.peapInfo, connSettingInfo);
                m_reConnectNetSecretSubmitted = true;
            }else{
                m_wirelessConnOpration->addAndActiveWirelessEnterPrisePeapConnect(m_info.peapInfo, connSettingInfo, m_devName, true);
            }
        } else if (eapType == TTLS) {
            m_secuWidget->updateTtlsChange(m_info.ttlsInfo);
            if(m_bReConnectNet){
                Q_EMIT reConnectTtlsConnect(m_info.ttlsInfo, connSettingInfo);
                m_reConnectNetSecretSubmitted = true;
            }else{
                m_wirelessConnOpration->addAndActiveWirelessEnterPriseTtlsConnect(m_info.ttlsInfo, connSettingInfo, m_devName, true);
            }
        } else if (eapType == LEAP) {
            m_secuWidget->updateLeapChange(m_info.leapInfo);
            if(m_bReConnectNet){
                Q_EMIT reConnectLeapConnect(m_info.leapInfo, connSettingInfo);
                m_reConnectNetSecretSubmitted = true;
            }else{
                m_wirelessConnOpration->addAndActiveWirelessEnterPriseLeapConnect(m_info.leapInfo, connSettingInfo, m_devName, true);
            }
        } else if (eapType == PWD) {
            m_secuWidget->updatePwdChange(m_info.pwdInfo);
            if(m_bReConnectNet){
                Q_EMIT reConnectPwdConnect(m_info.pwdInfo, connSettingInfo);
                m_reConnectNetSecretSubmitted = true;
            }else{
                m_wirelessConnOpration->addAndActiveWirelessEnterPrisePwdConnect(m_info.pwdInfo, connSettingInfo, m_devName, true);
            }
        } else if (eapType == FAST) {
            m_secuWidget->updateFastChange(m_info.fastInfo);
            if(m_bReConnectNet){
                Q_EMIT reConnectFastConnect(m_info.fastInfo, connSettingInfo);
                m_reConnectNetSecretSubmitted = true;
            }else{
                m_wirelessConnOpration->addAndActiveWirelessEnterPriseFastConnect(m_info.fastInfo, connSettingInfo, m_devName, true);
            }
        }
    } else {
        m_secuWidget->updateSecurityChange(connSettingInfo);
        m_wirelessConnOpration->addAndActiveWirelessConnect(m_devName, connSettingInfo, true);
    }

    close();
}

void JoinHiddenWiFiPage::onBtnShowListClicked()
{
    QDBusInterface sidebarIfc("org.ukui.Sidebar",
                              "/org/ukui/Sidebar",
                              "org.ukui.Sidebar",
                              QDBusConnection::sessionBus());
    sidebarIfc.call("shortcutWidgetActive", "org.ukui.shortcut.network", false);

}

void JoinHiddenWiFiPage::onSecuTypeChanged(const KySecuType &type)
{
    if (type != KySecuType::WPA_AND_WPA2_ENTERPRISE) {
        this->setFixedHeight(MIN_WINDOW_HEIGHT);
    }
}

void JoinHiddenWiFiPage::onEapTypeChanged(const KyEapMethodType &type)
{
    if (type == KyEapMethodType::TLS || type == KyEapMethodType::FAST) {
        this->setFixedHeight(TLS_WINDOW_HEIGHT);
    } else {
        this->setFixedHeight(EAPMIN_WINDOW_HEIGHT);
    }
}

void JoinHiddenWiFiPage::resetTimeout()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->start(120000);  // 重置计时器
    }
}

#if 0
void JoinHiddenWiFiPage::onPaletteChanged()
{
    QPalette pal = qApp->palette();

    QGSettings * styleGsettings = nullptr;
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
       styleGsettings = new QGSettings(style_id);
       QString currentTheme = styleGsettings->get(COLOR_THEME).toString();
       if(currentTheme == "ukui-default"){
           pal = lightPalette(this);
       }
    }
    this->setPalette(pal);
    setFramePalette(m_secuWidget, pal);
    setFramePalette(m_hiddenWifiScrollArea, pal);

    if (styleGsettings != nullptr) {
        delete styleGsettings;
        styleGsettings = nullptr;
    }
}
#endif
