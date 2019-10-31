/********************************************************************************
** Form generated from reading UI file 'dlgconnhidwifi.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGCONNHIDWIFI_H
#define UI_DLGCONNHIDWIFI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DlgConnHidWifi
{
public:
    QLabel *lbConn;
    QComboBox *cbxConn;
    QLabel *lbNetName;
    QComboBox *cbxSecurity;
    QLabel *lbSecurity;
    QLineEdit *leNetName;
    QPushButton *btnConnect;
    QPushButton *btnCancel;
    QLabel *lbUpback;
    QLabel *lbLeftup;
    QLabel *lbLeftupIcon;
    QLabel *lbLeftupTitle;
    QLabel *lbBottomback;
    QLabel *lbBoder;

    void setupUi(QDialog *DlgConnHidWifi)
    {
        if (DlgConnHidWifi->objectName().isEmpty())
            DlgConnHidWifi->setObjectName(QStringLiteral("DlgConnHidWifi"));
        DlgConnHidWifi->resize(432, 336);
        lbConn = new QLabel(DlgConnHidWifi);
        lbConn->setObjectName(QStringLiteral("lbConn"));
        lbConn->setGeometry(QRect(76, 60, 90, 20));
        QFont font;
        font.setPointSize(10);
        lbConn->setFont(font);
        lbConn->setLayoutDirection(Qt::LeftToRight);
        cbxConn = new QComboBox(DlgConnHidWifi);
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
        lbNetName = new QLabel(DlgConnHidWifi);
        lbNetName->setObjectName(QStringLiteral("lbNetName"));
        lbNetName->setGeometry(QRect(76, 156, 90, 20));
        lbNetName->setFont(font);
        cbxSecurity = new QComboBox(DlgConnHidWifi);
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
        lbSecurity = new QLabel(DlgConnHidWifi);
        lbSecurity->setObjectName(QStringLiteral("lbSecurity"));
        lbSecurity->setGeometry(QRect(76, 201, 90, 20));
        lbSecurity->setFont(font);
        leNetName = new QLineEdit(DlgConnHidWifi);
        leNetName->setObjectName(QStringLiteral("leNetName"));
        leNetName->setGeometry(QRect(175, 150, 182, 32));
        leNetName->setFont(font);
        btnConnect = new QPushButton(DlgConnHidWifi);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setGeometry(QRect(315, 290, 90, 30));
        btnConnect->setFont(font);
        btnCancel = new QPushButton(DlgConnHidWifi);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 290, 90, 30));
        btnCancel->setFont(font);
        lbUpback = new QLabel(DlgConnHidWifi);
        lbUpback->setObjectName(QStringLiteral("lbUpback"));
        lbUpback->setGeometry(QRect(0, 0, 432, 116));
        lbLeftup = new QLabel(DlgConnHidWifi);
        lbLeftup->setObjectName(QStringLiteral("lbLeftup"));
        lbLeftup->setGeometry(QRect(0, 0, 120, 32));
        lbLeftupIcon = new QLabel(DlgConnHidWifi);
        lbLeftupIcon->setObjectName(QStringLiteral("lbLeftupIcon"));
        lbLeftupIcon->setGeometry(QRect(9, 9, 16, 16));
        lbLeftupTitle = new QLabel(DlgConnHidWifi);
        lbLeftupTitle->setObjectName(QStringLiteral("lbLeftupTitle"));
        lbLeftupTitle->setGeometry(QRect(34, 6, 80, 20));
        QFont font1;
        font1.setPointSize(9);
        lbLeftupTitle->setFont(font1);
        lbBottomback = new QLabel(DlgConnHidWifi);
        lbBottomback->setObjectName(QStringLiteral("lbBottomback"));
        lbBottomback->setGeometry(QRect(0, 276, 432, 60));
        lbBoder = new QLabel(DlgConnHidWifi);
        lbBoder->setObjectName(QStringLiteral("lbBoder"));
        lbBoder->setGeometry(QRect(0, 0, 432, 336));
        lbBottomback->raise();
        lbUpback->raise();
        lbBoder->raise();
        lbLeftup->raise();
        lbConn->raise();
        cbxConn->raise();
        lbNetName->raise();
        cbxSecurity->raise();
        lbSecurity->raise();
        leNetName->raise();
        btnConnect->raise();
        btnCancel->raise();
        lbLeftupIcon->raise();
        lbLeftupTitle->raise();

        retranslateUi(DlgConnHidWifi);

        QMetaObject::connectSlotsByName(DlgConnHidWifi);
    } // setupUi

    void retranslateUi(QDialog *DlgConnHidWifi)
    {
        DlgConnHidWifi->setWindowTitle(QApplication::translate("DlgConnHidWifi", "Connect to Hidden Wi-Fi Network", Q_NULLPTR));
        lbConn->setText(QString());
        lbNetName->setText(QString());
        lbSecurity->setText(QString());
        btnConnect->setText(QString());
        btnCancel->setText(QString());
        lbUpback->setText(QString());
        lbLeftup->setText(QString());
        lbLeftupIcon->setText(QString());
        lbLeftupTitle->setText(QString());
        lbBottomback->setText(QString());
        lbBoder->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DlgConnHidWifi: public Ui_DlgConnHidWifi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGCONNHIDWIFI_H
