#ifndef KYENTERPRICESETTINGINFO_H
#define KYENTERPRICESETTINGINFO_H

#include "kylinnetworkresourcemanager.h"
#include <QObject>

class KyEapMethodTlsInfo
{
public:
    QString identity;
    QString domain;
    QString devIfaceName;
    QString caCertPath;
    bool    bNeedCa;
    QString clientCertPath;
    QString clientPrivateKey;
    QString clientPrivateKeyPWD;
    NetworkManager::Setting::SecretFlags m_privateKeyPWDFlag;
    // only valid when update
    bool    bChanged;
};

typedef enum{
    AuthEapMethodPeapUnknown = 0,
    AuthEapMethodPeapMd5,
    AuthEapMethodPeapMschapv2,
    AuthEapMethodPeapOtp,
    AuthEapMethodPeapGtc,
    AuthEapMethodPeapTls
} KyEapMethodPeapAuth;

class KyEapMethodPeapInfo
{
public:
    KyEapMethodPeapAuth phase2AuthMethod;
    QString userName;
    QString userPWD;
    NetworkManager::Setting::SecretFlags m_passwdFlag;
    // only valid when update
    bool    bChanged;
};

typedef enum {
    AuthEapMethodTtlsUnknown = 0,
    AuthEapMethodTtlsMd5,
    AuthEapMethodTtlsMschapv2,
    AuthEapMethodTtlsOtp,
    AuthEapMethodTtlsGtc,
    AuthEapMethodTtlsTls
} KyEapMethodTtlsAuth;

typedef enum{
    AuthMethodTtlsUnknown = 0,
    AuthMethodTtlsPap,
    AuthMethodTtlsChap,
    AuthMethodTtlsMschap,
    AuthMethodTtlsMschapv2,
    AuthMethodTtlsGtc,
    AuthMethodTtlsOtp,
    AuthMethodTtlsMd5,
    AuthMethodTtlsTls
} KyNoEapMethodTtlsAuth;

enum KyTtlsAuthMethod
{
    AUTH_EAP,
    AUTH_NO_EAP
};

class KyEapMethodTtlsInfo
{
public:
    KyTtlsAuthMethod authType;
    KyEapMethodTtlsAuth authEapMethod;
    KyNoEapMethodTtlsAuth authNoEapMethod;
    QString userName;
    QString userPWD;
    NetworkManager::Setting::SecretFlags m_passwdFlag;
    // only valid when update
    bool    bChanged;
};

void assembleEapMethodTlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTlsInfo &);
void assembleEapMethodPeapSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodPeapInfo &);
void assembleEapMethodTtlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTtlsInfo &);

void modifyEapMethodTlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTlsInfo &);
void modifyEapMethodPeapSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodPeapInfo &);
void modifyEapMethodTtlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTtlsInfo &);

#endif // KYENTERPRICESETTINGINFO_H
