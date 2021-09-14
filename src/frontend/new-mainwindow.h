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
    void activeWirelessAp(const QString apName, const QString apPassword, const QString apDevice);
    //断开热点
    void deactiveWirelessAp(const QString apName, const QString apPassword, const QString apDevice);
    //获取热点
    void getStoredApInfo(QStringList &list);
    //有线连接断开
    void activateWired(const QString& devName, const QString& connName);
    void deactivateWired(const QString& devName, const QString& connName);
    //无线连接断开
    void activateWireless(const QString& devName, const QString& ssid);
    void deactivateWireless(const QString& devName, const QString& ssid);

signals:
    //设备插拔
    void deviceStatusChanged();
    //设备名称变化
    void deviceNameChanged(QString oldName, QString newName);
    //设备有线无线列表更新（有线增删、无线增加减少）
    void listUpdate(QString devName);
    //控制面板连接中
    void wiredActivating(QString devName, QString ssid);
    void wirelessActivating(QString devName, QString ssid);
    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
    //热点断开
    void hotspotDeactivated(QString devName, QString ssid);
    void hotspotActivated(QString devName, QString ssid);
    void mainWindowVisibleChanged(const bool &visible);
public slots:

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *watched, QEvent *event) override;

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

private slots:
    void onTrayIconActivated();
    void onShowMainwindowActionTriggled();
    void onShowSettingsActionTriggled();
    void onThemeChanged(const QString &key);
};

#endif // MAINWINDOW_H
