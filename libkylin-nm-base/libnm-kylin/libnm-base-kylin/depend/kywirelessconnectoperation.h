/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef KYWIRELESSCONNECTOPERATION_H
#define KYWIRELESSCONNECTOPERATION_H

#include <QObject>
#include <QTimer>
#include "kyenterpricesettinginfo.h"
#include "kylinconnectoperation.h"
#include "kyenterpricesettinginfo.h"

const QByteArray GSETTINGS_SCHEMA = "org.ukui.kylin-nm.switch";
const QString    WIRELESS_SWITCH  = "wirelessswitch";

const QString    WIFI_BAND_2_4GHZ = "2.4Ghz";
const QString    WIFI_BAND_5GHZ   = "5Ghz";

enum KySecuType {
    UNKNOWN,
    NONE,
    WPA_AND_WPA2_PERSONAL,
    WPA_AND_WPA2_ENTERPRISE,
    WPA3_PERSONAL,
};

enum KyKeyMgmt {
    Unknown = -1,
    Wep, Ieee8021x,
    WpaNone,
    WpaPsk,
    WpaEap,
    SAE
};

class KyWirelessConnectSetting : public KyConnectSetting
{
public:
    QString m_ssid;
    QString m_psk;
    NetworkManager::Setting::SecretFlags m_secretFlag = NetworkManager::Setting::SecretFlagType::None;
    bool isHidden = false;
};


class KyWirelessConnectOperation : public KyConnectOperation
{
    Q_OBJECT
public:
    explicit KyWirelessConnectOperation(QObject *parent = nullptr);

public:

    void requestWirelessScan();
    //获取密码保存策略
    bool getConnSecretFlags(QString &connUuid, NetworkManager::Setting::SecretFlags &);

    //获取KeyMgmt
    KyKeyMgmt getConnectKeyMgmt(const QString &uuid);
    KySecuType getConnectSecuType(const QString &uuid);

    //激活连接
    void activeWirelessConnect(QString , QString);
    //断开连接
    void deActivateWirelessConnection(const QString activeConnectName, const QString &activeConnectUuid);
    //新增普通连接
    void addConnect(const KyWirelessConnectSetting &connSettingInfo, KySecuType &type);
    //新增TLS连接
    void addTlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTlsInfo &tlsinfo);
    //新增PEAP连接
    void addPeapConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodPeapInfo &peapInfo);
    //新增TTLS连接
    void addTtlsConnect(const KyWirelessConnectSetting &connSettingInfo, const KyEapMethodTtlsInfo &ttlsInfo);
    //新增连接并激活(普通wifi)
    void addAndActiveWirelessConnect(KyWirelessConnectSetting &connSettingInfo, KySecuType &type);

    //新增连接并激活(企业wifi)
    void addAndActiveWirelessEnterPriseTlsConnect(KyEapMethodTlsInfo &info, KyWirelessConnectSetting &connSettingInfo);
    void addAndActiveWirelessEnterPrisePeapConnect(KyEapMethodPeapInfo &info, KyWirelessConnectSetting &connSettingInfo);
    void addAndActiveWirelessEnterPriseTtlsConnect(KyEapMethodTtlsInfo &info, KyWirelessConnectSetting &connSettingInfo);
    //属性页 page1 AutoConnect
    void setWirelessAutoConnect(const QString &uuid, bool bAutoConnect);
    //属性页 page2 page3 ipv6
    void updateIpv4AndIpv6SettingInfo(const QString &uuid, const KyConnectSetting &connectSettingsInfo);
    //属性页 page4 wifi Security
    //连接修改(安全改为个人/None)
    void updateWirelessPersonalConnect(const QString &uuid, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange);
    //连接修改(安全改为改为企业)
    void updateWirelessEnterPriseTlsConnect(const QString &uuid, const KyEapMethodTlsInfo &tlsinfo);
    void updateWirelessEnterPrisePeapConnect(const QString &uuid, const KyEapMethodPeapInfo &peapInfo);
    void updateWirelessEnterPriseTtlsConnect(const QString &uuid, const KyEapMethodTtlsInfo &ttlsInfo);
    //忘记
    void deleteWirelessConnect(const QString &connectUuid);
    //获取密码
    QString getPsk(const QString &connectUuid);
    QString getPrivateKeyPassword(const QString &connectUuid);
    QString get8021xPassword(const QString &connectUuid);

    void activeWirelessAp(const QString apUuid, const QString apName,
                              const QString apPassword, const QString apDevice,
                              const QString wirelessBand);
    void deactiveWirelessAp(const QString apName, const QString apUuid);

    bool isApConnection(QString uuid);

private:
    NetworkManager::ConnectionSettings::Ptr createWirelessApSetting(const QString apSsid,
                                           const QString apPassword, const QString apDevice, const QString wirelessBand);
    void updateWirelessApSetting(NetworkManager::Connection::Ptr apConnectPtr, const QString apName,
                                const QString apPassword, const QString apDevice, const QString wirelessBand);

    NetworkManager::WirelessNetwork::Ptr checkWifiNetExist(QString ssid, QString devName);
    void updateWirelessSecu(NetworkManager::ConnectionSettings::Ptr connSettingPtr, const KyWirelessConnectSetting &connSettingInfo, bool bPwdChange = false);
    void setIpv4AndIpv6Setting(NetworkManager::ConnectionSettings::Ptr connSetting, const KyConnectSetting &connSettingInfo);
    void setWirelessSecuWpaXEap(NetworkManager::ConnectionSettings::Ptr connSettingPtr);
    void activateApConnectionByUuid(const QString apUuid, const QString apDevice);
};

#endif // KYWIRELESSCONNECTOPERATION_H
