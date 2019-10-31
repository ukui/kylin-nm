/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *wdgHead;
    QPushButton *btnWifiList;
    QPushButton *btnNetList;
    QLabel *lbBtnNetBG;
    QLabel *lbLanImg;
    QLabel *lbBtnNetT1;
    QLabel *lbBtnNetT2;
    QPushButton *btnNet;
    QLabel *lbBtnWifiBG;
    QLabel *lbWifiImg;
    QLabel *lbBtnWifiT1;
    QLabel *lbBtnWifiT2;
    QPushButton *btnWifi;
    QLabel *lbBtnConfBG;
    QLabel *lbConfImg;
    QLabel *lbBtnConfT2;
    QPushButton *btnAdvConf;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(316, 467);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        wdgHead = new QWidget(centralWidget);
        wdgHead->setObjectName(QStringLiteral("wdgHead"));
        wdgHead->setGeometry(QRect(1, 1, 314, 38));
        btnWifiList = new QPushButton(wdgHead);
        btnWifiList->setObjectName(QStringLiteral("btnWifiList"));
        btnWifiList->setGeometry(QRect(158, 8, 90, 22));
        btnNetList = new QPushButton(wdgHead);
        btnNetList->setObjectName(QStringLiteral("btnNetList"));
        btnNetList->setGeometry(QRect(67, 8, 90, 22));
        lbBtnNetBG = new QLabel(centralWidget);
        lbBtnNetBG->setObjectName(QStringLiteral("lbBtnNetBG"));
        lbBtnNetBG->setGeometry(QRect(4, 403, 90, 60));
        lbLanImg = new QLabel(centralWidget);
        lbLanImg->setObjectName(QStringLiteral("lbLanImg"));
        lbLanImg->setGeometry(QRect(10, 410, 22, 22));
        lbBtnNetT1 = new QLabel(centralWidget);
        lbBtnNetT1->setObjectName(QStringLiteral("lbBtnNetT1"));
        lbBtnNetT1->setGeometry(QRect(40, 412, 55, 18));
        lbBtnNetT2 = new QLabel(centralWidget);
        lbBtnNetT2->setObjectName(QStringLiteral("lbBtnNetT2"));
        lbBtnNetT2->setGeometry(QRect(10, 440, 55, 18));
        btnNet = new QPushButton(centralWidget);
        btnNet->setObjectName(QStringLiteral("btnNet"));
        btnNet->setGeometry(QRect(4, 403, 90, 60));
        lbBtnWifiBG = new QLabel(centralWidget);
        lbBtnWifiBG->setObjectName(QStringLiteral("lbBtnWifiBG"));
        lbBtnWifiBG->setGeometry(QRect(98, 403, 90, 60));
        lbWifiImg = new QLabel(centralWidget);
        lbWifiImg->setObjectName(QStringLiteral("lbWifiImg"));
        lbWifiImg->setGeometry(QRect(103, 410, 22, 22));
        lbBtnWifiT1 = new QLabel(centralWidget);
        lbBtnWifiT1->setObjectName(QStringLiteral("lbBtnWifiT1"));
        lbBtnWifiT1->setGeometry(QRect(132, 412, 55, 18));
        lbBtnWifiT2 = new QLabel(centralWidget);
        lbBtnWifiT2->setObjectName(QStringLiteral("lbBtnWifiT2"));
        lbBtnWifiT2->setGeometry(QRect(103, 440, 55, 18));
        btnWifi = new QPushButton(centralWidget);
        btnWifi->setObjectName(QStringLiteral("btnWifi"));
        btnWifi->setGeometry(QRect(98, 403, 90, 60));
        lbBtnConfBG = new QLabel(centralWidget);
        lbBtnConfBG->setObjectName(QStringLiteral("lbBtnConfBG"));
        lbBtnConfBG->setGeometry(QRect(222, 403, 90, 60));
        lbConfImg = new QLabel(centralWidget);
        lbConfImg->setObjectName(QStringLiteral("lbConfImg"));
        lbConfImg->setGeometry(QRect(227, 410, 22, 22));
        lbBtnConfT2 = new QLabel(centralWidget);
        lbBtnConfT2->setObjectName(QStringLiteral("lbBtnConfT2"));
        lbBtnConfT2->setGeometry(QRect(227, 440, 70, 18));
        btnAdvConf = new QPushButton(centralWidget);
        btnAdvConf->setObjectName(QStringLiteral("btnAdvConf"));
        btnAdvConf->setGeometry(QRect(222, 403, 90, 60));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "kylin-nm", Q_NULLPTR));
        btnWifiList->setText(QString());
        btnNetList->setText(QString());
        lbBtnNetBG->setText(QString());
        lbLanImg->setText(QString());
        lbBtnNetT1->setText(QString());
        lbBtnNetT2->setText(QString());
        btnNet->setText(QString());
        lbBtnWifiBG->setText(QString());
        lbWifiImg->setText(QString());
        lbBtnWifiT1->setText(QString());
        lbBtnWifiT2->setText(QString());
        btnWifi->setText(QString());
        lbBtnConfBG->setText(QString());
        lbConfImg->setText(QString());
        lbBtnConfT2->setText(QString());
        btnAdvConf->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
