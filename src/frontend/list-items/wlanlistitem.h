#ifndef WLANLISTITEM_H
#define WLANLISTITEM_H
#include "listitem.h"
#include "kywirelessnetitem.h"

class WlanListItem : public ListItem
{
    Q_OBJECT
public:
    WlanListItem(KyWirelessNetItem *data, QWidget *parent = nullptr);
    ~WlanListItem() = default;

public:
    void setWlanSignal(const int &signal);
    void setWlanState(const int &state);

private:
    void initWlanUI();
    void initWlanConnection();
    void refreshIcon();

private:
    KyWirelessNetItem * m_data = nullptr;

private slots:
    void onInfoButtonClicked();
    void onNetButtonClicked();
};

#endif // WLANLISTITEM_H
