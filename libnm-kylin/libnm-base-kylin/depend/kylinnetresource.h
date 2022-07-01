#ifndef KYNETRESOURCE_H
#define KYNETRESOURCE_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"
#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>

class KyDetailInfo
{
public:
    QString strSecType;
    QString strChan;
    QString strMac;
    QString strHz;
    QString strBandWidth;
    QString strDynamicIpv4;
    QString strDynamicIpv6;
    QString strDynamicIpv4Dns;
    bool    isAutoConnect = false;
};

class KyNetResource : public QObject
{
    Q_OBJECT
public:
    explicit KyNetResource(QObject *parent = nullptr);
    ~KyNetResource();

    void getConnectionSetting(QString connectUuid, KyConnectSetting &connectSetting);

protected:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;

    void getBaseInfo(QString devName,QString ssid, bool isWlan, bool isActive, KyDetailInfo &conInfo);
    void getDynamicIpInfo(QString uuid, KyDetailInfo &conInfo);

private:
    void getIpv4ConnectSetting(NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                                                    KyConnectSetting &connectSetting);
    void getIpv6ConnectSetting(NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                                                    KyConnectSetting &connectSetting);
};

#endif // KYNETRESOURCE_H
