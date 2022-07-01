#ifndef KYLINNETWORKMANAGER_H
#define KYLINNETWORKMANAGER_H

#include <QObject>
#include "libnm-base-kylin_global.h"

#include "depend/kylinnetworkresourcemanager.h"
#include "depend/kylinactiveconnectresource.h"
#include "depend/kylinnetworkdeviceresource.h"
#include "depend/kylinconnectoperation.h"
#include "depend/kylinwiredconnectoperation.h"
#include "depend/kylinwirednetresource.h"
#include "depend/kywirelessconnectoperation.h"
#include "depend/kywirelessnetresource.h"
#include "depend/kylinnetresource.h"
#include "depend/kyapnetresource.h"
#include "depend/kylingeneral.h"

class LIBNMBASEKYLIN_EXPORT KyNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit KyNetworkManager(QObject *parent = nullptr);
    ~KyNetworkManager();

private:
    bool m_init = false;
    KyNetworkResourceManager *m_networkResource;
    KyActiveConnectResourse *m_activeConnectResource;
    KyNetworkDeviceResourse *m_deviceResource;
    KylinGeneralOpration *m_general;
    KyWiredConnectResourse *m_wiredConnectResourse;
    KyWirelessNetResource *m_wirelessNetResource;
    KyApNetResource *m_apNetResource;

    void initConnect();

Q_SIGNALS:
    //activeConnection
    void wiredStateChange(QString deviceName, QString uuid, KyConnectState state);
    void wirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state);

    //device
    void deviceStateChange(QString deviceName, KyDeviceState newstate, KyDeviceState oldstate,
                     NetworkManager::Device::StateChangeReason reason);
    void carrierChange(QString deviceName, bool pluged);
    void deviceRemove(QString deviceName);

    //general
    void wifiEnabledChanged(bool);
    void networkingEnabledChanged(bool);
    void connectStatusChanged(KyConnectStatus);

    //wired
    void wiredConnectionAdd(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void wiredConnectionUpdate(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void connectionRemove(QString path);

    void wiredDeviceAdd(QString deviceName);
    void wiredDeviceUpdate(QString oldDeviceName, QString newDeviceName);

    //wireless
    void signalStrengthChange(QString deviceName, QString ssid, int strength);
    void secuTypeChange(QString deviceName, QString ssid, QString securityType);

    void wirelessConnectionRemove(QString deviceName, QString ssid);
    void wirelessConnectionAdd(QString deviceName, QString ssid, QString uuid, QString dbusPath);
    void wirelessConnectionUpdate(QString deviceName, QString ssid, QString uuid, QString dbusPath, KySecuType connectSecuType);

    void wifiNetworkUpdate(QString, QString, KyWirelessNetItem);
    void wifiNetworkAdd(QString deviceName, KyWirelessNetItem &item);
    void wifiNetworkRemove(QString deviceName, QString ssid);

    void wirelessDeviceAdd(QString deviceName);
    void wirelessDeviceNameUpdate(QString oldName, QString newName);

    void updateWifiList(QString, QList<KyActivateItem>, QList<KyWirelessNetItem>);

    //移动热点
    void wirelessApConnectStateChange();



public:
    bool isInitFinished(){return m_init;}
    //activeConnection
    void getActiveConnectionList(QString deviceName,
                                 KyConnectionType connectionType, QList<KyActivateItem> &connectItemList);
    int getAcivateWifiSignal();

    //device
    void getNetworkDeviceList(KyDeviceType deviceType, QStringList &networkDeviceList);

    //general
    bool getWirelessEnabled();
    bool getNetworkingEnabled();
    void getConnectStatus(KyConnectStatus &status);

    //wired
    void getWiredList(QMap<QString,QList<KyWiredItem>> &map);
    //wireless
    void getWifiNetworkList(QString devName, QList<KyWirelessNetItem> &list);
    //移动热点
    void getApConnections(QList<KyApConnectItem> &apConnectItemList);

    //详情页-->ipv4 + ipv6 + autoconnect
    void getConnectIpInfo(QString uuid, KyConnectSetting &connectSetting);

    //详情页-->有线连接detail
    void getWiredConnectDetail(QString interface, QString uuid, KyDetailInfo &connectSetting);

    //详情页-->无线连接detail
    void getWirelessConnectDetail(QString interface, QString ssid, QString uuid, KyDetailInfo &connectSetting);

    //无线连接-->安全
    bool getNormalWifiConnectSecuInfo(QString &uuid, KyWpaPasswordInfo &info);
    bool getEnterPriseInfoTls(QString &uuid, KyEapMethodTlsInfo &info);
    bool getEnterPriseInfoPeap(QString &uuid, KyEapMethodPeapInfo &info);
    bool getEnterPriseInfoTtls(QString &uuid, KyEapMethodTtlsInfo &info);

    //获取企业网类型
    bool getEnterpiseEapMethod(const QString &uuid, KyEapMethodType &type);

    bool wiredConnectIsActived();
    bool wirelessConnectIsActived();

public Q_SLOTS:
    void kylinNetworkManagerInit();
    //general
    void onSetWirelessNetworkEnabled(bool enabled);
    void onSetNetworkingEnabled(bool enabled);
    void onSetWiredEnabled(bool enabled);

    //有线无线公用
    void  onDeleteConnect(const QString &connectUuid);
    void  onActivateConnection(const QString connectUuid, const QString deviceName);
    void  onDeactivateConnection(const QString &activeConnectUuid);
    //wired 单设备启用禁用
    void  onSetWiredDeviceEnable(const QString &interface, bool enable);
    //wireless
    //申请扫描
    void onRequestWirelessScan();

    void onCreateWiredConnect(KyConnectSetting connectSettingsInfo);

    //详情页
    //=======================连接操作====================
    //连接无本地配置的非企业网热点
    void onAddAndActivateNormalWifi(KyWirelessConnectSetting connSettingInfo, KySecuType type);
    //连接无本地配置的企业网热点
    void onAddAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo info,
                                                    KyWirelessConnectSetting connSettingInfo);
    void onAddAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo info,
                                                    KyWirelessConnectSetting connSettingInfo);
    void onAddAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo info,
                                                     KyWirelessConnectSetting connSettingInfo);

    //=====================新增配置操作====================
    //新增普通连接
    void onAddNormalConnect(const KyWirelessConnectSetting &connSettingInfo, KySecuType &type);
    //新增TLS连接
    void onAddTlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTlsInfo &tlsinfo);
    //新增PEAP连接
    void onAddPeapConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodPeapInfo &peapInfo);
    //新增TTLS连接
    void onAddTtlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTtlsInfo &ttlsInfo);

    //=====================更新配置操作====================
    void onUpdateIpv4AndIpv6SettingInfo(const QString &uuid, const KyConnectSetting &connectSettingsInfo);
    //连接修改(安全改为个人/None)
    void onUpdateWirelessPersonalConnect(const QString &uuid, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange);
    //连接修改(安全改为改为企业)
    void onUpdateWirelessEnterPriseTlsConnect(const QString &uuid, const KyEapMethodTlsInfo &tlsinfo);
    void onUpdateWirelessEnterPrisePeapConnect(const QString &uuid, const KyEapMethodPeapInfo &peapInfo);
    void onUpdateWirelessEnterPriseTtlsConnect(const QString &uuid, const KyEapMethodTtlsInfo &ttlsInfo);
};

#endif // KYLINNETWORKMANAGER_H
