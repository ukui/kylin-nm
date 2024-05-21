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

#ifndef WIFI_AUTH_THREAD_H
#define WIFI_AUTH_THREAD_H
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QDBusInterface>
#include <QDBusReply>

class WifiAuthThread:public QThread
{
protected:
    virtual void run();
    QNetworkRequest request;
    QNetworkAccessManager* naManager=nullptr;
    bool _isConnect=true;//是否已联网
    bool _isReply=false;//联网状态检查是否收到回复

public:
    WifiAuthThread();
    ~WifiAuthThread();
};

#endif //WIFI_AUTH_THREAD_H
