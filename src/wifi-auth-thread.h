#ifndef WIFI_AUTH_THREAD_H
#define WIFI_AUTH_THREAD_H
#include <QThread>
#include <QDBusInterface>
#include <QDBusReply>

class WifiAuthThread:public QThread
{
protected:
    virtual void run();
};

#endif //WIFI_AUTH_THREAD_H
