/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
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

#include "entsecuritywidget.h"

#include <QFileDialog>

#define  LAYOUT_SPACING  16
#define  LABEL_MIN_WIDTH  150
#define  HORIZON_SPACING  8
#define  ROW_MIN_HEIGHT 36

EntSecurityWidget::EntSecurityWidget(bool isLockScreen, QWidget *parent)
    : m_isLockScreen(isLockScreen), QWidget(parent)
{
    initUI();
    initConnect();
}

EntSecurityWidget::~EntSecurityWidget()
{
//    if (m_loginHintDialog != nullptr) {
//        delete m_loginHintDialog;
//    }
}

void EntSecurityWidget::getEnterpriseType(KyEapMethodType &enterpriseType)
{
    enterpriseType = (KyEapMethodType)m_eapTypeCombox->currentData().toInt();
}

void EntSecurityWidget::initUI()
{
    //企业wifi共有
    m_eapTypeLabel = new QLabel(this);
    m_eapTypeCombox = new QComboBox(this);

    //TLS
    m_identityLable = new QLabel(this);
    m_domainLable = new QLabel(this);
    m_caCertPathLabel = new QLabel(this);
    m_caNeedFlagLabel = new QLabel(this);
    m_clientCertPathLabel = new QLabel(this);
    m_clientPrivateKeyLabel = new QLabel(this);
    m_clientPrivateKeyPwdLabel = new QLabel(this);
    m_pwdOptionLabel = new QLabel(this);
    //TLS
    m_identityEdit = new LineEdit(this);
    m_domainEdit = new LineEdit(this);
    m_caCertPathCombox = new QComboBox(this);
    m_caNeedBox = new QCheckBox(this);
    m_clientCertPathCombox = new QComboBox(this);
    m_clientPrivateKeyCombox = new QComboBox(this);
    m_clientPrivateKeyPwdEdit = new KPasswordEdit(this);
    m_pwdOptionCombox = new QComboBox(this);

//    m_loginHintDialog = new LogHintDialog(this->parentWidget());

    //PEAP TTLS共有
    m_eapMethodLabel = new QLabel(this);
    m_userNameLabel = new QLabel(this);
    m_userPwdLabel = new QLabel(this);
    m_eapMethodCombox = new QComboBox(this);
    m_userNameEdit = new LineEdit(this);
    m_userPwdEdit = new KPasswordEdit(this);

    QGridLayout *topLayout = new QGridLayout(this);
    topLayout->setContentsMargins(0, 16, 0, 0);
    topLayout->setColumnMinimumWidth(0, LABEL_MIN_WIDTH);
    topLayout->setVerticalSpacing(LAYOUT_SPACING);
    // EAP认证 Label和选项框 第0行，第0列，第1列
    topLayout->addWidget(m_eapTypeLabel, 0, 0);
    topLayout->addWidget(m_eapTypeCombox, 0, 1);
    //内部认证 Label和选项框 第1行，第0列，第1列
    topLayout->addWidget(m_eapMethodLabel, 1, 0);
    topLayout->addWidget(m_eapMethodCombox, 1, 1);
    //用户名 Label和输入框 第2行，第0列，第1列
    topLayout->addWidget(m_userNameLabel, 2, 0);
    topLayout->addWidget(m_userNameEdit, 2, 1);
    //密码 Label和密码框 第3行，第0列，第1列
    topLayout->addWidget(m_userPwdLabel, 3, 0);
    topLayout->addWidget(m_userPwdEdit, 3, 1);

    // 匿名身份 Label和输入框 第4行，第0列，第1列
    topLayout->addWidget(m_identityLable, 4, 0);
    topLayout->addWidget(m_identityEdit, 4, 1);

    m_tlsWidget = new QWidget(this);
    QGridLayout *bottomLayout = new QGridLayout(m_tlsWidget);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setColumnMinimumWidth(0, LABEL_MIN_WIDTH);
    bottomLayout->setVerticalSpacing(8);
    // 域 Label和输入框 第0行，第0列，第1列
    bottomLayout->addWidget(m_domainLable, 0, 0);
    bottomLayout->addWidget(m_domainEdit, 0, 1);

    // CA证书选项框及CheckBox布局
    QWidget *CaWidget = new QWidget(this);
    QGridLayout *checkLayout = new QGridLayout(CaWidget);
    checkLayout->setContentsMargins(0, 0, 0, 0);
    checkLayout->setVerticalSpacing(0);
    checkLayout->setColumnMinimumWidth(0, 16);
    checkLayout->addWidget(m_caCertPathCombox, 0, 0, 1, 2);
    checkLayout->addWidget(m_caNeedBox, 1, 0);
    checkLayout->addWidget(m_caNeedFlagLabel, 1, 1);

    // CA证书 Label第1行，第0列
    bottomLayout->addWidget(m_caCertPathLabel, 1, 0);
    // CA证书选项框 不需要CA证书复选框 从第6行，第1列开始，占2行1列
    bottomLayout->addWidget(CaWidget, 1, 1, 2, 1);

    // 用户证书 Label和选项框 第3行，第0列，第1列
    bottomLayout->addWidget(m_clientCertPathLabel, 3, 0);
    bottomLayout->addWidget(m_clientCertPathCombox, 3, 1);
    // 用户私钥 Label和选项框 第4行，第0列，第1列
    bottomLayout->addWidget(m_clientPrivateKeyLabel, 4, 0);
    bottomLayout->addWidget(m_clientPrivateKeyCombox, 4, 1);
    // 私钥密码 Label和密码框 第5行，第0列，第1列
    bottomLayout->addWidget(m_clientPrivateKeyPwdLabel, 5, 0);
    bottomLayout->addWidget(m_clientPrivateKeyPwdEdit, 5, 1);
    // 密码选项 Label和选项框 第6行，第0列，第1列
    bottomLayout->addWidget(m_pwdOptionLabel, 6, 0);
    bottomLayout->addWidget(m_pwdOptionCombox, 6, 1);

    topLayout->addWidget(m_tlsWidget, 5, 0, 6,2);

    //企业wifi共有
    m_eapTypeLabel->setText(tr("EAP type"));
    //TLS
    m_identityLable->setText(tr("Identity"));
    m_domainLable->setText(tr("Domain"));
    m_caCertPathLabel->setText(tr("CA certficate"));
    m_caNeedFlagLabel->setText(tr("no need for CA certificate"));
    m_clientCertPathLabel->setText(tr("User certificate"));
    m_clientPrivateKeyLabel->setText(tr("User private key"));
    m_clientPrivateKeyPwdLabel->setText(tr("User key password"));
    m_pwdOptionLabel->setText(tr("Password options"));

    QString hint = tr("Required");
    m_identityEdit->setPlaceholderText(tr("Required")); //必填
    m_clientPrivateKeyPwdEdit->setPlaceholderText(hint);

    //PEAP TTLS共有
    m_eapMethodLabel->setText(tr("Ineer authentication"));
    m_userNameLabel->setText(tr("Usename"));
    m_userPwdLabel->setText(tr("Password"));

    m_userNameEdit->setPlaceholderText(tr("Required"));
    m_userPwdEdit->setPlaceholderText(hint);

    m_eapTypeCombox->addItem("TLS", TLS);
    m_eapTypeCombox->addItem("PEAP", PEAP);
    m_eapTypeCombox->addItem("TTLS", TTLS);
    m_eapTypeCombox->setCurrentIndex(TLS);

    //TLS
    m_caCertPathCombox->addItem(tr("None"), QString(tr("None"))); //无
    if (!m_isLockScreen)
        m_caCertPathCombox->addItem(tr("Choose from file..."), QString(tr("Choose from file..."))); //从文件中选择...
    else
        m_caCertPathCombox->addItem(tr("Please log in to the system first."), QString(tr("Please log in to the system first.")));

    m_clientCertPathCombox->addItem(tr("None"), QString(tr("None"))); //无
    if (!m_isLockScreen)
        m_clientCertPathCombox->addItem(tr("Choose from file..."), QString(tr("Choose from file..."))); //从文件中选择...
    else
        m_clientCertPathCombox->addItem(tr("Please log in to the system first."), QString(tr("Please log in to the system first.")));

    m_clientPrivateKeyCombox->addItem(tr("None"), QString(tr("None"))); //无
    if (!m_isLockScreen)
        m_clientPrivateKeyCombox->addItem(tr("Choose from file..."), QString(tr("Choose from file..."))); //从文件中选择...
    else
        m_clientPrivateKeyCombox->addItem(tr("Please log in to the system first."), QString(tr("Please log in to the system first.")));

    m_pwdOptionCombox->addItem(tr("Store passwords only for this user"), QString(tr("Store passwords only for this user"))); //仅为该用户存储密码
    m_pwdOptionCombox->addItem(tr("Store passwords for all users"), QString(tr("Store passwords for all users"))); //存储所有用户的密码
    m_pwdOptionCombox->addItem(tr("Ask this password every time"), QString(tr("Ask this password every time"))); //每次询问这个密码

    QRegExp rx("^[A-Za-z0-9`~!@#$%^&*()_-+=<>,.\\\/]+$");
    QRegExpValidator *latitude = new QRegExpValidator(rx, this);
    m_clientPrivateKeyPwdEdit->setValidator(latitude);
    m_userPwdEdit->setValidator(latitude);
    //禁用ClearBtn按钮
    m_clientPrivateKeyPwdEdit->setClearButtonEnabled(false);
    m_userPwdEdit->setClearButtonEnabled(false);
    showTls();
}

