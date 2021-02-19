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

#ifndef ONELANCFORM_H
#define ONELANCFORM_H

#include <QWidget>
#include <QScreen>
#include <QThread>
#include <QEvent>

#include "confform.h"
#include "kylin-network-interface.h"
#include "backthread.h"
#include "ksimplenm.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QVariant>
#include <QVariantMap>

#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8

#define W_ITEM 424
#define H_ITEM 60
#define H_ITEM_EXTEND 162
#define Y_LINE 59
#define X_LINE 2
#define Y_LINE_EXTEND 161
#define X_LINE_EXTEND 2

class MainWindow;

namespace Ui {
class OneLancForm;
}

class OneLancForm : public QWidget
{
    Q_OBJECT

public:
    explicit OneLancForm(QWidget *parent = 0, MainWindow *mw = 0, ConfForm *confForm = 0, KSimpleNM *ksnm = 0);
    ~OneLancForm();

    void setName(QString ssid, QString transSsid, QString uuid, QString interface);
    void setIcon(bool isOn);
    void setLine(bool isShow);
    void setLanInfo(QString str1, QString str2, QString str3, QString str4);

    void setSelected(bool isSelected, bool isCurrName);
    void setTopItem(bool isSelected);
    void setAct(bool isAct);

    void setConnedString(bool showLable, QString str, QString ifname);

    bool isSelected;
    bool isTopItem;
    bool isActive;
    bool isConnected;
    QString ssidName;
    QString m_sConnectType;
    QString uuidName;
    QString ifName;

protected:
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_btnConn_clicked();
    void on_btnConnSub_clicked();
    void on_btnDisConn_clicked();
    void toConnectWiredNetwork();
    void toDisConnWiredNetwork(QString netUuid);

    void slotConnLan();

    void waitAnimStep();
    void startWaiting(bool isToConnect);
    void stopWaiting();

    void on_btnInfo_clicked();

    void on_btnCancel_clicked();

private:
    QTimer *waitTimer = nullptr;
    int waitPage;
    int countCurrentTime;

    Ui::OneLancForm *ui = nullptr;
    MainWindow *mw = nullptr;
    ConfForm *cf = nullptr;
    KSimpleNM *ks = nullptr;

    QString leQssLow, leQssHigh;

signals:
    void selectedOneLanForm(QString lanName, QString uniqueName);
    void connDone(int connFlag);
    void disconnActiveLan();

    void sigConnLan(QString, QString, QString);
};

#endif // ONELANCFORM_H
