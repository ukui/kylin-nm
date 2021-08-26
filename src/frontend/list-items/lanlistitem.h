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
    bool eventFilter(QObject *watched, QEvent *event);

private:
    KyConnectItem *m_data = nullptr;
    KyActiveConnectResourse  *m_wiredActivatelist = nullptr;
    KyWiredConnectOperation *m_connect_operation = nullptr;
    bool m_flag = false;       //用户点击连接与否
    bool m_activated = false;
    QString deviceName = nullptr;
};

#endif // LANLISTITEM_H
