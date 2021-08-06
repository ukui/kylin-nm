#ifndef NMDEMO_H
#define NMDEMO_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QLineEdit>
#include <QLabel>

#include "kylinnetworkresourcemanager.h"
#include "kywirelessconnectoperation.h"
#include "kywirelessnetresource.h"

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

public slots:
    void onWcoSignals(QString, QString, QString);
    void onBssidChange(QString, QString, QString);
    void onSecuTypeChange(QString, QString, QString);
    void onSignalStrengthChange(QString, QString, int);

    void onConnectionRemove(QString, QString);
    void onConnectionAdd(QString, QString);
    void onWifiNetworkAdd(QString, KyWirelessNetItem&);
    void onWifiNetworkRemove(QString, QString);

    void onConnectClicked();
    void onDisConnectClicked();
    void onModifyClicked();
    void onEnableClick();
    void onDisableClick();

    void onActiveConnectionAdd(NetworkManager::ActiveConnection::Ptr conn);
    void onActiveConnectionUpdate(NetworkManager::ActiveConnection * conn);
    void onActiveConnectionRemove(NetworkManager::ActiveConnection * conn);
    void onActiveConnectionsReset();



signals:

};

#endif // NMDEMO_H
