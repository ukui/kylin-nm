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
    void deviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void deviceRemove(QString deviceName);
    void deviceUpdate(QString deviceName);

    void deviceNameUpdate(QString oldName, QString newName);
    void stateChange(QString deviceName, int state);

    void carrierChanage(QString deviceName, bool pluged);
    void deviceBitRateChanage(QString deviceName, int bitRate);
    void deviceMacAddressChanaged(QString deviceName, const QString &hwAddress);

public slots:
    void onDeviceAdd(QString deviceName, QString uni, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName, QString uni);
    void onDeviceUpdate(NetworkManager::Device * dev);

public:
    void getNetworkDeviceList(NetworkManager::Device::Type deviceType, QStringList &networkDeviceList);
    void getHardwareInfo(QString ifaceName, QString &hardAddress, int &bandWith);
    void getDeviceActiveAPInfo(const QString devName, QString &strMac, uint &iHz, uint &iChan, QString &secuType);
    int getWirelessDeviceCapability(const QString deviceName);
    NetworkManager::Device::State getDeviceState(QString deviceName);

    bool wiredDeviceIsCarriered(QString deviceName);
    bool wirelessDeviceIsExist(const QString devName);
    bool deviceIsWired(QString deviceName);

    void setDeviceRefreshRate(QString deviceName, int ms);

private:
    KyWiredConnectOperation wiredOperation;
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    QStringList m_activeConnectUuidList;
    QMap<QString, QString> m_deviceMap;

    void initDeviceMap();
};
#endif // KYLINNETORKDEVICERESOURCE_H
