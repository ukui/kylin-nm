/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
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
#include "backthread.h"
#include "ksimplenm.h"

#define FRAME_SPEED 150
#define LIMIT_TIME 20*1000
#define TOTAL_PAGE 8

#define W_ITEM 424
#define H_ITEM 60
#define H_ITEM_BIG 138
#define H_ITEM_MIDDLE 106
#define H_WIFI_ITEM_BIG_EXTEND 78 //138 - 60
#define H_WIFI_ITEM_SMALL_EXTEND 46 //106 - 60
#define Y_LINE 59
#define X_LINE 0
#define Y_LINE_SMALL_EXTEND 105
#define X_LINE_SMALL_EXTEND 0
#define Y_LINE_BIG_EXTEND 137
#define X_LINE_BIG_EXTEND 0

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

    void setSignal(QString lv, QString secu);
    void setName(QString name);
    void setSpecialName(QString name);
    QString getName();
    void setRate(QString rate);
    void setLine(bool isShow);
    void setWifiInfo(QString str1, QString str2, QString str3);

    void setSelected(bool isSelected, bool isCurrName);
    void setHideItem(bool isHideItem, bool isShowHideBtn);
    void setTopItem(bool isSelected);
    void setAct(bool isAct);

    void setConnedString(bool showLable, QString str, QString str1);

    void setLePassword();

    bool isWifiConfExist(QString netName);

    QString wifiName;
    bool isSelected;
    bool isActive;
    bool isConnected;
    bool isTopItem;
    int signalLv;

protected:
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_btnConn_clicked();
    void on_btnConnSub_clicked();
    void on_btnDisConn_clicked();
    void toConnectWirelessNetwork();

    void slotConnWifi();
    void slotConnWifiPWD();
    void slotConnWifiResult(int connFlag);

    void on_btnConnPWD_clicked();

    void on_btnHideConn_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_lePassword_textEdited(const QString &arg1);

    void waitAnimStep();
    void startWaiting(bool isConn);
    void stopWaiting();

    void on_btnInfo_clicked();

private:
    QTimer *waitTimer = nullptr;
    int waitPage;
    int countCurrentTime;
    bool isWaiting = false;

    Ui::OneConnForm *ui = nullptr;
    MainWindow *mw = nullptr;
    ConfForm *cf = nullptr;
    KSimpleNM *ks = nullptr;
    bool hasPwd;

    QString leQssLow, leQssHigh;

signals:
    void selectedOneWifiForm(QString wifiName, int extendLength);
    void connDone(int connFlag);
    void disconnActiveWifi();

    void sigConnWifi(QString);
    void sigConnWifiPWD(QString, QString);
};

#endif // ONECONNFORM_H
