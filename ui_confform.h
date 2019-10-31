/********************************************************************************
** Form generated from reading UI file 'confform.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFFORM_H
#define UI_CONFFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfForm
{
public:
    QWidget *centralWidget;
    QWidget *wgManual;
    QLineEdit *leAddr;
    QComboBox *cbMask;
    QLineEdit *leGateway;
    QLineEdit *leDns;
    QLabel *lbTxt2;
    QLabel *lbTxt3;
    QLabel *lbTxt4;
    QLabel *lbTxt5;
    QLineEdit *leDns2;
    QLabel *lbTxt6;
    QLabel *lbName;
    QWidget *wdHead;
    QComboBox *cbType;
    QWidget *wdLeftup;
    QLabel *lbIcon;
    QLabel *lbTitle;
    QLabel *lbTxt1;
    QLabel *lbTraingle;
    QWidget *wdBottom;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QWidget *ConfForm)
    {
        if (ConfForm->objectName().isEmpty())
            ConfForm->setObjectName(QStringLiteral("ConfForm"));
        ConfForm->resize(432, 442);
        centralWidget = new QWidget(ConfForm);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setGeometry(QRect(0, 0, 432, 442));
        wgManual = new QWidget(centralWidget);
        wgManual->setObjectName(QStringLiteral("wgManual"));
        wgManual->setGeometry(QRect(1, 131, 430, 250));
        leAddr = new QLineEdit(wgManual);
        leAddr->setObjectName(QStringLiteral("leAddr"));
        leAddr->setGeometry(QRect(175, 20, 182, 32));
        cbMask = new QComboBox(wgManual);
        cbMask->setObjectName(QStringLiteral("cbMask"));
        cbMask->setGeometry(QRect(175, 65, 182, 32));
        leGateway = new QLineEdit(wgManual);
        leGateway->setObjectName(QStringLiteral("leGateway"));
        leGateway->setGeometry(QRect(175, 110, 182, 32));
        leDns = new QLineEdit(wgManual);
        leDns->setObjectName(QStringLiteral("leDns"));
        leDns->setGeometry(QRect(175, 155, 182, 32));
        lbTxt2 = new QLabel(wgManual);
        lbTxt2->setObjectName(QStringLiteral("lbTxt2"));
        lbTxt2->setGeometry(QRect(76, 26, 80, 20));
        lbTxt3 = new QLabel(wgManual);
        lbTxt3->setObjectName(QStringLiteral("lbTxt3"));
        lbTxt3->setGeometry(QRect(76, 71, 80, 20));
        lbTxt4 = new QLabel(wgManual);
        lbTxt4->setObjectName(QStringLiteral("lbTxt4"));
        lbTxt4->setGeometry(QRect(76, 116, 80, 20));
        lbTxt5 = new QLabel(wgManual);
        lbTxt5->setObjectName(QStringLiteral("lbTxt5"));
        lbTxt5->setGeometry(QRect(76, 161, 80, 20));
        leDns2 = new QLineEdit(wgManual);
        leDns2->setObjectName(QStringLiteral("leDns2"));
        leDns2->setGeometry(QRect(175, 200, 182, 32));
        lbTxt6 = new QLabel(wgManual);
        lbTxt6->setObjectName(QStringLiteral("lbTxt6"));
        lbTxt6->setGeometry(QRect(76, 206, 80, 20));
        lbName = new QLabel(centralWidget);
        lbName->setObjectName(QStringLiteral("lbName"));
        lbName->setGeometry(QRect(151, 1, 100, 32));
        wdHead = new QWidget(centralWidget);
        wdHead->setObjectName(QStringLiteral("wdHead"));
        wdHead->setGeometry(QRect(1, 1, 430, 116));
        cbType = new QComboBox(wdHead);
        cbType->setObjectName(QStringLiteral("cbType"));
        cbType->setGeometry(QRect(175, 55, 182, 32));
        wdLeftup = new QWidget(wdHead);
        wdLeftup->setObjectName(QStringLiteral("wdLeftup"));
        wdLeftup->setGeometry(QRect(0, 0, 100, 32));
        lbIcon = new QLabel(wdLeftup);
        lbIcon->setObjectName(QStringLiteral("lbIcon"));
        lbIcon->setGeometry(QRect(9, 9, 16, 15));
        lbTitle = new QLabel(wdLeftup);
        lbTitle->setObjectName(QStringLiteral("lbTitle"));
        lbTitle->setGeometry(QRect(34, 6, 60, 20));
        lbTxt1 = new QLabel(wdHead);
        lbTxt1->setObjectName(QStringLiteral("lbTxt1"));
        lbTxt1->setGeometry(QRect(76, 60, 80, 20));
        lbTraingle = new QLabel(centralWidget);
        lbTraingle->setObjectName(QStringLiteral("lbTraingle"));
        lbTraingle->setGeometry(QRect(204, 117, 22, 14));
        wdBottom = new QWidget(centralWidget);
        wdBottom->setObjectName(QStringLiteral("wdBottom"));
        wdBottom->setGeometry(QRect(1, 381, 430, 60));
        btnOk = new QPushButton(wdBottom);
        btnOk->setObjectName(QStringLiteral("btnOk"));
        btnOk->setGeometry(QRect(315, 15, 90, 30));
        btnCancel = new QPushButton(wdBottom);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));
        btnCancel->setGeometry(QRect(215, 15, 90, 30));

        retranslateUi(ConfForm);

        QMetaObject::connectSlotsByName(ConfForm);
    } // setupUi

    void retranslateUi(QWidget *ConfForm)
    {
        ConfForm->setWindowTitle(QString());
        lbTxt2->setText(QString());
        lbTxt3->setText(QString());
        lbTxt4->setText(QString());
        lbTxt5->setText(QString());
        lbTxt6->setText(QString());
        lbName->setText(QString());
        lbIcon->setText(QString());
        lbTitle->setText(QString());
        lbTxt1->setText(QString());
        lbTraingle->setText(QString());
        btnOk->setText(QString());
        btnCancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ConfForm: public Ui_ConfForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFFORM_H
