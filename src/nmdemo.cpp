#include "nmdemo.h"
#include "../wireless-security/dlghidewifi.h"
#include <QTime>
#include <QTimeZone>

QString enumToQstring(NetworkManager::ActiveConnection::State state)
{
    switch (state) {
    case NetworkManager::ActiveConnection::State::Unknown:
        return "Unknown";
        break;
    case NetworkManager::ActiveConnection::State::Activating:
        return "Activating";
        break;
    case NetworkManager::ActiveConnection::State::Activated:
        return "Activated";
        break;
    case NetworkManager::ActiveConnection::State::Deactivating:
        return "Deactivating";
        break;
    case NetworkManager::ActiveConnection::State::Deactivated:
        return "Deactivated";
        break;
    default:
        return "";
        break;
    }
}

NmDemo::NmDemo(QWidget *parent) : QDialog(parent)
{
    this->setFixedSize(510,810);

    //init ptr
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
    m_wco = new KyWirelessConnectOperation(this);
    m_wnr = new KyWirelessNetResource(this);
    m_timer = new QTimer(this);

    initUi();
    appendDebugLog("init...");
    initConnect();
    initTimer();
    appendDebugLog("init finish...");
    getWifiList();

    if (m_wco->getWirelessEnabled())
    {
        wlansStatus->setText("WLAN:ENABLE");
    } else {
        wlansStatus->setText("WLAN:DISABLE");
    }
}

NmDemo::~NmDemo()
{
    m_networkResourceInstance = nullptr;
}

void NmDemo::initUi()
{
    refreshButton    = new QPushButton(this);
    refreshButton->setText("Refresh");
    connectButton    = new QPushButton(this);
    connectButton->setText("Connect");
    disConnectButton = new QPushButton(this);
    disConnectButton->setText("DisConn");
    modifyButton     = new QPushButton(this);
    modifyButton->setText("Modify");
    enableButton     = new QPushButton(this);
    enableButton->setText("Enable");
    disableButton    = new QPushButton(this);
    disableButton->setText("Disable");
    wifiList         = new QTextEdit(this);
    debugLog         = new QTextEdit(this);
    ssidText         = new QLineEdit(this);
    ifaceNameText    = new QLineEdit(this);
    pwdText          = new QLineEdit(this);
    wlansStatus      = new QLabel(this);
    ssidLabel        = new QLabel(this);
    ssidLabel->setText("SSID");
    ifaceLabel       = new QLabel(this);
    ifaceLabel->setText("IfaceName");
    pwdLabel         = new QLabel(this);
    pwdLabel->setText("password");
    actConnection    = new QLabel(this);


    refreshButton->setGeometry( 5, 5, 80, 30);
    connectButton->setGeometry(105, 5, 80, 30);
    disConnectButton->setGeometry(205, 5, 80, 30);
    modifyButton->setGeometry(5, 55, 80, 30);
    enableButton->setGeometry(105, 55, 80, 30);
    disableButton->setGeometry(205, 55, 80, 30);
    wlansStatus->setGeometry(305, 55, 180, 30);
    ssidLabel->setGeometry(5, 105, 80, 30);
    ssidText->setGeometry(100, 105, 100, 30);
    pwdLabel->setGeometry(250, 105, 80, 30);
    pwdText->setGeometry(345, 105, 150, 30);
    actConnection->setGeometry(250, 145, 245, 30);
    ifaceLabel->setGeometry(5, 145, 80, 30);
    ifaceNameText->setGeometry(100, 145, 100, 30);
    wifiList->setGeometry(5, 200, 500,300);
    wifiList->setReadOnly(true);
    debugLog->setGeometry(5, 510, 500,300);
    debugLog->setReadOnly(true);
}

