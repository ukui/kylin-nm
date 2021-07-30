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
#include <QtConcurrent>

#define MAX_LEN 2048
#define MAX_PATH 1024

KSimpleNM::KSimpleNM(QObject *parent) : QObject(parent)
{
//    runProcessLan = new QProcess(this);
//    connect(runProcessLan, &QProcess::readyRead, this, &KSimpleNM::readProcessLan);
//    connect(runProcessLan, SIGNAL(finished(int)), this, SLOT(finishedProcessLan(int)));
//    connect(runProcessLan, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, &KSimpleNM::finishedProcessLan);

//    runProcessWifi = new QProcess(this);
//    connect(runProcessWifi, &QProcess::readyRead, this, &KSimpleNM::readProcessWifi);
//    connect(runProcessWifi, SIGNAL(finished(int)), this, SLOT(finishedProcessWifi(int)));
//    connect(runProcessWifi, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, &KSimpleNM::finishedProcessWifi);

//    runProcessConn = new QProcess(this);
//    connect(runProcessConn, &QProcess::readyRead, this, &KSimpleNM::readProcessConn);
//    connect(runProcessConn, SIGNAL(finished(int)), this, SLOT(finishedProcessConn(int)));
//    connect(runProcessConn, static_cast<void(QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, &KSimpleNM::finishedProcessConn);
}

KSimpleNM::~KSimpleNM()
{
//    delete runProcessLan;
//    delete runProcessWifi;
}

//获取有线网络列表数据
void KSimpleNM::execGetLanList()
{
    if (isExecutingGetLanList) {
        qDebug()<<"debug: it is running getting lan list when getting lan list";
        isUseOldLanSlist = true;
        QStringList slistmEmpty;
        slistmEmpty.append("Empty");
        emit getLanListFinished(slistmEmpty);
        return;
    }
    isExecutingGetLanList = true;

    QtConcurrent::run([=]() {
        QProcess *runProcessLan = new QProcess(this);
//        connect(runProcessLan, &QProcess::readyRead, this, &KSimpleNM::readProcessLan);
//        connect(runProcessLan, SIGNAL(finished(int)), this, SLOT(finishedProcessLan(int)));
        shellOutputLan = "";
        QString getCmd = "export LANG='zh_CN.UTF-8';export LANGUAGE='zh_CN:zh';nmcli -f type,uuid,name connection show";
        QStringList options;
        options << "-c" << getCmd;
        runProcessLan->start("/bin/bash",options);
//        runProcessLan->start(getCmd);
        runProcessLan->waitForFinished();
        shellOutputLan = runProcessLan->readAll();
        QStringList slist = shellOutputLan.split("\n");
        qDebug()<<"Get Lan list Finished!";
        emit this->getLanListFinished(slist);
        isExecutingGetLanList = false;
        delete runProcessLan;
        runProcessLan = NULL;
    });
    //runProcessLan->start("nmcli -f type,uuid,name connection show");
}

//获取无线网络列表数据
void KSimpleNM::execGetWifiList(const QString& wname, const bool &isHuaweiPc)
{
    if (isExecutingGetWifiList) {
        qDebug() << "It is running getting wifi list when getting wifi list";
        isUseOldWifiSlist = true;
        QStringList slistmEmpty;
        slistmEmpty.append("Empty");
        emit requestRevalueUpdateWifi();
        emit getWifiListFinished(slistmEmpty);
        return;
    }
    isExecutingGetWifiList = true;
    shellOutputWifi = "";
    QString cmd;
    //将ssid放置在最后一列以防ssid存在中文或特殊字符导致其后面的列不对齐
    if (wname.isEmpty() || wname == "") {
        if (isHuaweiPc)
            cmd = "nmcli -f in-use,signal,security,freq,bssid,dbus-path,category,ssid device wifi";
        else
            cmd = "nmcli -f in-use,signal,security,freq,bssid,dbus-path,ssid device wifi";
    } else {
        if (isHuaweiPc)
            cmd = "nmcli -f in-use,signal,security,freq,bssid,dbus-path,category,ssid device wifi list ifname " + wname;
        else
            cmd = "nmcli -f in-use,signal,security,freq,bssid,dbus-path,ssid device wifi list ifname " + wname;
    }
    QtConcurrent::run([=]() {
        QProcess *runProcessWifi = new QProcess(this);
        QStringList options;
        options << "-c" << cmd;
        runProcessWifi->start("/bin/bash",options);
        runProcessWifi->waitForFinished();
        shellOutputWifi = runProcessWifi->readAll();
        QStringList slist = shellOutputWifi.split("\n");
        qDebug()<<"Get Wifi list Finished!";
        emit this->getWifiListFinished(slist);
        isExecutingGetWifiList = false;
        delete runProcessWifi;
        runProcessWifi = NULL;
    });
//    runProcessWifi->start(cmd);
}

//获取保存的网络列表数据
void KSimpleNM::execGetConnList()
{
    shellOutputConn = "";
//    runProcessConn->start("nmcli -f name connection show");
    QtConcurrent::run([=]() {
        QProcess *runProcessConn = new QProcess(this);
        runProcessConn->start("nmcli -f name connection show");
        runProcessConn->waitForFinished();
        shellOutputConn = runProcessConn->readAll();
        QStringList slist = shellOutputConn.split("\n");
        qDebug()<<"Get Connection list Finished!";
        emit this->getConnListFinished(slist);
        delete runProcessConn;
        runProcessConn = NULL;
    });
}

////读取获取到的结果
//void KSimpleNM::readProcessLan()
//{
//    QString output = runProcessLan->readAll();
//    shellOutputLan += output;
//}
//void KSimpleNM::readProcessWifi()
//{
//    QString output = runProcessWifi->readAll();
//    shellOutputWifi += output;
//}
//void KSimpleNM::readProcessConn()
//{
//    QString output = runProcessConn->readAll();
//    shellOutputConn += output;
//}

////读取完所有列表数据后发信号，将数据发往mainwindow用于显示网络列表
//void KSimpleNM::finishedProcessLan(int msg)
//{
//    QStringList slist = shellOutputLan.split("\n");
//    emit getLanListFinished(slist);
//    isExecutingGetLanList = false;
//}
//void KSimpleNM::finishedProcessWifi(int msg)
//{
//    QStringList slist = shellOutputWifi.split("\n");
//    emit getWifiListFinished(slist);
//    isExecutingGetWifiList = false;
//}
//void KSimpleNM::finishedProcessConn(int msg)
//{
//    QStringList slist = shellOutputConn.split("\n");
//    emit getConnListFinished(slist);
//}
