#ifndef KYLINNETORKDEVICERESOURCE_H
#define KYLINNETORKDEVICERESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinconnectitem.h"
#include "kylinwiredconnectoperation.h"

class KyNetworkDeviceResourse : public QObject
{
    Q_OBJECT
public:
    explicit KyNetworkDeviceResourse(QObject *parent = nullptr);
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
    void getNetworkDeviceList(QStringList &networkDeviceList);
    void getHardwareInfo(QString ifaceName, QString &hardAddress, int &bandWith);
    NetworkManager::Device::State getDeviceState(QString deviceName);
    bool wiredDeviceCarriered(QString deviceName);
    //void DeviceSpeed(QString deviceName, KyWiredConnectItem *wiredItem);
    void setDeviceRefreshRate(QString deviceName, int ms);
    void disconnectDevice();
    void setDeviceAutoConnect();

private:
    KyWiredConnectOperation wiredOperation;
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    QStringList m_activeConnectUuidList;
    QMap<QString , QString> m_activeConnectUuidMap;
};
#endif // KYLINNETORKDEVICERESOURCE_H
