/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
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
    runShellProcess = new QProcess(this);

    connect(runShellProcess, &QProcess::readyRead, this, &KSimpleNM::readProcess);
    connect(runShellProcess, SIGNAL(finished(int)), this, SLOT(finishedProcess(int)));
}

void KSimpleNM::execGetLanList(){
    if (isExecutingGetWifiList){
        syslog(LOG_DEBUG, "It is running getting wifi list when getting lan list");
        qDebug()<<"debug: it is running getting wifi list when getting lan list";
        isUseOldLanSlist = true;
    }
    isExecutingGetLanList = true;
    shellOutput = "";
    type = 0;
    runShellProcess->start("nmcli -f type,device,name connection show");
}

void KSimpleNM::execGetWifiList(){
    isExecutingGetWifiList = true;
    shellOutput = "";
    type = 1;
    runShellProcess->start("nmcli -f signal,rate,security,ssid device wifi");
}

void KSimpleNM::readProcess(){
    QString output = runShellProcess->readAll();
    shellOutput += output;
}

void KSimpleNM::finishedProcess(int msg){
    QStringList slist = shellOutput.split("\n");
    if(type == 0){
        emit getLanListFinished(slist);
        isExecutingGetLanList = false;
    }else{
        emit getWifiListFinished(slist);
        isExecutingGetWifiList = false;
    }
}
