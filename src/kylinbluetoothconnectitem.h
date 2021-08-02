#ifndef KYLINBLUETOOTHCONNECTITEM_H
#define KYLINBLUETOOTHCONNECTITEM_H

#include <QObject>
#include <NetworkManagerQt/BluetoothDevice>
#include <NetworkManagerQt/BluetoothSetting>

class KyBluetoothConnectItem : public QObject
{
    Q_OBJECT
public:
    explicit KyBluetoothConnectItem();
    ~KyBluetoothConnectItem();

public:
    void dumpInfo();

public:
    QString m_connectName;
    QString m_connectUuid;

    QString m_deviceAddress;

    QString m_ipv4Address;
    QString m_ipv6Address;

    NetworkManager::ActiveConnection::State m_state;       //deactive、activing and actived
    //QString m_itemType;  //activeconnect or connect

};

#endif // KYLINBLUETOOTHCONNECTITEM_H
