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
#ifndef KYLINGENERAL_H
#define KYLINGENERAL_H

#include <QObject>
#include "kylinnetworkresourcemanager.h"

enum KyConnectStatus {
    NOT_CONNECTED = 0,
    LAN_CONNECTED,
    WLAN_CONNECTED,
    LAN_CONNECTED_LIMITED,
    WLAN_CONNECTED_LIMITED,
};

class KylinGeneralOpration : public QObject
{
    Q_OBJECT
public:
    explicit KylinGeneralOpration(QObject *parent = nullptr);
    ~KylinGeneralOpration();
    void getConnectStatus(KyConnectStatus &status);
    bool getWirelessEnabled();
    void setWirelessNetworkEnabled(bool enabled);
    bool getNetworkingEnabled();
    void setNetworkingEnabled(bool enabled);
    void setWiredEnabled(bool enabled);
    bool getWiredEnabled();

Q_SIGNALS:
    void connectStatusChanged(KyConnectStatus);
    void wifiEnabledChanged(bool);
    void wiredEnabledChanged(bool);
    void networkingEnabledChanged(bool);

private Q_SLOTS:
    void updateConnectStatus();

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    KyConnectStatus m_status = NOT_CONNECTED;

    void updateGsetting(bool);
    void getConnectivity(KyConnectivity &connectivity);
    void getPrimaryConnectionType(KyConnectionType &connectType);
};

#endif // KYLINGENERAL_H
