#ifndef KYLINWIREDCONNECTITEM_H
#define KYLINWIREDCONNECTITEM_H

#include <QString>
#include "kylinnetworkresourcemanager.h"

class KyWiredConnectItem : public QObject
{
    Q_OBJECT
public:
    explicit KyWiredConnectItem(QObject *parent = nullptr);
    ~KyWiredConnectItem();

public:
    void dumpInfo();

public:
    QString m_connectName;
    QString m_connectUuid;
    QString m_ifaceName;

    QString m_ipv4;
    QString m_ipv6;
    QString m_hardAddress;

    QString m_bandWith;
    qulonglong m_upSpeed;
    qulonglong m_downSpeed;

    NetworkManager::ActiveConnection::State m_state;       //deactive、activing and actived
    QString m_itemType;  //activeconnect or connect
};

#endif // KYLINWIREDCONNECTITEM_H
