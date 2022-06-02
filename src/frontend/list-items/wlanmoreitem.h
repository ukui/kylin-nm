#ifndef WLANMOREITEM_H
#define WLANMOREITEM_H

#include <QObject>
#include "listitem.h"

const QString WMI_OB_NAME = "WlanMoreItemObjName";

class WlanMoreItem : public ListItem
{
    Q_OBJECT

protected:
    void onRightButtonClicked();

public:
    WlanMoreItem(QWidget *parent = nullptr);
    ~WlanMoreItem();

    void onNetButtonClicked();
    void onMenuTriggered(QAction *action);
signals:
    void hiddenWlanClicked();
};

#endif // WLANMOREITEM_H
