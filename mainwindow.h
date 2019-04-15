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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QScrollBar>
#include <QScrollArea>
#include <QScreen>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QString>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <xcb/xcb.h>

#include "ksimplenm.h"
#include "loadingdiv.h"
#include "kylin-network-interface.h"

class OneConnForm;
class ConfForm;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startLoading();
    void stopLoading();

    void setTrayIcon(QIcon icon);
    void setTrayLoading(bool isLoading);
    QIcon iconLanOnline, iconLanOffline;
    QIcon iconWifiFull, iconWifiHigh, iconWifiMedium, iconWifiLow;
    QIcon iconConnecting;
    QList<QIcon> loadIcons;

private:
    void checkSingle();
    void getActiveInfo();
    void getIface();
    void createTrayIcon();
    void moveBottomRight();
    bool checkLanOn();
    bool checkWlOn();
    void getLanList();
    void getWifiList();

    Ui::MainWindow *ui;

    LoadingDiv *loading;

    KSimpleNM *ksnm;
    ConfForm *confForm;
    QWidget *lanListWidget;
    QWidget *wifiListWidget;
    QWidget *optWifiWidget;

    QScrollArea *scrollAreal, *scrollAreaw;
    QLabel *lbLanList, *lbWifiList;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *m_miniSizeAction;
    QAction *m_quitAction;

    // 以太网卡和无线网卡名称
    QString lname, wname;

    // 主界面按钮底色
    QString btnOffQss, btnOnQss;

    QTimer *iconTimer;
    int currentIconIndex;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    void on_btnNet_clicked();
    void on_btnWifi_clicked();
    void on_btnNetList_clicked(int flag=0);
    void on_btnWifiList_clicked();

    void getLanListDone(QStringList slist);
    void getWifiListDone(QStringList slist);

    void on_btnAdvConf_clicked();
    void on_btnNetList_pressed();
    void on_btnWifiList_pressed();

    void oneLanFormSelected(QString lanName);
    void oneWifiFormSelected(QString wifiName);
    void activeLanDisconn();
    void activeWifiDisconn();
    void on_btnAdvConf_pressed();
    void on_btnAdvConf_released();

    // 后台回调
    void enNetDone();
    void disNetDone();
    void enWifiDone();
    void disWifiDone();
    void connLanDone(int connFlag);
    void connDone(int connFlag);

    void iconStep();
};

#endif // MAINWINDOW_H
