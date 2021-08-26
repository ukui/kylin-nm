#ifndef WLANLISTITEM_H
#define WLANLISTITEM_H
#include "listitem.h"
#include "kywirelessnetitem.h"
#include "kywirelessnetresource.h"

#define NORMAL_HEIGHT 48
#define EXPANDED_HEIGHT 120

class WlanListItem : public ListItem
{
    Q_OBJECT
public:
    WlanListItem(KyWirelessNetResource *resource, KyWirelessNetItem *data, QWidget *parent = nullptr);
    WlanListItem(QWidget *parent = nullptr);
    ~WlanListItem();

public:
    void setWlanSignal(const int &signal);
    void setWlanState(const int &state);
    void setExpanded(const bool &expanded);

private:
    void initWlanUI();
    void initWlanConnection();
    void refreshIcon();

private:
    KyWirelessNetResource *m_resource = nullptr;
    KyWirelessNetItem *m_data = nullptr;
    bool m_isExpanded = false;
    bool m_hasPwd = true;

private slots:
    void onInfoButtonClicked();
    void onNetButtonClicked();
    void onSecurityChanged(QString interface, QString ssid, QString securityType);
};

#endif // WLANLISTITEM_H
