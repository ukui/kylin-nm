/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifiwep.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFIWEP_H
#define UI_DLGCONNHIDWIFIWEP_H

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

class Ui_DlgConnHidWifiWep
{
public:
    QLabel *lbKey;
    QLabel *lbWEPindex;
    QLabel *lbAuth;
    QComboBox *cbxSecurity;
    QPushButton *btnConnect;
    QComboBox *cbxAuth;
    QLabel *lbConn;
    QComboBox *cbxWEPindex;
    QPushButton *btnCancel;
    QCheckBox *checkBoxPwd;
    QLabel *lbNetName;
    QLineEdit *leKey;
    QLineEdit *leNetName;
    QLabel *lbSecurity;
    QComboBox *cbxConn;
    QLabel *lbBoder;
    QLabel *lbUpback;
    QLabel *lbBottomback;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbLeftupTitle;

    void setupUi(QDialog *DlgConnHidWifiWep)
    {
        if (DlgConnHidWifiWep->objectName().isEmpty())
            DlgConnHidWifiWep->setObjectName(QStringLiteral("DlgConnHidWifiWep"));
        DlgConnHidWifiWep->resize(432, 471);
        lbKey = new QLabel(DlgConnHidWifiWep);
        lbKey->setObjectName(QStringLiteral("lbKey"));
        lbKey->setGeometry(QRect(76, 246, 90, 20));
        QFont font;
        font.setPointSize(10);
        lbKey->setFont(font);
        lbWEPindex = new QLabel(DlgConnHidWifiWep);
        lbWEPindex->setObjectName(QStringLiteral("lbWEPindex"));
        lbWEPindex->setGeometry(QRect(76, 291, 90, 20));
        lbWEPindex->setFont(font);
        lbAuth = new QLabel(DlgConnHidWifiWep);
        lbAuth->setObjectName(QStringLiteral("lbAuth"));
        lbAuth->setGeometry(QRect(76, 336, 90, 20));
        lbAuth->setFont(font);
        cbxSecurity = new QComboBox(DlgConnHidWifiWep);
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
        btnConnect = new QPushButton(DlgConnHidWifiWep);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 425, 90, 30));
        btnConnect->setFont(font);
        cbxAuth = new QComboBox(DlgConnHidWifiWep);
        cbxAuth->setObjectName(QStringLiteral("cbxAuth"));
        cbxAuth->setGeometry(QRect(175, 330, 182, 32));
        cbxAuth->setFont(font);
        cbxAuth->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbConn = new QLabel(DlgConnHidWifiWep);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font);
        lbConn->setLayoutDirection(Qt::LeftToRight);
        cbxWEPindex = new QComboBox(DlgConnHidWifiWep);
        cbxWEPindex->setObjectName(QStringLiteral("cbxWEPindex"));
        cbxWEPindex->setGeometry(QRect(175, 285, 182, 32));
        cbxWEPindex->setFont(font);
        cbxWEPindex->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        btnCancel = new QPushButton(DlgConnHidWifiWep);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 425, 90, 30));
        btnCancel->setFont(font);
        checkBoxPwd = new QCheckBox(DlgConnHidWifiWep);
        checkBoxPwd->setObjectName(QStringLiteral("checkBoxPwd"));
        checkBoxPwd->setGeometry(QRect(332, 252, 18, 9));
        checkBoxPwd->setFont(font);
        lbNetName = new QLabel(DlgConnHidWifiWep);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 156, 90, 20));
        lbNetName->setFont(font);
        leKey = new QLineEdit(DlgConnHidWifiWep);
        leKey->setObjectName(QStringLiteral("leKey"));
        leKey->setGeometry(QRect(175, 240, 182, 32));
        leKey->setFont(font);
        leKey->setEchoMode(QLineEdit::Password);
        leNetName = new QLineEdit(DlgConnHidWifiWep);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(175, 150, 182, 32));
        leNetName->setFont(font);
        lbSecurity = new QLabel(DlgConnHidWifiWep);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 201, 90, 20));
        lbSecurity->setFont(font);
        cbxConn = new QComboBox(DlgConnHidWifiWep);
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
        lbBoder = new QLabel(DlgConnHidWifiWep);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 471));
        lbUpback = new QLabel(DlgConnHidWifiWep);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbBottomback = new QLabel(DlgConnHidWifiWep);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 411, 432, 60));
        lbLeftup = new QLabel(DlgConnHidWifiWep);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifiWep);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbLeftupTitle = new QLabel(DlgConnHidWifiWep);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        leKey->raise();
        lbKey->raise();
        lbWEPindex->raise();
        lbAuth->raise();
        cbxSecurity->raise();
        btnConnect->raise();
        cbxAuth->raise();
        lbConn->raise();
        cbxWEPindex->raise();
        btnCancel->raise();
        checkBoxPwd->raise();
        lbNetName->raise();
        leNetName->raise();
        lbSecurity->raise();
        cbxConn->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiWep);

        QMetaObject::connectSlotsByName(DlgConnHidWifiWep);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiWep)
    {
        DlgConnHidWifiWep->setWindowTitle(QApplication::translate("DlgConnHidWifiWep", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        lbKey->setText(QString());
        lbWEPindex->setText(QString());
        lbAuth->setText(QString());
        btnConnect->setText(QString());
        lbConn->setText(QString());
        btnCancel->setText(QString());
        checkBoxPwd->setText(QString());
        lbNetName->setText(QString());
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
    class DlgConnHidWifiWep: public Ui_DlgConnHidWifiWep {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFIWEP_H
