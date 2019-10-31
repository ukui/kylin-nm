/********************************************************************************
** Form generated from reading UI file 'onelancform.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONELANCFORM_H
#define UI_ONELANCFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OneLancForm
{
public:
    QLabel *lbName;
    QPushButton *btnConn;
    QPushButton *btnConf;
    QPushButton *btnDisConn;
    QLabel *lbConned;
    QWidget *wbg;
    QLabel *lbIcon;
    QLabel *lbBandWidth;
    QLabel *lbPoint;

    void setupUi(QWidget *OneLancForm)
    {
        if (OneLancForm->objectName().isEmpty())
            OneLancForm->setObjectName(QStringLiteral("OneLancForm"));
        OneLancForm->resize(314, 60);
        lbName = new QLabel(OneLancForm);
        lbName->setObjectName(QStringLiteral("lbName"));
        lbName->setGeometry(QRect(62, 8, 190, 20));
        btnConn = new QPushButton(OneLancForm);
        btnConn->setObjectName(QStringLiteral("btnConn"));
        btnConn->setGeometry(QRect(162, 70, 129, 32));
        btnConf = new QPushButton(OneLancForm);
        btnConf->setObjectName(QStringLiteral("btnConf"));
        btnConf->setGeometry(QRect(21, 70, 129, 32));
        btnDisConn = new QPushButton(OneLancForm);
        btnDisConn->setObjectName(QStringLiteral("btnDisConn"));
        btnDisConn->setGeometry(QRect(162, 70, 129, 32));
        lbConned = new QLabel(OneLancForm);
        lbConned->setObjectName(QStringLiteral("lbConned"));
        lbConned->setGeometry(QRect(180, 19, 90, 20));
        wbg = new QWidget(OneLancForm);
        wbg->setObjectName(QStringLiteral("wbg"));
        wbg->setGeometry(QRect(0, 0, 314, 111));
        lbIcon = new QLabel(OneLancForm);
        lbIcon->setObjectName(QStringLiteral("lbIcon"));
        lbIcon->setGeometry(QRect(15, 14, 32, 32));
        lbBandWidth = new QLabel(OneLancForm);
        lbBandWidth->setObjectName(QStringLiteral("lbBandWidth"));
        lbBandWidth->setGeometry(QRect(62, 31, 80, 18));
        lbPoint = new QLabel(OneLancForm);
        lbPoint->setObjectName(QStringLiteral("lbPoint"));
        lbPoint->setGeometry(QRect(280, 25, 10, 10));
        wbg->raise();
        lbName->raise();
        btnConn->raise();
        btnConf->raise();
        btnDisConn->raise();
        lbConned->raise();
        lbIcon->raise();
        lbBandWidth->raise();
        lbPoint->raise();

        retranslateUi(OneLancForm);

        QMetaObject::connectSlotsByName(OneLancForm);
    } // setupUi

    void retranslateUi(QWidget *OneLancForm)
    {
        OneLancForm->setWindowTitle(QApplication::translate("OneLancForm", "Form", Q_NULLPTR));
        lbName->setText(QString());
        btnConn->setText(QString());
        btnConf->setText(QString());
        btnDisConn->setText(QString());
        lbConned->setText(QString());
        lbIcon->setText(QString());
        lbBandWidth->setText(QString());
        lbPoint->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class OneLancForm: public Ui_OneLancForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONELANCFORM_H
