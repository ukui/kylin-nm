#ifndef KNMDBUSCALLER_H
#define KNMDBUSCALLER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QString>
#include <QVariant>
#include <QIcon>
#include <QtDBus/QtDBus>

#include "knmlandatakeeper.h"
#include "knmwlandatakeeper.h"
#include "CSingleton.h"

//dbus调用类
class KnmDBusCaller : public QObject
{
    Q_OBJECT
protected:
    explicit KnmDBusCaller(QObject *parent = nullptr);
    ~KnmDBusCaller(void);

//用于interface调用数据
public:
    QMap<QString, NetDevicePtr> wiredDeviceList();

    QMap<QString, NetDevicePtr> wirelessDeviceList();

    QVariantList wiredDeviceConnList(QString devName);

    QVariantList wirelessDeviceConnList(QString devName);

    QString getWiFiIcon(QString signalStrength, QString security, QString isApConnection, int category);

    bool wirelessSwitchState();

    bool wiredMainSwitchState();

    QString upwardRateDate();

    QString downwardRateDate();

    void rescanWirelessConn();

    void activateConnect(QString devName, QString conUid, int type);

    void deActivateConnect(QString devName, QString conUid, int type);

    void setWirelessSwitchEnable(bool enable);

    void passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect);

    void setWiredMainSwitchEnable(bool enable);

    void setUpwareRateData(QString str);

    void setDownwareRateData(QString str);

    void showPropertyWidget(QString devName, QString ssid);

    void deleteConnect(int type, QString ssid);

    void showAddOtherWlanPage(QString devName);
//用于调用kylin-nm的dbus
protected:
    QDBusPendingCallWatcher * asyncCall(const QString & methed, const QList<QVariant> & params);

    void getWiredDeviceMap();

    void getWirelessDeviceMap();

    void getWiredConList(QString dev);

    void getWirelessConList(QString dev);

    void getWirelessSwitchState();

    void getWiredMainSwitchState();

    void getWiredConListFinished(QDBusPendingCallWatcher *watcher, QString dev);

    void getWirelessConListFinished(QDBusPendingCallWatcher *watcher, QString dev);

//dbus调用的响应槽函数
protected slots:
    void updateWiredDeviceMap();

    void updateWirelessDeviceMap();

    void updateWirelessDevice();

    void updateWiredDeviceMapFinished(QDBusPendingCallWatcher *watcher);

    void updateWirelessDeviceMapFinished(QDBusPendingCallWatcher *watcher);

    void setWirelessSwitchEnableFinished(QDBusPendingCallWatcher *watcher);

    void getWiredDeviceMapFinished(QDBusPendingCallWatcher *watcher);

    void getWirelessDeviceMapFinished(QDBusPendingCallWatcher *watcher);

    void getWirelessSwitchStateFinished(QDBusPendingCallWatcher *watcher);

    void getWiredMainSwitchStateFinished(QDBusPendingCallWatcher *watcher);

    void setWiredMainSwitchEnableFinished(QDBusPendingCallWatcher *watcher);

protected:
    QDBusInterface      *m_pInterface = nullptr;

private:
    int m_pendingCount = 0;
    KnmLanDataKeeperPtr  lanDataKeeper;
    KnmWlanDataKeeperPtr wlanDataKeeper;

protected:
    friend class SingleTon<KnmDBusCaller>;
};

typedef SingleTon<KnmDBusCaller>  KNMDC;

#endif // KNMDBUSCALLER_H
