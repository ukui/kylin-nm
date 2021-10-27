#ifndef LANLISTITEM_H
#define LANLISTITEM_H
#include "listitem.h"
#include "kylinactiveconnectresource.h"

#include <QDBusInterface>
#include <QEvent>
#include <QMenu>
#include <QAction>

class LanListItem : public ListItem
{
    Q_OBJECT

public:
    LanListItem(const KyConnectItem *lanConnectItem, const QString &deviceName, QWidget *parent = nullptr);
    LanListItem(QWidget *parent = nullptr);

    ~LanListItem() = default;

public:
    void updateConnectionState(ConnectState state);

    QString getConnectionName();
    void updateConnectionName(QString connectionName);

    QString getConnectionPath();
    void updateConnectionPath(QString connectionPath);

protected:
    void setIcon(bool isOn);
    void onRightButtonClicked();

private:
    void connectItemCopy(const KyConnectItem *lanConnectItem);

private slots:
    void onInfoButtonClicked();
    void onNetButtonClicked();
    void onMenuTriggered(QAction *action);

private:
    KyConnectItem m_lanConnectItem;

    KyWiredConnectOperation *m_connectOperation = nullptr;
    KyNetworkDeviceResourse *m_deviceResource = nullptr;

    QString m_deviceName = "";
    QMenu *m_menu = nullptr;
};

#endif // LANLISTITEM_H
