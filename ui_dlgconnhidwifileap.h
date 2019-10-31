/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifileap.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFILEAP_H
#define UI_DLGCONNHIDWIFILEAP_H

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

class Ui_DlgConnHidWifiLeap
{
public:
    QLineEdit *leUserName;
    QComboBox *cbxSecurity;
    QPushButton *btnCancel;
    QLabel *lbConn;
    QComboBox *cbxConn;
    QLabel *lbUserName;
    QCheckBox *checkBoxPwd;
    QLabel *lbPassword;
    QLabel *lbNetName;
    QLabel *lbSecurity;
    QPushButton *btnConnect;
    QLineEdit *lePassword;
    QLineEdit *leNetName;
    QLabel *lbUpback;
    QLabel *lbBottomback;
    QLabel *lbBoder;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbLeftupTitle;

    void setupUi(QDialog *DlgConnHidWifiLeap)
    {
        if (DlgConnHidWifiLeap->objectName().isEmpty())
            DlgConnHidWifiLeap->setObjectName(QStringLiteral("DlgConnHidWifiLeap"));
        DlgConnHidWifiLeap->resize(432, 412);
        leUserName = new QLineEdit(DlgConnHidWifiLeap);
        leUserName->setObjectName(QStringLiteral("leUserName"));
        leUserName->setGeometry(QRect(175, 240, 182, 32));
        QFont font;
        font.setPointSize(10);
        leUserName->setFont(font);
        cbxSecurity = new QComboBox(DlgConnHidWifiLeap);
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
        btnCancel = new QPushButton(DlgConnHidWifiLeap);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 366, 90, 30));
        btnCancel->setFont(font);
        lbConn = new QLabel(DlgConnHidWifiLeap);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        lbConn->setLayoutDirection(Qt::LeftToRight);
        cbxConn = new QComboBox(DlgConnHidWifiLeap);
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
        lbUserName = new QLabel(DlgConnHidWifiLeap);
        lbUserName->setObjectName(QStringLiteral("lbUserName"));
        lbUserName->setGeometry(QRect(76, 246, 90, 20));
        lbUserName->setFont(font);
        checkBoxPwd = new QCheckBox(DlgConnHidWifiLeap);
        checkBoxPwd->setObjectName(QStringLiteral("checkBoxPwd"));
        checkBoxPwd->setGeometry(QRect(332, 297, 18, 9));
        checkBoxPwd->setFont(font);
        lbPassword = new QLabel(DlgConnHidWifiLeap);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(76, 291, 90, 20));
        lbPassword->setFont(font);
        lbNetName = new QLabel(DlgConnHidWifiLeap);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 156, 90, 20));
        lbNetName->setFont(font);
        lbSecurity = new QLabel(DlgConnHidWifiLeap);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 201, 90, 20));
        lbSecurity->setFont(font);
        btnConnect = new QPushButton(DlgConnHidWifiLeap);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 366, 90, 30));
        btnConnect->setFont(font);
        lePassword = new QLineEdit(DlgConnHidWifiLeap);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(175, 285, 182, 32));
        lePassword->setFont(font);
        lePassword->setEchoMode(QLineEdit::Password);
        leNetName = new QLineEdit(DlgConnHidWifiLeap);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(175, 150, 182, 32));
        leNetName->setFont(font);
        lbUpback = new QLabel(DlgConnHidWifiLeap);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbBottomback = new QLabel(DlgConnHidWifiLeap);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 352, 432, 60));
        lbBoder = new QLabel(DlgConnHidWifiLeap);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 412));
        lbLeftup = new QLabel(DlgConnHidWifiLeap);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifiLeap);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbLeftupTitle = new QLabel(DlgConnHidWifiLeap);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        leUserName->raise();
        cbxSecurity->raise();
        btnCancel->raise();
        lbConn->raise();
        cbxConn->raise();
        lbUserName->raise();
        lbPassword->raise();
        lbNetName->raise();
        lbSecurity->raise();
        btnConnect->raise();
        lePassword->raise();
        leNetName->raise();
        checkBoxPwd->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiLeap);

        QMetaObject::connectSlotsByName(DlgConnHidWifiLeap);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiLeap)
    {
        DlgConnHidWifiLeap->setWindowTitle(QApplication::translate("DlgConnHidWifiLeap", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        btnCancel->setText(QString());
        lbConn->setText(QString());
        lbUserName->setText(QString());
        checkBoxPwd->setText(QString());
        lbPassword->setText(QString());
        lbNetName->setText(QString());
        lbSecurity->setText(QString());
        btnConnect->setText(QString());
        lbUpback->setText(QString());
        lbBottomback->setText(QString());
        lbBoder->setText(QString());
        lbLeftup->setText(QString());
        lbLeftupIcon->setText(QString());
        lbLeftupTitle->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiLeap: public Ui_DlgConnHidWifiLeap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFILEAP_H
