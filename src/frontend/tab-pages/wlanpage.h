#ifndef WLANPAGE_H
#define WLANPAGE_H

#include "tabpage.h"
#include "kywirelessnetresource.h"
#include "kylinnetworkdeviceresource.h"

//#define SCROLLAREA_HEIGHT 150
#define MORE_TEXT_MARGINS 16,0,0,0

class WlanPage : public TabPage
{
    Q_OBJECT
public:
    explicit WlanPage(QWidget *parent = nullptr);
    ~WlanPage() = default;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void initWlanUI();
    void initConnections();
    void initDevice();//初始化默认设备
    void getActiveWlan();
    void getAllWlan();
    QMap<QString, QListWidgetItem*> m_itemsMap;
    QFrame * m_inactivatedWlanListAreaCentralWidget = nullptr;
    QVBoxLayout * m_inactivatedWlanListAreaLayout = nullptr;
    QFrame * m_hiddenWlanWidget = nullptr;
    QHBoxLayout * m_hiddenWlanLayout = nullptr;
    QLabel * m_hiddenWlanLabel = nullptr;

    QString m_activatedWlanSSid;
    QStringList devList;

    KyWirelessNetResource *m_resource = nullptr;
    KyNetworkDeviceResourse *m_device = nullptr;


private slots:
    void onWlanAdded(QString interface, KyWirelessNetItem &item);
    void onWlanRemoved(QString interface, QString ssid);
    void onWlanUpdated();

    void onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);
};

#endif // WLANPAGE_H
