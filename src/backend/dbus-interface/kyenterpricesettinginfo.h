#ifndef KYENTERPRICESETTINGINFO_H
#define KYENTERPRICESETTINGINFO_H

#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>

#include "kylinnetworkresourcemanager.h"
#include <QObject>

class KyEapMethodTlsInfo
{
public:
    QString connName;
    QString identity;
    QString domain;
    QString devIfaceName;
    QString caCertPath;
    QString clientCertPath;
    QString clientPrivateKey;
    QString clientPrivateKeyPWD;
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
    QString connName;
    KyEapMethodPeapAuth phase2AuthMethod;
    QString userName;
    QString userPWD;
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
    QString connName;
    KyTtlsAuthMethod authType;
    KyEapMethodTtlsAuth authEapMethod;
    KyNoEapMethodTtlsAuth authNoEapMethod;
    QString userName;
    QString userPWD;
};

class KyEnterPriceSettingInfo : public QObject
{
    Q_OBJECT
public:
    explicit KyEnterPriceSettingInfo(QObject *parent = nullptr);

    static NetworkManager::ConnectionSettings::Ptr assembleEapMethodTlsSettings(KyEapMethodTlsInfo &, bool, NetworkManager::Setting::SecretFlags);
    static NetworkManager::ConnectionSettings::Ptr assembleEapMethodPeapSettings(KyEapMethodPeapInfo &, bool, NetworkManager::Setting::SecretFlags);
    static NetworkManager::ConnectionSettings::Ptr assembleEapMethodTtlsSettings(KyEapMethodTtlsInfo &, bool, NetworkManager::Setting::SecretFlags);

};

#endif // KYENTERPRICESETTINGINFO_H
