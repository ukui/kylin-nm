/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifisecleap.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFISECLEAP_H
#define UI_DLGCONNHIDWIFISECLEAP_H

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

class Ui_DlgConnHidWifiSecLeap
{
public:
    QComboBox *cbxSecurity;
    QLineEdit *lePassword;
    QCheckBox *checkBox;
    QLabel *lbNetName;
    QLabel *lbSecurity;
    QLineEdit *leUserName;
    QPushButton *btnConnect;
    QComboBox *cbxConn;
    QLabel *lbUserName;
    QPushButton *btnCancel;
    QComboBox *cbxAuth;
    QLabel *lbAuth;
    QLineEdit *leNetName;
    QLabel *lbPassword;
    QLabel *lbConn;
    QLabel *lbLeftupTitle;
    QLabel *lbLeftupIcon;
    QLabel *lbUpback;
    QLabel *lbLeftup;
    QLabel *lbBoder;
    QLabel *lbBottomback;

    void setupUi(QDialog *DlgConnHidWifiSecLeap)
    {
        if (DlgConnHidWifiSecLeap->objectName().isEmpty())
            DlgConnHidWifiSecLeap->setObjectName(QStringLiteral("DlgConnHidWifiSecLeap"));
        DlgConnHidWifiSecLeap->resize(432, 471);
        cbxSecurity = new QComboBox(DlgConnHidWifiSecLeap);
        cbxSecurity->setObjectName(QStringLiteral("cbxSecurity"));
        cbxSecurity->setGeometry(QRect(175, 195, 182, 32));
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
        lePassword = new QLineEdit(DlgConnHidWifiSecLeap);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(175, 330, 182, 32));
        lePassword->setFont(font);
        lePassword->setEchoMode(QLineEdit::Password);
        checkBox = new QCheckBox(DlgConnHidWifiSecLeap);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(332, 342, 18, 9));
        checkBox->setFont(font);
        lbNetName = new QLabel(DlgConnHidWifiSecLeap);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 156, 90, 20));
        lbNetName->setFont(font);
        lbSecurity = new QLabel(DlgConnHidWifiSecLeap);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 201, 90, 20));
        lbSecurity->setFont(font);
        leUserName = new QLineEdit(DlgConnHidWifiSecLeap);
        leUserName->setObjectName(QStringLiteral("leUserName"));
        leUserName->setGeometry(QRect(175, 285, 182, 32));
        leUserName->setFont(font);
        btnConnect = new QPushButton(DlgConnHidWifiSecLeap);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 425, 90, 30));
        btnConnect->setFont(font);
        cbxConn = new QComboBox(DlgConnHidWifiSecLeap);
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
        lbUserName = new QLabel(DlgConnHidWifiSecLeap);
        lbUserName->setObjectName(QStringLiteral("lbUserName"));
        lbUserName->setGeometry(QRect(76, 291, 90, 20));
        lbUserName->setFont(font);
        btnCancel = new QPushButton(DlgConnHidWifiSecLeap);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 425, 90, 30));
        btnCancel->setFont(font);
        cbxAuth = new QComboBox(DlgConnHidWifiSecLeap);
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
        lbAuth = new QLabel(DlgConnHidWifiSecLeap);
        lbAuth->setObjectName(QStringLiteral("lbAuth"));
        lbAuth->setGeometry(QRect(76, 246, 90, 20));
        lbAuth->setFont(font);
        leNetName = new QLineEdit(DlgConnHidWifiSecLeap);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(175, 150, 182, 32));
        leNetName->setFont(font);
        lbPassword = new QLabel(DlgConnHidWifiSecLeap);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(76, 336, 90, 20));
        lbPassword->setFont(font);
        lbConn = new QLabel(DlgConnHidWifiSecLeap);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        lbLeftupTitle = new QLabel(DlgConnHidWifiSecLeap);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbLeftupIcon = new QLabel(DlgConnHidWifiSecLeap);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbUpback = new QLabel(DlgConnHidWifiSecLeap);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbLeftup = new QLabel(DlgConnHidWifiSecLeap);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbBoder = new QLabel(DlgConnHidWifiSecLeap);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 471));
        lbBottomback = new QLabel(DlgConnHidWifiSecLeap);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 411, 432, 60));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        cbxSecurity->raise();
        lePassword->raise();
        checkBox->raise();
        lbNetName->raise();
        lbSecurity->raise();
        leUserName->raise();
        btnConnect->raise();
        cbxConn->raise();
        lbUserName->raise();
        btnCancel->raise();
        cbxAuth->raise();
        lbAuth->raise();
        leNetName->raise();
        lbPassword->raise();
        lbConn->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiSecLeap);

        QMetaObject::connectSlotsByName(DlgConnHidWifiSecLeap);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiSecLeap)
    {
        DlgConnHidWifiSecLeap->setWindowTitle(QApplication::translate("DlgConnHidWifiSecLeap", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        checkBox->setText(QString());
        lbNetName->setText(QString());
        lbSecurity->setText(QString());
        btnConnect->setText(QString());
        lbUserName->setText(QString());
        btnCancel->setText(QString());
        lbAuth->setText(QString());
        lbPassword->setText(QString());
        lbConn->setText(QString());
        lbLeftupTitle->setText(QString());
        lbLeftupIcon->setText(QString());
        lbUpback->setText(QString());
        lbLeftup->setText(QString());
        lbBoder->setText(QString());
        lbBottomback->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiSecLeap: public Ui_DlgConnHidWifiSecLeap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFISECLEAP_H
