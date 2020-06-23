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

#ifndef KSIMPLENM_H
#define KSIMPLENM_H

#include <sys/syslog.h>

#include <QObject>
#include <QProcess>
#include <QDebug>

class KSimpleNM : public QObject
{
    Q_OBJECT
public:
    explicit KSimpleNM(QObject *parent = nullptr);

    QProcess *runShellProcess = nullptr;
    QString shellOutput;
    int type;
    bool isExecutingGetLanList = false; //是否正在执行获取有线网列表
    bool isExecutingGetWifiList = false; //是否正在执行获取无线网列表
    bool isUseOldLanSlist = false; //是否应该要用上一次获取的有线列表

    void execGetLanList();
    void execGetWifiList();

signals:
    void getLanListFinished(QStringList slist);
    void getWifiListFinished(QStringList slist);

public slots:
    void readProcess();
    void finishedProcess(int msg);
};

#endif // KSIMPLENM_H
