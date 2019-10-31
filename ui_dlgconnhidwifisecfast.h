/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifisecfast.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFISECFAST_H
#define UI_DLGCONNHIDWIFISECFAST_H

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

class Ui_DlgConnHidWifiSecFast
{
public:
    QCheckBox *checkBoxPwd;
    QLabel *lbNetName;
    QLineEdit *leNetName;
    QLabel *lbConn;
    QComboBox *cbxAuth;
    QLineEdit *lePassword;
    QLineEdit *leUserName;
    QLabel *lbUserName;
    QComboBox *cbxSecurity;
    QLabel *lbPassword;
    QPushButton *btnConnect;
    QComboBox *cbxConn;
    QLabel *lbAuth;
    QLabel *lbSecurity;
    QPushButton *btnCancel;
    QLabel *lbAnonyId;
    QLineEdit *leAnonyId;
    QCheckBox *checkBoxAutoPCA;
    QComboBox *cbxAutoPCA;
    QLabel *lbPCAfile;
    QLineEdit *lePCAfile;
    QLabel *lbInnerAuth;
    QComboBox *cbxInnerAuth;
    QLabel *lbLeftupTitle;
    QLabel *lbLeftupIcon;
    QLabel *lbBoder;
    QLabel *lbUpback;
    QLabel *lbLeftup;
    QLabel *lbBottomback;

    void setupUi(QDialog *DlgConnHidWifiSecFast)
    {
        if (DlgConnHidWifiSecFast->objectName().isEmpty())
            DlgConnHidWifiSecFast->setObjectName(QStringLiteral("DlgConnHidWifiSecFast"));
        DlgConnHidWifiSecFast->resize(432, 651);
        checkBoxPwd = new QCheckBox(DlgConnHidWifiSecFast);
        checkBoxPwd->setObjectName(QStringLiteral("checkBoxPwd"));
        checkBoxPwd->setGeometry(QRect(332, 522, 18, 9));
        QFont font;
        font.setPointSize(11);
        checkBoxPwd->setFont(font);
        lbNetName = new QLabel(DlgConnHidWifiSecFast);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 156, 90, 20));
        QFont font1;
        font1.setPointSize(10);
        lbNetName->setFont(font1);
        leNetName = new QLineEdit(DlgConnHidWifiSecFast);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(180, 150, 182, 30));
        leNetName->setFont(font1);
        lbConn = new QLabel(DlgConnHidWifiSecFast);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        lbConn->setFont(font1);
        cbxAuth = new QComboBox(DlgConnHidWifiSecFast);
        cbxAuth->setObjectName(QStringLiteral("cbxAuth"));
        cbxAuth->setGeometry(QRect(180, 240, 182, 30));
        cbxAuth->setFont(font1);
        cbxAuth->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lePassword = new QLineEdit(DlgConnHidWifiSecFast);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(180, 510, 182, 32));
        lePassword->setFont(font1);
        lePassword->setEchoMode(QLineEdit::Password);
        leUserName = new QLineEdit(DlgConnHidWifiSecFast);
        leUserName->setObjectName(QStringLiteral("leUserName"));
        leUserName->setGeometry(QRect(180, 465, 182, 30));
        leUserName->setFont(font1);
        lbUserName = new QLabel(DlgConnHidWifiSecFast);
        lbUserName->setObjectName(QStringLiteral("lbUserName"));
        lbUserName->setGeometry(QRect(76, 471, 90, 20));
        lbUserName->setFont(font1);
        cbxSecurity = new QComboBox(DlgConnHidWifiSecFast);
        cbxSecurity->setObjectName(QStringLiteral("cbxSecurity"));
        cbxSecurity->setGeometry(QRect(180, 195, 182, 30));
        cbxSecurity->setFont(font1);
        cbxSecurity->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbPassword = new QLabel(DlgConnHidWifiSecFast);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(76, 516, 90, 20));
        lbPassword->setFont(font1);
        btnConnect = new QPushButton(DlgConnHidWifiSecFast);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 605, 90, 30));
        btnConnect->setFont(font1);
        cbxConn = new QComboBox(DlgConnHidWifiSecFast);
        cbxConn->setObjectName(QStringLiteral("cbxConn"));
        cbxConn->setGeometry(QRect(180, 55, 182, 32));
        cbxConn->setFont(font1);
        cbxConn->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbAuth = new QLabel(DlgConnHidWifiSecFast);
        lbAuth->setObjectName(QStringLiteral("lbAuth"));
        lbAuth->setGeometry(QRect(76, 246, 90, 20));
        lbAuth->setFont(font1);
        lbSecurity = new QLabel(DlgConnHidWifiSecFast);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 201, 90, 20));
        lbSecurity->setFont(font1);
        btnCancel = new QPushButton(DlgConnHidWifiSecFast);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 605, 90, 30));
        btnCancel->setFont(font1);
        lbAnonyId = new QLabel(DlgConnHidWifiSecFast);
        lbAnonyId->setObjectName(QStringLiteral("lbAnonyId"));
        lbAnonyId->setGeometry(QRect(76, 291, 90, 20));
        lbAnonyId->setFont(font1);
        leAnonyId = new QLineEdit(DlgConnHidWifiSecFast);
        leAnonyId->setObjectName(QStringLiteral("leAnonyId"));
        leAnonyId->setGeometry(QRect(180, 285, 182, 32));
        leAnonyId->setFont(font1);
        checkBoxAutoPCA = new QCheckBox(DlgConnHidWifiSecFast);
        checkBoxAutoPCA->setObjectName(QStringLiteral("checkBoxAutoPCA"));
        checkBoxAutoPCA->setGeometry(QRect(76, 336, 95, 25));
        checkBoxAutoPCA->setFont(font1);
        cbxAutoPCA = new QComboBox(DlgConnHidWifiSecFast);
        cbxAutoPCA->setObjectName(QStringLiteral("cbxAutoPCA"));
        cbxAutoPCA->setGeometry(QRect(180, 330, 182, 32));
        cbxAutoPCA->setFont(font1);
        cbxAutoPCA->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbPCAfile = new QLabel(DlgConnHidWifiSecFast);
        lbPCAfile->setObjectName(QStringLiteral("lbPCAfile"));
        lbPCAfile->setGeometry(QRect(76, 381, 90, 20));
        lbPCAfile->setFont(font1);
        lePCAfile = new QLineEdit(DlgConnHidWifiSecFast);
        lePCAfile->setObjectName(QStringLiteral("lePCAfile"));
        lePCAfile->setGeometry(QRect(180, 375, 182, 32));
        lePCAfile->setFont(font1);
        lbInnerAuth = new QLabel(DlgConnHidWifiSecFast);
        lbInnerAuth->setObjectName(QStringLiteral("lbInnerAuth"));
        lbInnerAuth->setGeometry(QRect(76, 426, 90, 20));
        lbInnerAuth->setFont(font1);
        cbxInnerAuth = new QComboBox(DlgConnHidWifiSecFast);
        cbxInnerAuth->setObjectName(QStringLiteral("cbxInnerAuth"));
        cbxInnerAuth->setGeometry(QRect(180, 420, 182, 32));
        cbxInnerAuth->setFont(font1);
        cbxInnerAuth->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"	outline: 0; \n"