void NmDemo::initConnect()
{
    connect(m_wco, &KyWirelessConnectOperation::wifinEnabledChanged,[=](bool status){
        QString temp;
        temp.sprintf("wifinEnabledChanged %d",status);
        appendDebugLog(temp);
        if (status)
        {
            wlansStatus->setText("WLAN:ENABLE");
        } else {
            wlansStatus->setText("WLAN:DISABLE");
        }
    });

    //ui button
    connect(refreshButton, &QPushButton::clicked, [=](){
        appendDebugLog("init refreshButton clicked...");
        m_wco->requestWirelessScan();
    });

    connect(connectButton,      &QPushButton::clicked, this, &NmDemo::onConnectClicked);
    connect(disConnectButton,   &QPushButton::clicked, this, &NmDemo::onDisConnectClicked);
    connect(modifyButton,       &QPushButton::clicked, this, &NmDemo::onModifyClicked);
    connect(enableButton,       &QPushButton::clicked, this, &NmDemo::onEnableClick);
    connect(disableButton,      &QPushButton::clicked, this, &NmDemo::onDisableClick);
    //连接变化
    connect(m_wco, &KyWirelessConnectOperation::connectFail,      this, &NmDemo::onWcoSignals);
    connect(m_wco, &KyWirelessConnectOperation::disConnectFail,    this, &NmDemo::onWcoSignals);

    //列表变化
    connect(m_wnr, &KyWirelessNetResource::bssidChange,             this ,&NmDemo::onBssidChange);
    connect(m_wnr, &KyWirelessNetResource::secuTypeChange,          this ,&NmDemo::onSecuTypeChange);
    connect(m_wnr, &KyWirelessNetResource::signalStrengthChange,    this ,&NmDemo::onSignalStrengthChange);

    connect(m_wnr, &KyWirelessNetResource::connectionRemove,   this ,&NmDemo::onConnectionRemove);
    connect(m_wnr, &KyWirelessNetResource::connectionAdd,   this ,&NmDemo::onConnectionAdd);
    connect(m_wnr, &KyWirelessNetResource::wifiNetworkAdd,   this ,&NmDemo::onWifiNetworkAdd);
    connect(m_wnr, &KyWirelessNetResource::wifiNetworkRemove,   this ,&NmDemo::onWifiNetworkRemove);

    connect(m_networkResourceInstance, &KyNetworkResourceManager::activeConnectionAdd, this, &NmDemo::onActiveConnectionAdd);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::activeConnectionUpdate, this, &NmDemo::onActiveConnectionUpdate);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::activeConnectionRemove, this, &NmDemo::onActiveConnectionRemove);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::activeConnectionsReset, this, &NmDemo::onActiveConnectionsReset);
}

void NmDemo::initTimer()
{
    //30s扫描一次wifi
    m_timer->setTimerType(Qt::PreciseTimer);
    QObject::connect(m_timer, &QTimer::timeout, [=](){
        m_wco->requestWirelessScan();
        getWifiList();
    });
    m_timer->start(30*1000);
}


void NmDemo::onWcoSignals(QString ssid, QString devIFace, QString reason)
{
    appendDebugLog(ssid + " connect or disconnect faild because " + reason);
}

void NmDemo::onBssidChange(QString devIface, QString ssid, QString bssid)
{
    appendDebugLog(ssid + " belongs to " + devIface + " bssid change to " + bssid);
}

void NmDemo::onSecuTypeChange(QString devIface, QString ssid, QString secuType)
{
    appendDebugLog(ssid + " belongs to " + devIface + " security change to " + secuType);
}

void NmDemo::onSignalStrengthChange(QString devIface , QString ssid, int signal)
{
   appendDebugLog(ssid + " belongs to " + devIface + " SignalStrength change to " + QString::number(signal));
}

void NmDemo::onConnectionRemove(QString devIface, QString ssid)
{
    appendDebugLog("onConnectionRemove..." + devIface + " " + ssid);
    getWifiList();
}

void NmDemo::onConnectionAdd(QString devIface, QString ssid)
{
    appendDebugLog("onConnectionAdd..." + devIface + " " + ssid);
    getWifiList();
}

