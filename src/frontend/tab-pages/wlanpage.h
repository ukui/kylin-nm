#ifndef WLANPAGE_H
#define WLANPAGE_H

#include "tabpage.h"
#include "kywirelessnetresource.h"
#include "kylinactiveconnectresource.h"
#include "kylinnetworkdeviceresource.h"
#include "kywirelessconnectoperation.h"
#include "wlanlistitem.h"
#include "wlanmoreitem.h"
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

    //for dbus
    void getWirelessList(QMap<QString, QVector<QStringList> > &map);
    //开启热点
    void activeWirelessAp(const QString apName, const QString apPassword, const QString wirelessBand, const QString apDevice);
    //断开热点
    void deactiveWirelessAp(const QString apName, const QString uuid);
    //获取热点
    void getStoredApInfo(QStringList &list);

    void activateWirelessConnection(const QString& devName, const QString& ssid);
    void deactivateWirelessConnection(const QString& devName, const QString& ssid);

    void showDetailPage(QString devName, QString uuid);

    bool wlanIsConnected();

    void getApInfoBySsid(QString devName, QString ssid, QStringList &list);
    //无线总开关
    void setWirelessSwitchEnable(bool enable);
    void getWirelessDeviceCap(QMap<QString, int> &map);

signals:
    void oneItemExpanded(const QString &ssid);
    void wlanAdd(QString devName, QStringList info);
    void wlanRemove(QString devName,QString ssid);
    void wlanActiveConnectionStateChanged(QString interface, QString ssid, QString uuid, int status);
    void hotspotDeactivated(QString devName, QString ssid);
    void hotspotActivated(QString devName, QString ssid, QString uuid);
    void signalStrengthChange(QString devName, QString ssid, int strength);
    void secuTypeChange(QString devName, QString ssid, QString secuType);
    void hiddenWlanClicked();
    void wlanConnectChanged(int state);

public slots:
    void onMainWindowVisibleChanged(const bool &visible);
    void onSecurityTypeChange(QString devName, QString ssid, QString secuType);
    void requestScan();

private slots:
    void onWlanAdded(QString interface, KyWirelessNetItem &item);
    void onWlanRemoved(QString interface, QString ssid);

    void onConnectionAdd(QString deviceName, QString ssid);
    void onConnectionRemove(QString deviceName, QString ssid);

    void onDeviceAdd(QString deviceName, NetworkManager::Device::Type deviceType);
    void onDeviceRemove(QString deviceName);
    void onDeviceNameUpdate(QString oldName, QString newName);

    void onConnectionStateChanged(QString uuid,
                                NetworkManager::ActiveConnection::State state,
                                NetworkManager::ActiveConnection::Reason reason);
    void onItemHeightChanged(const bool isExpanded, const QString &ssid);

    void onWlanSwitchClicked();
    void onWlanSwithGsettingsChanged(const QString &key);

    void onDeviceComboxIndexChanged(int currentIndex);
    void onHiddenWlanClicked();
    void showControlCenter();
    void onWifiEnabledChanged(bool isWifiOn);
    void onRefreshIconTimer();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    //定时触发扫描的定时器
    void initTimer();
    void initWlanUI();
    void initConnections();
    void initDevice();//初始化默认设备
    void initDeviceCombox();
    void initWlanSwitchState();
    void initWlanArea();
    void addWlanMoreItem();

    QListWidgetItem *addEmptyItem(QListWidget *wirelessListWidget);
    QListWidgetItem *addNewItem(KyWirelessNetItem &wirelessNetItem,
                                          QListWidget *wirelessListWidget);
    QListWidgetItem *insertNewItem(KyWirelessNetItem &wirelessNetItem,
                                             QListWidget *wirelessListWidget,
                                             int row);
    QListWidgetItem *insertNewItemWithSort(KyWirelessNetItem &wirelessNetItem,
                                             QListWidget *p_ListWidget);

    void clearWirelessNetItemMap(QMap<QString, QListWidgetItem*> &wirelessNetItem,
                                           QListWidget *wirelessListWidget);
    void deleteWirelessItemFormMap(QMap<QString, QListWidgetItem*> &wirelessNetItemMap,
                                             QListWidget *wirelessListWidget, QString ssid);

    void updateWlanItemState(QListWidget *p_wirelessListWidget,
                                       QListWidgetItem *p_listWidgetItem,
                                       ConnectState state);
    void updateWlanListItem(QString ssid);

    void constructWirelessNetArea();
    void constructActivateConnectionArea();

    void updateActivatedArea(QString uuid, QString ssid, QString devName);
    void updateWirelessNetArea(QString uuid, QString ssid, QString devName);

    void addDeviceToCombox(QString deviceName);
    void deleteDeviceFromCombox(QString deviceName);
    void updateDeviceForCombox(QString oldDeviceName, QString newDeviceName);

    void sendApStateChangeSignal(QString uuid, QString ssid, QString deviceName,
                                     NetworkManager::ActiveConnection::State state);
    void wlanShowNotify(QString ssid, NetworkManager::ActiveConnection::State state,
                                  NetworkManager::ActiveConnection::Reason reason);

private:
    QMap<QString, QListWidgetItem*> m_wirelessNetItemMap;
    QMap<QString, QListWidgetItem*> m_activateConnectionItemMap;

    QListWidgetItem *m_expandedItem = nullptr;

    QFrame * m_inactivatedWlanListAreaCentralWidget = nullptr;
    QVBoxLayout * m_inactivatedWlanListAreaLayout = nullptr;

    WlanMoreItem * m_hiddenWlanWidget = nullptr;
    QListWidgetItem *m_hiddenItem = nullptr;

    QListWidget * m_activatedNetListWidget = nullptr;

    QStringList m_devList;
    QString m_currentDevice;

    KyWirelessNetResource *m_wirelessNetResource = nullptr;
    KyActiveConnectResourse *m_activatedConnectResource = nullptr;
    KyNetworkDeviceResourse *m_netDeviceResource = nullptr;
    KyWirelessConnectOperation * m_wirelessConnectOpreation = nullptr;
    KyConnectResourse * m_connectResource = nullptr;

    QGSettings *m_switchGsettings = nullptr;
    bool m_wlanSwitchEnable = true;

    QTimer *m_scanTimer = nullptr;
    QTimer *m_refreshIconTimer = nullptr;
};

#endif // WLANPAGE_H
