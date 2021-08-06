#ifndef KYWIRELESSCONNECTOPERATION_H
#define KYWIRELESSCONNECTOPERATION_H

#include <QObject>
#include "kylinnetworkresourcemanager.h"
#include "kyenterpricesettinginfo.h"
//#include "kylinconnectinfo.h"

enum KySecuType {
    NONE = 0,
    WPA_AND_WPA2_PERSONAL,
    WPA_AND_WPA2_ENTERPRISE,
    WPA2_AND_WPA3_PERSONAL
};

class KyWirelessConnectOperation : public QObject
{
    Q_OBJECT
public:
    explicit KyWirelessConnectOperation(QObject *parent = nullptr);
    ~KyWirelessConnectOperation();

    void setWirelessEnabled(bool enabled);
    bool getWirelessEnabled();

    bool getConnSecretFlags(QString &, NetworkManager::Setting::SecretFlags &);

    void activeWirelessConnect(QString , QString);
//todo:
//    void activeWirelessConnectWithPwd(QString , QString, QString);
    void deActiveWirelessConnect(QString &);
    void addAndActiveWirelessConnect(QString &, QString &, QString, bool, NetworkManager::Setting::SecretFlags);
    void requestWirelessScan();
    //仅普通隐藏wifi
    void addAndActiveWirelessHiddenConnect(KySecuType &, QString &, QString &, QString &, bool, NetworkManager::Setting::SecretFlags flags = 0);
    //企业wifi
    void addAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo &, QString &, bool, bool, NetworkManager::Setting::SecretFlags flags = 0);
    void addAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo &, QString &, bool, bool, NetworkManager::Setting::SecretFlags flags = 0);
    void addAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo &, QString &, bool, bool, NetworkManager::Setting::SecretFlags flags = 0);


signals:
    void connectFail(QString, QString, QString);//连接失败信号
    void disConnectFail(QString, QString, QString);
    void wifinEnabledChanged(bool);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;

};

#endif // KYWIRELESSCONNECTOPERATION_H
