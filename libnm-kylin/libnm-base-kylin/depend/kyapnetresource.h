#ifndef KYAPNETRESOURCE_H
#define KYAPNETRESOURCE_H

#include <QObject>
#include "kylinapconnectitem.h"
#include "kylinnetworkresourcemanager.h"

class KyApNetResource : public QObject
{
    Q_OBJECT
public:
    explicit KyApNetResource(QObject *parent = nullptr);
    ~KyApNetResource();

    void getApConnections(QList<KyApConnectItem> &apConnectItemList);
Q_SIGNALS:
    void wirelessApConnectStateChange();
private:
    KyApConnectItem getApConnectItem(NetworkManager::Connection::Ptr connectPtr);
    KyApConnectItem getApConnectionByUuid(QString connectUuid);

    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};

#endif // KYAPNETRESOURCE_H
