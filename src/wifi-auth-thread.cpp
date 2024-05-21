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

#include "wifi-auth-thread.h"

#include <QDebug>

WifiAuthThread::WifiAuthThread()
{
    //检查连接状态
    naManager = new QNetworkAccessManager(this);
//    QString url=QString("http://nmcheck.gnome.org/check_network_status.txt");
    QString url=QString("http://connectivity-check.ubuntu.com/");
    request.setUrl(QUrl(url));
    naManager->get(request);
    //qDebug()<<"-------------------Network state check!----------------------";
    connect(naManager,&QNetworkAccessManager::finished,this,[=](QNetworkReply* reply){
        _isReply=true;
        QNetworkReply::NetworkError err = reply->error();
        //qDebug()<<"----------------NetworkReply error:----------------------"<<err;
        if(err==QNetworkReply::NoError)
        {
            if(!reply->readAll().isEmpty())
                _isConnect=false;
        }
        else
            _isConnect=false;
    });
}

WifiAuthThread::~WifiAuthThread()
{
    delete naManager;
}

void WifiAuthThread::run()
{
    sleep(3);
    QDBusInterface interface( "org.freedesktop.NetworkManager",
            "/org/freedesktop/NetworkManager",
            "org.freedesktop.NetworkManager",
            QDBusConnection::systemBus() );
    QDBusReply<uint> reply= interface.call("CheckConnectivity");
    if (reply.isValid()) {
        //qDebug()<<"---------------------wifi type:----------------------"<<reply.value();
        if (reply.value() == 2) {
            while(1)
            {
                if(!_isConnect)
                    QProcess::startDetached(QString("xdg-open http://nmcheck.gnome.org"));
                if(_isReply)
                    break;
            }
        }
    }
}
