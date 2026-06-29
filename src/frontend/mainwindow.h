/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "lanpage.h"
#include "wlanpage.h"
#include "netdetails/netdetail.h"
#include "netdetails/joinhiddenwifipage.h"
#include "connectivity/connectivitypage.h"
#include "kylable.h"
//安全中心-网络防火墙模式配置
#include "../networkmode/networkmodeconfig.h"

#include "../backend/dbus-interface/kylinagent.h"

#ifdef WITHKYSEC
#include <kysec/libkysec.h>
#include <kysec/status.h>
#endif

enum IconActiveType {
    NOT_CONNECTED = 0,
    LAN_CONNECTED,
    LAN_CONNECTED_INTRANET,
    WLAN_CONNECTED,
    WLAN_CONNECTED_INTRANET,
    LAN_CONNECTED_LIMITED,
    WLAN_CONNECTED_LIMITED,
    ACTIVATING,
};

const QByteArray TRANSPARENCY_GSETTINGS = "org.ukui.control-center.personalise";

class LanPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QString display, QWidget *parent = nullptr);
    void showMainwindow();
    void hideMainwindow();

    void setWiredDefaultDevice(QString deviceName);
    void setWirelessDefaultDevice(QString deviceName);

    //for dbus
    void getWirelessList(QString devName, QList<QStringList> &list);
    void getWiredList(QString devName, QList<QStringList> &list);
    //开启热点
    void activeWirelessAp(const QString apName, const QString apPassword, const QString wirelessBand, const QString apDevice);
    //断开热点
    void deactiveWirelessAp(const QString apName, const QString uuid);
    //获取热点
    void getStoredApInfo(QStringList &list);
    void getApInfoBySsid(QString devName, QString ssid, QStringList &list);

    //获取热点path
    void getApConnectionPath(QString &path, QString uuid);
    //获取热点ActivePath
    void getActiveConnectionPath(QString &path, QString uuid);

    //有线连接断开
    void activateWired(const QString& devName, const QString& connUuid);
    void deactivateWired(const QString& devName, const QString& connUuid, bool concise = false);
    void deleteWiredConnect(int type, const QString& connUuid);
    void setWiredDeviceAutoconnect(const QString& devName, bool state);
    void setWiredConnectAutoconnect(const QString& uuid, bool state);
    //无线连接断开
    void activateWireless(const QString& devName, const QString& ssid);
    void deactivateWireless(const QString& devName, const QString& ssid);
    void deleteWireleeConnect(int type, const QString& connUuid);
    void setWirelessConnectAutoconnect(const QString& uuid, bool state);

    //无线总开关
    void setWirelessSwitchEnable(bool enable);

    void setWiredEnableStatus(bool enable);

    void setWiredDeviceEnable(const QString& devName, bool enable);

    //唤起属性页 根据网卡类型 参数2 为ssid/uuid
    void showPropertyWidget(QString devName, QString ssid);
    //唤起新建有线连接界面
    void showCreateWiredConnectWidget(const QString devName);
    //唤起加入其他无线网络界面
    void showAddOtherWlanWidget(QString devName);

    void getWirelessDeviceCap(QMap<QString, int> &map);

    void passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect);
    void rescan();

    bool getWirelessSwitchBtnState();
    bool getWiredEnabledState();
    int getDeviceConnectivity(const QString deviceName);
    bool getCableStateByDevice(const QString &deviceName);
    QString getDefaultDeviceName(int type);
    void handleEnterpriseWifiReconnection(const QString &requestId, const QString &deviceName, const QString &ssid, const QString &password, const QVariantMap &enterpriseInfo);
    KylinSecretAgent* secretAgent();

Q_SIGNALS:
    //设备插拔
    void deviceStatusChanged();
    void wirelessDeviceStatusChanged();
    //设备名称变化
    void deviceNameChanged(QString oldName, QString newName, int type);
    void wirelessSwitchBtnChanged(bool state);
    void wiredEnabledChanged(bool state);
    void wiredMainSwitchBtnChanged(bool state);
    //有线无线列表更新（有线增删、无线增加减少）
    void lanAdd(QString devName, QStringList info);
    void lanRemove(QString dbusPath);
    void lanUpdate(QString devName, QStringList info);
    void wlanAdd(QString devName, QStringList info);
    void wlanRemove(QString devName,QString ssid);
    void wlanactiveConnectionStateChanged(QString devName, QString ssid, QString uuid, int status);
    void lanActiveConnectionStateChanged(QString devName, QString uuid, int status);
    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
    //热点断开
    void hotspotDeactivated(QString devName, QString ssid);
    void hotspotActivated(QString devName, QString ssid, QString uuid, QString activePath, QString settingPath);
    void hotspotDeactivating(QString devName, QString ssid);
    void hotspotActivating(QString devName, QString ssid);
    //信号强度变化
    void signalStrengthChange(QString devName, QString ssid, int strength);
    //安全性变化
    void secuTypeChange(QString devName, QString ssid, QString secuType);
    void mainWindowVisibleChanged(const bool &visible);
    //列表排序
    void timeToUpdate();
    void deviceConnectivityChanged(QString devName, int connectivity);
    void sigNetworkPropChanged(QVariantMap parm);

