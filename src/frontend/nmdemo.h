#ifndef NMDEMO_H
#define NMDEMO_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QLineEdit>
#include <QLabel>

#include "backend/dbus-interface/kylinnetworkresourcemanager.h"
#include "backend/dbus-interface/kywirelessconnectoperation.h"
#include "backend/dbus-interface/kywirelessnetresource.h"
#include "backend/dbus-interface/kylinactiveconnectresource.h"
#include "backend/dbus-interface/kylinconnectresource.h"
#include "backend/dbus-interface/kylinactiveconnectresource.h"

class NmDemo : public QDialog
{
    Q_OBJECT

public:
    QPushButton *refreshButton;
    QPushButton *connectButton;
    QPushButton *disConnectButton;
    QPushButton *modifyButton;
    QPushButton *enableButton;
    QPushButton *disableButton;
    QPushButton *addButton;
    QTextEdit   *wifiList;
    QTextEdit   *debugLog;
    QLabel      *ssidLabel;
    QLabel      *ifaceLabel;
    QLabel      *wlansStatus;
    QLabel      *pwdLabel;
    QLabel      *actConnection;
    QLineEdit   *ssidText;
    QLineEdit   *ifaceNameText;
    QLineEdit   *pwdText;

    KyNetworkResourceManager   *m_networkResourceInstance = nullptr;
    KyWirelessConnectOperation *m_wco;
    KyWirelessNetResource      *m_wnr;
    KyActiveConnectResourse    *m_acr;
    KyConnectResourse          *m_cr;

private:
    QTimer      *m_timer;

public:
    explicit NmDemo(QWidget *parent = nullptr);
    ~NmDemo();

private:
    void initUi();
    void initConnect();
    void initTimer();
    void appendDebugLog(QString);
    void getWifiList();

    void showIpv4Ipv6DnsInfo(const QString &uuid, KyConnectSetting &sett);

public slots:
    void onWcoSignals(QString);
    void onBssidChange(QString, QString, QString);
    void onSecuTypeChange(QString, QString, QString);
    void onSignalStrengthChange(QString, QString, int);

    void onConnectionRemove(QString, QString);
    void onConnectionAdd(QString, QString);
    void onWifiNetworkAdd(QString, KyWirelessNetItem&);
    void onWifiNetworkRemove(QString, QString);
    void onWifiNetworkUpdate();

    void onConnectClicked();
    void onDisConnectClicked();
    void onModifyClicked();
    void onEnableClick();
    void onDisableClick();
    void onAddClick();

    void onActiveConnectionAdd(NetworkManager::ActiveConnection::Ptr conn);
    void onActiveConnectionUpdate(NetworkManager::ActiveConnection * conn);
    void onActiveConnectionRemove(NetworkManager::ActiveConnection * conn);
    void onActiveConnectionsReset();



signals:

};

#endif // NMDEMO_H
