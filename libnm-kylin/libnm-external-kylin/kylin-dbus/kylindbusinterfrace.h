#ifndef KYLINDBUSINTERFRACE_H
#define KYLINDBUSINTERFRACE_H

#include "kylinactiveconnectresource.h"
#include "kylinwirednetresource.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinnetworkresourcemanager.h"
#include "kylinwiredconnectoperation.h"
#include "kywirelessconnectoperation.h"
#include "kywirelessnetresource.h"
#include "kylinagent.h"

#include <QObject>

const QString NotApConnection   = "0";
const QString IsApConnection    = "1";

class KylinDbusInterface : public QObject
{
    Q_OBJECT

public:
    explicit KylinDbusInterface(QObject *parent = nullptr);
    ~KylinDbusInterface();

public:
    void getWirelessList(QMap<QString, QVector<QStringList> > &map);
    void getWiredList(QMap<QString, QVector<QStringList>> &map);

    //开启/断开热点
    void activeWirelessAp(const QString apName, const QString apPassword,
                                const QString wirelessBand, const QString apDevice);
    void deactiveWirelessAp(const QString apName, const QString uuid);

    //获取热点信息
    void getStoredApInfo(QStringList &list);
    void getApInfoBySsid(QString devName, QString ssid, QStringList &list);

    //激活/断开有线连接
    void activateWiredConnection(const QString &devName, const QString &connUuid);
    void deactivateWiredConnection(const QString &devName, const QString &connUuid);

    //激活/断开无线连接
    void activateWirelessConnection(const QString &devName, const QString &ssid);
    void deactivateWirelessConnection(const QString &devName, const QString &ssid);

    //无线总开关
    void setWirelessSwitchEnable(bool enable);
    void setWiredDeviceEnable(const QString &devName, bool enable);

    void getWirelessDeviceCapability(QMap<QString, int> &map);

    void rescanWireless();

    void startAgent();
    void stopAgent();

Q_SIGNALS:
    void wiredDeviceAdd(QString deviceName);
    void wiredDeviceRemove(QString deviceName);
    void wiredDeviceNameUPdate(QString oldName, QString newName);

    void wirelessDeviceAdd(QString deviceName);
    void wirelessDeviceRemove(QString deviceName);
    void wirelessDeviceNameUPdate(QString oldName, QString newName);


    //有线无线列表更新（有线增删、无线增加减少）
    void lanAdded(QString devName, QStringList info);
    void lanRemoved(QString dbusPath);
    void lanUpdated(QString devName, QStringList info);

    void wlanAdded(QString devName, QStringList info);
    void wlanRemoved(QString devName,QString ssid);

    void wlanConnectionStateChanged(QString devName, QString ssid, QString uuid, int status);
    void lanConnectionStateChanged(QString devName, QString uuid, int status);

    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);

    //激活/断开热点连接
    void hotspotActivated(QString deviceName, QString ssid, QString uuid);
    void hotspotDeactivated(QString deviceName, QString ssid);

    //信号强度变化
    void signalStrengthChanged(QString deviceName, QString ssid, int strength);

    //安全性变化
    void securityTypeChanged(QString deviceName, QString ssid, QString securityType);

private Q_SLOTS:
    void onLanAdd(QString connectUuid, QString connectName, QString connectPath);
    void onLanUpdate(QString connectUuid, QString connectName, QString connectPath);

    void onWlanAdd(QString deviceName, KyWirelessNetItem &item);
    void onWlanStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state);

private:
    KyActiveConnectResourse  *m_activeConnectResource;
    KyConnectResourse *m_connectResource;
    KyNetworkDeviceResourse *m_deviceResource;
    KyWirelessNetResource  *m_wirelessResource;

    KyWirelessConnectOperation *m_wirelessOperation;
    KyWiredConnectOperation *m_wiredOperation;

    KylinAgent *m_secretAgent;
};


#endif // KYLINDBUSINTERFRACE_H