void NmDemo::onWifiNetworkAdd(QString devIface, KyWirelessNetItem& item)
{
    appendDebugLog("onWifiNetworkAdd..." + devIface + " " + item.m_NetSsid);
    getWifiList();
}

void NmDemo::onWifiNetworkRemove(QString devIface, QString ssid)
{
    appendDebugLog("onWifiNetworkRemove..." + devIface + " " + ssid);
    getWifiList();
}

void NmDemo::onConnectClicked()
{
    appendDebugLog("onConnectClicked...");
    QString ssid = ssidText->text();
    QString devIface = ifaceNameText->text();
    QString pwd = pwdText->text();
    QString uuid;
    bool isNew = true;
    bool isHidden = true;
    bool isEnterPirse = false;

    appendDebugLog("about to connect " + ssid + " " + devIface + " " + pwd);
    QList<KyWirelessNetItem> list;
    m_wnr->getDeviceWifiNetwork(devIface, list);
    appendDebugLog("agetDeviceWifiNetwork " + devIface + " size: " + QString::number(list.size()));
    QList<KyWirelessNetItem>::iterator iter = list.begin();
    while (iter != list.end())
    {
        if (iter->m_NetSsid == ssid)
        {
            isHidden = false;
            QString secuType = iter->m_secuType;
            qDebug() << secuType;
            if (secuType.indexOf("802.1X") >= 0)
            {
                isEnterPirse = true;
            }

            if (iter->m_isConfigured == true)
            {
                isNew = false;
                uuid = iter->m_connectUuid;
            }
        }
        iter++;
    }
    if (isEnterPirse)
    {
        if(!isNew)
        {
            KyEapMethodPeapInfo a;
            a.connName = ssid;
            a.phase2AuthMethod = AuthEapMethodPeapMschapv2;
            a.userName = "steve";
            a.userPWD = "testing";
            appendDebugLog("modifyEnterPriseInfoPeap");
            if (!(m_wnr->modifyEnterPriseInfoPeap(uuid,a)))
            {
                appendDebugLog("modifyEnterPriseInfoPeap fail");
                return;
            }
            appendDebugLog("activeWirelessConnect...");
            m_wco->activeWirelessConnect(devIface,uuid);
            return;
        } else {
            KyEapMethodTtlsInfo c;
            c.connName = ssid;
            c.authType = AUTH_NO_EAP;
            c.authNoEapMethod = AuthMethodTtlsMschapv2;
            c.userName = "steve";
            c.userPWD = "testing";
            appendDebugLog("addAndActiveWirelessEnterPriseTtlsConnect...");
            qDebug() << "addAndActiveWirelessEnterPriseTtlsConnect";
            m_wco->addAndActiveWirelessEnterPriseTtlsConnect(c, devIface, isHidden, true, 0);
            return;
        }
    }
    KySecuType secuType = WPA_AND_WPA2_PERSONAL;
    if (isHidden)
    {
        appendDebugLog("addAndActiveWirelessHiddenConnect...");
        m_wco->addAndActiveWirelessHiddenConnect(secuType, ssid, devIface, pwd, true, 0);
        return;
    }
    if (isNew)
    {
        appendDebugLog("addAndActiveWirelessConnect...");
        m_wco->addAndActiveWirelessConnect(ssid, devIface, pwd, true, 0);
    } else {
        appendDebugLog("activeWirelessConnectWithPwd...");
        m_wco->activeWirelessConnect(devIface,uuid);
    }
}

void NmDemo::onDisConnectClicked()
{
    appendDebugLog("onDisConnectClicked...");
    QString ssid = ssidText->text();
    QString devIface = ifaceNameText->text();
    QString uuid;
    bool isFind = false;
    QList<KyWirelessNetItem> list;
    m_wnr->getDeviceWifiNetwork(devIface, list);
    QList<KyWirelessNetItem>::iterator iter = list.begin();
    while (iter != list.end())
    {
        if (iter->m_NetSsid == ssid)
        {
            if (iter->m_isConfigured == true)
            {
                isFind = true;
                uuid = iter->m_connectUuid;
                break;
            }
        }
        iter++;
    }
    if (!isFind)
    {
        return;
    }
    appendDebugLog("deActiveWirelessConnect " + ssid);
    m_wco->deActiveWirelessConnect(uuid);
}

