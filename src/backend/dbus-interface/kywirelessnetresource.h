#ifndef KYWIRELESSNETRESOURCE_H
#define KYWIRELESSNETRESOURCE_H

#include <QObject>
#include "kywirelessnetitem.h"
#include "kylinnetworkresourcemanager.h"
#include "kyenterpricesettinginfo.h"


//class KyWirelessNetItem;

class KyWirelessNetResource : public QObject
{
    Q_OBJECT
public:
    explicit KyWirelessNetResource(QObject *parent = nullptr);
    ~KyWirelessNetResource();

    //ui层调用接口
    bool getWifiNetwork(QString &devIfaceName, QString &ssid, KyWirelessNetItem &wirelessNetResource);
    bool getAllDeviceWifiNetwork(QMap<QString, QList<KyWirelessNetItem> > &map);
    bool getDeviceWifiNetwork(QString devIfaceName, QList<KyWirelessNetItem> &wirelessNetResource);

    bool getEnterPriseInfoTls(QString &uuid, KyEapMethodTlsInfo &info);
    bool getEnterPriseInfoPeap(QString &uuid, KyEapMethodPeapInfo &info);
    bool getEnterPriseInfoTtls(QString &uuid, KyEapMethodTtlsInfo &info);

    bool getWirelessActiveConnection(QMap<QString, QStringList> &map);


private:
    void kyWirelessNetItemListInit();
    QString getDeviceIFace(NetworkManager::WirelessNetwork::Ptr net);
    QString getDeviceIFace(NetworkManager::ActiveConnection::Ptr actConn, QString &wirelessNetResourcessid);

public slots:
    void onWifiNetworkAdded(QString, QString);
    void onWifiNetworkRemoved(QString, QString);
    void onWifiNetworkPropertyChange(NetworkManager::WirelessNetwork * net);
    void onWifiNetworkDeviceDisappear();

    void onConnectionAdd(NetworkManager::Connection::Ptr conn);
    void onConnectionRemove(QString);

signals:
    void signalStrengthChange(QString, QString, int);
    void bssidChange(QString, QString, QString);
    void secuTypeChange(QString, QString, QString);
    void connectionRemove(QString, QString);
    void connectionAdd(QString, QString);
    void wifiNetworkAdd(QString, KyWirelessNetItem&);
    void wifiNetworkRemove(QString, QString);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    QMap<QString, QList<KyWirelessNetItem> >      m_WifiNetworkList;

};

#endif // KYWIRELESSNETRESOURCE_H
