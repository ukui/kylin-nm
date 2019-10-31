/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifisectls.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFISECTLS_H
#define UI_DLGCONNHIDWIFISECTLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DlgConnHidWifiSecTls
{
public:
    QComboBox *cbxSecurity;
    QLineEdit *leNetName;
    QLineEdit *leUserCertifyPwd;
    QLineEdit *leCaPwd;
    QCheckBox *checkBoxPwd;
    QLabel *lbConn;
    QLabel *lbNetName;
    QLabel *lbDomain;
    QLabel *lbCA;
    QComboBox *cbxUserCertify;
    QLabel *lbIdentity;
    QLabel *lbUserCertifyPwd;
    QPushButton *btnCancel;
    QLabel *lbUserKeyPwd;
    QComboBox *cbxCA;
    QLabel *lbUserPriKey;
    QLabel *lbCaPwd;
    QLineEdit *leDomain;
    QCheckBox *checkBoxPwdSec;
    QLineEdit *leIdentity;
    QLineEdit *leUserKeyPwd;
    QLabel *lbUserCertify;
    QLabel *lbAuth;
    QComboBox *cbxAuth;
    QPushButton *btnConnect;
    QCheckBox *checkBoxCA;
    QComboBox *cbxConn;
    QComboBox *cbxUserPriKey;
    QLabel *lbSecurity;
    QLabel *lbBoder;
    QLabel *lbUpback;
    QLabel *lbBottomback;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbLeftupTitle;

    void setupUi(QDialog *DlgConnHidWifiSecTls)
    {
        if (DlgConnHidWifiSecTls->objectName().isEmpty())
            DlgConnHidWifiSecTls->setObjectName(QStringLiteral("DlgConnHidWifiSecTls"));
        DlgConnHidWifiSecTls->resize(432, 680);
        cbxSecurity = new QComboBox(DlgConnHidWifiSecTls);
        cbxSecurity->setObjectName(QStringLiteral("cbxSecurity"));
        cbxSecurity->setGeometry(QRect(180, 165, 200, 32));
        QFont font;
        font.setPointSize(10);
        cbxSecurity->setFont(font);
        cbxSecurity->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        leNetName = new QLineEdit(DlgConnHidWifiSecTls);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(180, 125, 200, 32));
        leNetName->setFont(font);
        leUserCertifyPwd = new QLineEdit(DlgConnHidWifiSecTls);
        leUserCertifyPwd->setObjectName(QStringLiteral("leUserCertifyPwd"));
        leUserCertifyPwd->setGeometry(QRect(180, 485, 200, 32));
        leUserCertifyPwd->setFont(font);
        leUserCertifyPwd->setEchoMode(QLineEdit::Password);
        leCaPwd = new QLineEdit(DlgConnHidWifiSecTls);
        leCaPwd->setObjectName(QStringLiteral("leCaPwd"));
        leCaPwd->setGeometry(QRect(180, 365, 200, 32));
        leCaPwd->setFont(font);
        leCaPwd->setEchoMode(QLineEdit::Password);
        checkBoxPwd = new QCheckBox(DlgConnHidWifiSecTls);
        checkBoxPwd->setObjectName(QStringLiteral("checkBoxPwd"));
        checkBoxPwd->setGeometry(QRect(357, 377, 18, 9));
        checkBoxPwd->setFont(font);
        lbConn = new QLabel(DlgConnHidWifiSecTls);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        lbNetName = new QLabel(DlgConnHidWifiSecTls);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 130, 90, 20));
        lbNetName->setFont(font);
        lbDomain = new QLabel(DlgConnHidWifiSecTls);
        lbDomain->setObjectName(QStringLiteral("lbDomain"));
        lbDomain->setGeometry(QRect(76, 290, 90, 20));
        lbDomain->setFont(font);
        lbCA = new QLabel(DlgConnHidWifiSecTls);
        lbCA->setObjectName(QStringLiteral("lbCA"));
        lbCA->setGeometry(QRect(76, 330, 90, 20));
        lbCA->setFont(font);
        cbxUserCertify = new QComboBox(DlgConnHidWifiSecTls);
        cbxUserCertify->setObjectName(QStringLiteral("cbxUserCertify"));
        cbxUserCertify->setGeometry(QRect(180, 445, 200, 32));
        cbxUserCertify->setFont(font);
        cbxUserCertify->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbIdentity = new QLabel(DlgConnHidWifiSecTls);
        lbIdentity->setObjectName(QStringLiteral("lbIdentity"));
        lbIdentity->setGeometry(QRect(76, 250, 90, 20));
        lbIdentity->setFont(font);
        lbUserCertifyPwd = new QLabel(DlgConnHidWifiSecTls);
        lbUserCertifyPwd->setObjectName(QStringLiteral("lbUserCertifyPwd"));
        lbUserCertifyPwd->setGeometry(QRect(76, 490, 90, 20));
        lbUserCertifyPwd->setFont(font);
        btnCancel = new QPushButton(DlgConnHidWifiSecTls);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 634, 90, 30));
        btnCancel->setFont(font);
        lbUserKeyPwd = new QLabel(DlgConnHidWifiSecTls);
        lbUserKeyPwd->setObjectName(QStringLiteral("lbUserKeyPwd"));
        lbUserKeyPwd->setGeometry(QRect(76, 570, 90, 20));
        lbUserKeyPwd->setFont(font);
        cbxCA = new QComboBox(DlgConnHidWifiSecTls);
        cbxCA->setObjectName(QStringLiteral("cbxCA"));
        cbxCA->setGeometry(QRect(180, 325, 200, 32));
        cbxCA->setFont(font);
        cbxCA->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbUserPriKey = new QLabel(DlgConnHidWifiSecTls);
        lbUserPriKey->setObjectName(QStringLiteral("lbUserPriKey"));
        lbUserPriKey->setGeometry(QRect(76, 530, 90, 20));
        lbUserPriKey->setFont(font);
        lbCaPwd = new QLabel(DlgConnHidWifiSecTls);
        lbCaPwd->setObjectName(QStringLiteral("lbCaPwd"));
        lbCaPwd->setGeometry(QRect(76, 370, 90, 20));
        lbCaPwd->setFont(font);
        leDomain = new QLineEdit(DlgConnHidWifiSecTls);
        leDomain->setObjectName(QStringLiteral("leDomain"));
        leDomain->setGeometry(QRect(180, 285, 200, 32));
        leDomain->setFont(font);
        checkBoxPwdSec = new QCheckBox(DlgConnHidWifiSecTls);
        checkBoxPwdSec->setObjectName(QStringLiteral("checkBoxPwdSec"));
        checkBoxPwdSec->setGeometry(QRect(337, 577, 18, 8));
        checkBoxPwdSec->setFont(font);
        leIdentity = new QLineEdit(DlgConnHidWifiSecTls);
        leIdentity->setObjectName(QStringLiteral("leIdentity"));
        leIdentity->setGeometry(QRect(180, 245, 200, 32));
        leIdentity->setFont(font);
        leUserKeyPwd = new QLineEdit(DlgConnHidWifiSecTls);
        leUserKeyPwd->setObjectName(QStringLiteral("leUserKeyPwd"));
        leUserKeyPwd->setGeometry(QRect(180, 565, 200, 32));
        leUserKeyPwd->setFont(font);
        leUserKeyPwd->setEchoMode(QLineEdit::Password);
        lbUserCertify = new QLabel(DlgConnHidWifiSecTls);
        lbUserCertify->setObjectName(QStringLiteral("lbUserCertify"));
        lbUserCertify->setGeometry(QRect(76, 450, 90, 20));
        lbUserCertify->setFont(font);
        lbAuth = new QLabel(DlgConnHidWifiSecTls);
        lbAuth->setObjectName(QStringLiteral("lbAuth"));
        lbAuth->setGeometry(QRect(76, 210, 90, 20));
        lbAuth->setFont(font);
        cbxAuth = new QComboBox(DlgConnHidWifiSecTls);
        cbxAuth->setObjectName(QStringLiteral("cbxAuth"));
        cbxAuth->setGeometry(QRect(180, 205, 200, 32));
        cbxAuth->setFont(font);
        cbxAuth->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        btnConnect = new QPushButton(DlgConnHidWifiSecTls);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 634, 90, 30));
        btnConnect->setFont(font);
        checkBoxCA = new QCheckBox(DlgConnHidWifiSecTls);
        checkBoxCA->setObjectName(QStringLiteral("checkBoxCA"));
        checkBoxCA->setGeometry(QRect(180, 405, 200, 25));
        checkBoxCA->setFont(font);
        cbxConn = new QComboBox(DlgConnHidWifiSecTls);
        cbxConn->setObjectName(QStringLiteral("cbxConn"));
        cbxConn->setGeometry(QRect(180, 55, 200, 32));
        cbxConn->setFont(font);
        cbxConn->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        cbxUserPriKey = new QComboBox(DlgConnHidWifiSecTls);
        cbxUserPriKey->setObjectName(QStringLiteral("cbxUserPriKey"));
        cbxUserPriKey->setGeometry(QRect(180, 525, 200, 32));
        cbxUserPriKey->setFont(font);
        cbxUserPriKey->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbSecurity = new QLabel(DlgConnHidWifiSecTls);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 170, 90, 20));
        lbSecurity->setFont(font);
        lbBoder = new QLabel(DlgConnHidWifiSecTls);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 680));
        lbUpback = new QLabel(DlgConnHidWifiSecTls);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbBottomback = new QLabel(DlgConnHidWifiSecTls);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 620, 432, 60));
        lbLeftup = new QLabel(DlgConnHidWifiSecTls);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifiSecTls);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbLeftupTitle = new QLabel(DlgConnHidWifiSecTls);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        cbxSecurity->raise();
        leNetName->raise();
        leUserCertifyPwd->raise();
        leCaPwd->raise();
        checkBoxPwd->raise();
        lbConn->raise();
        lbNetName->raise();
        lbDomain->raise();
        lbCA->raise();
        cbxUserCertify->raise();
        lbIdentity->raise();
        lbUserCertifyPwd->raise();
        btnCancel->raise();
        lbUserKeyPwd->raise();
        cbxCA->raise();
        lbUserPriKey->raise();
        lbCaPwd->raise();
        leDomain->raise();
        leIdentity->raise();
        leUserKeyPwd->raise();
        lbUserCertify->raise();
        lbAuth->raise();
        cbxAuth->raise();
        btnConnect->raise();
        checkBoxCA->raise();
        cbxConn->raise();
        cbxUserPriKey->raise();
        lbSecurity->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();
        checkBoxPwdSec->raise();

        retranslateUi(DlgConnHidWifiSecTls);

        QMetaObject::connectSlotsByName(DlgConnHidWifiSecTls);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiSecTls)
    {
        DlgConnHidWifiSecTls->setWindowTitle(QApplication::translate("DlgConnHidWifiSecTls", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        checkBoxPwd->setText(QString());
        lbConn->setText(QString());
        lbNetName->setText(QString());
        lbDomain->setText(QString());
        lbCA->setText(QString());
        lbIdentity->setText(QString());
        lbUserCertifyPwd->setText(QString());
        btnCancel->setText(QString());
        lbUserKeyPwd->setText(QString());
        lbUserPriKey->setText(QString());
        lbCaPwd->setText(QString());
        checkBoxPwdSec->setText(QString());
        lbUserCertify->setText(QString());
        lbAuth->setText(QString());
        btnConnect->setText(QString());
        checkBoxCA->setText(QString());
        lbSecurity->setText(QString());
        lbBoder->setText(QString());
        lbUpback->setText(QString());
        lbBottomback->setText(QString());
        lbLeftup->setText(QString());
        lbLeftupIcon->setText(QString());
        lbLeftupTitle->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiSecTls: public Ui_DlgConnHidWifiSecTls {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFISECTLS_H
