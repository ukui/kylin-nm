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
#include "lanpage.h"
#include "wlanpage.h"
#include "netdetails/netdetail.h"

enum IconActiveType {
    NOT_CONNECTED = 0,
    LAN_CONNECTED,
    WLAN_CONNECTED,
    LAN_CONNECTED_LIMITED,
    WLAN_CONNECTED_LIMITED,
    ACTIVATING,
};

class LanPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showMainwindow();
    void hideMainwindow();

    void setWiredDefaultDevice(QString deviceName);
    void setWirelessDefaultDevice(QString deviceName);

    //for dbus
    void getWirelessList(QMap<QString, QVector<QStringList> > &map);
    void getWiredList(QMap<QString, QVector<QStringList>> &map);
    //开启热点
    void activeWirelessAp(const QString apName, const QString apPassword, const QString wirelessBand, const QString apDevice);
    //断开热点
    void deactiveWirelessAp(const QString apName, const QString uuid);
    //获取热点
    void getStoredApInfo(QStringList &list);
    void getApInfoBySsid(QString devName, QString ssid, QStringList &list);
    //有线连接断开
    void activateWired(const QString& devName, const QString& connUuid);
    void deactivateWired(const QString& devName, const QString& connUuid);
    //无线连接断开
    void activateWireless(const QString& devName, const QString& ssid);
    void deactivateWireless(const QString& devName, const QString& ssid);
    //无线总开关
    void setWirelessSwitchEnable(bool enable);

    void setWiredDeviceEnable(const QString& devName, bool enable);

    //唤起属性页 根据网卡类型 参数2 为ssid/uuid
    void showPropertyWidget(QString devName, QString ssid);
    //唤起新建有线连接界面
    void showCreateWiredConnectWidget(const QString devName);

    void getWirelessDeviceCap(QMap<QString, int> &map);

    void rescan();

signals:
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
    //热点断开
    void hotspotDeactivated(QString devName, QString ssid);
    void hotspotActivated(QString devName, QString ssid, QString uuid);
    //信号强度变化
    void signalStrengthChange(QString devName, QString ssid, int strength);
    //安全性变化
    void secuTypeChange(QString devName, QString ssid, QString secuType);
    void mainWindowVisibleChanged(const bool &visible);
    //列表排序
    void timeToUpdate();
public slots:

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event);

private:
    void firstlyStart(); //一级启动
    void secondaryStart(); //二级启动
    bool m_loadFinished = false; //是否二级启动已执行完
    QTimer * m_secondaryStartTimer = nullptr; //执行二级启动的倒计时
    void initWindowProperties();
    void initUI();
    void initDbusConnnect();
    void initTrayIcon();
    void resetTrayIconTool();
    void initWindowTheme();
    void resetWindowTheme();
    void showControlCenter();
    int currentIconIndex=0;
    QList<QIcon> loadIcons;
    QTimer *iconTimer = nullptr;

    //主窗口的主要构成控件
    QTabWidget * m_centralWidget = nullptr;
    QHBoxLayout * m_tabBarLayout = nullptr;
    QLabel * m_lanLabel = nullptr;
    QLabel * m_wlanLabel = nullptr;

    LanPage * m_lanWidget = nullptr;
    WlanPage * m_wlanWidget = nullptr;

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

    bool m_lanIsLoading = false;
    bool m_wlanIsLoading = false;

    IconActiveType iconStatus = IconActiveType::NOT_CONNECTED;

private slots:
    void onTrayIconActivated();
    void onShowMainwindowActionTriggled();
    void onShowSettingsActionTriggled();
    void onThemeChanged(const QString &key);
    void onRefreshTrayIcon();
    void onSetTrayIconLoading();
    void onLanConnectStatusToChangeTrayIcon(int state);
    void onWlanConnectStatusToChangeTrayIcon(int state);
    void onShowByWlanPage();
    void onConnectivityChanged(NetworkManager::Connectivity connectivity);
};

#endif // MAINWINDOW_H
