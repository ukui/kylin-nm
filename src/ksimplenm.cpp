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

#include "ksimplenm.h"

#include <stdio.h>
#include <unistd.h>
#include <QThread>

#define MAX_LEN 2048
#define MAX_PATH 1024

KSimpleNM::KSimpleNM(QObject *parent) : QObject(parent)
{
    runProcessLan = new QProcess(this);
    connect(runProcessLan, &QProcess::readyRead, this, &KSimpleNM::readProcessLan);
    connect(runProcessLan, SIGNAL(finished(int)), this, SLOT(finishedProcessLan(int)));

    runProcessWifi = new QProcess(this);
    connect(runProcessWifi, &QProcess::readyRead, this, &KSimpleNM::readProcessWifi);
    connect(runProcessWifi, SIGNAL(finished(int)), this, SLOT(finishedProcessWifi(int)));
}

KSimpleNM::~KSimpleNM()
{
    delete runProcessLan;
    delete runProcessWifi;
}

//获取有线网络列表数据
void KSimpleNM::execGetLanList()
{
    if (isExecutingGetLanList || isExecutingGetWifiList) {
        syslog(LOG_DEBUG, "It is running getting lan list when getting lan list");
        qDebug()<<"debug: it is running getting lan list when getting lan list";
        isUseOldLanSlist = true;
        QStringList slistmEmpty;
        slistmEmpty.append("Empty");
        emit getLanListFinished(slistmEmpty);
        return;
    }
    isExecutingGetLanList = true;

    shellOutputLan = "";
    runProcessLan->start("nmcli -f type,uuid,name connection show");
}

//获取无线网络列表数据
void KSimpleNM::execGetWifiList()
{
    if (isExecutingGetWifiList || isExecutingGetLanList) {
        syslog(LOG_DEBUG, "It is running getting wifi list when getting wifi list");
        qDebug()<<"debug: it is running getting wifi list when getting wifi list";
        isUseOldWifiSlist = true;
        QStringList slistmEmpty;
        slistmEmpty.append("Empty");
        emit getWifiListFinished(slistmEmpty);
        return;
    }
    isExecutingGetWifiList = true;

    shellOutputWifi = "";
    runProcessWifi->start("nmcli -f signal,security,freq,ssid device wifi");
}

//读取获取到的结果
void KSimpleNM::readProcessLan()
{
    QString output = runProcessLan->readAll();
    shellOutputLan += output;
}
void KSimpleNM::readProcessWifi()
{
    QString output = runProcessWifi->readAll();
    shellOutputWifi += output;
}

//读取完所有列表数据后发信号，将数据发往mainwindow用于显示网络列表
void KSimpleNM::finishedProcessLan(int msg)
{
    QStringList slist = shellOutputLan.split("\n");
    emit getLanListFinished(slist);
    isExecutingGetLanList = false;
}
void KSimpleNM::finishedProcessWifi(int msg)
{
    QStringList slist = shellOutputWifi.split("\n");
    emit getWifiListFinished(slist);
    isExecutingGetWifiList = false;
}
