#ifndef KYLINDBUSINTERFACE_H
#define KYLINDBUSINTERFACE_H

#include <sys/syslog.h>
#include <QObject>
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QDBusObjectPath>
#include <QDBusReply>
#include <QVariant>
#include <QVariantMap>
#include <QTimer>

class MainWindow;

class KylinDBus : public QObject
{
    Q_OBJECT
public:
    explicit KylinDBus(MainWindow *mw = 0, QObject *parent = nullptr);

    void getObjectPath();
    int getAccessPointsNumber();
    int getLanConnState();

    QDBusObjectPath wiredPath;
    QDBusObjectPath wirelessPath;

    bool isWiredCableOn = false;
    bool isWirelessCardOn = false;

    QString dbusLanCardName;
    QString dbusLanIpv4 = "";
    QString dbusLanIpv6 = "";
    QString dbusLanMac = "";
    QString dbusWiFiCardName;
    QString dbusWifiMac = "";

public slots:
    void onLanPropertyChanged(QVariantMap qvm);
    void onWifiPropertyChanged(QVariantMap qvm);
    void onAccessPointAdded(QDBusObjectPath objPath);
    void getPhysicalCarrierState(int n);
    void getLanHwAddressState();
    void getWiredCardName();
    void getWirelessCardName();
    void getLanIp(QString netName);
    void getWifiMac(QString netName, int num);
    void getActWifiMac(QString netName);
    void slot_timeout();

private:
    MainWindow *mw;

    int a = 0;
    bool isRunningFunction = false;
    QTimer *time;
};

#endif // KYLINDBUSINTERFACE_H
