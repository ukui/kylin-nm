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
