/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifisecpeap.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFISECPEAP_H
#define UI_DLGCONNHIDWIFISECPEAP_H

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

class Ui_DlgConnHidWifiSecPeap
{
public:
    QLineEdit *leNetName;
    QComboBox *cbxCA;
    QLabel *lbConn;
    QLineEdit *leDomain;
    QComboBox *cbxConn;
    QLineEdit *leCaPwd;
    QPushButton *btnConnect;
    QComboBox *cbxSecurity;
    QLineEdit *leAnonyId;
    QCheckBox *checkBoxCA;
    QLabel *lbNetName;
    QPushButton *btnCancel;
    QLabel *lbDomain;
    QLabel *lbCaPwd;
    QCheckBox *checkBoxPwdSec;
    QCheckBox *checkBoxPwd;
    QLabel *lbSecurity;
    QComboBox *cbxPEAPver;
    QLabel *lbAuth;
    QLabel *lbPEAPver;
    QComboBox *cbxAuth;
    QLabel *lbCA;
    QLabel *lbAnonyId;
    QLabel *lbPassword;
    QLineEdit *leUserName;
    QComboBox *cbxInnerAuth;
    QLineEdit *lePassword;
    QLabel *lbInnerAuth;
    QLabel *lbUserName;
    QLabel *lbBoder;
    QLabel *lbUpback;
    QLabel *lbBottomback;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbLeftupTitle;

