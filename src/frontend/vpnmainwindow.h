/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef VPNMAINWINDOW_H
#define VPNMAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QGSettings/QGSettings>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDBusInterface>
#include <QMap>
#include <QScreen>
#include "vpnpage.h"
#include "mainwindow.h"

#ifdef WITHKYSEC
#include <kysec/libkysec.h>
#include <kysec/status.h>
#endif

//enum IconActiveType {
//    NOT_CONNECTED = 0,
//    LAN_CONNECTED,
//    WLAN_CONNECTED,
//    LAN_CONNECTED_LIMITED,
//    WLAN_CONNECTED_LIMITED,
//    ACTIVATING,
//};

//const QByteArray TRANSPARENCY_GSETTINGS = "org.ukui.control-center.personalise";

class VpnPage;

class vpnMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit vpnMainWindow(QWidget *parent = nullptr);
    void showMainwindow();
    void hideMainwindow();

    void getVirtualList(QMap<QString, QVector<QStringList>> &map);

//    void setWiredDefaultDevice(QString deviceName);

//    //有线连接断开
    void activateVpn(const QString& connUuid);
    void deactivateVpn(const QString& connUuid);

//    //唤起属性页 根据网卡类型 参数2 为ssid/uuid
//    void showPropertyWidget(QString devName, QString ssid);
//    //唤起新建有线连接界面
//    void showCreateWiredConnectWidget(const QString devName);

Q_SIGNALS:
//    //设备插拔
//    void deviceStatusChanged();
//    //设备名称变化
//    void deviceNameChanged(QString oldName, QString newName, int type);
    void vpnAdd(QString devName, QStringList info);
    void vpnRemove(QString dbusPath);
    void vpnUpdate(QString devName, QStringList info);
    void vpnActiveConnectionStateChanged(QString devName, QString uuid, int status);
//    void activateFailed(QString errorMessage);
//    void deactivateFailed(QString errorMessage);
    void mainWindowVisibleChanged(const bool &visible);
//    //列表排序
//    void timeToUpdate();
public Q_SLOTS:

protected:
//    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event);

private:
    void firstlyStart(); //一级启动
    void secondaryStart(); //二级启动
    bool m_loadFinished = false; //是否二级启动已执行完
    QTimer * m_secondaryStartTimer = nullptr; //执行二级启动的倒计时
    void initPlatform();
    void initWindowProperties();
    void initTransparency();
    void paintWithTrans();
    void initUI();
    void initDbusConnnect();
    void initTrayIcon();
//    void resetTrayIconTool();
    void initWindowTheme();
//    void resetWindowTheme();
//    void showControlCenter();
    void showByWaylandHelper();
    void setCentralWidgetType(IconActiveType iconStatus);
    double m_transparency=1.0;  //透明度
    QGSettings * m_transGsettings;   //透明度配置文件
    int m_currentIconIndex = 0;
    QList<QIcon> m_loadIcons;
    QTimer *m_iconTimer = nullptr;

//    //主窗口的主要构成控件
    QWidget * m_vpnWidget = nullptr;
//    QHBoxLayout * m_tabBarLayout = nullptr;
    QLabel * m_lanLabel = nullptr;
    VpnPage * m_vpnPage = nullptr;
    QVBoxLayout * m_vpnLayout = nullptr;

    //监听主题的Gsettings
    QGSettings * m_styleGsettings = nullptr;

//    //获取和重置窗口位置
    void resetWindowPosition();
    QDBusInterface * m_positionInterface = nullptr;

//    //托盘图标，托盘图标右键菜单
    QSystemTrayIcon * m_vpnTrayIcon = nullptr;
    QMenu * m_vpnTrayIconMenu = nullptr;
//    QAction * m_showMainwindowAction = nullptr;
//    QAction * m_showSettingsAction = nullptr;

//    bool m_lanIsLoading = false;

    bool m_isShowInCenter = false;

    IconActiveType m_iconStatus = IconActiveType::NOT_CONNECTED;

    QMap<QString, NetDetail*> m_createPagePtrMap;
////    QMap<QString, NetDetail*> m_addOtherPagePtrMap;
//    QMap<QString, JoinHiddenWiFiPage*> m_addOtherPagePtrMap;

public Q_SLOTS:
//    void onShowMainWindow(int type);

private Q_SLOTS:
    void onTransChanged();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
//    void onShowMainwindowActionTriggled();
//    void onShowSettingsActionTriggled();
    void onThemeChanged(const QString &key);
    void onRefreshTrayIcon();
//    void onSetTrayIconLoading();
//    void onLanConnectStatusToChangeTrayIcon(int state);
//    void onWlanConnectStatusToChangeTrayIcon(int state);
//    void onConnectivityChanged(NetworkManager::Connectivity connectivity);
//    void onTabletModeChanged(bool mode);
};

#endif // MAINWINDOW_H
