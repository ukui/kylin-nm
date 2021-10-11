#ifndef KYLINCONNECTITEM_H
#define KYLINCONNECTITEM_H

#include <QString>
#include "kylinnetworkresourcemanager.h"

class KyConnectItem : public QObject
{
    Q_OBJECT
public:
    explicit KyConnectItem(QObject *parent = nullptr);
    ~KyConnectItem();

public:
    void dumpInfo();

    void setConnectUuid(QString uuid);

public:
    QString m_connectName;
    QString m_connectUuid;
    QString m_connectPath;

    QString m_ifaceName;

    NetworkManager::ActiveConnection::State m_connectState;       //deactive、activing and actived

    NetworkManager::ConnectionSettings::ConnectionType m_itemType;  //wired or wireless  vpn etc
};

#endif // KYLINWIREDCONNECTITEM_H
