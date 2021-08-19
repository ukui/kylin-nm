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
    m_acr = new KyActiveConnectResourse(this);
    m_cr = new KyConnectResourse(this);
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
    addButton = new QPushButton(this);
    addButton->setText("Add");
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
    addButton->setGeometry(305, 5 ,80 ,30);
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
    connect(addButton,          &QPushButton::clicked, this, &NmDemo::onAddClick);

    //连接变化


    connect(m_wco, &KyWirelessConnectOperation::createConnectionError,      this, &NmDemo::onWcoSignals);
    connect(m_wco, &KyWirelessConnectOperation::updateConnectionError,    this, &NmDemo::onWcoSignals);
    connect(m_wco, &KyWirelessConnectOperation::deleteConnectionError,      this, &NmDemo::onWcoSignals);
    connect(m_wco, &KyWirelessConnectOperation::activateConnectionError,    this, &NmDemo::onWcoSignals);
    connect(m_wco, &KyWirelessConnectOperation::deactivateConnectionError,      this, &NmDemo::onWcoSignals);
    connect(m_wco, &KyWirelessConnectOperation::andAndActivateConnectionError,    this, &NmDemo::onWcoSignals);

    //列表变化
    connect(m_wnr, &KyWirelessNetResource::bssidChange,             this ,&NmDemo::onBssidChange);
    connect(m_wnr, &KyWirelessNetResource::secuTypeChange,          this ,&NmDemo::onSecuTypeChange);
    connect(m_wnr, &KyWirelessNetResource::signalStrengthChange,    this ,&NmDemo::onSignalStrengthChange);

    connect(m_wnr, &KyWirelessNetResource::connectionRemove,   this ,&NmDemo::onConnectionRemove);
    connect(m_wnr, &KyWirelessNetResource::connectionAdd,   this ,&NmDemo::onConnectionAdd);
    connect(m_wnr, &KyWirelessNetResource::wifiNetworkAdd,   this ,&NmDemo::onWifiNetworkAdd);
    connect(m_wnr, &KyWirelessNetResource::wifiNetworkRemove,   this ,&NmDemo::onWifiNetworkRemove);

    connect(m_acr, &KyActiveConnectResourse::stateChangeReason, this, [this](QString uuid,
            NetworkManager::ActiveConnection::State state,
            NetworkManager::ActiveConnection::Reason reason){
        appendDebugLog(QString("%1 %2 %3").arg(uuid).arg(state).arg(reason));
    });
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


void NmDemo::onWcoSignals(QString errorMessage)
{
    appendDebugLog(errorMessage);
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
            a.phase2AuthMethod = KyAuthMethodMschapv2;
            a.userName = "steve";
            a.userPWD = "testing";
            appendDebugLog("modifyEnterPriseInfoPeap");

            appendDebugLog("activeWirelessConnect...");
            m_wco->activeWirelessConnect(devIface,uuid);
            return;
        } else {
            KyEapMethodPeapInfo c;
            c.m_passwdFlag = NetworkManager::Setting::SecretFlagType::None;
            c.phase2AuthMethod = KyAuthMethodMschapv2;
            c.userName = "steve";
            c.userPWD = "testing";

            KyWirelessConnectSetting a;
            a.m_connectName = ssid;
            a.m_ssid = ssid;
            a.isAutoConnect = true;
            a.m_type = WpaEap;

            appendDebugLog("addAndActiveWirelessEnterPriseTtlsConnect...");
            qDebug() << "addAndActiveWirelessEnterPriseTtlsConnect";
//            m_wco->addAndActiveWirelessEnterPriseTtlsConnect(c, devIface, isHidden, true, 0);
            m_wco->addAndActiveWirelessEnterPrisePeapConnect(c, a, devIface, false);
            return;
        }
    }
    if (isNew)
    {
        appendDebugLog("addAndActiveWirelessConnect...");
        KyWirelessConnectSetting a;
        a.m_connectName = ssid;
        a.m_ssid = ssid;
        a.isAutoConnect = true;
        a.m_psk = pwd;
        a.m_type = WpaNone;


        m_wco->addAndActiveWirelessConnect(devIface,a, isHidden);
    } else {
//        NetworkManager::Setting::SecretFlags c;
//        m_wco->getConnSecretFlags(uuid, c);
//        appendDebugLog(QString("%1").arg(c));
        appendDebugLog("activeWirelessConnect...");
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
    m_wco->deActivateWirelessConnection(ssid, uuid);
}