"	border: 1px solid #000000;\n"
"	border-color: rgba(0, 0, 0,25%);\n"
"	selection-color: rgb(0, 0, 0);\n"
"	background-color: #F1F3F3;\n"
"}"));
        lbLeftupTitle = new QLabel(DlgConnHidWifiSecFast);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        QFont font2;
        font2.setPointSize(9);
        lbLeftupTitle->setFont(font2);
        lbLeftupIcon = new QLabel(DlgConnHidWifiSecFast);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbBoder = new QLabel(DlgConnHidWifiSecFast);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 651));
        lbUpback = new QLabel(DlgConnHidWifiSecFast);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbLeftup = new QLabel(DlgConnHidWifiSecFast);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbBottomback = new QLabel(DlgConnHidWifiSecFast);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 591, 432, 60));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        lbNetName->raise();
        leNetName->raise();
        lbConn->raise();
        cbxAuth->raise();
        lePassword->raise();
        leUserName->raise();
        lbUserName->raise();
        cbxSecurity->raise();
        lbPassword->raise();
        btnConnect->raise();
        cbxConn->raise();
        lbAuth->raise();
        lbSecurity->raise();
        btnCancel->raise();
        lbAnonyId->raise();
        leAnonyId->raise();
        checkBoxAutoPCA->raise();
        cbxAutoPCA->raise();
        lbPCAfile->raise();
        lePCAfile->raise();
        lbInnerAuth->raise();
        cbxInnerAuth->raise();
        checkBoxPwd->raise();
        lbLeftup->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifiSecFast);

        QMetaObject::connectSlotsByName(DlgConnHidWifiSecFast);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifiSecFast)
    {
        DlgConnHidWifiSecFast->setWindowTitle(QApplication::translate("DlgConnHidWifiSecFast", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        checkBoxPwd->setText(QString());
        lbNetName->setText(QString());
        lbConn->setText(QString());
        lbUserName->setText(QString());
        lbPassword->setText(QString());
        btnConnect->setText(QString());
        lbAuth->setText(QString());
        lbSecurity->setText(QString());
        btnCancel->setText(QString());
        lbAnonyId->setText(QString());
        checkBoxAutoPCA->setText(QString());
        lbPCAfile->setText(QString());
        lbInnerAuth->setText(QString());
        lbLeftupTitle->setText(QString());
        lbLeftupIcon->setText(QString());
        lbBoder->setText(QString());
        lbUpback->setText(QString());
        lbLeftup->setText(QString());
        lbBottomback->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifiSecFast: public Ui_DlgConnHidWifiSecFast {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFISECFAST_H
