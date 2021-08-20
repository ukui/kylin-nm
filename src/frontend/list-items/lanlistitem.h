#ifndef LANLISTITEM_H
#define LANLISTITEM_H
#include "listitem.h"
#include "kylinconnectitem.h"

class LanListItem : public ListItem
{
    Q_OBJECT
public:
    LanListItem(KyConnectItem *data, QWidget *parent = nullptr);
    ~LanListItem() = default;
protected:

private:
    KyConnectItem * m_data = nullptr;

private:
    void initLanUI();
    void refreshIcon();

private slots:
    void onInfoButtonClicked();
    void onNetButtonClicked();
};

#endif // LANLISTITEM_H
