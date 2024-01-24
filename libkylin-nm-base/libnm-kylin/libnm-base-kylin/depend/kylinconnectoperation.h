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
#ifndef KYLINCONNECTOPERATION_H
#define KYLINCONNECTOPERATION_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"

class KyConnectOperation : public QObject
{
    Q_OBJECT
public:
    explicit KyConnectOperation(QObject *parent = nullptr);
    ~KyConnectOperation();

    void  updateConnect(QString connectUuid, const KyConnectSetting &connectSettingsInfo);
    void  deleteConnect(const QString &connectUuid);
    void  activateConnection(const QString connectUuid, const QString deviceName);
    void  deactivateConnection(const QString &activeConnectUuid);

protected:
    void connectSettingSet(NetworkManager::ConnectionSettings::Ptr connectionSettings,
                                                const KyConnectSetting &connectSettingsInfo);
    void ipv4SettingSet(NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                                                const KyConnectSetting &connectSettingsInfo);
    void ipv6SettingSet(NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                                                const KyConnectSetting &connectSettingsInfo);
    void setAutoConnect(NetworkManager::ConnectionSettings::Ptr &connectSetting, bool bAutoConnect);
    inline void errorProcess(QString errorMessage);

Q_SIGNALS:
    void createConnectionError(QString errorMessage);
    void updateConnectionError(QString errorMessage);
    void deleteConnectionError(QString errorMessage);
    void activateConnectionError(QString errorMessage);
    void deactivateConnectionError(QString errorMessage);

protected:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};

#endif // KYLINCONNECTOPERATION_H
