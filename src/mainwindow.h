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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ksimplenm.h"
#include "loadingdiv.h"
#include "confform.h"
#include "kylin-dbus-interface.h"
#include "kylin-network-interface.h"
#include "utils.h"
#include "switchbutton.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <xcb/xcb.h>

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QScrollBar>
#include <QScrollArea>
#include <QPushButton>
#include <QScreen>
#include <QMenu>
#include <QAction>
#include <QAction>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QDBusObjectPath>
#include <QVariant>
#include <QStandardPaths>
#include <QStringList>
#include <QToolTip>
#include <QSvgRenderer>
//#include <kysec/libkysec.h>
//#include <kysec/status.h>
//#include <QMessageBox>

#define W_LEFT_AREA 41
#define W_VERTICAL_LINE 1 //左边竖线宽度
#define W_RIGHT_AREA 438 //41 + 1 + 438 = 480
#define L_VERTICAL_LINE_TO_ITEM 4 //竖线到item左侧的距离

#define X_LEFT_WIFI_BALL 416 //白色小球在左边
#define X_RIGHT_WIFI_BALL 440 //白色小球在右边
#define Y_WIFI_BALL 23 //白色小球y坐标
#define X_ITEM 46 //item到窗口左侧的距离 41 + 1 + 4 = 46
#define W_ITEM 424

#define Y_TOP_ITEM 57 //顶部item、topLanListWidget、topWifiListWidget的y坐标
#define H_NORMAL_ITEM 60 //一个没有扩展的，普通item的高度
#define H_GAP_UP 10 //H_NORMAL_ITEM 与 H_MIDDLE_WORD之间的间距
#define H_MIDDLE_WORD 46 //"显示‘可用网络列表’的label"
#define H_GAP_DOWN 5 //57 + 60 + 10 + 46 + 5 = 178
#define X_MIDDLE_WORD 19
#define W_MIDDLE_WORD 260

#define H_LAN_ITEM_EXTEND 102 //162 - 60
#define H_WIFI_ITEM_BIG_EXTEND 78 //138 - 60
#define H_WIFI_ITEM_SMALL_EXTEND 46 //106 - 60

#define Y_SCROLL_AREA 178
#define W_SCROLL_AREA 440
#define H_SCROLL_AREA 360

#define W_TOP_LIST_WIDGET 435
#define W_LIST_WIDGET 440

#define W_BTN_FUN 59
#define H_BTN_FUN 14
#define X_BTN_FUN 360
#define Y_BTN_FUN 87 //新建网络，加入网络按钮的宽高、x坐标、y坐标

#define W_NO_ITEM_TIP 235
#define H_NO_ITEM_TIP 20

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

    void editQssString();
    void createTopLanUI();
    void createTopWifiUI();
    void createOtherUI();
    void createListAreaUI();
    void createLeftAreaUI();

    void startLoading();
    void stopLoading();

    void setTrayIcon(QIcon icon);
    void setTrayLoading(bool isLoading);
    void getActiveInfo();

    void initTimer();
    void checkIsWirelessDeviceOn();

    void initActNetDNS();

    QIcon iconLanOnline, iconLanOffline;
    QIcon iconWifiFull, iconWifiHigh, iconWifiMedium, iconWifiLow;
    QIcon iconConnecting;
    QList<QIcon> loadIcons;
    QString mwBandWidth;
    KylinDBus *objKyDBus = nullptr;
    NetworkSpeed *objNetSpeed = nullptr;
    SwitchButton *btnWireless;

    //状态设置,0为假，1为真
    int is_update_wifi_list = 0; //是否是update wifi列表，而不是load wifi列表
    int is_init_wifi_list = 0; //是否在启动软件时正在获取wifi的列表
    int is_btnLanList_clicked = 1; //是否处于有线网界面
    int is_btnWifiList_clicked = 0; //是否处于无线网界面
    int is_wireless_adapter_ready = 1; //主机是否插入无线网卡
    int is_keep_wifi_turn_on_state = 1; //是否要执行wifi开关变为打开样式
    int is_stop_check_net_state = 0; //是否要在进行其他操作时停止检查网络状态
    int is_connect_net_failed = 0; //刚才是否连接网络失败
    int is_fly_mode_on = 0; //是否已经打开飞行模式
    int is_hot_sopt_on = 0; //是否已经打开热点
    int is_connect_hide_wifi = 0; //是否正在连接隐藏wifi

    QString currSelNetName = ""; //当前ScrollArea中选中的网络名称
    QString currConnIfname = ""; //当前连接的有线网对应网卡名称，只有一个有线网连接的情况
    int currSelNetNum = 0; //当前选中的item序号
    bool isLanBeConnUp = false; //lan是否连接上
    bool isWifiBeConnUp = false; //wifi是否是连接上
    bool isToSetLanValue = true; //本次执行是否进行赋值
    bool isToSetWifiValue = true; //本次执行是否进行赋值

