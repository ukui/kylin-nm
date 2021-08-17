#ifndef LANLISTITEM_H
#define LANLISTITEM_H
#include "listitem.h"

class LanListItem : public ListItem
{
    Q_OBJECT
public:
    LanListItem(QWidget *parent = nullptr);
    ~LanListItem() = default;
protected:

};

#endif // LANLISTITEM_H
