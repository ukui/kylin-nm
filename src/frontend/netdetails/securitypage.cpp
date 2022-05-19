#include "securitypage.h"
#include "netdetail.h"

#include <QFileDialog>

SecurityPage::SecurityPage(QWidget *parent) : QFrame(parent)
{
    initUI();
    initConnect();
}

void SecurityPage::initUI()
{
    secuTypeLabel = new QLabel(this);
    pwdLabel = new QLabel(this);
    secuTypeLabel = new QLabel(this);
    pwdLabel = new QLabel(this);
    //企业wifi共有
    eapTypeLabel = new QLabel(this);
    //TLS
    identityLable = new QLabel(this);
    domainLable = new QLabel(this);
    caCertPathLabel = new QLabel(this);
    caNeedFlagLabel = new QLabel(this);
    clientCertPathLabel = new QLabel(this);
    clientPrivateKeyLabel = new QLabel(this);
    clientPrivateKeyPwdLabel = new QLabel(this);

    //PEAP TTLS共有
    eapMethodLabel = new QLabel(this);
    userNameLabel = new QLabel(this);
    userPwdLabel = new QLabel(this);
    userPwdFlagLabel = new QLabel(this);

    secuTypeCombox = new QComboBox(this);
    pwdEdit = new KPasswordEdit(this);
    eapTypeCombox = new QComboBox(this);
    //TLS
    identityEdit = new LineEdit(this);
    domainEdit = new LineEdit(this);
    caCertPathCombox = new QComboBox(this);
    caNeedBox = new QCheckBox(this);
    clientCertPathCombox = new QComboBox(this);
    clientPrivateKeyCombox = new QComboBox(this);
    clientPrivateKeyPwdEdit = new KPasswordEdit(this);

    //PEAP && TTLS
    eapMethodCombox = new QComboBox(this);
    userNameEdit = new LineEdit(this);
    userPwdEdit = new KPasswordEdit(this);
    userPwdFlagBox = new QCheckBox(this);


    mSecuLayout = new QFormLayout(this);
    mSecuLayout->addRow(secuTypeLabel, secuTypeCombox);
    mSecuLayout->addRow(pwdLabel, pwdEdit);
    mSecuLayout->addRow(eapTypeLabel, eapTypeCombox);
    mSecuLayout->addRow(identityLable, identityEdit);
    mSecuLayout->addRow(domainLable, domainEdit);
    mSecuLayout->addRow(caCertPathLabel, caCertPathCombox);
    mSecuLayout->addRow(caNeedBox, caNeedFlagLabel);
    mSecuLayout->addRow(clientCertPathLabel, clientCertPathCombox);
    mSecuLayout->addRow(clientPrivateKeyLabel, clientPrivateKeyCombox);
    mSecuLayout->addRow(clientPrivateKeyPwdLabel,clientPrivateKeyPwdEdit);
    mSecuLayout->addRow(eapMethodLabel, eapMethodCombox);
    mSecuLayout->addRow(userNameLabel, userNameEdit);
    mSecuLayout->addRow(userPwdLabel, userPwdEdit);
    mSecuLayout->addRow(userPwdFlagBox, userPwdFlagLabel);


    secuTypeLabel->setText(tr("Security"));
    pwdLabel->setText(tr("Password"));
    //企业wifi共有
    eapTypeLabel->setText(tr("EAP type"));
    //TLS
    identityLable->setText(tr("Identity"));
    domainLable->setText(tr("Domain"));
    caCertPathLabel->setText(tr("CA certficate"));
    caNeedFlagLabel->setText(tr("no need for CA certificate"));
    clientCertPathLabel->setText(tr("User certificate"));
    clientPrivateKeyLabel->setText(tr("User private key"));
    clientPrivateKeyPwdLabel->setText(tr("User key password"));

    //PEAP TTLS共有
    eapMethodLabel->setText(tr("Ineer authentication"));
    userNameLabel->setText(tr("Usename"));
    userPwdLabel->setText(tr("Password"));
    userPwdFlagLabel->setText(tr("Ask pwd each query"));

    secuTypeCombox->addItem(tr("None"),NONE);
    secuTypeCombox->addItem(tr("WPA&WPA2 Personal"),WPA_AND_WPA2_PERSONAL);
    secuTypeCombox->addItem(tr("WPA&WPA2 Enterprise"), WPA_AND_WPA2_ENTERPRISE);
    secuTypeCombox->addItem(tr("WPA3 Personal"), WPA3_PERSONAL);

    eapTypeCombox->addItem("TLS", TLS);
    eapTypeCombox->addItem("PEAP", PEAP);
    eapTypeCombox->addItem("TTLS", TTLS);
    eapTypeCombox->setCurrentIndex(TLS);
    //TLS
    caCertPathCombox->addItem(tr("None"), QString(tr("None"))); //无
    caCertPathCombox->addItem(tr("Choose from file..."), QString(tr("Choose from file..."))); //从文件中选择...

    clientCertPathCombox->addItem(tr("None"), QString(tr("None"))); //无
    clientCertPathCombox->addItem(tr("Choose from file..."), QString(tr("Choose from file..."))); //从文件中选择...

    clientPrivateKeyCombox->addItem(tr("None"), QString(tr("None"))); //无
    clientPrivateKeyCombox->addItem(tr("Choose from file..."), QString(tr("Choose from file..."))); //从文件中选择...

    //禁用ClearBtn按钮
    pwdEdit->setClearButtonEnabled(false);
    clientPrivateKeyPwdEdit->setClearButtonEnabled(false);
    userPwdEdit->setClearButtonEnabled(false);

    QRegExp rx("^[A-Za-z0-9`~!@#$%^&*()_-+=<>,.\\\/]+$");
    QRegExpValidator *latitude = new QRegExpValidator(rx, this);
    pwdEdit->setValidator(latitude);
    clientPrivateKeyPwdEdit->setValidator(latitude);
    userPwdEdit->setValidator(latitude);

    showNone();
}

