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
#ifndef KYWIRELESSNETRESOURCE_H
#define KYWIRELESSNETRESOURCE_H

#include <QObject>
#include "kylinnetresource.h"
#include "kywirelessnetitem.h"
#include "kylinnetworkresourcemanager.h"
#include "kyenterpricesettinginfo.h"
#include "kywirelessconnectoperation.h"
#include "kylinactiveconnectresource.h"

class KyWpaPasswordInfo{
public:
    QString pwd;
    NetworkManager::Setting::SecretFlags flag;
};

class KyWirelessNetResource : public KyNetResource
{
    Q_OBJECT
public:
    explicit KyWirelessNetResource(QObject *parent = nullptr);

public:
    //初始化列表用到的接口
    void getWifiNetworkList(QString devName, QList<KyWirelessNetItem> &list);

    //属性页---->安全
    bool getNormalWifiConnectSecuInfo(QString &uuid, KyWpaPasswordInfo &info);
    bool getEnterPriseInfoTls(QString &uuid, KyEapMethodTlsInfo &info);
    bool getEnterPriseInfoPeap(QString &uuid, KyEapMethodPeapInfo &info);
    bool getEnterPriseInfoTtls(QString &uuid, KyEapMethodTtlsInfo &info);

    //获取企业网类型
    bool getEnterpiseEapMethod(const QString &uuid, KyEapMethodType &type);

    void getWirelessConnectDetail(QString interface, QString ssid, QString uuid, KyDetailInfo &connectSetting);

private:
    void kyWirelessNetItemListInit();
    void wirelessNetItemInit(KyWirelessNetItem &wirelessItem,
                                                    NetworkManager::WirelessNetwork::Ptr wirelessNetPtr);
    void getUuidBySsid(const QString &ssid, QString &deviceName, QString &uuid, QString &dbusPath);

public Q_SLOTS:
    void onWifiNetworkAdded(QString devIfaceName, QString ssid);
    void onWifiNetworkRemoved(QString devIfaceName, QString ssid);
    void onWifiNetworkPropertyChange(NetworkManager::WirelessNetwork::Ptr net);
    void onWifiNetworkDeviceDisappear();

    void onConnectionAdd(QString connectUuid, QString connectName, QString connectPath);
    void onConnectionRemove(QString);

    void onDeviceAdd(QString deviceName);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);
private Q_SLOTS:
    void updateList();

Q_SIGNALS:
    void signalStrengthChange(QString deviceName, QString ssid, int strength);
    void secuTypeChange(QString deviceName, QString ssid, QString securityType);

    void wirelessConnectionRemove(QString deviceName, QString ssid);
    void wirelessConnectionAdd(QString deviceName, QString ssid, QString uuid, QString dbusPath);
    void wirelessConnectionUpdate(QString deviceName, QString ssid, QString uuid, QString dbusPath, KySecuType connectSecuType);

    void wifiNetworkUpdate(QString, QString, KyWirelessNetItem);
    void wifiNetworkAdd(QString deviceName, KyWirelessNetItem &item);
    void wifiNetworkRemove(QString deviceName, QString ssid);

    void wirelessDeviceAdd(QString deviceName);
    void deviceRemove(QString deviceName);
    void wirelessDeviceNameUpdate(QString oldName, QString newName);

    void updateWifiList(QString, QList<KyActivateItem>, QList<KyWirelessNetItem>);

private:
    QMap<QString, QList<KyWirelessNetItem>>      m_WifiNetworkList;
    QTimer * m_updateTimer = nullptr;

    QString getDeviceIFace(NetworkManager::WirelessNetwork::Ptr net);
};

#endif // KYWIRELESSNETRESOURCE_H
