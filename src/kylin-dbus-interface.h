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

#ifndef KYLINDBUSINTERFACE_H
#define KYLINDBUSINTERFACE_H

#include <sys/syslog.h>

#include <QObject>
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QVariant>
#include <QVariantMap>
#include <QGSettings/QGSettings>
#include <QTimer>
#include <QThread>

class MainWindow;
class Utils;

class KylinDBus : public QObject
{
    Q_OBJECT
public:
    explicit KylinDBus(MainWindow *mw = 0, QObject *parent = nullptr);
    ~KylinDBus();

    void getObjectPath();
    int getAccessPointsNumber();
    int getWiredNetworkNumber();
    QStringList getWifiSsidList();
    QString checkHasWifiConfigFile(QString wifiName);
    void showDesktopNotify(QString message);
    void initConnectionInfo();
    QList<QString> getAtiveLanSsidUuidState();
    QString getActiveWifiUuid();
    QList<QString> getAtiveWifiBSsidUuid(QStringList wifilist);
    void reConnectWiredNet(QString netUuid);
    bool toConnectWiredNet(QString netUuid, QString netIfName);
    void getConnectNetIp(QString netUuid);
    void getLanIpChanged();
    void onWiredSettingNumChanged();

    int getTaskBarPos(QString str);
    int getTaskBarHeight(QString str);
    void initTaskbarGsetting();
    int getTaskbarHeight();
    int getTaskbarPos();

    void getWifiSwitchState();
    bool getSwitchStatus(QString key);
    void setWifiSwitchState(bool signal);
    void setWifiCardState(bool signal);

    void initTransparentState();
    double getTransparentData();
    int checkWifiConnectivity();
    bool checkNetworkConnectivity();
    int getActiveWifiSignal();
    QString getWifiSsid(QString accessPointPath);

    void toGetWifiList();

    QDBusObjectPath wirelessPath; //无线设备的路径
    QList<QDBusObjectPath> multiWirelessPaths; //Wireless Device的对象路径列表
    QList<QDBusObjectPath> multiWiredPaths; //Wired Device的对象路径列表
    QList<QString> multiWiredCableState;//多有线网卡的情况，判断有线网卡对应网线是否插入
    QList<QString> multiWiredMac; //对应有线网卡的Mac地址
    QList<QString> multiWiredIfName; //对应有线网的接口

    bool isWiredCableOn = false; //是否插入了网线
    bool isWirelessCardOn = false; //是否插入了无线网卡

    QString dbusLanIpv4 = "";
    QString dbusLanIpv6 = "";
    QString dbusLanIpv6Method = "";
    QString dbusWifiIpv4Method = "";
    QString dbusWifiIpv6Method = "";
    QString dbusActiveLanIpv4 = "";
    QString dbusActiveLanIpv6 = "";
    QString dbusActiveWifiIpv4 = "";
    QString dbusActiveWifiIpv6 = "";
    QString dbusWifiIpv4 = "";
    QString dbusWifiIpv6 = "";
    QString dbusLanGateway = "";
    QString dbusWiFiCardName = "";
    QString dbusWifiMac = "";
    QString dbusIfName;
    QString dbusMacDefault;
    int dbusActLanDNS;

public slots:
    void onNewConnection(QDBusObjectPath objPath);
    void onConnectionRemoved(QDBusObjectPath objPath);
    void toCreateNewLan();
    bool getWiredCableStateByIfname(QString ifname);
    QString getConnLanNameByIfname(QString ifname);
    void onPropertiesChanged(QVariantMap qvm);
    void onAutoConnect();
    void onLanPropertyChanged(QVariantMap qvm);
    void onLanIpPropertiesChanged();
    void onWifiIpPropertiesChanged();
    void getPhysicalCarrierState(int n);
    void getLanHwAddressState();
    void getWiredCardName();
    void getWirelessCardName();
    void getLanIpDNS(QString uuidName, bool isActNet);
    void getWifiIp(QString uuid);
    QString getLanMAC(QString ifname);
    void getWifiMac(QString netName);
    void slot_timeout();
    void requestScanWifi();

private:
    MainWindow *mw;
    Utils *mUtils;
    QThread *mUtilsThread;

    int a = 0;
    bool isRunningFunction = false;
    QTimer *time = nullptr;
    QList<QDBusObjectPath> oldPaths; //已连接网络的对象路径列表
    QList<QDBusObjectPath> oldSettingPaths; //保存之前的路径
    QStringList oldPathInfo; //某个已连接网络对象路径对应的网络类型(ethernet or wifi)
    bool oldWifiSwitchState; //上一次获取到的wifi开关状态

    QGSettings *m_tastbar_gsettings = nullptr;
    QGSettings *m_gsettings = nullptr;
    QGSettings *m_transparency_gsettings = nullptr;
    QStringList m_lanPathList;//有线网dbuspath列表

signals:
    void updateWiredList(int n);
    void updateWirelessList();
    void requestSendDesktopNotify(QString message);
    void newConnAdded(int type);
    void toGetWifiListFinished(QStringList slist);
};

#endif // KYLINDBUSINTERFACE_H