public Q_SLOTS:
    //供QML插件调用刷新托盘图标
    void refreshTrayIcon();

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void firstlyStart(); //一级启动
    void secondaryStart(); //二级启动
    bool m_loadFinished = false; //是否二级启动已执行完
    QTimer * m_secondaryStartTimer = nullptr; //执行二级启动的倒计时
    void initPlatform();
    void initWindowProperties();
    void initTransparency();
    void paintWithTrans();
    void initPanelGSettings();
    void initUI();
    void initDbusConnnect();
    void registerTrayIcon();
    void initTrayIcon();
    void resetTrayIconTool();
    void initWindowTheme();
    void resetWindowTheme();
    void showByWaylandHelper();
    void slideWindowByPanelPosition();

    void setCentralWidgetType(IconActiveType iconStatus);
    void assembleTrayIconTooltip(QMap<QString, QString> &map, QString &tip);
    void setThemePalette();

    void initNetCtrl();
    void netCtrlDiscon(QMap<QString, QString> lanMap,QMap<QString, QString> wlanMap);

    void kylinAgentInit();
    double m_transparency=1.0;  //透明度
    QGSettings * m_transGsettings;   //透明度配置文件
    int currentIconIndex=0;
    QList<QIcon> loadIcons;
    QTimer *iconTimer = nullptr;
    QTimer *m_iconTimeoutTimer = nullptr;

    //主窗口的主要构成控件
    QTabWidget * m_centralWidget = nullptr;
    QHBoxLayout * m_tabBarLayout = nullptr;
    QLabel * m_lanLabel = nullptr;
    QLabel * m_wlanLabel = nullptr;

    LanPage * m_lanWidget = nullptr;
    WlanPage * m_wlanWidget = nullptr;

    ConnectivityPage* m_connectivityPage = nullptr;

    //监听主题的Gsettings
    QGSettings * m_styleGsettings = nullptr;


    //获取任务栏位置和大小
    QGSettings *m_panelGSettings = nullptr;
    int m_panelPosition;
    int m_panelSize;
    int m_panelType;
    int m_settingsIslandPosition;
    int m_topbarSize;

    //获取和重置窗口位置
    void resetWindowPosition();
    //QDBusInterface * m_positionInterface = nullptr;

    //托盘图标，托盘图标右键菜单
    QSystemTrayIcon * m_trayIcon = nullptr;
    QMenu * m_trayIconMenu = nullptr;
    QAction * m_showMainwindowAction = nullptr;
    QAction * m_showSettingsAction = nullptr;
    QAction * m_showConnectivityPageAction = nullptr;

    bool m_lanIsLoading = false;
    bool m_wlanIsLoading = false;

    bool m_isShowInCenter = false;

    IconActiveType iconStatus = IconActiveType::NOT_CONNECTED;

    QMap<QString, NetDetail*> m_createPagePtrMap;

    NetworkMode *m_networkMode;

    QString m_display;
    uint m_intervalTime = 100;
    uint m_registerCount = 0;

    inline void updateTrayiconMenuStyle() {
        QPalette pal = this->palette();
        //托盘watcher异常时的规避方案 bug#214664
        if (m_trayIconMenu != nullptr) {
            pal.setColor(QPalette::Base, pal.color(QPalette::Base)); //处理Wayland环境setPalette(pal)不生效问题
            pal.setColor(QPalette::Text, pal.color(QPalette::Text));
            m_trayIconMenu->setPalette(pal);
        }
    }

    //网络界面配置
    QMap<QString, QVariant> getModuleHideStatus();
    void initDisplayConfig();
    bool m_lan_display_flag = true;
    bool m_wlan_display_flag = true;

    KylinAgent* m_agent = nullptr;
    QThread *m_agentThread = nullptr;

public Q_SLOTS:
    void onShowMainWindow(int type);
    void updateNetCtrl(QString modName,QVariantMap value);
    void keyRingInit();
    void keyRingClear();

private Q_SLOTS:
    void onTransChanged();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onShowMainwindowActionTriggled();
    void onShowSettingsActionTriggled();
    void onThemeChanged(const QString &key);
    void onRefreshTrayIcon();
    void onSetTrayIconLoading();
    void onConnectStatusToChangeTrayIcon(int state);
    void onLanConnectStatusToChangeTrayIcon(int state);
    void onWlanConnectStatusToChangeTrayIcon(int state);
    void onConnectivityChanged(NetworkManager::Connectivity connectivity);
    void onConnectivityCheckSpareUriChanged();
    void onTimeUpdateTrayIcon();
    void onTabletModeChanged(bool mode);
    void onRefreshTrayIconTooltip();
    void onPrimaryScreenChanged();

    void onShowKylinNMSlot(QString display, int type);
    //唤起属性页 根据网卡类型 参数2 为ssid/uuid
    void onShowPropertyWidgetSlot(QString display, QString devName, QString ssid);
    //唤起新建有线连接界面
    void onShowCreateWiredConnectWidgetSlot(QString display, QString devName);
    //唤起加入其他无线网络界面
    void onShowAddOtherWlanWidgetSlot(QString display, QString devName);
    void showControlCenter();

    void onIconLoadingTimeout();
};

#endif // MAINWINDOW_H
