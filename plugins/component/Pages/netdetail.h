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
#ifndef NETDETAIL_H
#define NETDETAIL_H

#include <QDialog>
#include <QPainter>
#include <QObject>
#include <QStackedWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QVariantMap>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QThread>

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>
#include <QCloseEvent>

#include "detailpage.h"
#include "ipv4page.h"
#include "ipv6page.h"
#include "securitypage.h"
#include "creatnetpage.h"
#include "configpage.h"
#include "coninfo.h"
#include "kwidget.h"
#include "ktabbar.h"
#include <kylin-nm/kylinnetworkmanager.h>

using namespace kdk;

#define  TAB_WIDTH  60
#define  TAB_HEIGHT 36
#define  TAB_HEIGHT_TABLET 48

enum NetworkModeType {
    DBUS_INVAILD = -2,
    NO_CONFIG = -1,
    KSC_FIREWALL_PUBLIC = 0,
    KSC_FIREWALL_PRIVATE
};

class NetTabBar : public KTabBar
{
    Q_OBJECT
public:
    explicit NetTabBar(QWidget *parent = nullptr);
    ~NetTabBar();
protected:
    QSize sizeHint() const;
    QSize minimumTabSizeHint(int index) const;

private Q_SLOTS:
    void onModeChanged(bool mode);

};

class ThreadObject : public QObject
{
    Q_OBJECT
public:
    ThreadObject(QString deviceName, QObject *parent = nullptr);
    ~ThreadObject();
    void stop();
private:
    QString m_devName;
    volatile bool m_isStop;

public Q_SLOTS:
//    void checkIpv4ConflictThread(const QString &ipv4Address);
//    void checkIpv6ConflictThread(const QString &ipv6Address);

Q_SIGNALS:
//    bool ipv4IsConflict(bool isConflict);
//    bool ipv6IsConflict(bool isConflict);
};

class NetDetail : public QWidget
{
    Q_OBJECT

public:
    NetDetail(QString interface, QString name, QString uuid, bool isActive, bool isWlan, bool isCreateNet, QWidget *parent = nullptr);
    ~NetDetail();

    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *w, QEvent *event);

    void setDetailPageShowed(bool state);

private:
    void initUI();
    void centerToScreen();
    void initComponent();
    void getConInfo(ConInfo &conInfo);
    void loadPage();
    void pagePadding(QString netName, bool isWlan);

    void setConfirmEnable();

    //详情ssid 带宽 物理地址 无线额外(安全性 频带 通道)
    void getBaseInfo(KyDetailInfo &detailInfo, ConInfo &conInfo);
    //详情ipv4 ipv6 ipv4Dns
    void getDynamicIpInfo(KyConnectSetting &connectSetting, ConInfo &conInfo);

    bool createWiredConnect();
    bool updateConnect();
    bool checkWirelessSecurity(KySecuType secuType);
    void updateWirelessPersonalConnect();
    void updateWirelessEnterPriseConnect(KyEapMethodType enterpriseType);

    void showDesktopNotify(const QString &message, QString soundName);

    void setNetdetailSomeEnable(bool on);
    void startObjectThread();
    void setNetTabToolTip();

    //安全中心-获取网络模式配置
    int getNetworkModeConfig(QString uuid);
    //安全中心-设置网络模式配置
    void setNetworkModeConfig(QString uuid, QString cardName, QString ssid, int mode);

private:
    QStackedWidget * stackWidget;

    DetailPage     * detailPage;
    Ipv4Page       * ipv4Page;
    Ipv6Page       * ipv6Page;
    SecurityPage   * securityPage;
    CreatNetPage   * createNetPage;
    ConfigPage     * configPage;

    QWidget      * centerWidget;
    QWidget      * bottomWidget;
    QScrollArea  * m_secuPageScrollArea;
    QScrollArea  * m_ipv4ScrollArea;
    QScrollArea  * m_ipv6ScrollArea;

    QPushButton  * cancelBtn;
    QPushButton  * forgetBtn;
    QPushButton  * confimBtn;

    QFrame       * pageFrame;
    NetTabBar    *m_netTabBar = nullptr;

    QString      m_name;
    QString      m_uuid;
    QString      m_deviceName;

    bool         isWlan;
    bool         m_isCreateNet;
    bool         isActive;
    bool         isHideWlan;

    bool         isCreateOk;
    bool         isDetailOk;
    bool         isIpv4Ok;
    bool         isIpv6Ok;
    bool         isSecuOk;
    bool         isConfirmBtnEnable;
    bool         m_hasDetailPageShowed = false;
    ConInfo      m_info;

    ThreadObject *m_object;
    QThread *m_objectThread;

    KyNetworkManager* m_manager;
    QThread*          m_thread;
    KyDetailInfo      m_detailInfo;
    KyConnectSetting  m_connectSetting;
    KyWpaPasswordInfo m_pwdInfo;
    NetworkModeType   m_networkMode = DBUS_INVAILD;

private Q_SLOTS:
    void onBtnConfirmClicked();
    void onBtnForgetClicked();
    void onPaletteChanged();

protected Q_SLOTS:
    void currentRowChangeSlot(int row);

Q_SIGNALS:
    void detailPageClose(QString, QString, QString);
    void createPageClose(QString);
    void currentChanged(int);
    void checkCurrentIpv4Conflict(const QString &address);
    void checkCurrentIpv6Conflict(const QString &address);
    void deleteConnect(QString uuid);
    void sigCreateWiredConnect(KyConnectSetting connectSettingsInfo);
    //Ipv4/Ipv6修改
    void sigUpdateIpv4AndIpv6SettingInfo(const QString &uuid, const KyConnectSetting &connectSettingsInfo);
    //连接修改(安全改为个人/None)
    void sigUpdateWirelessPersonalConnect(const QString &uuid, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange);
    //连接修改(安全改为改为企业)
    void sigUpdateWirelessEnterPriseTlsConnect(const QString &uuid, const KyEapMethodTlsInfo &tlsinfo);
    void sigUpdateWirelessEnterPrisePeapConnect(const QString &uuid, const KyEapMethodPeapInfo &peapInfo);
    void sigUpdateWirelessEnterPriseTtlsConnect(const QString &uuid, const KyEapMethodTtlsInfo &ttlsInfo);
    void sigUpdateWirelessEnterPriseLeapConnect(const QString &uuid, const KyEapMethodLeapInfo &leapInfo);
    void sigUpdateWirelessEnterPrisePwdConnect(const QString &uuid, const KyEapMethodPwdInfo &pwdInfo);
    void sigUpdateWirelessEnterPriseFastConnect(const QString &uuid, const KyEapMethodFastInfo &fastInfo);

    void sigActivateConnection(const QString connectUuid, const QString deviceName);
    void sigWirelessAutoConnectStateChanged(const QString &uuid, bool bAutoConnect);
};

#endif // NETDETAIL_H
