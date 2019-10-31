/********************************************************************************
** Form generated from reading UI file 'oneconnform.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONECONNFORM_H
#define UI_ONECONNFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OneConnForm
{
public:
    QLabel *lbName;
    QPushButton *btnConf;
    QPushButton *btnConn;
    QLabel *lbSignal;
    QLabel *lbSafe;
    QLineEdit *lePassword;
    QLabel *lbConned;
    QLabel *lbPoint;
    QWidget *wbg;
    QPushButton *btnDisConn;
    QLabel *lbPassword;
    QPushButton *btnConnPWD;
    QPushButton *btnHideConn;

    void setupUi(QWidget *OneConnForm)
    {
        if (OneConnForm->objectName().isEmpty())
            OneConnForm->setObjectName(QStringLiteral("OneConnForm"));
        OneConnForm->resize(314, 60);
        lbName = new QLabel(OneConnForm);
        lbName->setObjectName(QStringLiteral("lbName"));
        lbName->setGeometry(QRect(62, 8, 190, 20));
        btnConf = new QPushButton(OneConnForm);
        btnConf->setObjectName(QStringLiteral("btnConf"));
        btnConf->setGeometry(QRect(21, 70, 129, 32));
        btnConn = new QPushButton(OneConnForm);
        btnConn->setObjectName(QStringLiteral("btnConn"));
        btnConn->setGeometry(QRect(162, 70, 129, 32));
        lbSignal = new QLabel(OneConnForm);
        lbSignal->setObjectName(QStringLiteral("lbSignal"));
        lbSignal->setGeometry(QRect(15, 14, 32, 32));
        lbSafe = new QLabel(OneConnForm);
        lbSafe->setObjectName(QStringLiteral("lbSafe"));
        lbSafe->setGeometry(QRect(62, 31, 80, 18));
        lePassword = new QLineEdit(OneConnForm);
        lePassword->setObjectName(QStringLiteral("lePassword"));
        lePassword->setGeometry(QRect(100, 20, 190, 30));
        lbConned = new QLabel(OneConnForm);
        lbConned->setObjectName(QStringLiteral("lbConned"));
        lbConned->setGeometry(QRect(180, 19, 90, 20));
        lbPoint = new QLabel(OneConnForm);
        lbPoint->setObjectName(QStringLiteral("lbPoint"));
        lbPoint->setGeometry(QRect(280, 25, 10, 10));
        wbg = new QWidget(OneConnForm);
        wbg->setObjectName(QStringLiteral("wbg"));
        wbg->setGeometry(QRect(0, 0, 314, 111));
        btnDisConn = new QPushButton(OneConnForm);
        btnDisConn->setObjectName(QStringLiteral("btnDisConn"));
        btnDisConn->setGeometry(QRect(162, 70, 129, 32));
        lbPassword = new QLabel(OneConnForm);
        lbPassword->setObjectName(QStringLiteral("lbPassword"));
        lbPassword->setGeometry(QRect(25, 25, 70, 18));
        btnConnPWD = new QPushButton(OneConnForm);
        btnConnPWD->setObjectName(QStringLiteral("btnConnPWD"));
        btnConnPWD->setGeometry(QRect(162, 70, 129, 32));
        btnHideConn = new QPushButton(OneConnForm);
        btnHideConn->setObjectName(QStringLiteral("btnHideConn"));
        btnHideConn->setGeometry(QRect(162, 70, 129, 32));
        wbg->raise();
        lbName->raise();
        btnConf->raise();
        btnConn->raise();
        lbSignal->raise();
        lbSafe->raise();
        lePassword->raise();
        lbConned->raise();
        lbPoint->raise();
        btnDisConn->raise();
        lbPassword->raise();
        btnConnPWD->raise();
        btnHideConn->raise();

        retranslateUi(OneConnForm);

        QMetaObject::connectSlotsByName(OneConnForm);
    } // setupUi

    void retranslateUi(QWidget *OneConnForm)
    {
        OneConnForm->setWindowTitle(QApplication::translate("OneConnForm", "Form", Q_NULLPTR));
        lbName->setText(QString());
        btnConf->setText(QString());
        btnConn->setText(QString());
        lbSignal->setText(QString());
        lbSafe->setText(QString());
        lbConned->setText(QString());
        lbPoint->setText(QString());
        btnDisConn->setText(QString());
        lbPassword->setText(QString());
        btnConnPWD->setText(QString());
        btnHideConn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class OneConnForm: public Ui_OneConnForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONECONNFORM_H
