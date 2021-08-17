#ifndef WLANLISTITEM_H
#define WLANLISTITEM_H
#include "listitem.h"

class WlanListItem : public ListItem
{
    Q_OBJECT
public:
    WlanListItem(QWidget *parent = nullptr);
    ~WlanListItem() = default;
protected:

};

#endif // WLANLISTITEM_H