public slots:
    void onPhysicalCarrierChanged(bool flag);
    void onCarrierUpHandle();
    void onCarrierDownHandle();
    void onDeleteLan();
    void onNetworkDeviceAdded(QDBusObjectPath objPath);
    void onNetworkDeviceRemoved(QDBusObjectPath objPath);
    void getLanBandWidth();
    void checkIfWiredNetExist();

    void onExternalConnectionChange(QString type, bool isConnUp);
    void onExternalLanChange();
    void onExternalWifiChange();
    void onWifiSwitchChange();
    void onExternalWifiSwitchChange(bool wifiEnabled);

    void oneLanFormSelected(QString lanName, QString uniqueName);
    void oneTopLanFormSelected(QString lanName, QString uniqueName);
    void oneWifiFormSelected(QString wifiName, int extendLength);
    void oneTopWifiFormSelected(QString wifiName, int extendLength);

    void on_btnHotspot_clicked();
    void on_btnHotspotState();
    void on_btnWifiList_clicked();

    void connWifiDone(int connFlag);

    //flag =0或1为普通点击、2为收到打开信息、3为收到关闭信息、4为无线网卡插入、5为无线网卡拔出
    void onBtnWifiClicked(int flag = 0);

    void on_showWindowAction();

    void checkIfConnectedWifiExist();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);

