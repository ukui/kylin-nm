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