void NmDemo::onModifyClicked()
{
    appendDebugLog("onModifyClicked...");
    //TODO
}

void NmDemo::onEnableClick()
{
    appendDebugLog("onEanbleClick...");
    m_wco->setWirelessEnabled(true);
}

void NmDemo::onDisableClick()
{
    appendDebugLog("onDisableClick...");
    m_wco->setWirelessEnabled(false);
}

void NmDemo::appendDebugLog(QString log)
{
    QTimeZone timeZone(QString::fromLatin1(QTimeZone::systemTimeZoneId()).toLatin1());
    QDateTime tzNow = QDateTime::currentDateTime().toTimeZone(timeZone);
    QString time= tzNow.toString("hh:mm:ss AP");
    debugLog->append(time + "===="+ log);
    qDebug() << log;
}

void NmDemo::onActiveConnectionAdd(NetworkManager::ActiveConnection::Ptr conn)
{
    if (conn->type() != NetworkManager::ConnectionSettings::ConnectionType::Wireless)
    {
        return;
    }
    appendDebugLog(conn->id()+ " " + enumToQstring(conn->state()));
    actConnection->setText(conn->id()+ " " + enumToQstring(conn->state()));
}

void NmDemo::onActiveConnectionUpdate(NetworkManager::ActiveConnection * conn)
{
    if (conn->type() != NetworkManager::ConnectionSettings::ConnectionType::Wireless)
    {
        return;
    }
    appendDebugLog(conn->id()+ " " + enumToQstring(conn->state()));
    actConnection->setText(conn->id()+ " " + enumToQstring(conn->state()));
}

void NmDemo::onActiveConnectionRemove(NetworkManager::ActiveConnection * conn)
{
    if (conn->type() != NetworkManager::ConnectionSettings::ConnectionType::Wireless)
    {
        return;
    }
    appendDebugLog(conn->id()+ " " + enumToQstring(conn->state()));
    actConnection->setText(conn->id()+ " " + enumToQstring(conn->state()));
}

void NmDemo::onActiveConnectionsReset()
{
    appendDebugLog("onActiveConnectionsReset");
}

void NmDemo::getWifiList()
{
    qDebug() << "getWifiList";
    wifiList->clear();
    QMap<QString,QStringList> actMap;
    if (!m_wnr->getWirelessActiveConnection(actMap))
    {
        return;
    }
    QMap<QString,QStringList>::iterator iter1 = actMap.begin();
    while (iter1 != actMap.end())
    {
        wifiList->append(iter1.key());
        for (int i = 0; i < iter1->size(); i++)
        {
            wifiList->append(iter1->at(i));
        }
        wifiList->append("====================================");
        iter1++;
    }
    QMap<QString, QList<KyWirelessNetItem> > map;
    if (!m_wnr->getAllDeviceWifiNetwork(map))
    {
        return;
    }
    QMap<QString, QList<KyWirelessNetItem> >::iterator iter = map.begin();
    while (iter != map.end())
    {
        for (int i = 0; i < iter.value().size(); i++)
        {
            qDebug() << iter.value().at(i).m_NetSsid;
            wifiList->append("SSID: " + iter.value().at(i).m_NetSsid + "  Configed: " + QString::number(iter.value().at(i).m_isConfigured));
            wifiList->append("frequency: " + QString::number(iter.value().at(i).m_frequency));
            wifiList->append("signalStrength: " + QString::number(iter.value().at(i).m_signalStrength));
            QString security = iter.value().at(i).m_secuType;
            if (security.isEmpty())
            {
                security = "NONE";
            }
            wifiList->append("security: " + security);
            wifiList->append("MAC: " + iter.value().at(i).m_bssid);
            wifiList->append("====================================");
        }
        iter++;
    }
}
