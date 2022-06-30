#include "kynetworkicon.h"

#define EXCELLENT_SIGNAL 80
#define GOOD_SIGNAL 55
#define OK_SIGNAL 30
#define LOW_SIGNAL 5
#define NONE_SIGNAL 0

#define EXCELLENT_SIGNAL_ICON   "network-wireless-signal-excellent-symbolic"
#define GOOD_SIGNAL_ICON        "network-wireless-signal-good-symbolic"
#define OK_SIGNAL_ICON          "network-wireless-signal-ok-symbolic"
#define LOW_SIGNAL_ICON         "network-wireless-signal-weak-symbolic"
#define NONE_SIGNAL_ICON        "network-wireless-signal-none-symbolic"

#define EXCELLENT_SIGNAL_LIMIT_ICON   "ukui-network-wireless-signal-excellent-error-symbolic"
#define GOOD_SIGNAL_LIMIT_ICON        "ukui-network-wireless-signal-good-error-symbolic"
#define OK_SIGNAL_LIMIT_ICON          "ukui-network-wireless-signal-ok-error-symbolic"
#define LOW_SIGNAL_LIMIT_ICON         "ukui-network-wireless-signal-weak-error-symbolic"
#define NONE_SIGNAL_LIMIT_ICON        "ukui-network-wireless-signal-none-error-symbolic"

KyNetworkIcon::KyNetworkIcon(QWidget *parent):
    QPushButton(parent)
{
    qRegisterMetaType<KySecuType>("KyConnectState");
    qRegisterMetaType<KySecuType>("KyConnectStatus");
    qRegisterMetaType<KyConnectionType>("KyConnectionType");

    thread = new QThread;
    manager = new KyNetworkManager();
    manager->moveToThread(thread);
    connect(thread, &QThread::started, manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(thread, &QThread::finished, manager, &KyNetworkManager::deleteLater);
    thread->start();

    while (!manager->isInitFinished()) {
        ::usleep(1000);
    }

    loadIcons.append(QIcon::fromTheme("kylin-network-1"));
    loadIcons.append(QIcon::fromTheme("kylin-network-2"));
    loadIcons.append(QIcon::fromTheme("kylin-network-3"));
    loadIcons.append(QIcon::fromTheme("kylin-network-4"));
    loadIcons.append(QIcon::fromTheme("kylin-network-5"));
    loadIcons.append(QIcon::fromTheme("kylin-network-6"));
    loadIcons.append(QIcon::fromTheme("kylin-network-7"));
    loadIcons.append(QIcon::fromTheme("kylin-network-8"));
    loadIcons.append(QIcon::fromTheme("kylin-network-9"));
    loadIcons.append(QIcon::fromTheme("kylin-network-10"));
    loadIcons.append(QIcon::fromTheme("kylin-network-11"));
    loadIcons.append(QIcon::fromTheme("kylin-network-12"));

    loadingTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &KyNetworkIcon::onSetTrayIconLoading);

    manager->getConnectStatus(iconStatus);
    updateIcon();
    initConnect();

    refreshTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &KyNetworkIcon::updateIcon);
    refreshTimer->start(10 * 1000);
}

KyNetworkIcon::~KyNetworkIcon()
{
    delete thread;
    delete manager;
    thread = nullptr;
    manager = nullptr;
}

void KyNetworkIcon::initConnect()
{
    connect(manager, &KyNetworkManager::wiredStateChange, this, &KyNetworkIcon::onWiredStateChange);
    connect(manager, &KyNetworkManager::wirelessStateChange, this, &KyNetworkIcon::onWirelessStateChange);

    connect(manager, &KyNetworkManager::connectStatusChanged , [=](KyConnectStatus status){
        iconStatus = status;
        updateIcon();
    });
}

void KyNetworkIcon::updateIcon()
{
    if (loadingTimer->isActive()) {
        return;
    }
    int signalStrength = 0;

    if (iconStatus == LAN_CONNECTED) {
        this->setIcon(QIcon::fromTheme("network-wired-symbolic"));
        return;
    } else if (iconStatus == WLAN_CONNECTED
               || iconStatus == WLAN_CONNECTED_LIMITED) {
        signalStrength = manager->getAcivateWifiSignal();
    } else if (iconStatus == NOT_CONNECTED) {
        this->setIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
        return;
    } else if (iconStatus == LAN_CONNECTED_LIMITED) {
        this->setIcon(QIcon::fromTheme("network-error-symbolic"));
    }

    if (iconStatus == WLAN_CONNECTED) {
        if (signalStrength > EXCELLENT_SIGNAL){
            this->setIcon(QIcon::fromTheme(EXCELLENT_SIGNAL_ICON));
        } else if (signalStrength > GOOD_SIGNAL) {
            this->setIcon(QIcon::fromTheme(GOOD_SIGNAL_ICON));
        } else if (signalStrength > OK_SIGNAL) {
            this->setIcon(QIcon::fromTheme(OK_SIGNAL_ICON));
       } else if (signalStrength > LOW_SIGNAL) {
            this->setIcon(QIcon::fromTheme(LOW_SIGNAL_ICON));
       } else {
            this->setIcon(QIcon::fromTheme(NONE_SIGNAL_ICON));
       }
    } else if (iconStatus == WLAN_CONNECTED_LIMITED) {
        if (signalStrength > EXCELLENT_SIGNAL){
            this->setIcon(QIcon::fromTheme(EXCELLENT_SIGNAL_LIMIT_ICON));
        } else if (signalStrength > GOOD_SIGNAL) {
            this->setIcon(QIcon::fromTheme(GOOD_SIGNAL_LIMIT_ICON));
        } else if (signalStrength > OK_SIGNAL) {
            this->setIcon(QIcon::fromTheme(OK_SIGNAL_LIMIT_ICON));
       } else if (signalStrength > LOW_SIGNAL) {
            this->setIcon(QIcon::fromTheme(LOW_SIGNAL_LIMIT_ICON));
       } else {
            this->setIcon(QIcon::fromTheme(NONE_SIGNAL_LIMIT_ICON));
       }
    }
}

void KyNetworkIcon::startLoading()
{
    if (!loadingTimer->isActive()) {
        loadingTimer->start(60);
    }
}

void KyNetworkIcon::stopLoading()
{
    if (loadingTimer->isActive()) {
        loadingTimer->stop();
    }
}

void KyNetworkIcon::onSetTrayIconLoading()
{
    if (currentIconIndex > 11) {
        currentIconIndex = 0;
    }
    this->setIcon(loadIcons.at(currentIconIndex));
    currentIconIndex ++;
}

void KyNetworkIcon::onWiredStateChange(QString deviceName, QString uuid, KyConnectState state)
{
    Q_UNUSED(deviceName)
    Q_UNUSED(uuid)
    if (state == CONNECT_STATE_ACTIVATING
            || state == CONNECT_STATE_DEACTIVATING) {
        startLoading();
    } else {
        stopLoading();
        updateIcon();
    }
}

void KyNetworkIcon::onWirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state)
{
    Q_UNUSED(deviceName)
    Q_UNUSED(ssid)
    Q_UNUSED(uuid)
    if (state == CONNECT_STATE_ACTIVATING
            || state == CONNECT_STATE_DEACTIVATING) {
        startLoading();
    } else {
        stopLoading();
        updateIcon();
    }
}
