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
#ifndef NETCONNECT_H
#define NETCONNECT_H

#include "kswitchbutton.h"
#include "kborderlessbutton.h"
using namespace kdk;


#include <QObject>
#include <QtPlugin>
#include <QTranslator>
#include <QApplication>


#include <QTimer>
#include <QStringList>
#include <QString>
#include <QGSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMap>
#include <QPropertyAnimation>
#include <QPoint>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <kylin-nm/kylin-nm-interface.h>
#include <kylin-nm/kylinnetworkmanager.h>
#include "lanitem.h"
#include "../component/DeviceFrame/deviceframe.h"
#include "itemframe.h"
#include "../component/Divider/divider.h"
#include "../component/Pages/netdetail.h"
#include "../component/NetworkMode/networkmodeconfig.h"
#include "../component/NetworkMode/firewalldialog.h"

enum {
    DISCONNECTED,
    NOINTERNET,
    CONNECTED
};

class NetConnect : public QObject, Interface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kylin.network")
    Q_INTERFACES(Interface)

public:
    NetConnect();
    ~NetConnect();

    QWidget * pluginUi() Q_DECL_OVERRIDE;   // 插件主界面---setPluginType后调用
    void setPluginType(PluginType type, bool useSwitch = true) Q_DECL_OVERRIDE;     // 设置插件类型
    void setParentWidget(QWidget*){;}

private:
    bool m_isSimpleMode = true;
    bool m_useSwitch = true;

    void initUi();
    void initComponent();
    void initConnect();
    void runExternalApp();

//    void showDesktopNotify(const QString &message);

    //开关相关
    void setSwitchStatus();
    void hideLayout(QVBoxLayout * layout);
    void showLayout(QVBoxLayout * layout);

    int getInsertPos(QString connName, QVBoxLayout* layout);

    void activeConnect(QString uuid, QString deviceName);
    void deActiveConnect(QString uuid);

    //获取设备列表
    void getDeviceStatusMap(QMap<QString, bool> &map);
    void initNet();
    void initNetListFromDevice(QString deviceName);
    //处理列表增加
    void addLanItem(ItemFrame *frame, QString devName, KyWiredItem item);
    //增加设备
    void addDeviceFrame(QString devName, bool isEnable);
    //减少设备
    void removeDeviceFrame(QString devName);
    //增加一项
    void addOneLanFrame(ItemFrame *frame, QString devName, QStringList infoList);
    //减少一项
    void removeOneLanFrame(ItemFrame *frame, QString deviceName, QString uuid);

    //单个lan连接状态变化
    void itemActiveConnectionStatusChanged(LanItem *item, KyConnectState status);

    //显示网络属性页
    void showLanDetailPage(QString deviceName, LanItem *item);

    //初始化已激活网络的网络模式
    void initActiveNetworkMode(QString deviceName, KyActivateItem activeItem);
protected:
    bool eventFilter(QObject *w,QEvent *e);

private:

    KyNetworkManager* manager;
    QThread*        thread;

    QWidget*        pluginWidget = nullptr;
    bool            mFirstLoad = true;

    QVBoxLayout*    m_mainLayout = nullptr;
    QFrame*         m_titleFrame = nullptr;
    QHBoxLayout*    m_titleLayout = nullptr;
    QLabel*         m_titleLabel = nullptr;
    KSwitchButton*  m_wiredSwitch = nullptr;
    Divider*        m_titleDivider = nullptr;

    QFrame*         m_scrollFrame = nullptr;
    QVBoxLayout*    m_scrollLayout = nullptr;
    QScrollArea*    m_scrollArea = nullptr;

    QWidget*        m_listWidget = nullptr;
    QVBoxLayout*    m_scrollAreaLayout = nullptr;

    Divider*        m_settingsDivider = nullptr;
    QFrame*         m_settingsFrame = nullptr;
    QHBoxLayout*    m_settingsLayout = nullptr;
    KBorderlessButton* m_settingsLabel = nullptr;

    QGSettings      *m_switchGsettings = nullptr;
    QGSettings      *m_styleGsettings = nullptr;

    QMap<QString, bool> deviceStatusMap;
    QMap<QString, ItemFrame *> deviceFrameMap;
    QMap<QString, NetDetail*> m_lanDetailPagePtrMap;

Q_SIGNALS:

    //general
    void setWiredEnabled(bool enabled);
    //有线无线公用
    void deleteConnect(const QString &connectUuid);
    void activateConnection(const QString connectUuid, const QString deviceName);
    void deactivateConnection(const QString &activeConnectUuid);
    //wired 单设备启用禁用
    void setWiredDeviceEnable(const QString &interface, bool enable);

    void createWiredConnect(KyConnectSetting connectSettingsInfo);
    void updateIpv4AndIpv6SettingInfo(const QString &uuid, const KyConnectSetting &connectSettingsInfo);

    void connectStateChanged(QString uuid, KyConnectState status);

private Q_SLOTS:
    void updateLanInfo(QString deviceName, QString connectUuid, QString connectName, QString connectPath);

    void onLanAdd(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void onLanRemove(QString dbusPath);

    void onActiveConnectionChanged(QString deviceName, QString uuid, KyConnectState status);

//    void onDeviceStatusChanged();
    void onDeviceNameChanged(QString, QString);
    void onDeviceCarrierChange(QString deviceName, bool pluged);

    void onDeviceManagedChange(QString, bool);
    void onWiredEnabledChanged(bool);

    void onDeviceAdd(QString deviceName);
    void onDeviceRemove(QString deviceName);
    void updateNetworkModeState(QString deviceName, QString ssid, QString uuid, KyConnectState status);
};

#endif // NETCONNECT_H