void NmDemo::onModifyClicked()
{
    appendDebugLog("onModifyClicked...");
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

//    KyEapMethodTtlsInfo info;
//    m_wco->updateWirelessEnterPriseTtlsConnect(uuid, info);

//    QString psk = m_wco->getPsk(uuid);
//    appendDebugLog(ssid +" getPsk  is " + psk);
//    return;

    appendDebugLog("getConnectKeyMgmt" + QString::number(m_wco->getConnectKeyMgmt(uuid)));

    KyWirelessConnectSetting wcs;
    wcs.m_type = WpaPsk;
    wcs.m_psk = "123456zsx";
    m_wco->updateWirelessPersonalConnect(uuid, wcs ,true);
    appendDebugLog("getConnectKeyMgmt" + QString::number(m_wco->getConnectKeyMgmt(uuid)));
    return;

    m_wco->setWirelessAutoConnect(uuid, false);
    return;

    KyConnectSetting sett;
    showIpv4Ipv6DnsInfo(uuid, sett);

    sett.m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    sett.setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
    sett.m_ipv4Address.clear();
    QStringList ipv4DnsList;
    ipv4DnsList << "1.1.1.1";
    QString a("192.168.1.17"),b("255.255.255.0"),c("192.168.1.0");
    sett.ipv4AddressConstruct(a, b, c, ipv4DnsList);

//    sett.setIpConfigType(IPADDRESS_V6, CONFIG_IP_MANUAL);
//    QStringList ipv6DnsList;
//    ipv6DnsList << "1.1.1.1";
//    QString d("1.1.1.1"),e("1.1.1.1"),f("1.1.1.1");
//    sett.ipv6AddressConstruct(d, e, f, ipv6DnsList);

    m_wco->updateIpv4AndIpv6SettingInfo(uuid, sett);

    showIpv4Ipv6DnsInfo(uuid, sett);
}

void NmDemo::showIpv4Ipv6DnsInfo(const QString &uuid, KyConnectSetting &settInfo)
{
    m_cr->getConnectionSetting(uuid, settInfo);
    appendDebugLog("connect setting info:");
    appendDebugLog("connect name " + settInfo.m_connectName);
    appendDebugLog("iface name " + settInfo.m_ifaceName);
    appendDebugLog("ipv4 type " + QString::number(settInfo.m_ipv4ConfigIpType));

    for (int index = 0 ; index < settInfo.m_ipv4Address.size(); index++) {
        appendDebugLog("ipv4 address " + settInfo.m_ipv4Address.at(index).ip().toString());
        appendDebugLog("ipv4 gate way " + settInfo.m_ipv4Address.at(index).gateway().toString());
        appendDebugLog("ipv4 net mask " + settInfo.m_ipv4Address.at(index).netmask().toString());
    }

    for (int index = 0; index < settInfo.m_ipv4Dns.size(); ++index) {
        appendDebugLog("ipv4 dns " + settInfo.m_ipv4Dns.at(index).toString());
    }

    appendDebugLog("ipv6 type " + QString::number(settInfo.m_ipv6ConfigIpType));

    for (int index = 0 ; index < settInfo.m_ipv6Address.size(); index++) {
        appendDebugLog("ipv6 address " + settInfo.m_ipv6Address.at(index).ip().toString());
        appendDebugLog("ipv6 gate way " + settInfo.m_ipv6Address.at(index).gateway().toString());
        appendDebugLog("ipv6 net mask " + settInfo.m_ipv6Address.at(index).netmask().toString());
    }

    for (int index = 0; index < settInfo.m_ipv6Dns.size(); ++index) {
        appendDebugLog("ipv6 dns " + settInfo.m_ipv6Dns.at(index).toString());
    }
    QString ipv4,ipv6;
    QList<QHostAddress> ipv4Dns,ipv6Dns;
    ipv4Dns.clear();
    ipv6Dns.clear();
    m_acr->getActiveConnectIpInfo(uuid, ipv4, ipv6);
    appendDebugLog("Dynamic ipv4 address " + ipv4);
    appendDebugLog("Dynamic ipv6 address " + ipv6);
    m_acr->getActiveConnectDnsInfo(uuid, ipv4Dns, ipv6Dns);
    appendDebugLog(QString::number(ipv4Dns.size()) + QString::number(ipv6Dns.size()));
    for (int index = 0 ; index < ipv4Dns.size(); index++) {
        appendDebugLog("Dynamic ipv4 dns " + ipv4Dns.at(index).toString());
    }
    for (int index = 0 ; index < ipv6Dns.size(); index++) {
        appendDebugLog("Dynamic ipv6 dns " + ipv6Dns.at(index).toString());
    }
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

void NmDemo::onAddClick()
{
    appendDebugLog("onAddClick...");
    KyWirelessConnectSetting sett;
    sett.m_connectName = "test";
    sett.m_ssid = "test";
    sett.isAutoConnect = false;
    sett.m_psk = "";
    sett.m_type = WpaEap;
//    //ipv4
    sett.setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
//    QString ipv4Address("192.168.1.17"), ipv4NetMask("255.255.255.0"), ipv4GateWay("192.168.1.0");
//    QStringList list;
//    list << "127.17.50.100";
//    sett.ipv4AddressConstruct(ipv4Address, ipv4NetMask, ipv4GateWay, list);
//    m_wco->addConnect(sett);
    KyEapMethodPeapInfo peap;
    peap.phase2AuthMethod = KyAuthMethodMschapv2;
    peap.userName = "steve";
    peap.userPWD = "testing";
    m_wco->addPeapConnect(sett,peap);
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
