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

class IFace{
public:
    QString lname;
    QString wname;
    int lstate; // 0已连接 1未连接 2已关闭
    int wstate; // 0已连接 1未连接 2已关闭
};

class BackThread : public QObject
{
    Q_OBJECT
public:
    explicit BackThread(QObject *parent = nullptr);
    ~BackThread();

    IFace* execGetIface();
    QString getConnProp(QString connName);
    QString execChkLanWidth(QString ethName);
    QProcess *cmdConnWifi = nullptr;
    QProcess *process;
    QString currConnLanUuid;
    QString currConnLanType;

public slots:
    void execEnNet();
    void execDisNet();
    void execEnWifi();
    void execDisWifi();
    void execConnLan(QString connName, QString ifname, QString connectType);
    void execConnWifi(QString connName);
    void execReconnWIfi(QString uuid);
    void execConnWifiPWD(QString connName, QString password, QString connType);
    void execConnWifiPsk(QString cmd);
    void execConnHiddenWifiWPA(QString connName, QString password);
    void execConnRememberedHiddenWifi(QString connName);

    void disConnSparedNetSlot(QString type);
    void disConnLanOrWifi(QString type);

    void onReadOutputWifi();
    void onReadErrorWifi();
    void dellConnectWifiResult(QString info);

    void onReadOutputLan();
    void onReadErrorLan();
    void dellConnectLanResult(QString info);


signals:
    void enNetDone();
    void disNetDone();
    void enWifiDone();
    void disWifiDone();

    void connDone(int connFlag);

    void btFinish();
    void disFinish();
    void ttFinish();
};

#endif // BACKTHREAD_H
