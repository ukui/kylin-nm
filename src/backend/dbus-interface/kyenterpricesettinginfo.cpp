#include "kyenterpricesettinginfo.h"

void assembleEapMethodTlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTlsInfo &info)
{
    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
            = connSettingPtr->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

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
    wifi_8021x_sett->setPrivateKeyPasswordFlags(info.m_privateKeyPWDFlag);

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaEap);

    return;
}

void assembleEapMethodPeapSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodPeapInfo &info)
{
    qDebug() << "assembleEapMethodPeapSettings";

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
            = connSettingPtr->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodPeap);
    wifi_8021x_sett->setInitialized(true);
    wifi_8021x_sett->setEapMethods(list);
    wifi_8021x_sett->setPhase2AuthEapMethod((NetworkManager::Security8021xSetting::AuthEapMethod)info.phase2AuthMethod);
    wifi_8021x_sett->setIdentity(info.userName);
    wifi_8021x_sett->setPassword(info.userPWD);
    wifi_8021x_sett->setPasswordFlags(info.m_passwdFlag);


    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaEap);
    return;
}

void assembleEapMethodTtlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTtlsInfo &info)
{

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
            = connSettingPtr->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

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
    wifi_8021x_sett->setPasswordFlags(info.m_passwdFlag);

    NetworkManager::WirelessSecuritySetting::Ptr security_sett
            = connSettingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    security_sett->setInitialized(true);
    security_sett->setKeyMgmt(NetworkManager::WirelessSecuritySetting::WpaEap);
    return;
}


void modifyEapMethodTlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTlsInfo &tlsInfo)
{
    NetworkManager::Security8021xSetting::Ptr setting = connSettingPtr->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    setting->setInitialized(true);

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodTls);
    setting->setEapMethods(list);
    setting->setIdentity(tlsInfo.identity);
    if(!tlsInfo.domain.isEmpty())
    {
        setting->setDomainSuffixMatch(tlsInfo.domain);
    }
    if (tlsInfo.bNeedCa)
    {
        QByteArray caCerEndWithNull(tlsInfo.caCertPath.toUtf8() + '\0');
        setting->setCaCertificate(caCerEndWithNull);
    }

    QByteArray cliCertEndWithNull(tlsInfo.clientCertPath.toUtf8() + '\0');
    setting->setClientCertificate(cliCertEndWithNull);
    QByteArray cliPriKeyEndWithNull(tlsInfo.clientPrivateKey.toUtf8() + '\0');
    setting->setPrivateKey(cliPriKeyEndWithNull);
    setting->setPrivateKeyPasswordFlags(tlsInfo.m_privateKeyPWDFlag);
    if(tlsInfo.bChanged)
    {
        setting->setPrivateKeyPassword(tlsInfo.clientPrivateKeyPWD);
    }
    return;
}

void modifyEapMethodPeapSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodPeapInfo &peapInfo)
{
    qDebug() << "assembleEapMethodPeapSettings";

    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
            = connSettingPtr->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();
    wifi_8021x_sett->setInitialized(true);

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodPeap);
    wifi_8021x_sett->setInitialized(true);
    wifi_8021x_sett->setEapMethods(list);
    wifi_8021x_sett->setPhase2AuthEapMethod((NetworkManager::Security8021xSetting::AuthEapMethod)peapInfo.phase2AuthMethod);
    wifi_8021x_sett->setIdentity(peapInfo.userName);
    if(peapInfo.bChanged)
    {
        wifi_8021x_sett->setPassword(peapInfo.userPWD);
    }
    wifi_8021x_sett->setPasswordFlags(peapInfo.m_passwdFlag);

    return;
}

void modifyEapMethodTtlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTtlsInfo &ttlsInfo)
{
    NetworkManager::Security8021xSetting::Ptr wifi_8021x_sett
            = connSettingPtr->setting(NetworkManager::Setting::Security8021x).dynamicCast<NetworkManager::Security8021xSetting>();

    QList<NetworkManager::Security8021xSetting::EapMethod> list;
    list.append(NetworkManager::Security8021xSetting::EapMethod::EapMethodTtls);
    wifi_8021x_sett->setInitialized(true);
    wifi_8021x_sett->setEapMethods(list);
    if (ttlsInfo.authType == KyTtlsAuthMethod::AUTH_EAP)
    {
        wifi_8021x_sett->setPhase2AuthEapMethod((NetworkManager::Security8021xSetting::AuthEapMethod)ttlsInfo.authEapMethod);//gtc md5 mschapv2 otp tls
    } else if (ttlsInfo.authType == KyTtlsAuthMethod::AUTH_NO_EAP)
    {
        wifi_8021x_sett->setPhase2AuthMethod((NetworkManager::Security8021xSetting::AuthMethod)ttlsInfo.authNoEapMethod);//chap md5 mschapv2 pap gtc mschap otp tls
    }
    wifi_8021x_sett->setIdentity(ttlsInfo.userName);
    if(ttlsInfo.bChanged)
    {
        wifi_8021x_sett->setPassword(ttlsInfo.userPWD);
    }
    wifi_8021x_sett->setPasswordFlags(ttlsInfo.m_passwdFlag);
    return;
}
