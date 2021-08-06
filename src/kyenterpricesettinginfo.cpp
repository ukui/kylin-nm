#include "kyenterpricesettinginfo.h"

NetworkManager::ConnectionSettings::Ptr KyEnterPriceSettingInfo::assembleEapMethodTlsSettings(KyEapMethodTlsInfo &info,
                                                                                              bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(info.connName);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(info.connName.toUtf8());
    wifi_sett->setSecurity("802-11-wireless-security");

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
        = settings->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodTls);
    wifi_8021x_sett->setInitialized(true);
    wifi_8021x_sett->setEapMethods(list);
    wifi_8021x_sett->setIdentity(info.identity);
    if (!info.domain.isEmpty()){
        wifi_8021x_sett->setDomainSuffixMatch(info.domain);
    }
    QByteArray caCerEndWithNull(info.caCertPath.toUtf8() + '\0');
    wifi_8021x_sett->setCaCertificate(caCerEndWithNull);
    QByteArray cliCertEndWithNull(info.clientCertPath.toUtf8() + '\0');
    wifi_8021x_sett->setClientCertificate(cliCertEndWithNull);
    QByteArray cliPriKeyEndWithNull(info.clientPrivateKey.toUtf8() + '\0');
    wifi_8021x_sett->setPrivateKey(cliPriKeyEndWithNull);
    wifi_8021x_sett->setPrivateKeyPassword(info.clientPrivateKeyPWD);
    wifi_8021x_sett->setPrivateKeyPasswordFlags(secretFlags);

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaEap);

    return settings;
}

NetworkManager::ConnectionSettings::Ptr KyEnterPriceSettingInfo::assembleEapMethodPeapSettings(KyEapMethodPeapInfo &info,
                                                                                               bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    qDebug() << "assembleEapMethodPeapSettings";
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(info.connName);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(info.connName.toUtf8());
    wifi_sett->setSecurity("802-11-wireless-security");

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
        = settings->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodPeap);
    wifi_8021x_sett->setInitialized(true);
    wifi_8021x_sett->setEapMethods(list);
    wifi_8021x_sett->setPhase2AuthEapMethod((NetworkManager::Security8021xSetting::AuthEapMethod)info.phase2AuthMethod);
    wifi_8021x_sett->setIdentity(info.userName);
    wifi_8021x_sett->setPassword(info.userPWD);
    wifi_8021x_sett->setPrivateKeyPasswordFlags(secretFlags);


    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaEap);
    security_sett->setAuthAlg(NetworkManager::WirelessSecuritySetting::Open);

    return settings;
}

NetworkManager::ConnectionSettings::Ptr KyEnterPriceSettingInfo::assembleEapMethodTtlsSettings(KyEapMethodTtlsInfo &info,
                                                                                               bool isAutoConnect, NetworkManager::Setting::SecretFlags secretFlags)
{
    NetworkManager::ConnectionSettings::Ptr settings{new NetworkManager::ConnectionSettings{NetworkManager::ConnectionSettings::Wireless}};
    settings->setId(info.connName);
    settings->setUuid(NetworkManager::ConnectionSettings::createNewUuid());
    settings->setAutoconnect(isAutoConnect);
    //Note: workaround for wrongly (randomly) initialized gateway-ping-timeout
    settings->setGatewayPingTimeout(0);

    NetworkManager::WirelessSetting::Ptr wifi_sett
        = settings->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    wifi_sett->setInitialized(true);
    wifi_sett->setSsid(info.connName.toUtf8());
    wifi_sett->setSecurity("802-11-wireless-security");

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
        = settings->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodTtls);
    wifi_8021x_sett->setInitialized(true);
    wifi_8021x_sett->setEapMethods(list);
    if (info.authType == KyTtlsAuthMethod::AUTH_EAP)
    {
        wifi_8021x_sett->setPhase2AuthEapMethod((NetworkManager::Security8021xSetting::AuthEapMethod)info.authEapMethod);//gtc md5 mschapv2 otp tls
    } else if (info.authType == KyTtlsAuthMethod::AUTH_NO_EAP)
    {
        wifi_8021x_sett->setPhase2AuthMethod((NetworkManager::Security8021xSetting::AuthMethod)info.authNoEapMethod);//chap md5 mschapv2 pap gtc mschap otp tls
    }
    wifi_8021x_sett->setIdentity(info.userName);
    wifi_8021x_sett->setPassword(info.userPWD);
    wifi_8021x_sett->setPrivateKeyPasswordFlags(secretFlags);

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
        = settings->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaEap);

    return settings;
}
