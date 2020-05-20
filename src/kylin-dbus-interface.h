#ifndef KYLINDBUSINTERFACE_H
#define KYLINDBUSINTERFACE_H

#include <sys/syslog.h>

#include <QObject>
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QVariant>
#include <QVariantMap>
#include <QGSettings/QGSettings>
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
    void showDesktopNotify(QString message);
    void initConnectionInfo();
    void connectWiredNet(QString netName);

    int getTaskBarPos(QString str);
    int getTaskBarHeight(QString str);
    void initTaskbarGsetting();
    int getTaskbarHeight();
    int getTaskbarPos();

    void getWifiSwitchState();
    bool getSwitchStatus(QString key);
    void setWifiSwitchState(bool signal);
    void setWifiCardState(bool signal);

    QDBusObjectPath wiredPath;
    QDBusObjectPath wirelessPath;

    bool isWiredCableOn = false;
    bool isWirelessCardOn = false;

    QString dbusLanCardName = "";
    QString dbusLanIpv4 = "";
    QString dbusLanIpv6 = "";
    QString dbusLanGateway = "";
    QString dbusLanMac = "";
    QString dbusWiFiCardName = "";
    QString dbusWifiMac = "";

public slots:
    void onNewConnection(QDBusObjectPath objPath);
    void onConnectionRemoved(QDBusObjectPath objPath);
    void onPropertiesChanged(QVariantMap qvm);
    void onLanPropertyChanged(QVariantMap qvm);
    void onWifiPropertyChanged(QVariantMap qvm);
    void onAccessPointAdded(QDBusObjectPath objPath);
    void getPhysicalCarrierState(int n);
    void getLanHwAddressState();
    void getWiredCardName();
    void getWirelessCardName();
    void getLanIp(QString netName);
    void getWifiMac(QString netName);
    void slot_timeout();

private:
    MainWindow *mw;

    int a = 0;
    bool isRunningFunction = false;
    QTimer *time = nullptr;
    QStringList oldPathInfo;
    QList<QDBusObjectPath> oldPaths;

    QGSettings *m_tastbar_gsettings = nullptr;
    QGSettings *m_gsettings = nullptr;

signals:
    void updateWiredList(int n);
};

#endif // KYLINDBUSINTERFACE_H