private:
    void checkSingleAndShowTrayicon();
    void initNetwork();
    void createTrayIcon();
    void handleIconClicked();
    void showTrayIconMenu();
    bool checkLanOn();
    bool checkWlOn();
    void getLanList();
    void getWifiList();
    void getInitLanSlist();
    QPixmap drawSymbolicColoredPixmap(const QPixmap &source);
    QPixmap drawSymbolicBlackColoredPixmap(const QPixmap &source);
    const QPixmap loadSvg(const QString &fileName, const int size);
    void getSystemFontFamily();

    Ui::MainWindow *ui;

    LoadingDiv *loading = nullptr;

    QDesktopWidget desktop;
    KSimpleNM *ksnm = nullptr;
    ConfForm *confForm = nullptr;
    QWidget *topLanListWidget = nullptr;
    QWidget *topWifiListWidget = nullptr;
    QWidget *lanListWidget = nullptr;
    QWidget *wifiListWidget = nullptr;
    QWidget *optWifiWidget = nullptr;

    QLabel *lbLoadDown = nullptr;
    QLabel *lbLoadDownImg = nullptr;
    QLabel *lbLoadUp = nullptr;
    QLabel *lbLoadUpImg = nullptr;

    QLabel *lbNoItemTip = nullptr;
    bool ifLanConnected;
    bool ifWLanConnected;

    QScrollArea *scrollAreal = nullptr;
    QScrollArea *scrollAreaw = nullptr;
    QLabel *lbTopLanList = nullptr;
    QLabel *lbTopWifiList = nullptr;
    QLabel *lbLanList = nullptr;
    QLabel *lbWifiList = nullptr;
    QPushButton *btnAddNet = nullptr;
    QPushButton *btnCreateNet = nullptr;

    QSystemTrayIcon *trayIcon = nullptr;
    QMenu *trayIconMenu = nullptr;
    QAction *mShowWindow = nullptr;
    QAction *mAdvConf = nullptr;
    QWidget *widShowWindow = nullptr;
    QWidget *widAdvConf = nullptr;

    QString lname, wname; // 以太网卡和无线网卡名称

    QString btnOffQss, btnOnQss, btnBgOffQss, btnBgOnQss, btnBgHoverQss, btnBgLeaveQss; // 主界面按钮底色
    QString scrollBarQss, leftBtnQss, funcBtnQss;

    QStringList oldLanSlist; //上一次获取Lan列表
    QStringList oldWifiSlist; //上一次获取wifi列表
    QStringList oldConnSlist; //上一次获取的以保存网络列表
    bool isInitConnList = true;
    bool isAddedWifi = false;
    QString lastAddedConn = "";
    QString oldActLanName = ""; //上一次获取的已连接有线网名称
    int oldDbusActLanDNS = 0; //上一次获取的已连接有线网的DNS代号

    //循环检测网络连接状态
    QTimer *iconTimer = nullptr;
    QTimer *wiredCableUpTimer = nullptr;
    QTimer *wiredCableDownTimer = nullptr;
    QTimer *deleteLanTimer = nullptr;
    QTimer *checkWifiListChanged = nullptr;
    QTimer *checkIfLanConnect = nullptr;
    QTimer *checkIfWifiConnect = nullptr;
    QTimer *checkIfNetworkOn = nullptr;
    QTimer *setNetSpeed = nullptr;

    int currentIconIndex;
    int activeWifiSignalLv;
    int disconnect_time = 0;
    int currTopLanItem = 1; //当前连接的有线网个数

    long int start_rcv_rates = 0;	//保存开始时的流量计数
    long int end_rcv_rates = 0;	//保存结束时的流量计数
    long int start_tx_rates = 0;   //保存开始时的流量计数
    long int end_tx_rates = 0; //保存结束时的流量计数
    QString actWifissid = "--"; //当前连接wifi的ssid
    QStringList actWifiBssidList; //当前连接wifi的bssid
    QString actWifiUuid = "--"; //当前连接wifi的uuid

    bool hasWifiConnected;//当前是否有wifi连接

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void onBtnNetClicked();
//    void on_btnWifiList_clicked();
    void onBtnNetListClicked(int flag=0);
    void onNewConnAdded(int type);

    void onRequestRevalueUpdateWifi();
    void getLanListDone(QStringList slist);
    void getWifiListDone(QStringList slist);
    void getConnListDone(QStringList slist);
    void loadWifiListDone(QStringList slist);
    void updateWifiListDone(QStringList slist);
    QString TranslateLanName(QString lanName);
    QString getMacByUuid(QString uuidName);

    void on_btnAdvConf_clicked();

    void activeLanDisconn();
    void activeWifiDisconn();
    void activeStartLoading();
    void activeGetWifiList();
    void on_checkWifiListChanged();
    void on_setNetSpeed();
    void on_checkOverTime();

    // 后台回调
    void enNetDone();
    void disNetDone();
    void enWifiDone();
    void disWifiDone();
    void disWifiStateKeep();
    void disWifiDoneChangeUI();
    void connLanDone(int connFlag);
//    void connWifiDone(int connFlag);

    void iconStep();
    void on_btnFlyMode_clicked();

    void onBtnAddNetClicked();
    void onBtnCreateNetClicked();
    void actionTriggerSlots();

signals:
    void disConnSparedNet(QString type);

    void waitWifiStop();
    void waitLanStop();
};

#endif // MAINWINDOW_H
