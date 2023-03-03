#ifndef KYLINNMDBUS_H
#define KYLINNMDBUS_H

#include <QObject>
#include <QThread>
#include <QDBusConnection>
#include <QDBusMetaType>
#include <QMetaObject>
#include <QMetaType>
#include <QGSettings>

#include <kylin-nm/kylinnetworkmanager.h>
#include "mainwindow.h"

class KylinNmDbus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.network")
public:
    KylinNmDbus(MainWindow *parent);
    ~KylinNmDbus();
    inline MainWindow *parent() const
    { return static_cast<MainWindow *>(QObject::parent()); }

private:
    KyNetworkManager* m_manager;
    QThread*        m_thread;
    void initDbusConnect();

public Q_SLOTS:
    //无线列表
    QMap<QString, QVector<QStringList>> getWirelessList();
    //有线列表
    QMap<QString, QVector<QStringList>> getWiredList();
    //有线总开关
    Q_NOREPLY void setWiredSwitchEnable(bool enable);
    //无线总开关
    Q_NOREPLY void setWirelessSwitchEnable(bool enable);
    //有线网卡开关
    Q_NOREPLY void setDeviceEnable(QString devName, bool enable);

    //刪除 根据网络名称 参数1 0:lan 1:wlan 参数2 为ssid/uuid
    Q_NOREPLY void deleteConnect(int type, const QString &connectUuid);
    //连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
    Q_NOREPLY void activateConnect(int type, QString devName, QString uuid);
    //断开连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为uuid
    Q_NOREPLY void deActivateConnect(int type, QString devName, QString activeConnectUuid);

    //获取设备列表和启用/禁用状态
    QMap<QString, bool> getDeviceListAndEnabled(int devType);
    //获取无线设备能力
    QMap<QString, int> getWirelessDeviceCap();

    //开启热点
    void activeWirelessAp(const QString apName, const QString apPassword,
                          const QString band, const QString apDevice);
    //断开热点
    void deactiveWirelessAp(const QString apName, const QString uuid);
    //获取热点
    QStringList getStoredApInfo();
    QStringList getApInfoBySsid(QString devName, QString ssid);
    //wifi扫描
    void reScan();
    //keyring
    void keyRingInit();
    void keyRingClear();
    //just show
    void showKylinNM(int type);

private Q_SLOTS:
    void onWiredConnectionAdd(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void onWiredConnectionUpdate(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void onWifiNetworkAdd(QString deviceName, KyWirelessNetItem &item);
    void onWiredDeviceUpdate(QString oldDeviceName, QString newDeviceName);
    void onWirelessDeviceNameUpdate(QString oldDeviceName, QString newDeviceName);
    void onWirelessApConnectStateChange(QString devName, QString ssid, QString uuid, KyConnectState state);

Q_SIGNALS:
    void lanAdd(QString devName, QStringList info);
    void lanRemove(QString dbusPath);
    void lanUpdate(QString devName, QStringList info);
    void lanActiveConnectionStateChanged(QString devName, QString uuid, int status);
    void wlanAdd(QString devName, QStringList info);
    void wlanRemove(QString devName, QString ssid);
    void wlanactiveConnectionStateChanged(QString devName, QString ssid, QString uuid, int status);

    //仅失败，若成功直接发listUpdate--未使用
    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
    //设备插拔
    void deviceStatusChanged();
    void deviceNameChanged(QString oldName, QString newName, int type);
    //热点断开
    void hotspotDeactivated(QString devName, QString ssid);
    //热点连接
    void hotspotActivated(QString devName, QString ssid, QString uuid);
    //信号强度变化
    void signalStrengthChange(QString devName, QString ssid, int strength);
    //安全性变化
    void secuTypeChange(QString devName, QString ssid, QString secuType);
    //更新列表排序
    void updateWifiList(QMap<QString, QVector<QStringList>>);
};

#endif // KYLINNMDBUS_H
