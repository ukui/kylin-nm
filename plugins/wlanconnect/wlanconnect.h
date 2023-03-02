 /* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
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

#ifndef WLANCONNECT_H
#define WLANCONNECT_H

#include "kswitchbutton.h"
#include "kborderlessbutton.h"
using namespace kdk;

#include <QObject>
#include <QtPlugin>

#include <QNetworkInterface>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QtNetwork>

#include <QTimer>
#include <QStringList>
#include <QString>
#include <QGSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMap>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QtDBus>
#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>

#include <kylin-nm/kylin-nm-interface.h>
#include <kylin-nm/kylinnetworkmanager.h>
#include <kylin-nm/kylin-agent/kylinagent.h>
#include "../component/DeviceFrame/deviceframe.h"
#include "itemframe.h"
#include "wlanitem.h"
#include "hiddenwifi/enterprisewlanpage.h"
#include "../component/Pages/netdetail.h"

class WlanConnect : public QObject, Interface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kylin.network")
    Q_INTERFACES(Interface)

public:
    WlanConnect();
    ~WlanConnect();

    QWidget * pluginUi() Q_DECL_OVERRIDE;   // 插件主界面---setPluginType后调用
    void setPluginType(PluginType type, bool useSwitch = true) Q_DECL_OVERRIDE;     // 设置插件类型
    void setParentWidget(QWidget*);

private:
    bool m_isSimpleMode = true;
    QDBusInterface *m_interfaceUi = nullptr;
    int m_initDbusCount = 0;

    void initUi();
    void initComponent();
    void initConnect();
    void initDbus();
    void runExternalApp();
    void initSearchText();

//    void showDesktopNotify(const QString &message);

    void updateIcon(WlanItem *item, int signalStrength, QString security, bool isApConnection);
    void resortWifiList(ItemFrame *frame, QList<KyActivateItem> connectItemList, QList<KyWirelessNetItem> list);

    //单wifi图标
    int  setSignal(int lv);
    QString wifiIcon(bool isLock, int strength);


    //开关相关
    void setSwitchStatus(bool status);
    void hideLayout(QVBoxLayout * layout);
    void showLayout(QVBoxLayout * layout);


    //获取设备列表
    void getDeviceList(QStringList &list);
    //初始化设备wifi列表
    void initNet();
    void initNetListFromDevice(QString deviceName);

    //增加设备
    void addDeviceFrame(QString devName);
    //减少设备
    void removeDeviceFrame(QString devName);
    //无序增加ap
    void addOneWlanFrame(ItemFrame *frame, QString deviceName, KyWirelessNetItem &wlanInfo);
    //减少ap
    void removeOneWlanFrame(ItemFrame *frame, QString deviceName, QString ssid);
    //有序增加ap
    void insertOneWlanFrame(ItemFrame *frame, QString devName, KyWirelessNetItem &wlanInfo);


    //单个wifi连接状态变化
    void itemActiveConnectionStatusChanged(WlanItem *item, int status);

    void setOtherItemExpandedFalse(QString devName, QString ssid);
    //显示网络属性页
    void showWlanDetailPage(QString deviceName, WlanItem *item);

protected:
    bool eventFilter(QObject *w,QEvent *e);

private:
    KyNetworkManager* manager;
    QThread*        thread;

    QWidget* parentWidget = nullptr;

    QWidget* pluginWidget = nullptr;

    //设备列表
    QStringList deviceList;
    //设备名 + 单设备frame
    QMap<QString, ItemFrame *> deviceFrameMap;

    QTimer * m_scanTimer = nullptr;
    QTimer * m_updateTimer = nullptr;

    bool            m_firstLoad = true;

    QVBoxLayout*    m_mainLayout = nullptr;
    QFrame*         m_titleFrame = nullptr;
    QHBoxLayout*    m_titleLayout = nullptr;
    QLabel*         m_titleLabel = nullptr;
    KSwitchButton*  m_wirelessSwitch = nullptr;
    Divider*        m_titleDivider = nullptr;

    QFrame*         m_scrollFrame = nullptr;
    QVBoxLayout*    m_scrollLayout = nullptr;
    QScrollArea*    m_scrollArea = nullptr;

    QWidget*        m_listWidget = nullptr;
    QVBoxLayout*    m_scrollAreaLayout = nullptr;

    Divider*        m_settingsDivider = nullptr;
    QFrame*         m_settingsFrame = nullptr;
    QHBoxLayout*    m_settingsLayout = nullptr;
    KyLable         * m_settingsLabel = nullptr;

    WlanItem* findItem(QString devName, QString ssid);
    QMap<QString, QMap<QString, NetDetail*>> m_wlanDetailPagePtrMap;

    QGSettings      *m_styleGsettings = nullptr;

Q_SIGNALS:
    void requestWirelessScan();
    void setWirelessNetworkEnabled(bool enabled);
    //=======================连接/断开操作====================
    void  activateConnection(const QString connectUuid, const QString deviceName);
    void  deactivateConnection(const QString &activeConnectUuid);
    //连接无本地配置的非企业网热点
    void addAndActivateNormalWifi(KyWirelessConnectSetting connSettingInfo, KySecuType type);
//    //连接无本地配置的企业网热点
//    void addAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo &info,
//                                                    KyWirelessConnectSetting &connSettingInfo);
//    void addAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo &info,
//                                                    KyWirelessConnectSetting &connSettingInfo);
//    void addAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo &info,
//                                                     KyWirelessConnectSetting &connSettingInfo);

    void deleteConnect(QString uuid);

    void connectStateChanged(QString uuid, KyConnectState status);

private Q_SLOTS:
    void onNetworkAdd(QString deviceName, KyWirelessNetItem wlanInfo);
    void onNetworkRemove(QString deviceName, QString wlannName);
    void onNetworkUpdate(QString deviceName, QString wlannName, KyWirelessNetItem secuType);
    void onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, KyConnectState status);

    void updateList(QString devName, QList<KyActivateItem> connectItemList, QList<KyWirelessNetItem> list);
    void onDeviceStatusChanged();
    void onDeviceNameChanged(QString, QString);

    void reScan();

    void onNetworkSecuTypeChange(QString deviceName, QString ssid, QString securityType);
    void onWifiEnabledChanged(bool);

    void onWirelessConnectionRemove(QString deviceName, QString ssid);
    void onWirelessConnectionAdd(QString deviceName, QString ssid, QString uuid, QString dbusPath);
    void onWirelessConnectionUpdate(QString deviceName, QString ssid, QString uuid, QString dbusPath, KySecuType connectSecuType);
    void onWirelessDeviceAdd(QString deviceName);
    void onWirelessDeviceRemove(QString deviceName);

    //for dbus
    void showDetailPage(QString deviceName, QString ssid);
    void showAddNetworkPage(const QString deviceName);
    void activateWirelessConnection(QString deviceName, QString ssid);
};
#endif // WLANCONNECT_H
