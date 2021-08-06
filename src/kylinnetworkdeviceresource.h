#ifndef KYLINNETORKDEVICERESOURCE_H
#define KYLINNETORKDEVICERESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinconnectitem.h"

class KyNetworkDeviceResourse : public QObject
{
    Q_OBJECT
public:
    KyNetworkDeviceResourse();
    ~KyNetworkDeviceResourse();

signals:
    void deviceAdd(QString deviceName);
    void deviceUpdate(QString deviceName);
    void deviceRemove(QString deviceName);

    void stateChange(QString deviceName, int state);
    void carrierChanage(QString deviceName, bool pluged);
    void deviceBitRateChanage(QString deviceName, int bitRate);
    void deviceMacAddressChanaged(QString deviceName, const QString &hwAddress);

public:
    void getNetworkDevices(QStringList &networkDeviceList);
    void getHardwareInfo(QString ifaceName, QString &hardAddress, int &bandWith);
    NetworkManager::Device::State getDeviceState(QString deviceName);
    bool wiredDeviceCarriered(QString deviceName);
    //void DeviceSpeed(QString deviceName, KyWiredConnectItem *wiredItem);
    void setDeviceRefreshRate(QString deviceName, int ms);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};
#endif // KYLINNETORKDEVICERESOURCE_H
