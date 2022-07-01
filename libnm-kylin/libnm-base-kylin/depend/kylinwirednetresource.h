#ifndef KYLINCONNECTRESOURCE_H
#define KYLINCONNECTRESOURCE_H

#include <QString>
#include "kylinnetworkresourcemanager.h"
#include "kylinnetresource.h"
#include "kylinconnectitem.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinconnectsetting.h"
#include "kylinapconnectitem.h"

class KyWiredConnectResourse : public KyNetResource
{
    Q_OBJECT
public:
    explicit KyWiredConnectResourse(QObject *parent = nullptr);

public:
    void getWiredList(QMap<QString,QList<KyWiredItem>> &map);

    void getWiredConnectDetail(QString interface, QString uuid, KyDetailInfo &connectSetting);
private:
    void kyWirdNetItemListInit();
    KyWiredItem getConnectionItem(NetworkManager::Connection::Ptr connectPtr);

Q_SIGNALS:
    void wiredConnectionAdd(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void wiredConnectionUpdate(QString devName, QString connectUuid, QString connectName, QString connectPath);
    void connectionRemove(QString path);

    void wiredDeviceAdd(QString deviceName);
    void wiredDeviceUpdate(QString oldDeviceName, QString newDeviceName);
    void deviceRemove(QString deviceName);

private:
    QMap<QString, QList<KyWiredItem>>   m_wiredNetworkMap;
};
#endif // KYLINCONNECTRESOURCE_H
