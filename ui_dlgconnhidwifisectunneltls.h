/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifisectunneltls.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFISECTUNNELTLS_H
#define UI_DLGCONNHIDWIFISECTUNNELTLS_H

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

class Ui_DlgConnHidWifiSecTunnelTLS
{
public:
    QComboBox *cbxConn;
    QLineEdit *leCaPwd;
    QLineEdit *leNetName;
    QPushButton *btnConnect;
    QLineEdit *lePassword;
    QPushButton *btnCancel;
    QLabel *lbDomain;
    QCheckBox *checkBoxPwdSec;
    QLabel *lbUserName;
    QLabel *lbSecurity;
    QComboBox *cbxSecurity;
    QComboBox *cbxCA;
    QComboBox *cbxInnerAuth;
    QLineEdit *leUserName;
    QLabel *lbCA;
    QComboBox *cbxAuth;
    QLabel *lbAnonyId;
    QLabel *lbNetName;
    QCheckBox *checkBoxCA;
    QLabel *lbInnerAuth;
    QLineEdit *leDomain;
    QLabel *lbAuth;
    QLabel *lbConn;
    QLabel *lbCaPwd;
    QLabel *lbPassword;
    QCheckBox *checkBoxPwd;
    QLineEdit *leAnonyId;
    QLabel *lbBoder;
    QLabel *lbUpback;
    QLabel *lbBottomback;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbLeftupTitle;

