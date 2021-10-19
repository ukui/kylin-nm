#ifndef LANLISTITEM_H
#define LANLISTITEM_H
#include "listitem.h"
#include "kylinactiveconnectresource.h"

#include <QDBusInterface>
#include <QEvent>

class LanListItem : public ListItem
{
    Q_OBJECT

public:
    LanListItem(KyConnectItem *data, QString deviceName, QWidget *parent = nullptr);
    LanListItem(QWidget *parent = nullptr);
    ~LanListItem() = default;

    KyConnectItem *m_data = nullptr;
    KyWiredConnectOperation *m_connectOperation = nullptr;
    KyActiveConnectResourse *m_activeConnectResource = nullptr;
    KyConnectResourse *m_connectResource = nullptr;
    KyNetworkDeviceResourse *m_deviceResource = nullptr;

    QString deviceName = "";

protected:
    void setIcon(bool isOn);
    void onRightButtonClicked();


private:

private slots:
    void onInfoButtonClicked();
    void onNetButtonClicked();
    void onLanStatusChange(QString uuid, NetworkManager::ActiveConnection::State state, NetworkManager::ActiveConnection::Reason reason);
};

#endif // LANLISTITEM_H
