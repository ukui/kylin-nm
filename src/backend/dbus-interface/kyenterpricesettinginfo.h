#ifndef KYENTERPRICESETTINGINFO_H
#define KYENTERPRICESETTINGINFO_H

#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>

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

typedef enum {
    KyAuthEapMethodUnknown = 0,
    KyAuthEapMethodMd5,
    KyAuthEapMethodMschapv2,
    KyAuthEapMethodOtp,
    KyAuthEapMethodGtc,
    KyAuthEapMethodTls
} KyEapMethodAuth;

typedef enum{
    KyAuthMethodUnknown = 0,
    KyAuthMethodPap,
    KyAuthMethodChap,
    KyAuthMethodMschap,
    KyAuthMethodMschapv2,
    KyAuthMethodGtc,
    KyAuthMethodOtp,
    KyAuthMethodMd5,
    KyAuthMethodTls
} KyNoEapMethodAuth;


class KyEapMethodPeapInfo
{
public:
    KyNoEapMethodAuth phase2AuthMethod;
    QString userName;
    QString userPWD;
    NetworkManager::Setting::SecretFlags m_passwdFlag;
    // only valid when update
    bool    bChanged;
};

enum KyTtlsAuthMethod
{
    AUTH_EAP,
    AUTH_NO_EAP
};

class KyEapMethodTtlsInfo
{
public:
    KyTtlsAuthMethod authType;
    KyEapMethodAuth authEapMethod;
    KyNoEapMethodAuth authNoEapMethod;
    QString userName;
    QString userPWD;
    NetworkManager::Setting::SecretFlags m_passwdFlag;
    // only valid when update
    bool    bChanged;
};

void assembleEapMethodTlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTlsInfo &tlsInfo);
void assembleEapMethodPeapSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodPeapInfo &peapInfo);
void assembleEapMethodTtlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTtlsInfo &ttlsInfo);

void modifyEapMethodTlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTlsInfo &tlsInfo);
void modifyEapMethodPeapSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodPeapInfo &peapInfo);
void modifyEapMethodTtlsSettings(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyEapMethodTtlsInfo &ttlsInfo);

#endif // KYENTERPRICESETTINGINFO_H
