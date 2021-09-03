#ifndef LANLISTITEM_H
#define LANLISTITEM_H
#include "listitem.h"
#include "kylinwiredwidget.h"
#include "kylinactiveconnectresource.h"

#include <QDBusInterface>
#include <QEvent>

class LanListItem : public ListItem
{
    Q_OBJECT

public:
    LanListItem(KyConnectItem *data, QString deviceName, QWidget *parent = nullptr);
    ~LanListItem() = default;

protected:
    void setIcon(bool isOn);

private:
    KyConnectItem *m_data = nullptr;
    KyWiredConnectOperation *m_connectOperation = nullptr;

    QString deviceName = nullptr;

private slots:
    void onInfoButtonClicked();
    void onNetButtonClicked();
};

#endif // LANLISTITEM_H
