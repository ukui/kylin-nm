#ifndef KYLINWIRELESSCONNECTSETTING_H
#define KYLINWIRELESSCONNECTSETTING_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"
#include "kylinwiredconnectoperation.h"


enum KyKeyMgmt {
    Unknown = -1,
    Wep, Ieee8021x,
    WpaNone,
    WpaPsk,
    WpaEap,
    SAE
};

enum KyEapMethodType {
    TLS,
    PEAP,
    TTLS,
};

class KyWirelessConnectSetting : public KyConnectSetting
{
//    Q_OBJECT

public:
    QString m_ssid;
    bool isAutoConnect;
    QString m_psk;
    NetworkManager::Setting::SecretFlags m_secretFlag;
    KyKeyMgmt m_type;
    //only if m_type == WpaEap
    KyEapMethodType m_eapMethodType;
};

//配置connectSettings
NetworkManager::ConnectionSettings::Ptr assembleWirelessSettings(const KyWirelessConnectSetting &connSettingInfo);
//配置IPV4&&IPV6
void setIpv4AndIpv6Setting(NetworkManager::ConnectionSettings::Ptr connSetting, const KyWirelessConnectSetting &connSettingInfo);





#endif // KYLINWIRELESSCONNECTSETTING_H
