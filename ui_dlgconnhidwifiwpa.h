/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifiwpa.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFIWPA_H
#define UI_DLGCONNHIDWIFIWPA_H

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

class Ui_DlgConnHidWifiWpa
{
public:
    QLabel *lbNetName;
    QLabel *lbSecurity;
    QPushButton *btnCancel;
    QComboBox *cbxConn;
    QCheckBox *checkBoxPwd;
    QLabel *lbPassword;
    QComboBox *cbxSecurity;
    QLineEdit *lePassword;
    QPushButton *btnConnect;
    QLabel *lbConn;
    QLineEdit *leNetName;
    QLabel *lbBoder;
    QLabel *lbUpback;
    QLabel *lbBottomback;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbLeftupTitle;

    void setupUi(QDialog *DlgConnHidWifiWpa)
    {
        if (DlgConnHidWifiWpa->objectName().isEmpty())
            DlgConnHidWifiWpa->setObjectName(QStringLiteral("DlgConnHidWifiWpa"));
        DlgConnHidWifiWpa->resize(432, 412);
        lbNetName = new QLabel(DlgConnHidWifiWpa);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 156, 90, 20));
        QFont font;
        font.setPointSize(10);
        lbNetName->setFont(font);
        lbSecurity = new QLabel(DlgConnHidWifiWpa);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 201, 90, 20));
        lbSecurity->setFont(font);
        btnCancel = new QPushButton(DlgConnHidWifiWpa);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 367, 90, 30));
        btnCancel->setFont(font);
        cbxConn = new QComboBox(DlgConnHidWifiWpa);
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
        checkBoxPwd = new QCheckBox(DlgConnHidWifiWpa);
        checkBoxPwd->setObjectName(QStringLiteral("checkBoxPwd"));
        checkBoxPwd->setGeometry(QRect(332, 253, 18, 9));
        checkBoxPwd->setFont(font);
        lbPassword = new QLabel(DlgConnHidWifiWpa);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(76, 246, 90, 20));
        lbPassword->setFont(font);
        cbxSecurity = new QComboBox(DlgConnHidWifiWpa);
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
        lePassword = new QLineEdit(DlgConnHidWifiWpa);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(175, 241, 182, 32));
        lePassword->setFont(font);
        lePassword->setEchoMode(QLineEdit::Password);
        btnConnect = new QPushButton(DlgConnHidWifiWpa);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 367, 90, 30));
        btnConnect->setFont(font);
        lbConn = new QLabel(DlgConnHidWifiWpa);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        lbConn->setLayoutDirection(Qt::LeftToRight);
        leNetName = new QLineEdit(DlgConnHidWifiWpa);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(175, 150, 182, 32));
        leNetName->setFont(font);
        lbBoder = new QLabel(DlgConnHidWifiWpa);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 412));
        lbUpback = new QLabel(DlgConnHidWifiWpa);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbBottomback = new QLabel(DlgConnHidWifiWpa);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 352, 432, 60));
        lbLeftup = new QLabel(DlgConnHidWifiWpa);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifiWpa);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbLeftupTitle = new QLabel(DlgConnHidWifiWpa);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        lePassword->raise();
        lbNetName->raise();
        lbSecurity->raise();
        btnCancel->raise();
        cbxConn->raise();
        checkBoxPwd->raise();
        lbPassword->raise();
        cbxSecurity->raise();
        btnConnect->raise();
        lbConn->raise();
        leNetName->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiWpa);

        QMetaObject::connectSlotsByName(DlgConnHidWifiWpa);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiWpa)
    {
        DlgConnHidWifiWpa->setWindowTitle(QApplication::translate("DlgConnHidWifiWpa", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        lbNetName->setText(QString());
        lbSecurity->setText(QString());
        btnCancel->setText(QString());
        checkBoxPwd->setText(QString());
        lbPassword->setText(QString());
        btnConnect->setText(QString());
        lbConn->setText(QString());
        lbBoder->setText(QString());
        lbUpback->setText(QString());
        lbBottomback->setText(QString());
        lbLeftup->setText(QString());
        lbLeftupIcon->setText(QString());
        lbLeftupTitle->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiWpa: public Ui_DlgConnHidWifiWpa {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFIWPA_H
