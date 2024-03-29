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

#ifndef BACKTHREAD_H
#define BACKTHREAD_H

#include "kylin-dbus-interface.h"
#include "kylin-network-interface.h"

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusArgument>
#include <QSettings>

#define CONFIG_FILE_PATH QDir::homePath() + "/.config/ukui/kylin-nm.conf"
#define WIFI_SWITCH_OPENED "wifi_switch_opened"
#define LAN_SWITCH_OPENED "lan_switch_opened"

#define DEVICE_CONNECTED 0
#define DEVICE_DISCONNECTED 1
#define DEVICE_UNMANAGED 2
#define DEVICE_CONNECTING 3
#define DEVICE_UNAVALIABLE 4

#define PSKFLAG 0

class IFace{
public:
    QString lname;
    QString wname;
    QStringList lcards;
    QStringList wcards;
    int lstate; // 0已连接 1未连接 2已关闭
    int wstate; // 0已连接 1未连接 2已关闭
    bool lmanaged;
};

class BackThread : public QObject
{
    Q_OBJECT
public:
    explicit BackThread(QObject *parent = nullptr);
    ~BackThread();

    static IFace* execGetIface();
    static void saveSwitchButtonState(const QString &key, const QVariant &value);
    static QVariant getSwitchState(const QString &key);
    QString getConnProp(QString connName);
    static QString execChkLanWidth(QString ethName);
    void getTheWifiCardName();
    QProcess *cmdProcessWifi = nullptr;
    QProcess *cmdProcessLan;
    QString currConnLanUuid;
    QString currConnLanType;
    QList<QDBusObjectPath> wifiCardPaths;
    QString wifiIfnameInUse;

public slots:
    void execEnNet();
    void execDisNet();
    void execEnWifi();
    void rfKillexecEnWifi();
    void execDisWifi();
    void rfkillExecDisWifi();
    void execConnLan(QString connName, QString ifname, QString connectType);
    void execConnWifi(QString connName, QString connIfName);
    void execReconnWIfi(QString uuid);
    void execConnWifiPWD(QString connName, QString password, QString connType, QString security, QString ifname);
    void execConnWifiPsk(QString cmd);
    void execConnHiddenWifiWPA(int secuType, QString connName, QString password);
    void execConnRememberedHiddenWifi(QString connName);

    void disConnSparedNetSlot(QString type);
    void disConnLanOrWifi(QString type);

    void onReadOutputWifi();
    void onReadErrorWifi();
    void dellConnectWifiResult(QString info);

    void onReadOutputLan();
    void onReadErrorLan();
    void dellConnectLanResult(QString info);
    void getInitStatus();
signals:
    void enNetDone();
    void disNetDone();
    void enWifiDone();
    void enWifiDoneByRfkill();
    void disWifiDone();
    void disWifiDoneByRfkill();
    void wifiStatus(bool status);
    void getWifiStatusComplete();
    void connDone(int connFlag);

    void btFinish();
    void btFinishByRfkill();
};

#endif // BACKTHREAD_H
