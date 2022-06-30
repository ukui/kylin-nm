#ifndef KYNETWORKICON_H
#define KYNETWORKICON_H

#include "libnm-base-kylin_global.h"

#include <kylin-nm/kylinnetworkmanager.h>

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QThread>

//enum IconActiveType {
//    NOT_CONNECTED = 0,
//    LAN_CONNECTED,
//    WLAN_CONNECTED,
//    LAN_CONNECTED_LIMITED,
//    WLAN_CONNECTED_LIMITED,
//    ACTIVATING,
//};

class LIBNMBASEKYLIN_EXPORT KyNetworkIcon : public QPushButton
{
    Q_OBJECT
public:
    KyNetworkIcon(QWidget *parent = nullptr);
    ~KyNetworkIcon();

private:
    KyNetworkManager* manager;
    QThread*        thread;

    QTimer *loadingTimer;
    QTimer *refreshTimer;

    int currentIconIndex=0;
    QList<QIcon> loadIcons;

    KyConnectStatus iconStatus;

    void initConnect();

    void startLoading();
    void stopLoading();

private Q_SLOTS:
    void onSetTrayIconLoading();
    void updateIcon();
    void onWiredStateChange(QString deviceName, QString uuid, KyConnectState state);
    void onWirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state);
};

#endif // KYNETWORKICON_H
