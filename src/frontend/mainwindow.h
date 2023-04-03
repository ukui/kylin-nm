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
#include <QHBoxLayout>
#include <QLabel>
#include <QThread>
#include <QPluginLoader>
#include <kylin-nm/kylin-nm-interface.h>
#include <kylin-nm/kylinnetworkmanager.h>
//安全中心-网络防火墙模式配置
#include "networkmodeconfig.h"

#ifdef WITHKYSEC
#include <kysec/libkysec.h>
#include <kysec/status.h>
#endif

enum DeviceType
{
    WIRED,
    WIRELESS
};

const QByteArray TRANSPARENCY_GSETTINGS = "org.ukui.control-center.personalise";

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showMainwindow();
    void hideMainwindow();

    void setWiredDefaultDevice(QString deviceName);
    void setWirelessDefaultDevice(QString deviceName);

#if 0
    void getWirelessList(QMap<QString, QVector<QStringList> > &map);
    void getWiredList(QMap<QString, QVector<QStringList>> &map);
    //开启热点
    void activeWirelessAp(const QString apName, const QString apPassword, const QString wirelessBand, const QString apDevice);
    //断开热点
    void deactiveWirelessAp(const QString apName, const QString uuid);
    //获取热点
    void getStoredApInfo(QStringList &list);
    void getApInfoBySsid(QString devName, QString ssid, QStringList &list);

    //删除有线连接
    void deleteWired(const QString& connUuid);
    //有线连接断开
    void activateWired(const QString& devName, const QString& connUuid);
    void deactivateWired(const QString& devName, const QString& connUuid);
    //无线连接断开
    void activateWireless(const QString& devName, const QString& ssid);
    void deactivateWireless(const QString& devName, const QString& ssid);
    //无线总开关
//    void setWirelessSwitchEnable(bool enable);

    void setWiredDeviceEnable(const QString& devName, bool enable);

    //唤起属性页 根据网卡类型 参数2 为ssid/uuid
//    void showPropertyWidget(QString devName, QString ssid);
    //唤起新建有线连接界面
//    void showCreateWiredConnectWidget(const QString devName);
    //唤起加入其他无线网络界面
//    void showAddOtherWlanWidget(QString devName);

    void getWirelessDeviceCap(QMap<QString, int> &map);

    void rescan();

    void keyRingInit();
    void keyRingClear();
#endif
signals:
    void mainWindowVisibleChanged(const bool &visible);
 #if 0
    //设备插拔
    void deviceStatusChanged();
    //设备名称变化
    void deviceNameChanged(QString oldName, QString newName, int type);
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
    热点断开
    void hotspotDeactivated(QString devName, QString ssid);
    void hotspotActivated(QString devName, QString ssid, QString uuid);
    //信号强度变化
    void signalStrengthChange(QString devName, QString ssid, int strength);
    //安全性变化
    void secuTypeChange(QString devName, QString ssid, QString secuType);
    //列表排序
    void timeToUpdate();
#endif
protected:
    void keyPressEvent(QKeyEvent *event);
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
    void registerTrayIcon();
    void initTrayIcon();
    void resetTrayIconTool();
    void initWindowTheme();
    void resetWindowTheme();
    void showControlCenter();
    void showByWaylandHelper();
    void loadLanPlugin(); //加载有线网络插件
    void loadWlanPlugin(); //加载无线网络插件
    void startLoading();
    void stopLoading();

    double m_transparency=1.0;  //透明度
    QGSettings * m_transGsettings;   //透明度配置文件

    //主窗口的主要构成控件
    QTabWidget * m_centralWidget = nullptr;
    QHBoxLayout * m_tabBarLayout = nullptr;
    QLabel * m_lanLabel = nullptr;
    QLabel * m_wlanLabel = nullptr;

    QWidget * m_lanWidget = nullptr;
    QWidget * m_wlanWidget = nullptr;

    //监听主题的Gsettings
    QGSettings * m_styleGsettings = nullptr;

    //获取和重置窗口位置
    void resetWindowPosition();
    QDBusInterface * m_positionInterface = nullptr;

    //托盘图标，托盘图标右键菜单
    QSystemTrayIcon * m_trayIcon = nullptr;
    QMenu * m_trayIconMenu = nullptr;
    QAction * m_showMainwindowAction = nullptr;
    QAction * m_showSettingsAction = nullptr;
    uint m_intervalTime = 100;
    uint m_registerCount = 0;

    bool m_lanIsLoading = false;
    bool m_wlanIsLoading = false;

    bool m_isShowInCenter = false;

    QTimer * m_loadingTimer = nullptr;
    QTimer * m_refreshTimer = nullptr;
    QList<QIcon> m_loadIcons;
    int m_currentIconIndex=0;
    KyConnectStatus m_iconStatus;
    KyNetworkManager * m_manager = nullptr;
    QThread * m_thread = nullptr;

    NetworkModeConfig *m_networkMode;

public slots:
    void onShowMainWindow(int type);

private slots:
    void onTransChanged();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onShowMainwindowActionTriggled();
    void onShowSettingsActionTriggled();
    void onThemeChanged(const QString &key);
    void onSetTrayIconLoading();
    void updateIcon();
    void onWiredStateChange(QString deviceName, QString uuid, KyConnectState state);
    void onWirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state);
    void onTabletModeChanged(bool mode);
};

#endif // MAINWINDOW_H