void SecurityPage::initConnect()
{
    //安全类型变化
//    connect(secuTypeCombox, &QComboBox::currentTextChanged, this, &SecurityPage::onSecuTypeComboxIndexChanged);
    connect(secuTypeCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SecurityPage::onSecuTypeComboxIndexChanged);
    //EAP方式变化
//    connect(eapTypeCombox, &QComboBox::currentTextChanged, this, &SecurityPage::onEapTypeComboxIndexChanged);
    connect(eapTypeCombox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SecurityPage::onEapTypeComboxIndexChanged);

    connect(caNeedBox, &QCheckBox::clicked, this, &SecurityPage::onCaNeedBoxClicked);

    connect(caCertPathCombox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &SecurityPage::onCaCertPathComboxIndexChanged);

    connect(clientCertPathCombox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &SecurityPage::onClientCertPathComboxIndexChanged);

    connect(clientPrivateKeyCombox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &SecurityPage::onClientPrivateKeyComboxIndexChanged);

    connect(secuTypeCombox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(pwdEdit, &LineEdit::textChanged, this, &SecurityPage::setEnableOfSaveBtn);
    connect(eapTypeCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(identityEdit, &LineEdit::textChanged, this, &SecurityPage::setEnableOfSaveBtn);
    connect(caCertPathCombox, SIGNAL(currentTextChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(caNeedBox, &QCheckBox::stateChanged, this, &SecurityPage::setEnableOfSaveBtn);
    connect(clientCertPathCombox, SIGNAL(currentTextChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(clientPrivateKeyCombox, SIGNAL(currentTextChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(clientPrivateKeyPwdEdit, &LineEdit::textChanged, this, &SecurityPage::setEnableOfSaveBtn);
    connect(eapMethodCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(userNameEdit, &LineEdit::textChanged, this, &SecurityPage::setEnableOfSaveBtn);
    connect(userPwdEdit, &LineEdit::textChanged, this, &SecurityPage::setEnableOfSaveBtn);

}

void SecurityPage::setSecurity(KySecuType index)
{
    secuTypeCombox->setCurrentIndex(index);
    onSecuTypeComboxIndexChanged();
}

void SecurityPage::setPsk(const QString &psk)
{
    pwdEdit->setText(psk);
}

void SecurityPage::setTlsInfo(KyEapMethodTlsInfo &info)
{
    showTls();
    identityEdit->setText(info.identity);
    domainEdit->setText(info.domain);
    if (info.caCertPath.isEmpty()) {
        caCertPathCombox->setItemText(0, QString(tr("None")));
        caNeedBox->setChecked(true);
        caCertPathCombox->setEnabled(false);
    } else {
        caCertPathCombox->setItemText(0, info.caCertPath);
        caNeedBox->setChecked(false);
        caCertPathCombox->setEnabled(true);
    }

    if (info.clientCertPath.isEmpty()) {
        clientCertPathCombox->setItemText(0, "");
    } else {
        clientCertPathCombox->setItemText(0, info.clientCertPath);
    }

    if (info.clientPrivateKey.isEmpty()) {
        clientPrivateKeyCombox->setItemText(0, "");
    } else {
        clientPrivateKeyCombox->setItemText(0, info.clientPrivateKey);
    }

    clientPrivateKeyPwdEdit->setText(info.clientPrivateKeyPWD);

}

void SecurityPage::setPeapInfo(KyEapMethodPeapInfo &info)
{
    showPeapOrTtls();
    eapTypeCombox->setCurrentIndex(PEAP);
    onEapTypeComboxIndexChanged();
    if (info.phase2AuthMethod == KyAuthMethodMschapv2) {
        eapMethodCombox->setCurrentIndex(MSCHAPV2_PEAP);
    } else if (info.phase2AuthMethod == KyAuthMethodMd5){
        eapMethodCombox->setCurrentIndex(MD5_PEAP);
    } else if (info.phase2AuthMethod == KyAuthMethodGtc) {
        eapMethodCombox->setCurrentIndex(GTC_PEAP);
    }
    userNameEdit->setText(info.userName);
    userPwdEdit->setText(info.userPWD);
    if (info.m_passwdFlag) {
        userPwdFlagBox->setChecked(true);
    } else {
        userPwdFlagBox->setChecked(false);
    }
}

void SecurityPage::setTtlsInfo(KyEapMethodTtlsInfo &info)
{
    showPeapOrTtls();
    eapTypeCombox->setCurrentIndex(TTLS);
    onEapTypeComboxIndexChanged();

    if (info.authType == AUTH_EAP) {
        if (info.authEapMethod = KyAuthEapMethodMschapv2) {
            eapMethodCombox->setCurrentIndex(MSCHAPV2_EAP);
        } else if (info.authEapMethod = KyAuthEapMethodMd5) {
            eapMethodCombox->setCurrentIndex(MD5_EAP);
        } else if (info.authEapMethod = KyAuthEapMethodMd5) {
            eapMethodCombox->setCurrentIndex(MD5_EAP);
        } else {
            qDebug() << "not support yet. AUTH_EAP method" << info.authEapMethod;
        }
    } else {
        if (info.authNoEapMethod == KyAuthMethodPap) {
            eapMethodCombox->setCurrentIndex(PAP);
        } else if (info.authNoEapMethod == KyAuthMethodMschap) {
            eapMethodCombox->setCurrentIndex(MSCHAP);
        } else if (info.authNoEapMethod == KyAuthMethodMschapv2) {
            eapMethodCombox->setCurrentIndex(MSCHAPV2);
        } else if (info.authNoEapMethod == KyAuthMethodChap) {
            eapMethodCombox->setCurrentIndex(CHAP);
        } else {
            qDebug() << "not support yet. AUTH_NO_EAP method" << info.authNoEapMethod;
        }
    }
    userNameEdit->setText(info.userName);
    userPwdEdit->setText(info.userPWD);
    if (info.m_passwdFlag) {
        userPwdFlagBox->setChecked(true);
    } else {
        userPwdFlagBox->setChecked(false);
    }
}

void SecurityPage::setSecurityVisible(const bool &visible)
{
    if (secuTypeLabel) {
        secuTypeLabel->setVisible(visible);
    } else {
        qWarning() << "Set visible of secuTypeLabel failed because of null pointer" << Q_FUNC_INFO << __LINE__;
    }
    if (secuTypeCombox) {
        secuTypeCombox->setVisible(visible);
    } else {
        qWarning() << "Set visible of secuTypeCombox failed because of null pointer" << Q_FUNC_INFO << __LINE__;
    }
}

void SecurityPage::updateTlsChange(KyEapMethodTlsInfo &info)
{
    KyEapMethodTlsInfo tlsInfo = assembleTlsInfo();
    if (tlsInfo.clientPrivateKeyPWD != info.clientPrivateKeyPWD) {
        tlsInfo.bChanged = true;
    }
    tlsInfo.devIfaceName = info.devIfaceName;
    info = tlsInfo;
}

void SecurityPage::updatePeapChange(KyEapMethodPeapInfo &info)
{
    KyEapMethodPeapInfo peapInfo = assemblePeapInfo();
    if (peapInfo.userPWD != info.userPWD) {
        peapInfo.bChanged = true;
    }
    info = peapInfo;
}

void SecurityPage::updateTtlsChange(KyEapMethodTtlsInfo &info)
{
    KyEapMethodTtlsInfo ttlsInfo = assembleTtlsInfo();
    if (ttlsInfo.userPWD != info.userPWD) {
        ttlsInfo.bChanged = true;
    }
    info = ttlsInfo;
}

void SecurityPage::getSecuType(KySecuType &secuType, KyEapMethodType &enterpriseType)
{
    secuType = (KySecuType)secuTypeCombox->currentData().toInt();
    enterpriseType = (KyEapMethodType)eapTypeCombox->currentData().toInt();
}

bool SecurityPage::checkIsChanged(const ConInfo info)
{
    if (info.secType != secuTypeCombox->currentData().toInt()) {
        return true;
    } else {
        if (info.secType == NONE) {
            return false;
        } else if (info.secType == WPA_AND_WPA2_PERSONAL || info.secType == WPA3_PERSONAL) {
                return !(info.strPassword == pwdEdit->text());
        } else {
            if (info.enterpriseType != eapTypeCombox->currentData().toInt()) {
                return true;
            } else {
                if (info.enterpriseType == TLS) {
                    return !(info.tlsInfo == assembleTlsInfo());
                } else if (info.enterpriseType == PEAP) {
                    return !(info.peapInfo == assemblePeapInfo());
                } else if (info.enterpriseType == TTLS) {
                    return !(info.ttlsInfo == assembleTtlsInfo());
                }
            }
        }
    }
}

void SecurityPage::showNone()
{
    pwdEdit->hide();
    eapTypeCombox->hide();

    identityEdit->hide();
    domainEdit->hide();
    caCertPathCombox->hide();
    caNeedBox->hide();
    clientCertPathCombox->hide();
    clientPrivateKeyCombox->hide();
    clientPrivateKeyPwdEdit->hide();

    eapMethodCombox->hide();
    userNameEdit->hide();
    userPwdEdit->hide();
    userPwdFlagBox->hide();

    pwdLabel->hide();
    //企业wifi共有
    eapTypeLabel->hide();
    //TLS
    identityLable->hide();
    domainLable->hide();
    caCertPathLabel->hide();
    caNeedFlagLabel->hide();
    clientCertPathLabel->hide();
    clientPrivateKeyLabel->hide();
    clientPrivateKeyPwdLabel->hide();

    //PEAP TTLS共有
    eapMethodLabel->hide();
    userNameLabel->hide();
    userPwdLabel->hide();
    userPwdFlagLabel->hide();
}

void SecurityPage::showPsk()
{
    pwdEdit->show();
    eapTypeCombox->hide();

    identityEdit->hide();
    domainEdit->hide();
    caCertPathCombox->hide();
    caNeedBox->hide();
    clientCertPathCombox->hide();
    clientPrivateKeyCombox->hide();
    clientPrivateKeyPwdEdit->hide();

    eapMethodCombox->hide();
    userNameEdit->hide();
    userPwdEdit->hide();
    userPwdFlagBox->hide();

    pwdLabel->show();
    //企业wifi共有
    eapTypeLabel->hide();
    //TLS
    identityLable->hide();
    domainLable->hide();
    caCertPathLabel->hide();
    caNeedFlagLabel->hide();
    clientCertPathLabel->hide();
    clientPrivateKeyLabel->hide();
    clientPrivateKeyPwdLabel->hide();

    //PEAP TTLS共有
    eapMethodLabel->hide();
    userNameLabel->hide();
    userPwdLabel->hide();
    userPwdFlagLabel->hide();
}

void SecurityPage::showTls()
{
    pwdEdit->hide();
    eapTypeCombox->show();

    identityEdit->show();
    domainEdit->show();
    caCertPathCombox->show();
    caNeedBox->show();
    clientCertPathCombox->show();
    clientPrivateKeyCombox->show();
    clientPrivateKeyPwdEdit->show();

    eapMethodCombox->hide();
    userNameEdit->hide();
    userPwdEdit->hide();
    userPwdFlagBox->hide();

    pwdLabel->hide();
    //企业wifi共有
    eapTypeLabel->show();
    //TLS
    identityLable->show();
    domainLable->show();
    caCertPathLabel->show();
    caNeedFlagLabel->show();
    clientCertPathLabel->show();
    clientPrivateKeyLabel->show();
    clientPrivateKeyPwdLabel->show();

    //PEAP TTLS共有
    eapMethodLabel->hide();
    userNameLabel->hide();
    userPwdLabel->hide();
    userPwdFlagLabel->hide();
}

void SecurityPage::showPeapOrTtls()
{
    pwdEdit->hide();
    eapTypeCombox->show();

    identityEdit->hide();
    domainEdit->hide();
    caCertPathCombox->hide();
    caNeedBox->hide();
    clientCertPathCombox->hide();
    clientPrivateKeyCombox->hide();
    clientPrivateKeyPwdEdit->hide();

    eapMethodCombox->show();
    userNameEdit->show();
    userPwdEdit->show();
    userPwdFlagBox->show();

    pwdLabel->hide();
    //企业wifi共有
    eapTypeLabel->show();
    //TLS
    identityLable->hide();
    domainLable->hide();
    caCertPathLabel->hide();
    caNeedFlagLabel->hide();
    clientCertPathLabel->hide();
    clientPrivateKeyLabel->hide();
    clientPrivateKeyPwdLabel->hide();

    //PEAP TTLS共有
    eapMethodLabel->show();
    userNameLabel->show();
    userPwdLabel->show();
    userPwdFlagLabel->show();
}

KyEapMethodTlsInfo SecurityPage::assembleTlsInfo()
{
    KyEapMethodTlsInfo info;
    info.identity = identityEdit->text();
    info.domain = domainEdit->text();
    info.caCertPath = caCertPathCombox->currentText();
    info.bNeedCa = !caNeedBox->isChecked();
    info.clientCertPath = clientCertPathCombox->currentText();
    info.clientPrivateKey = clientPrivateKeyCombox->currentText();
    info.clientPrivateKeyPWD = clientPrivateKeyPwdEdit->text();
    info.m_privateKeyPWDFlag = NetworkManager::Setting::None;

    return info;
}

KyEapMethodPeapInfo SecurityPage::assemblePeapInfo()
{
    KyEapMethodPeapInfo info;
//    info.phase2AuthMethod = (KyNoEapMethodAuth)eapMethodCombox->currentData().toInt();
    switch (eapMethodCombox->currentIndex()) {
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
    info.userName = userNameEdit->text();
    info.userPWD = userPwdEdit->text();
    info.m_passwdFlag = (userPwdFlagBox->isChecked() ? NetworkManager::Setting::NotSaved : NetworkManager::Setting::None);

    return info;
}
KyEapMethodTtlsInfo SecurityPage::assembleTtlsInfo()
{
    KyEapMethodTtlsInfo info;
    switch (eapMethodCombox->currentIndex()) {
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
    info.userName = userNameEdit->text();
    info.userPWD = userPwdEdit->text();
    info.m_passwdFlag = (userPwdFlagBox->isChecked() ? NetworkManager::Setting::NotSaved : NetworkManager::Setting::None);
    return info;
}

void SecurityPage::updateSecurityChange(KyWirelessConnectSetting &setting)
{
    qDebug() << "secuTypeCombox->currentData()" << secuTypeCombox->currentData().toInt() << pwdEdit->text();
    if (secuTypeCombox->currentData().toInt() == NONE) {
        setting.m_psk = "";
    } else {
        setting.m_psk = pwdEdit->text();
    }

    if (secuTypeCombox->currentData().toInt() == NONE) {
        setting.m_type = WpaNone;
    } else if (secuTypeCombox->currentData().toInt() == WPA_AND_WPA2_PERSONAL) {
        setting.m_type = WpaPsk;
    } else if (secuTypeCombox->currentData().toInt() == WPA3_PERSONAL) {
        setting.m_type = SAE;
    }
}

bool SecurityPage::checkConnectBtnIsEnabled()
{
    int index = secuTypeCombox->currentData().toInt();
    if (index == NONE) {

    } else if (index == WPA_AND_WPA2_PERSONAL || index == WPA3_PERSONAL) {
        if (pwdEdit->text().isEmpty() || pwdEdit->text().length() < 8 ) {
            qDebug() << "password is empty or length < 8";
            return false;
        }
    }  else if (index == WPA_AND_WPA2_ENTERPRISE) {
        int type = eapTypeCombox->currentData().toInt();
        if (type == TLS) {
            if (identityEdit->text().isEmpty()) {
                qDebug() << "tls identity is empty";
                return false;
            }
            QFile cafile(caCertPathCombox->currentText());
            if(!caNeedBox->isChecked() && !cafile.exists()) {
                qDebug() << "ca cert filepath " << caCertPathCombox->currentText() << " is invalid";
                return false;
            }

            QFile cliCafile(clientCertPathCombox->currentText());
            if(!cliCafile.exists()) {
                qDebug() << "client cert filepath " << clientCertPathCombox->currentText() << " is invalid";
                return false;
            }

            QFile cliKeyfile(clientPrivateKeyCombox->currentText());
            if(!cliKeyfile.exists()) {
                qDebug() << "client private key filepath " << clientPrivateKeyCombox->currentText() << " is invalid";
                return false;
            }

            if(clientPrivateKeyPwdEdit->text().isEmpty()) {
                qDebug() << "client Private Key password is empty";
                return false;
            }
        } else if (type == PEAP || type == TTLS) {
            if(userNameEdit->text().isEmpty() || userPwdEdit->text().isEmpty()) {
                qDebug() << "user name or user password is empty";
                return false;
            }
        }
    }
    return true;
}

void SecurityPage::setEnableOfSaveBtn()
{
    emit setSecuPageState(checkConnectBtnIsEnabled());
}

void SecurityPage::onSecuTypeComboxIndexChanged()
{
    int index = secuTypeCombox->currentData().toInt();
    if (index == WPA_AND_WPA2_PERSONAL || index == WPA3_PERSONAL) {
        showPsk();
    } else if (index == WPA_AND_WPA2_ENTERPRISE) {
        onEapTypeComboxIndexChanged();
    } else if (index == NONE) {
        showNone();
    }
}

void SecurityPage::onEapTypeComboxIndexChanged()
{
    qDebug() << "onEapTypeComboxIndexChanged";
    int index = eapTypeCombox->currentData().toInt();
    if (index == TLS) {
        showTls();
        emit this->eapTypeChanged(TLS);
    } else if (index == PEAP) {
        showPeapOrTtls();
        eapMethodCombox->clear();
        eapMethodCombox->addItem("MSCHAPv2", MSCHAPV2_PEAP);
        eapMethodCombox->addItem("MD5", MD5_PEAP);
        eapMethodCombox->addItem("GTC", GTC_PEAP);
        emit this->eapTypeChanged(PEAP);
    } else if (index == TTLS) {
        showPeapOrTtls();
        eapMethodCombox->clear();
        eapMethodCombox->addItem("pap", PAP);
        eapMethodCombox->addItem("mschap", MSCHAP);
        eapMethodCombox->addItem("mschapv2(eap)", MSCHAPV2_EAP);
        eapMethodCombox->addItem("mschapv2", MSCHAPV2);
        eapMethodCombox->addItem("chap", CHAP);
        eapMethodCombox->addItem("md5(eap)", MD5_EAP);
        eapMethodCombox->addItem("gtc(eap)", GTC_EAP);
        emit this->eapTypeChanged(TTLS);
    }
}

void SecurityPage::onCaNeedBoxClicked()
{
    if (caNeedBox->isChecked()) {
        caCertPathCombox->setItemText(0, QString(tr("None")));
        caCertPathCombox->setEnabled(false);
    }  else {
        caCertPathCombox->setEnabled(true);
    }
}

void SecurityPage::onCaCertPathComboxIndexChanged(QString str)
{
    if (str.contains("Choose from file...") || str.contains("从文件选择..."))
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CA certificate"), "recent:///",
                                                        tr("CA Files (*.pem *.der *.p12 *.crt *.cer *.pfx)"));
        if (!fileName.isNull()) {
            QStringList nameList = fileName.split("/");
            caCertPathCombox->blockSignals(true);
            caCertPathCombox->setItemText(0, fileName);
            caCertPathCombox->setCurrentIndex(0);
            caCertPathCombox->blockSignals(false);
        } else {
            caCertPathCombox->blockSignals(true);
            caCertPathCombox->setItemText(0, tr("None"));
            caCertPathCombox->setCurrentIndex(0);
            caCertPathCombox->blockSignals(false);
        }
    } else {
        qWarning() << "Choose file is null or unvalible";
    }
}

void SecurityPage::onClientCertPathComboxIndexChanged(QString str)
{
    if (str.contains("Choose from file...") || str.contains("从文件选择..."))
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CA certificate"), "recent:///",
                                                        tr("CA Files (*.pem *.der *.p12 *.crt *.cer *.pfx)"));
        if (!fileName.isNull()) {
            clientCertPathCombox->blockSignals(true);
            clientCertPathCombox->setItemText(0, fileName);
            clientCertPathCombox->setCurrentIndex(0);
            clientCertPathCombox->blockSignals(false);
        } else {
            clientCertPathCombox->blockSignals(true);
            clientCertPathCombox->setItemText(0, tr("None"));
            clientCertPathCombox->setCurrentIndex(0);
            clientCertPathCombox->blockSignals(false);
        }
    } else {
        qWarning() << "Choose file is null or unvalible";
    }
}

void SecurityPage::onClientPrivateKeyComboxIndexChanged(QString str)
{
    if (str.contains("Choose from file...") || str.contains("从文件选择..."))
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a CA certificate"), "recent:///",
                                                        tr("CA Files (*.pem *.der *.p12 *.crt *.cer *.pfx)"));
        if (!fileName.isNull()) {
            QStringList nameList = fileName.split("/");
            clientPrivateKeyCombox->blockSignals(true);
            clientPrivateKeyCombox->setItemText(0, fileName);
            clientPrivateKeyCombox->setCurrentIndex(0);
            clientPrivateKeyCombox->blockSignals(false);
        } else {
            clientPrivateKeyCombox->blockSignals(true);
            clientPrivateKeyCombox->setItemText(0, tr("None"));
            clientPrivateKeyCombox->setCurrentIndex(0);
            clientPrivateKeyCombox->blockSignals(false);
        }
    } else {
        qWarning() << "Choose file is null or unvalible";
    }
}
