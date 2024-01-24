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
#ifndef KYLINNETORKDEVICERESOURCE_H
#define KYLINNETORKDEVICERESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinconnectitem.h"
#include "kylinwiredconnectoperation.h"

//设备操作和信息获取
class KyNetworkDeviceResourse : public QObject
{
    Q_OBJECT
public:
    explicit KyNetworkDeviceResourse(QObject *parent = nullptr);
    ~KyNetworkDeviceResourse();

public:
    void getNetworkDeviceList(KyDeviceType deviceType, QStringList &networkDeviceList);
    void getHardwareInfo(QString ifaceName, QString &hardAddress, int &bandWith);

    //移动热点有关
    void getDeviceActiveAPInfo(const QString devName, QString &strMac, uint &iHz, uint &iChan, QString &secuType);
    int  getWirelessDeviceCapability(const QString deviceName);

    KyDeviceState getDeviceState(QString deviceName);

    bool wiredDeviceIsCarriered(QString deviceName);
    bool wirelessDeviceIsExist(const QString devName);

    void setDeviceManaged(QString devName, bool managed);
    bool getDeviceManaged(QString devName);

private:
    void getActiveConnection(QString &deviceName, QString &connectUuid);
    void saveActiveConnection(QString &deviceName, QString &connectUuid);

Q_SIGNALS:
    void deviceStateChange(QString deviceName, KyDeviceState newstate, KyDeviceState oldstate,
                     NetworkManager::Device::StateChangeReason reason);
    void carrierChange(QString deviceName, bool pluged);

    void wiredDeviceEnableChange(QString, bool);
    void deviceManagedChange(QString deviceName, bool managed);

//    void deviceBitRateChanage(QString deviceName, int bitRate);
//    void deviceMacAddressChanage(QString deviceName, const QString &hwAddress);
//    void deviceActiveChanage(QString deviceName, bool deivceActive);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};
#endif // KYLINNETORKDEVICERESOURCE_H
