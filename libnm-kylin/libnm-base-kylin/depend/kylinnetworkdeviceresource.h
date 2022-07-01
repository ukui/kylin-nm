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

    //设置单个有线设备开关
    void setWiredDeviceEnable(const QString& devName, bool enable);
    void getWiredDeviceEnableState(QMap<QString, bool> &map);

    int closeWiredNetworkWithDevice(QString deviceName);
    int openWiredNetworkWithDevice(QString deviceName);

private:
    void getActiveConnection(QString &deviceName, QString &connectUuid);
    void saveActiveConnection(QString &deviceName, QString &connectUuid);

Q_SIGNALS:
    void deviceStateChange(QString deviceName, KyDeviceState newstate, KyDeviceState oldstate,
                     NetworkManager::Device::StateChangeReason reason);
    void carrierChange(QString deviceName, bool pluged);

    void wiredDeviceEnableChange(QString, bool);

//    void deviceBitRateChanage(QString deviceName, int bitRate);
//    void deviceMacAddressChanage(QString deviceName, const QString &hwAddress);
//    void deviceActiveChanage(QString deviceName, bool deivceActive);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};
#endif // KYLINNETORKDEVICERESOURCE_H
