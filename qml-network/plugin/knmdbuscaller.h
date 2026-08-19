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
public:
    // 测试模式控制 - 必须放在public中
    static void setTestMode(bool enable);
    static bool isTestMode();
    // 重新初始化测试设备（用于每个测试用例）
    void reinitTestDevices();
    // 用于测试D-Bus相关分支的函数
    void testDbusBranches();
    friend class TestKnmInterface;
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

    void getWirelessConList(QString dev);

    void updateWirelessConListSync(QString dev);
    void getCableStateByDevice(const QString &dev);

    void registerInputPasswdAgent();

    void setNetworkConnectionAutoConnectState(int netType, QString uuid, bool state);

    void setDefaultWiredDevice(QString devName);

    void setDefaultWirelessDevice(QString devName);

    QString getDefaultWiredDevice();

    QString getDefaultWirelessDevice();

//用于调用kylin-nm的dbus - 将protected函数改为public以便测试
public:
    QDBusPendingCallWatcher * asyncCall(const QString & methed, const QList<QVariant> & params);

    void getWiredDeviceMap();

    void getWirelessDeviceMap();

    void getWiredConList(QString dev);


    void getWirelessSwitchState();

    void getWiredMainSwitchState();

    void getWiredConListFinished(QDBusPendingCallWatcher *watcher, QString dev);

    void getWirelessConListFinished(QDBusPendingCallWatcher *watcher, QString dev);

//dbus调用的响应槽函数 - 将protected slots改为public以便测试
public slots:
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

    void updateCableState(QDBusPendingCallWatcher *watcher);

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
