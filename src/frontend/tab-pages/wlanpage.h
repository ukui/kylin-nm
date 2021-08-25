#ifndef WLANPAGE_H
#define WLANPAGE_H

#include "tabpage.h"
#include "kywirelessnetresource.h"
#include "kylinactiveconnectresource.h"
#include "kylinnetworkdeviceresource.h"
//#define SCROLLAREA_HEIGHT 150
#define MORE_TEXT_MARGINS 16,0,0,0

class WlanPage : public TabPage
{
    Q_OBJECT
public:
    explicit WlanPage(QWidget *parent = nullptr);
    ~WlanPage() = default;

signals:

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void initWlanUI();
    void initConnections();
    QString m_wlanDevice; //临时用来存储网卡名
    void getWirelessIface();  //一个临时用于获取网卡的函数
    void getActiveWlan();
    void getAllWlan();
    QMap<QString, QListWidgetItem*> m_itemsMap;
    QFrame * m_inactivatedWlanListAreaCentralWidget = nullptr;
    QVBoxLayout * m_inactivatedWlanListAreaLayout = nullptr;
    QFrame * m_hiddenWlanWidget = nullptr;
    QHBoxLayout * m_hiddenWlanLayout = nullptr;
    QLabel * m_hiddenWlanLabel = nullptr;

    QString m_activatedWlanSSid;

    KyWirelessNetResource *m_resource = nullptr;
    KyActiveConnectResourse *m_connectResource = nullptr;
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    KyNetworkDeviceResourse *m_netDeviceResource=nullptr;

private slots:
    void onWlanAdded(QString interface, KyWirelessNetItem &item);
    void onWlanRemoved(QString interface, QString ssid);
    void onWlanUpdated();
    void onActivatedWlanChanged(QString uuid,
                                NetworkManager::ActiveConnection::State state,
                                NetworkManager::ActiveConnection::Reason reason);
};

#endif // WLANPAGE_H