void EntSecurityWidget::initConnect()
{
    //EAP方式变化
    connect(m_eapTypeCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EntSecurityWidget::onEapTypeComboxIndexChanged);

    connect(m_caNeedBox, &QCheckBox::clicked, this, &EntSecurityWidget::onCaNeedBoxClicked);

    connect(m_caCertPathCombox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &EntSecurityWidget::onCaCertPathComboxIndexChanged);

    connect(m_clientCertPathCombox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &EntSecurityWidget::onClientCertPathComboxIndexChanged);

    connect(m_clientPrivateKeyCombox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &EntSecurityWidget::onClientPrivateKeyComboxIndexChanged);

    connect(m_eapTypeCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(m_identityEdit, &LineEdit::textChanged, this, &EntSecurityWidget::setEnableOfSaveBtn);
    connect(m_caCertPathCombox, SIGNAL(currentTextChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(m_caNeedBox, &QCheckBox::stateChanged, this, &EntSecurityWidget::setEnableOfSaveBtn);
    connect(m_clientCertPathCombox, SIGNAL(currentTextChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(m_clientPrivateKeyCombox, SIGNAL(currentTextChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(m_clientPrivateKeyPwdEdit, &LineEdit::textChanged, this, &EntSecurityWidget::setEnableOfSaveBtn);
    connect(m_eapMethodCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(m_userNameEdit, &LineEdit::textChanged, this, &EntSecurityWidget::setEnableOfSaveBtn);
    connect(m_userPwdEdit, &LineEdit::textChanged, this, &EntSecurityWidget::setEnableOfSaveBtn);

//    connect(m_loginHintDialog, &LogHintDialog::LogHintDialogClosed, this, &EntSecurityWidget::setComboxIndex);
}

void EntSecurityWidget::showTls()
{
    //TLS
    m_identityLable->show();
    m_identityEdit->show();
    m_tlsWidget->show();

    //PEAP TTLS共有
    m_eapMethodLabel->hide();
    m_userNameLabel->hide();
    m_userPwdLabel->hide();

    m_eapMethodCombox->hide();
    m_userNameEdit->hide();
    m_userPwdEdit->hide();
}

void EntSecurityWidget::showPeapOrTtls()
{
    //TLS
    m_identityLable->hide();
    m_identityEdit->hide();
    m_tlsWidget->hide();

    //PEAP TTLS共有
    m_eapMethodLabel->show();
    m_userNameLabel->show();
    m_userPwdLabel->show();

    m_eapMethodCombox->show();
    m_userNameEdit->show();
    m_userPwdEdit->show();
}

bool EntSecurityWidget::checkConnectBtnIsEnabled()
{
    int type = m_eapTypeCombox->currentData().toInt();
    if (type == TLS) {
        return false;
        if (m_identityEdit->text().isEmpty()) {
            qDebug() << "tls identity is empty";
            return false;
        }
        QFile cafile(m_caCertPathCombox->currentText());
        if(!m_caNeedBox->isChecked() && !cafile.exists()) {
            qDebug() << "ca cert filepath " << m_caCertPathCombox->currentText() << " is invalid";
            return false;
        }

        QFile cliCafile(m_clientCertPathCombox->currentText());
        if(!cliCafile.exists()) {
            qDebug() << "client cert filepath " << m_clientCertPathCombox->currentText() << " is invalid";
            return false;
        }

        QFile cliKeyfile(m_clientPrivateKeyCombox->currentText());
        if(!cliKeyfile.exists()) {
            qDebug() << "client private key filepath " << m_clientPrivateKeyCombox->currentText() << " is invalid";
            return false;
        }

        if(m_clientPrivateKeyPwdEdit->text().isEmpty()) {
            qDebug() << "client Private Key password is empty";
            return false;
        }
    } else if (type == PEAP || type == TTLS) {
        if(m_userNameEdit->text().isEmpty() || m_userPwdEdit->text().isEmpty()) {
            qDebug() << "user name or user password is empty";
            return false;
        }
    }
    return true;
}

void EntSecurityWidget::onEapTypeComboxIndexChanged()
{
    qDebug() << "onEapTypeComboxIndexChanged";
    int index = m_eapTypeCombox->currentData().toInt();
    if (index == TLS) {
        showTls();
        Q_EMIT this->eapTypeChanged(TLS);
    } else if (index == PEAP) {
        showPeapOrTtls();
        m_eapMethodCombox->clear();
        m_eapMethodCombox->addItem("MSCHAPv2", MSCHAPV2_PEAP);
        m_eapMethodCombox->addItem("MD5", MD5_PEAP);
        m_eapMethodCombox->addItem("GTC", GTC_PEAP);
        Q_EMIT this->eapTypeChanged(PEAP);
    } else if (index == TTLS) {
        showPeapOrTtls();
        m_eapMethodCombox->clear();
        m_eapMethodCombox->addItem("pap", PAP);
        m_eapMethodCombox->addItem("mschap", MSCHAP);
        m_eapMethodCombox->addItem("mschapv2(eap)", MSCHAPV2_EAP);
        m_eapMethodCombox->addItem("mschapv2", MSCHAPV2);
        m_eapMethodCombox->addItem("chap", CHAP);
        m_eapMethodCombox->addItem("md5(eap)", MD5_EAP);
        m_eapMethodCombox->addItem("gtc(eap)", GTC_EAP);
        Q_EMIT this->eapTypeChanged(TTLS);
    }
}

void EntSecurityWidget::setEnableOfSaveBtn()
{
    Q_EMIT setSecuPageState(checkConnectBtnIsEnabled());
}

void EntSecurityWidget::onCaNeedBoxClicked()
{
    if (m_caNeedBox->isChecked()) {
        m_caCertPathCombox->setItemText(0, tr("None"));
        m_caCertPathCombox->setEnabled(false);
    }  else {
        m_caCertPathCombox->setEnabled(true);
    }
}

void EntSecurityWidget::onCaCertPathComboxIndexChanged(QString str)
{
    if (m_isLockScreen) {
        if (m_caCertPathCombox->isEnabled()) {
//            m_loginHintDialog->show();
        }
    } else {
        if (str.contains("Choose from file...") || str.contains("从文件选择..."))
        {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CA certificate"), "recent:///",
                                                            tr("CA Files (*.pem *.der *.p12 *.crt *.cer *.pfx)"));
            if (!fileName.isNull()) {
                QStringList nameList = fileName.split("/");
                m_caCertPathCombox->blockSignals(true);
                m_caCertPathCombox->setItemText(0, fileName);
                m_caCertPathCombox->setCurrentIndex(0);
                m_caCertPathCombox->blockSignals(false);
            } else {
                m_caCertPathCombox->blockSignals(true);
                m_caCertPathCombox->setItemText(0, tr("None"));
                m_caCertPathCombox->setCurrentIndex(0);
                m_caCertPathCombox->blockSignals(false);
            }
        } else {
            qWarning() << "Choose file is null or unvalible";
        }
    }
}

void EntSecurityWidget::onClientCertPathComboxIndexChanged(QString str)
{
    if (m_isLockScreen) {
        if (m_clientCertPathCombox->isEnabled()) {
//            m_loginHintDialog->show();
        }
    } else {
        if (str.contains("Choose from file...") || str.contains("从文件选择..."))
        {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CA certificate"), "recent:///",
                                                            tr("CA Files (*.pem *.der *.p12 *.crt *.cer *.pfx)"));
            if (!fileName.isNull()) {
                m_clientCertPathCombox->blockSignals(true);
                m_clientCertPathCombox->setItemText(0, fileName);
                m_clientCertPathCombox->setCurrentIndex(0);
                m_clientCertPathCombox->blockSignals(false);
            } else {
                m_clientCertPathCombox->blockSignals(true);
                m_clientCertPathCombox->setItemText(0, tr("None"));
                m_clientCertPathCombox->setCurrentIndex(0);
                m_clientCertPathCombox->blockSignals(false);
            }
        } else {
            qWarning() << "Choose file is null or unvalible";
        }
    }
}

void EntSecurityWidget::onClientPrivateKeyComboxIndexChanged(QString str)
{
    if (m_isLockScreen) {
        if (m_clientPrivateKeyCombox->isEnabled()) {
//            m_loginHintDialog->show();
        }
    } else {
        if (str.contains("Choose from file...") || str.contains("从文件选择..."))
        {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CA certificate"), "recent:///",
                                                            tr("CA Files (*.pem *.der *.p12 *.crt *.cer *.pfx)"));
            if (!fileName.isNull()) {
                QStringList nameList = fileName.split("/");
                m_clientPrivateKeyCombox->blockSignals(true);
                m_clientPrivateKeyCombox->setItemText(0, fileName);
                m_clientPrivateKeyCombox->setCurrentIndex(0);
                m_clientPrivateKeyCombox->blockSignals(false);
            } else {
                m_clientPrivateKeyCombox->blockSignals(true);
                m_clientPrivateKeyCombox->setItemText(0, tr("None"));
                m_clientPrivateKeyCombox->setCurrentIndex(0);
                m_clientPrivateKeyCombox->blockSignals(false);
            }
        } else {
            qWarning() << "Choose file is null or unvalible";
        }
    }
}

void EntSecurityWidget::setComboxIndex()
{
    if (m_caCertPathCombox->isEnabled()) {
        m_caCertPathCombox->setCurrentIndex(0);
    }
    if (m_clientCertPathCombox->isEnabled()) {
        m_clientCertPathCombox->setCurrentIndex(0);
    }
    if (m_clientPrivateKeyCombox->isEnabled()) {
        m_clientPrivateKeyCombox->setCurrentIndex(0);
    }
}

KyEapMethodPeapInfo EntSecurityWidget::assemblePeapInfo()
{
    KyEapMethodPeapInfo info;
    switch (m_eapMethodCombox->currentIndex()) {
    case 0:
        info.phase2AuthMethod = KyAuthMethodMschapv2;
        break;
    case 1:
        info.phase2AuthMethod = KyAuthMethodMd5;
        break;
    case 2:
        info.phase2AuthMethod = KyAuthMethodGtc;
        break;
    default:
        break;
    }
    info.userName = m_userNameEdit->text();
    info.userPWD = m_userPwdEdit->text();
    info.m_passwdFlag = NetworkManager::Setting::None;
//    switch (m_pwdOptionCombox->currentIndex()) {
//        case 0:
//            info.m_passwdFlag = NetworkManager::Setting::AgentOwned;
//            info.userPWD = m_userPwdEdit->text();
//        break;
//        case 1:
//            info.m_passwdFlag = NetworkManager::Setting::None;
//            info.userPWD = m_userPwdEdit->text();
//        break;
//        case 2:
//            info.m_passwdFlag = NetworkManager::Setting::NotSaved;
//        break;
//        default:
//        break;
//    }

    return info;
}
KyEapMethodTtlsInfo EntSecurityWidget::assembleTtlsInfo()
{
    KyEapMethodTtlsInfo info;
    switch (m_eapMethodCombox->currentIndex()) {
    case PAP:
        info.authType = AUTH_NO_EAP;
        info.authNoEapMethod = KyAuthMethodPap;
        break;
    case MSCHAP:
        info.authType = AUTH_NO_EAP;
        info.authNoEapMethod = KyAuthMethodChap;
        break;
    case MSCHAPV2_EAP:
        info.authType = AUTH_EAP;
        info.authEapMethod = KyAuthEapMethodMschapv2;
        break;
    case MSCHAPV2:
        info.authType = AUTH_NO_EAP;
        info.authNoEapMethod = KyAuthMethodMschapv2;
        break;
    case CHAP:
        info.authType = AUTH_NO_EAP;
        info.authNoEapMethod = KyAuthMethodChap;
        break;
    case MD5_EAP:
        info.authType = AUTH_EAP;
        info.authEapMethod = KyAuthEapMethodMd5;
        break;
    case GTC_EAP:
        info.authType = AUTH_EAP;
        info.authEapMethod = KyAuthEapMethodGtc;
        break;
    default:
        break;
    }
    info.userName = m_userNameEdit->text();
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.userPWD = m_userPwdEdit->text();
//    switch (m_pwdOptionCombox->currentIndex()) {
//        case 0:
//            info.m_passwdFlag = NetworkManager::Setting::AgentOwned;
//            info.userPWD = m_userPwdEdit->text();
//        break;
//        case 1:
//            info.m_passwdFlag = NetworkManager::Setting::None;
//            info.userPWD = m_userPwdEdit->text();
//        break;
//        case 2:
//            info.m_passwdFlag = NetworkManager::Setting::NotSaved;
//        break;
//        default:
//        break;
//    }
    return info;
}



#define ICON_SIZE 16,16

LogHintDialog::LogHintDialog(QWidget *parent) : KDialog(parent)
{
    //弹窗 TLS添加本地文件 登录系统提示
    m_iconLabel = new QLabel(this);
    m_contentLabel = new QLabel(this);
    m_confirmBtn = new QPushButton(this);
    m_dialogLayout = new QVBoxLayout(this);
    m_dialogLayout->setContentsMargins(20, 0, 24, 23);
    m_dialogLayout->setSpacing(0);
    this->setFixedSize(420, 168);
    this->closeButton();

    QIcon m_icon = QIcon::fromTheme("dialog-warning");
    m_iconLabel->setPixmap(m_icon.pixmap(ICON_SIZE));
    m_contentLabel->setText(tr("Please log in to the system first."));
    m_confirmBtn->setText(tr("Confirm"));

    QWidget *tipWidget = new QWidget(this);
    QHBoxLayout *tipLayout = new QHBoxLayout(tipWidget);
    tipLayout->addWidget(m_iconLabel);
    tipLayout->addWidget(m_contentLabel);
    tipLayout->addStretch();

    QWidget *btnWidget = new QWidget(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->addStretch();
    btnLayout->addWidget(m_confirmBtn);

    m_dialogLayout->addWidget(tipWidget);
    m_dialogLayout->addStretch();
    m_dialogLayout->addWidget(btnWidget);
    this->mainWidget()->setLayout(m_dialogLayout);

    connect(m_confirmBtn, &QPushButton::clicked, this, [=] {     
        Q_EMIT LogHintDialogClosed();
        close();
    });

    connect(this->closeButton(), &QPushButton::clicked, this, [=] {
        Q_EMIT LogHintDialogClosed();
        close();
    });
}

