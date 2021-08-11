#ifndef KYWIRELESSCONNECTOPERATION_H
#define KYWIRELESSCONNECTOPERATION_H

#include <QObject>
#include "kyenterpricesettinginfo.h"
#include "kylinconnectoperation.h"
#include "kylinwirelessconnectsetting.h"
#include "kyenterpricesettinginfo.h"

enum KySecuType {
    NONE = 0,
    WPA_AND_WPA2_PERSONAL,
    WPA_AND_WPA2_ENTERPRISE,
    WPA2_AND_WPA3_PERSONAL
};

class KyWirelessConnectOperation : public KyConnectOperation
{
    Q_OBJECT
public:
    explicit KyWirelessConnectOperation(QObject *parent = nullptr);
    ~KyWirelessConnectOperation();

    //无线开关
    void setWirelessEnabled(bool enabled);
    bool getWirelessEnabled();

    //获取密码保存策略
    bool getConnSecretFlags(QString &connUuid, NetworkManager::Setting::SecretFlags &);

    //激活连接
    void activeWirelessConnect(QString , QString);
    //断开连接
    void deActivateWirelessConnection(const QString activeConnectName, const QString &activeConnectUuid);
    //新增普通连接
    void addConnect(const KyWirelessConnectSetting &connSettingInfo);
    //新增TLS连接
    void addTlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTlsInfo &tlsinfo);
    //新增PEAP连接
    void addPeapConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodPeapInfo &peapInfo);
    //新增TTLS连接
    void addTtlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTtlsInfo &ttlsInfo);
    //新增连接并激活(普通wifi)
    void addAndActiveWirelessConnect(QString & devIface,KyWirelessConnectSetting &connSettingInfo,bool isHidden);

    //新增连接并激活(企业wifi)
    void addAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo &info, KyWirelessConnectSetting &connSettingInfo,
                                                  QString & devIface, bool isHidden);
    void addAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo &info, KyWirelessConnectSetting &connSettingInfo,
                                                   QString & devIface, bool isHidden);
    void addAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo &info, KyWirelessConnectSetting &connSettingInfo,
                                                   QString & devIface, bool isHidden);

    //连接修改(安全改为个人/None)
    void updateWirelessPersonalConnect(const QString &uuid, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange);
    //连接修改(安全改为改为企业)
    void updateWirelessEnterPriseTlsConnect(const QString &uuid, const KyEapMethodTlsInfo &tlsinfo, const KyWirelessConnectSetting &connSettingInfo);
    void updateWirelessEnterPrisePeapConnect(const QString &uuid, const KyEapMethodPeapInfo &peapInfo, const KyWirelessConnectSetting &connSettingInfo);
    void updateWirelessEnterPriseTtlsConnect(const QString &uuid, const KyEapMethodTtlsInfo &ttlsInfo, const KyWirelessConnectSetting &connSettingInfo);

    //申请扫描
    void requestWirelessScan();

signals:
    void wifinEnabledChanged(bool);
    void andAndActivateConnectionError(QString errorMessage);

private:
    NetworkManager::WirelessNetwork::Ptr checkWifiNetExist(QString ssid, QString devName);
    void updateWirelessSecu(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange = false);
    KyKeyMgmt getConnectKeyMgmt(NetworkManager::ConnectionSettings::Ptr connSettingPtr);

    KyNetworkResourceManager *m_networkResourceInstance = nullptr;

};

#endif // KYWIRELESSCONNECTOPERATION_H
