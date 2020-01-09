/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#ifndef ONECONNFORM_H
#define ONECONNFORM_H

#include <QWidget>
#include <QScreen>
#include <QThread>
#include <QDialog>
#include <QLineEdit>
#include <QShortcut>
#include <QKeyEvent>

#include "confform.h"
#include "kylin-network-interface.h"
#include "backthread.h"
#include "ksimplenm.h"

class MainWindow;

namespace Ui {
class OneConnForm;
}

class OneConnForm : public QWidget
{
    Q_OBJECT

public:
    explicit OneConnForm(QWidget *parent = 0, MainWindow *mw = 0, ConfForm *confForm = 0, KSimpleNM *ksnm = 0);
    ~OneConnForm();

    void setSignal(QString lv);
    void setName(QString name);
    void setSpecialName(QString name);
    QString getName();
    void setSafe(QString safe);
    void setRate(QString rate);

    void setSelected(bool isSelected);
    void setHideSelected(bool isSelected);
    void setAct(bool isAct);

    void setSafeString(QString str);
    void setConnedString(QString str);
    void setShowPoint(bool flag);

    QString wifiName;
    bool isSelected;
    bool isActive;
    int signalLv;

signals:
    void selectedOneWifiForm(QString wifiName);
    void connDone(int connFlag);
    void disconnActiveWifi();

    void sigConnWifi(QString);
    void sigConnWifiPWD(QString, QString);

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_btnConf_clicked();

    void on_btnConn_clicked();
    void on_btnDisConn_clicked();

    void slotConnWifi();
    void slotConnWifiPWD();
    void slotConnWifiResult(int connFlag);

    void on_btnConnPWD_clicked();

    void on_btnHideConn_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

private:
    Ui::OneConnForm *ui;
    MainWindow *mw;
    ConfForm *cf;
    KSimpleNM *ks;
    bool isSafe;
};

#endif // ONECONNFORM_H