    void setupUi(QDialog *DlgConnHidWifiSecPeap)
    {
        if (DlgConnHidWifiSecPeap->objectName().isEmpty())
            DlgConnHidWifiSecPeap->setObjectName(QStringLiteral("DlgConnHidWifiSecPeap"));
        DlgConnHidWifiSecPeap->resize(432, 680);
        leNetName = new QLineEdit(DlgConnHidWifiSecPeap);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(180, 125, 200, 32));
        QFont font;
        font.setPointSize(10);
        leNetName->setFont(font);
        cbxCA = new QComboBox(DlgConnHidWifiSecPeap);
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
        lbConn = new QLabel(DlgConnHidWifiSecPeap);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        leDomain = new QLineEdit(DlgConnHidWifiSecPeap);
        leDomain->setObjectName(QStringLiteral("leDomain"));
        leDomain->setGeometry(QRect(180, 285, 200, 32));
        leDomain->setFont(font);
        cbxConn = new QComboBox(DlgConnHidWifiSecPeap);
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
        leCaPwd = new QLineEdit(DlgConnHidWifiSecPeap);
        leCaPwd->setObjectName(QStringLiteral("leCaPwd"));
        leCaPwd->setGeometry(QRect(180, 365, 200, 32));
        leCaPwd->setFont(font);
        leCaPwd->setEchoMode(QLineEdit::Password);
        btnConnect = new QPushButton(DlgConnHidWifiSecPeap);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 634, 90, 30));
        btnConnect->setFont(font);
        cbxSecurity = new QComboBox(DlgConnHidWifiSecPeap);
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
        leAnonyId = new QLineEdit(DlgConnHidWifiSecPeap);
        leAnonyId->setObjectName(QStringLiteral("leAnonyId"));
        leAnonyId->setGeometry(QRect(180, 245, 200, 32));
        leAnonyId->setFont(font);
        checkBoxCA = new QCheckBox(DlgConnHidWifiSecPeap);
        checkBoxCA->setObjectName(QStringLiteral("checkBoxCA"));
        checkBoxCA->setGeometry(QRect(183, 405, 200, 25));
        checkBoxCA->setFont(font);
        lbNetName = new QLabel(DlgConnHidWifiSecPeap);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 130, 90, 20));
        lbNetName->setFont(font);
        btnCancel = new QPushButton(DlgConnHidWifiSecPeap);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 634, 90, 30));
        btnCancel->setFont(font);
        lbDomain = new QLabel(DlgConnHidWifiSecPeap);
        lbDomain->setObjectName(QStringLiteral("lbDomain"));
        lbDomain->setGeometry(QRect(76, 290, 90, 20));
        lbDomain->setFont(font);
        lbCaPwd = new QLabel(DlgConnHidWifiSecPeap);
        lbCaPwd->setObjectName(QStringLiteral("lbCaPwd"));
        lbCaPwd->setGeometry(QRect(76, 370, 90, 20));
        lbCaPwd->setFont(font);
        checkBoxPwdSec = new QCheckBox(DlgConnHidWifiSecPeap);
        checkBoxPwdSec->setObjectName(QStringLiteral("checkBoxPwdSec"));
        checkBoxPwdSec->setGeometry(QRect(340, 577, 18, 9));
        checkBoxPwdSec->setFont(font);
        checkBoxPwd = new QCheckBox(DlgConnHidWifiSecPeap);
        checkBoxPwd->setObjectName(QStringLiteral("checkBoxPwd"));
        checkBoxPwd->setGeometry(QRect(357, 377, 18, 9));
        checkBoxPwd->setFont(font);
        lbSecurity = new QLabel(DlgConnHidWifiSecPeap);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 170, 90, 20));
        lbSecurity->setFont(font);
        cbxPEAPver = new QComboBox(DlgConnHidWifiSecPeap);
        cbxPEAPver->setObjectName(QStringLiteral("cbxPEAPver"));
        cbxPEAPver->setGeometry(QRect(180, 445, 200, 32));
        cbxPEAPver->setFont(font);
        cbxPEAPver->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbAuth = new QLabel(DlgConnHidWifiSecPeap);
        lbAuth->setObjectName(QStringLiteral("lbAuth"));
        lbAuth->setGeometry(QRect(76, 210, 90, 20));
        lbAuth->setFont(font);
        lbPEAPver = new QLabel(DlgConnHidWifiSecPeap);
        lbPEAPver->setObjectName(QStringLiteral("lbPEAPver"));
        lbPEAPver->setGeometry(QRect(76, 450, 90, 20));
        lbPEAPver->setFont(font);
        cbxAuth = new QComboBox(DlgConnHidWifiSecPeap);
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
        lbCA = new QLabel(DlgConnHidWifiSecPeap);
        lbCA->setObjectName(QStringLiteral("lbCA"));
        lbCA->setGeometry(QRect(76, 330, 90, 20));
        lbCA->setFont(font);
        lbAnonyId = new QLabel(DlgConnHidWifiSecPeap);
        lbAnonyId->setObjectName(QStringLiteral("lbAnonyId"));
        lbAnonyId->setGeometry(QRect(76, 250, 90, 20));
        lbAnonyId->setFont(font);
        lbPassword = new QLabel(DlgConnHidWifiSecPeap);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(76, 570, 90, 20));
        lbPassword->setFont(font);
        leUserName = new QLineEdit(DlgConnHidWifiSecPeap);
        leUserName->setObjectName(QStringLiteral("leUserName"));
        leUserName->setGeometry(QRect(180, 525, 200, 32));
        leUserName->setFont(font);
        cbxInnerAuth = new QComboBox(DlgConnHidWifiSecPeap);
        cbxInnerAuth->setObjectName(QStringLiteral("cbxInnerAuth"));
        cbxInnerAuth->setGeometry(QRect(180, 485, 200, 32));
        cbxInnerAuth->setFont(font);
        cbxInnerAuth->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lePassword = new QLineEdit(DlgConnHidWifiSecPeap);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(180, 565, 200, 32));
        lePassword->setFont(font);
        lePassword->setEchoMode(QLineEdit::Password);
        lbInnerAuth = new QLabel(DlgConnHidWifiSecPeap);
        lbInnerAuth->setObjectName(QStringLiteral("lbInnerAuth"));
        lbInnerAuth->setGeometry(QRect(76, 490, 90, 20));
        lbInnerAuth->setFont(font);
        lbUserName = new QLabel(DlgConnHidWifiSecPeap);
        lbUserName->setObjectName(QStringLiteral("lbUserName"));
        lbUserName->setGeometry(QRect(76, 530, 90, 20));
        lbUserName->setFont(font);
        lbBoder = new QLabel(DlgConnHidWifiSecPeap);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 680));
        lbUpback = new QLabel(DlgConnHidWifiSecPeap);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbBottomback = new QLabel(DlgConnHidWifiSecPeap);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 620, 432, 60));
        lbLeftup = new QLabel(DlgConnHidWifiSecPeap);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifiSecPeap);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbLeftupTitle = new QLabel(DlgConnHidWifiSecPeap);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        leNetName->raise();
        cbxCA->raise();
        lbConn->raise();
        leDomain->raise();
        cbxConn->raise();
        leCaPwd->raise();
        btnConnect->raise();
        cbxSecurity->raise();
        leAnonyId->raise();
        checkBoxCA->raise();
        lbNetName->raise();
        btnCancel->raise();
        lbDomain->raise();
        lbCaPwd->raise();
        checkBoxPwd->raise();
        lbSecurity->raise();
        cbxPEAPver->raise();
        lbAuth->raise();
        lbPEAPver->raise();
        cbxAuth->raise();
        lbCA->raise();
        lbAnonyId->raise();
        lbPassword->raise();
        leUserName->raise();
        cbxInnerAuth->raise();
        lePassword->raise();
        lbInnerAuth->raise();
        lbUserName->raise();
        checkBoxPwdSec->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiSecPeap);

        QMetaObject::connectSlotsByName(DlgConnHidWifiSecPeap);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiSecPeap)
    {
        DlgConnHidWifiSecPeap->setWindowTitle(QApplication::translate("DlgConnHidWifiSecPeap", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        lbConn->setText(QString());
        btnConnect->setText(QString());
        checkBoxCA->setText(QString());
        lbNetName->setText(QString());
        btnCancel->setText(QString());
        lbDomain->setText(QString());
        lbCaPwd->setText(QString());
        checkBoxPwdSec->setText(QString());
        checkBoxPwd->setText(QString());
        lbSecurity->setText(QString());
        lbAuth->setText(QString());
        lbPEAPver->setText(QString());
        lbCA->setText(QString());
        lbAnonyId->setText(QString());
        lbPassword->setText(QString());
        lbInnerAuth->setText(QString());
        lbUserName->setText(QString());
        lbBoder->setText(QString());
        lbUpback->setText(QString());
        lbBottomback->setText(QString());
        lbLeftup->setText(QString());
        lbLeftupIcon->setText(QString());
        lbLeftupTitle->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiSecPeap: public Ui_DlgConnHidWifiSecPeap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFISECPEAP_H
