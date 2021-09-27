#ifndef WLANPAGE_H
#define WLANPAGE_H

#include "tabpage.h"
#include "kywirelessnetresource.h"
#include "kylinactiveconnectresource.h"
#include "kylinnetworkdeviceresource.h"
#include "kywirelessconnectoperation.h"
#include "wlanlistitem.h"
#include "kylinconnectoperation.h"
#include <QGSettings>
#include "netdetails/netdetail.h"
#include <QProcess>
#include "kylinactiveconnectresource.h"
#include "kywirelessnetresource.h"

//#define SCROLLAREA_HEIGHT 150
#define MORE_TEXT_MARGINS 16,0,0,0
#define SCROLLAREA_HEIGHT 200

class WlanListItem;

class WlanPage : public TabPage
{
    Q_OBJECT
public:
    explicit WlanPage(QWidget *parent = nullptr);
    ~WlanPage() = default;
    bool m_wlanIsConnected = false;

    //for dbus
    void getWirelessList(QMap<QString, QVector<QStringList> > &map);
    //开启热点
    void activeWirelessAp(const QString apName, const QString apPassword, const QString apDevice);
    //断开热点
    void deactiveWirelessAp(const QString apName, const QString apPassword, const QString apDevice);
    //获取热点
    void getStoredApInfo(QStringList &list);
    void activateWireless(const QString& devName, const QString& ssid);
    void deactivateWireless(const QString& devName, const QString& ssid);

    void showDetailPage(QString devName, QString uuid);
signals:
    void oneItemExpanded(const QString &ssid);
    void wlanAdd(QString devName, QStringList info);
    void wlanRemove(QString devName,QString ssid);
    void wlanActiveConnectionStateChanged(QString interface, QString ssid, int status);
    void hotspotDeactivated(QString devName, QString ssid);
    void hotspotActivated(QString devName, QString ssid);
    void signalStrengthChange(QString devName, QString ssid, int strength);
    void secuTypeChange(QString devName, QString ssid, QString secuType);
    void hiddenWlanClicked();
    void wlanConnectChanged();

public slots:
    void onMainWindowVisibleChanged(const bool &visible);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void initWlanUI();
    void initConnections();
    QString m_activedssid;
    QString m_disconnectuuid;
    int m_disconnecting;
    bool m_disconnectingflag = false;

    //定时触发扫描的定时器
    void initTimer();
    QTimer * m_scanTimer = nullptr;
    QTimer * m_refreshIconTimer = nullptr;

    void initDevice();//初始化默认设备
    void initDeviceCombox();

    void getActiveWlan();
    void appendActiveWlan(const QString &uuid, int &height);
    void getAllWlan();

    QMap<QString, QPair<QListWidgetItem*, WlanListItem*>> m_itemsMap;
    QListWidgetItem *m_expandedItem = nullptr;
    QFrame * m_inactivatedWlanListAreaCentralWidget = nullptr;
    QVBoxLayout * m_inactivatedWlanListAreaLayout = nullptr;
    QFrame * m_hiddenWlanWidget = nullptr;
    QHBoxLayout * m_hiddenWlanLayout = nullptr;
    QListWidget * m_activatedNetListWidget = nullptr;
    QLabel * m_hiddenWlanLabel = nullptr;

    QString m_activatedWlanSSid;
    QString m_activatedWlanUuid;
    QStringList m_devList;

    KyWirelessNetResource *m_resource = nullptr;
    KyActiveConnectResourse *m_connectResource = nullptr;
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    KyNetworkDeviceResourse *m_netDeviceResource = nullptr;
    KyWirelessConnectOperation * m_wirelessConnectOpreation = nullptr;
    KyConnectOperation *m_connectoperation = nullptr;
    KyConnectResourse * m_apConnectResource = nullptr;
    QGSettings *m_switchGsettings = nullptr;


private slots:
    void onWlanAdded(QString interface, KyWirelessNetItem &item);
    void onWlanRemoved(QString interface, QString ssid);
    void onWlanUpdated();

    void onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);
    void onActivatedWlanChanged(QString uuid,
                                NetworkManager::ActiveConnection::State state,
                                NetworkManager::ActiveConnection::Reason reason);
    void onItemHeightChanged(const QString &ssid);
    void onConnectButtonClicked(KyWirelessConnectSetting &connSettingInfo, const bool &isHidden);
    void onWlanSwitchClicked();
    void onWlanSwitchStatusChanged(const bool &checked);
    void onDeviceComboxIndexChanged(int currentIndex);
    void requestScan();
    void onHiddenWlanClicked();
    void showControlCenter();
    void onWifiEnabledChanged(bool isWifiOn);
    void onRefreshIconTimer();
};

#endif // WLANPAGE_H
