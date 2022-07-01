#ifndef AGENTUTILS_H
#define AGENTUTILS_H

#define VPN_TYPE                 "vpn"
#define VPN_NAME                 "vpn"
#define VPN_SERVICE              "service-type"
#define VPN_DATA                 "data"
#define VPN_SECRET               "secrets"
#define VPN_FLAG                 "password-flags"

#define CONNECTION_KEY           "connection"
#define UUID_KEY                 "uuid"
#define SSID_KEY                 "ssid"
#define ID_KEY                   "id"
#define TYPE_KEY                 "type"


#define WIRELESS_SECURITY_NAME   "802-11-wireless-security"
#define KEY_MGMT                 "key-mgmt"

#define PSK_MGMT                 "wpa-psk"
#define SAE_MGMT                 "sae"
#define PSK_FLAG                 "psk-flags"
#define PSK_KEY                  "psk"

#define WEP_MGMT                 "none"
#define WEP128_MGMT              "passphrase"
#define WEP_FLAG_KEY             "wep-key-flags"
#define WEP_ID_KEY               "wep-tx-keyidx"
#define WEP_KEY0                 "wep-key0"
#define WEP_KEY1                 "wep-key1"
#define WEP_KEY2                 "wep-key2"
#define WEP_KEY3                 "wep-key3"

#define LEAP_MGMT                "ieee8021x"
#define LEAP_PASSWORD_FLAG       "leap-password-flags"
#define LEAP_PASSWORD_KEY        "leap-password"
#define LEAP_USER_KEY            "leap-username"

#define ENTERPRICE_NAME            "802-1x"
#define EAP_KEY                    "eap"
#define PWD_EAP                    "pwd"
#define LEAP_EAP                   "leap"
#define FAST_EAP                   "fast"
#define PEAP_EAP                   "peap"
#define TTLS_EAP                   "ttls"
#define TLS_EAP                    "tls"

#define IDENTITY_KEY               "identity"

#define PASSWORD_FLAG              "password-flags"
#define PASSWORD_KEY               "password"

#define PIN_FLAG                   "pin-flags"
#define PIN_KEY                    "pin"

#define PHASE2_PRIVATE_KEY_FLAG    "phase2-private-key-password-flags"
#define PHASE2_PRIVATE_KEY         "phase2-private-key-password"

#define PRIVATE_KEY_FLAG           "private-key-password-flags"
#define PRIVATE_KEY                "private-key-password"
#define DOMAIN_KEY                 "domain-suffix-match"

#define PHASE2_CLIENT_FLAG         "phase2-client-cert-password-flags"
#define PHASE2_CLIENT              "phase2-client-cert-password"

#define PHASE2_CACERT_FLAG         "phase2-ca-cert-password-flags"
#define PHASE2_CACERT              "phase2-ca-cert-password"

#define PHASE1_AUTH_FLAG           "phase1-auth-flags"
#define PHASE1_AUTH                "phase1-auth"

#define CLIENT_CERT_FLAG           "client-cert-password-flags"
#define CLIENT_CERT                "client-cert-password"

#define CACERT_FLAG                "ca-cert-password-flags"
#define CACERT                     "ca-cert-password"



enum SecretPolicyFlag{
    POLICY_NONE = 0,
    POLICY_AGENT_OWNED = 0x01,
    POLICY_NOT_SAVED = 0x02,
    POLICY_NOT_REQUIRED = 0x04
};

enum SecretWepKeyId{
    KEY_TYPE0 = 0,
    KEY_TYPE1 = 1,
    KEY_TYPE2 = 2,
    KEY_TYPE3 = 3
};


#endif // AGENTUTILS_H
