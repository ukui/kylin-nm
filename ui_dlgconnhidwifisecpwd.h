/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifisecpwd.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFISECPWD_H
#define UI_DLGCONNHIDWIFISECPWD_H

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

class Ui_DlgConnHidWifiSecPwd
{
public:
    QCheckBox *checkBox;
    QPushButton *btnCancel;
    QLabel *lbBoder;
    QLabel *lbLeftupTitle;
    QLabel *lbUpback;
    QLineEdit *leNetName;
    QLabel *lbAuth;
    QLabel *lbUserName;
    QLabel *lbConn;
    QComboBox *cbxConn;
    QLabel *lbBottomback;
    QPushButton *btnConnect;
    QComboBox *cbxSecurity;
    QLineEdit *leUserName;
    QLineEdit *lePassword;
    QLabel *lbNetName;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbSecurity;
    QLabel *lbPassword;
    QComboBox *cbxAuth;

    void setupUi(QDialog *DlgConnHidWifiSecPwd)
    {
        if (DlgConnHidWifiSecPwd->objectName().isEmpty())
            DlgConnHidWifiSecPwd->setObjectName(QStringLiteral("DlgConnHidWifiSecPwd"));
        DlgConnHidWifiSecPwd->resize(432, 471);
        checkBox = new QCheckBox(DlgConnHidWifiSecPwd);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(336, 346, 18, 9));
        QFont font;
        font.setPointSize(10);
        checkBox->setFont(font);
        btnCancel = new QPushButton(DlgConnHidWifiSecPwd);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 425, 90, 30));
        btnCancel->setFont(font);
        lbBoder = new QLabel(DlgConnHidWifiSecPwd);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 471));
        lbLeftupTitle = new QLabel(DlgConnHidWifiSecPwd);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbUpback = new QLabel(DlgConnHidWifiSecPwd);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        leNetName = new QLineEdit(DlgConnHidWifiSecPwd);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(175, 150, 182, 32));
        leNetName->setFont(font);
        lbAuth = new QLabel(DlgConnHidWifiSecPwd);
        lbAuth->setObjectName(QStringLiteral("lbAuth"));
        lbAuth->setGeometry(QRect(76, 246, 90, 20));
        lbAuth->setFont(font);
        lbUserName = new QLabel(DlgConnHidWifiSecPwd);
        lbUserName->setObjectName(QStringLiteral("lbUserName"));
        lbUserName->setGeometry(QRect(76, 291, 90, 20));
        lbUserName->setFont(font);
        lbConn = new QLabel(DlgConnHidWifiSecPwd);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        cbxConn = new QComboBox(DlgConnHidWifiSecPwd);
        cbxConn->setObjectName(QStringLiteral("cbxConn"));
        cbxConn->setGeometry(QRect(175, 55, 182, 32));
        cbxConn->setFont(font);
        cbxConn->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbBottomback = new QLabel(DlgConnHidWifiSecPwd);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 411, 432, 60));
        btnConnect = new QPushButton(DlgConnHidWifiSecPwd);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 425, 90, 30));
        btnConnect->setFont(font);
        cbxSecurity = new QComboBox(DlgConnHidWifiSecPwd);
        cbxSecurity->setObjectName(QStringLiteral("cbxSecurity"));
        cbxSecurity->setGeometry(QRect(175, 195, 182, 32));
        cbxSecurity->setFont(font);
        cbxSecurity->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        leUserName = new QLineEdit(DlgConnHidWifiSecPwd);
        leUserName->setObjectName(QStringLiteral("leUserName"));
        leUserName->setGeometry(QRect(175, 285, 182, 32));
        leUserName->setFont(font);
        lePassword = new QLineEdit(DlgConnHidWifiSecPwd);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(175, 330, 182, 32));
        lePassword->setFont(font);
        lePassword->setEchoMode(QLineEdit::Password);
        lbNetName = new QLabel(DlgConnHidWifiSecPwd);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 156, 90, 20));
        lbNetName->setFont(font);
        lbLeftup = new QLabel(DlgConnHidWifiSecPwd);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifiSecPwd);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbSecurity = new QLabel(DlgConnHidWifiSecPwd);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 201, 90, 20));
        lbSecurity->setFont(font);
        lbPassword = new QLabel(DlgConnHidWifiSecPwd);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(76, 336, 90, 20));
        lbPassword->setFont(font);
        cbxAuth = new QComboBox(DlgConnHidWifiSecPwd);
        cbxAuth->setObjectName(QStringLiteral("cbxAuth"));
        cbxAuth->setGeometry(QRect(175, 240, 182, 32));
        cbxAuth->setFont(font);
        cbxAuth->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbUpback->raise();
        lbBottomback->raise();
        lbBoder->raise();
        checkBox->raise();
        btnCancel->raise();
        leNetName->raise();
        lbAuth->raise();
        lbUserName->raise();
        lbConn->raise();
        cbxConn->raise();
        btnConnect->raise();
        cbxSecurity->raise();
        leUserName->raise();
        lePassword->raise();
        lbNetName->raise();
        lbLeftup->raise();
        lbSecurity->raise();
        lbPassword->raise();
        cbxAuth->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiSecPwd);

        QMetaObject::connectSlotsByName(DlgConnHidWifiSecPwd);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiSecPwd)
    {
        DlgConnHidWifiSecPwd->setWindowTitle(QApplication::translate("DlgConnHidWifiSecPwd", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        checkBox->setText(QString());
        btnCancel->setText(QString());
        lbBoder->setText(QString());
        lbLeftupTitle->setText(QString());
        lbUpback->setText(QString());
        lbAuth->setText(QString());
        lbUserName->setText(QString());
        lbConn->setText(QString());
        lbBottomback->setText(QString());
        btnConnect->setText(QString());
        lbNetName->setText(QString());
        lbLeftup->setText(QString());
        lbLeftupIcon->setText(QString());
        lbSecurity->setText(QString());
        lbPassword->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiSecPwd: public Ui_DlgConnHidWifiSecPwd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFISECPWD_H