    void setupUi(QDialog *DlgConnHidWifiSecTunnelTLS)
    {
        if (DlgConnHidWifiSecTunnelTLS->objectName().isEmpty())
            DlgConnHidWifiSecTunnelTLS->setObjectName(QStringLiteral("DlgConnHidWifiSecTunnelTLS"));
        DlgConnHidWifiSecTunnelTLS->resize(432, 640);
        cbxConn = new QComboBox(DlgConnHidWifiSecTunnelTLS);
        cbxConn->setObjectName(QStringLiteral("cbxConn"));
        cbxConn->setGeometry(QRect(180, 55, 200, 32));
        QFont font;
        font.setPointSize(10);
        cbxConn->setFont(font);
        cbxConn->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        leCaPwd = new QLineEdit(DlgConnHidWifiSecTunnelTLS);
        leCaPwd->setObjectName(QStringLiteral("leCaPwd"));
        leCaPwd->setGeometry(QRect(180, 365, 200, 32));
        leCaPwd->setFont(font);
        leCaPwd->setEchoMode(QLineEdit::Password);
        leNetName = new QLineEdit(DlgConnHidWifiSecTunnelTLS);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(180, 125, 200, 32));
        leNetName->setFont(font);
        btnConnect = new QPushButton(DlgConnHidWifiSecTunnelTLS);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 594, 90, 30));
        btnConnect->setFont(font);
        lePassword = new QLineEdit(DlgConnHidWifiSecTunnelTLS);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(180, 525, 200, 32));
        lePassword->setFont(font);
        lePassword->setEchoMode(QLineEdit::Password);
        btnCancel = new QPushButton(DlgConnHidWifiSecTunnelTLS);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 594, 90, 30));
        btnCancel->setFont(font);
        lbDomain = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbDomain->setObjectName(QStringLiteral("lbDomain"));
        lbDomain->setGeometry(QRect(76, 290, 90, 20));
        lbDomain->setFont(font);
        checkBoxPwdSec = new QCheckBox(DlgConnHidWifiSecTunnelTLS);
        checkBoxPwdSec->setObjectName(QStringLiteral("checkBoxPwdSec"));
        checkBoxPwdSec->setGeometry(QRect(340, 537, 18, 9));
        checkBoxPwdSec->setFont(font);
        lbUserName = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbUserName->setObjectName(QStringLiteral("lbUserName"));
        lbUserName->setGeometry(QRect(76, 490, 90, 20));
        lbUserName->setFont(font);
        lbSecurity = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 170, 90, 20));
        lbSecurity->setFont(font);
        cbxSecurity = new QComboBox(DlgConnHidWifiSecTunnelTLS);
        cbxSecurity->setObjectName(QStringLiteral("cbxSecurity"));
        cbxSecurity->setGeometry(QRect(180, 165, 200, 32));
        cbxSecurity->setFont(font);
        cbxSecurity->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        cbxCA = new QComboBox(DlgConnHidWifiSecTunnelTLS);
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
        cbxInnerAuth = new QComboBox(DlgConnHidWifiSecTunnelTLS);
        cbxInnerAuth->setObjectName(QStringLiteral("cbxInnerAuth"));
        cbxInnerAuth->setGeometry(QRect(180, 445, 200, 32));
        cbxInnerAuth->setFont(font);
        cbxInnerAuth->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        leUserName = new QLineEdit(DlgConnHidWifiSecTunnelTLS);
        leUserName->setObjectName(QStringLiteral("leUserName"));
        leUserName->setGeometry(QRect(180, 485, 200, 32));
        leUserName->setFont(font);
        leUserName->setEchoMode(QLineEdit::Normal);
        lbCA = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbCA->setObjectName(QStringLiteral("lbCA"));
        lbCA->setGeometry(QRect(76, 330, 90, 20));
        lbCA->setFont(font);
        cbxAuth = new QComboBox(DlgConnHidWifiSecTunnelTLS);
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
        lbAnonyId = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbAnonyId->setObjectName(QStringLiteral("lbAnonyId"));
        lbAnonyId->setGeometry(QRect(76, 250, 90, 20));
        lbAnonyId->setFont(font);
        lbNetName = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 130, 90, 20));
        lbNetName->setFont(font);
        checkBoxCA = new QCheckBox(DlgConnHidWifiSecTunnelTLS);
        checkBoxCA->setObjectName(QStringLiteral("checkBoxCA"));
        checkBoxCA->setGeometry(QRect(180, 405, 200, 25));
        checkBoxCA->setFont(font);
        lbInnerAuth = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbInnerAuth->setObjectName(QStringLiteral("lbInnerAuth"));
        lbInnerAuth->setGeometry(QRect(76, 450, 90, 20));
        lbInnerAuth->setFont(font);
        leDomain = new QLineEdit(DlgConnHidWifiSecTunnelTLS);
        leDomain->setObjectName(QStringLiteral("leDomain"));
        leDomain->setGeometry(QRect(180, 285, 200, 32));
        leDomain->setFont(font);
        lbAuth = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbAuth->setObjectName(QStringLiteral("lbAuth"));
        lbAuth->setGeometry(QRect(76, 210, 90, 20));
        lbAuth->setFont(font);
        lbConn = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        lbCaPwd = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbCaPwd->setObjectName(QStringLiteral("lbCaPwd"));
        lbCaPwd->setGeometry(QRect(76, 370, 90, 20));
        lbCaPwd->setFont(font);
        lbPassword = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(76, 530, 90, 20));
        lbPassword->setFont(font);
        checkBoxPwd = new QCheckBox(DlgConnHidWifiSecTunnelTLS);
        checkBoxPwd->setObjectName(QStringLiteral("checkBoxPwd"));
        checkBoxPwd->setGeometry(QRect(357, 377, 18, 9));
        checkBoxPwd->setFont(font);
        leAnonyId = new QLineEdit(DlgConnHidWifiSecTunnelTLS);
        leAnonyId->setObjectName(QStringLiteral("leAnonyId"));
        leAnonyId->setGeometry(QRect(180, 245, 200, 32));
        leAnonyId->setFont(font);
        lbBoder = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 640));
        lbUpback = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbBottomback = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 580, 432, 60));
        lbLeftup = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbLeftupTitle = new QLabel(DlgConnHidWifiSecTunnelTLS);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbUpback->raise();
        lbBottomback->raise();
        lbBoder->raise();
        cbxConn->raise();
        leCaPwd->raise();
        leNetName->raise();
        btnConnect->raise();
        lePassword->raise();
        btnCancel->raise();
        lbDomain->raise();
        checkBoxPwdSec->raise();
        lbUserName->raise();
        lbSecurity->raise();
        cbxSecurity->raise();
        cbxCA->raise();
        cbxInnerAuth->raise();
        leUserName->raise();
        lbCA->raise();
        cbxAuth->raise();
        lbAnonyId->raise();
        lbNetName->raise();
        checkBoxCA->raise();
        lbInnerAuth->raise();
        leDomain->raise();
        lbAuth->raise();
        lbConn->raise();
        lbCaPwd->raise();
        lbPassword->raise();
        checkBoxPwd->raise();
        leAnonyId->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiSecTunnelTLS);

        QMetaObject::connectSlotsByName(DlgConnHidWifiSecTunnelTLS);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiSecTunnelTLS)
    {
        DlgConnHidWifiSecTunnelTLS->setWindowTitle(QApplication::translate("DlgConnHidWifiSecTunnelTLS", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        btnConnect->setText(QString());
        btnCancel->setText(QString());
        lbDomain->setText(QString());
        checkBoxPwdSec->setText(QString());
        lbUserName->setText(QString());
        lbSecurity->setText(QString());
        lbCA->setText(QString());
        lbAnonyId->setText(QString());
        lbNetName->setText(QString());
        checkBoxCA->setText(QString());
        lbInnerAuth->setText(QString());
        lbAuth->setText(QString());
        lbConn->setText(QString());
        lbCaPwd->setText(QString());
        lbPassword->setText(QString());
        checkBoxPwd->setText(QString());
        lbBoder->setText(QString());
        lbUpback->setText(QString());
        lbBottomback->setText(QString());
        lbLeftup->setText(QString());
        lbLeftupIcon->setText(QString());
        lbLeftupTitle->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiSecTunnelTLS: public Ui_DlgConnHidWifiSecTunnelTLS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFISECTUNNELTLS_H
