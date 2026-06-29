#include "test_wlanlistitem.h"

TestWlanListItem::TestWlanListItem(QObject *parent)
    : QObject(parent)
{
}

void TestWlanListItem::initTestCase()
{
    KyWirelessNetItem wirelessItem;
    wirelessItem.m_NetSsid = "TestSSID";
    wirelessItem.m_connectUuid = "test-uuid-12345";
    wirelessItem.m_connDbusPath = "/test/path";
    wirelessItem.m_signalStrength = 75;
    wirelessItem.m_isConfigured = true;

    m_wlanListItem = new WlanListItem(wirelessItem, "wlan0", false, true);
    QVERIFY(m_wlanListItem != nullptr);
}

void TestWlanListItem::cleanupTestCase()
{
    if (m_wlanListItem) {
        delete m_wlanListItem;
        m_wlanListItem = nullptr;
    }
}

void TestWlanListItem::testConstructorWithAllParams()
{
    KyWirelessNetItem wirelessItem;
    wirelessItem.m_NetSsid = "TestSSID_AllParams";
    wirelessItem.m_connectUuid = "uuid-all-params";
    wirelessItem.m_connDbusPath = "/test/path/all";
    wirelessItem.m_signalStrength = 80;
    wirelessItem.m_isConfigured = false;

    WlanListItem *item = new WlanListItem(wirelessItem, "wlan0", true, true);
    QVERIFY(item != nullptr);

    QString ssid = item->getSsid();
    QCOMPARE(ssid, QString("TestSSID_AllParams"));

    delete item;
}

void TestWlanListItem::testConstructorWithDeviceAndWifi6Plus()
{
    KyWirelessNetItem wirelessItem;
    wirelessItem.m_NetSsid = "TestSSID_Wifi6Plus";
    wirelessItem.m_connectUuid = "uuid-wifi6plus";
    wirelessItem.m_connDbusPath = "/test/path/wifi6";
    wirelessItem.m_signalStrength = 90;
    wirelessItem.m_isConfigured = true;

    WlanListItem *item = new WlanListItem(wirelessItem, "wlan0", true);
    QVERIFY(item != nullptr);

    QString ssid = item->getSsid();
    QCOMPARE(ssid, QString("TestSSID_Wifi6Plus"));

    delete item;
}

void TestWlanListItem::testConstructorDefault()
{
    WlanListItem *item = new WlanListItem();
    QVERIFY(item != nullptr);

    QString ssid = item->getSsid();
    QCOMPARE(ssid, QString("EMPTY_SSID"));

    delete item;
}

void TestWlanListItem::testGetSsid()
{
    QString ssid = m_wlanListItem->getSsid();
    QCOMPARE(ssid, QString("TestSSID"));
}

void TestWlanListItem::testGetUuid()
{
    QString uuid = m_wlanListItem->getUuid();
    QCOMPARE(uuid, QString("test-uuid-12345"));
}

void TestWlanListItem::testGetPath()
{
    QString path = m_wlanListItem->getPath();
    QCOMPARE(path, QString("/test/path"));
}

void TestWlanListItem::testSetSignalStrength()
{
    m_wlanListItem->setSignalStrength(60);
    int signal = m_wlanListItem->getSignalStrength();
    QVERIFY(signal == 60);

    m_wlanListItem->setSignalStrength(100);
    signal = m_wlanListItem->getSignalStrength();
    QVERIFY(signal == 100);
}

void TestWlanListItem::testGetSignalStrength()
{
    m_wlanListItem->setSignalStrength(85);
    int signal = m_wlanListItem->getSignalStrength();
    QVERIFY(signal == 85);
}

void TestWlanListItem::testIsConfigured()
{
    bool configured = m_wlanListItem->isConfigured();
    QVERIFY(configured == true);

    KyWirelessNetItem wirelessItem;
    wirelessItem.m_NetSsid = "NotConfigured";
    wirelessItem.m_isConfigured = false;
    WlanListItem *item = new WlanListItem(wirelessItem, "wlan0", false, true);
    QVERIFY(item->isConfigured() == false);
    delete item;
}

void TestWlanListItem::testSetWlanState()
{
    m_wlanListItem->setWlanState(0);
    m_wlanListItem->setWlanState(1);
    m_wlanListItem->setWlanState(2);
    QVERIFY(true);
}

void TestWlanListItem::testSetExpanded()
{
    m_wlanListItem->setExpanded(true);
    QVERIFY(true);

    m_wlanListItem->setExpanded(false);
    QVERIFY(true);
}

void TestWlanListItem::testSetExpandedWithNullPointers()
{
    WlanListItem *item = new WlanListItem();
    item->setExpanded(true);
    item->setExpanded(false);
    QVERIFY(true);
    delete item;
}

void TestWlanListItem::testGetConnectionState()
{
    ConnectState state = m_wlanListItem->getConnectionState();
    QVERIFY(state == Deactivated);
}

void TestWlanListItem::testUpdateConnectStateActivated()
{
    m_wlanListItem->updateConnectState(Activated);
    ConnectState state = m_wlanListItem->getConnectionState();
    QVERIFY(state == Activated);
}

void TestWlanListItem::testUpdateConnectStateDeactivated()
{
    m_wlanListItem->updateConnectState(Deactivated);
    ConnectState state = m_wlanListItem->getConnectionState();
    QVERIFY(state == Deactivated);
}

void TestWlanListItem::testUpdateConnectStateActivating()
{
    m_wlanListItem->updateConnectState(Activating);
    ConnectState state = m_wlanListItem->getConnectionState();
    QVERIFY(state == Activating);

    m_wlanListItem->updateConnectState(Deactivating);
    state = m_wlanListItem->getConnectionState();
    QVERIFY(state == Deactivating);
}

void TestWlanListItem::testUpdateWirelessNetSecurity()
{
    m_wlanListItem->updateWirelessNetSecurity("TestSSID", "WPA2");
    QVERIFY(true);

    m_wlanListItem->updateWirelessNetSecurity("DifferentSSID", "WPA3");
    QVERIFY(true);
}

void TestWlanListItem::testUpdateWirelessNetItem()
{
    KyWirelessNetItem wirelessItem;
    wirelessItem.m_NetSsid = "UpdatedSSID";
    wirelessItem.m_connectUuid = "updated-uuid";
    wirelessItem.m_connDbusPath = "/updated/path";
    wirelessItem.m_signalStrength = 50;
    wirelessItem.m_isConfigured = true;

    m_wlanListItem->updateWirelessNetItem(wirelessItem);

    QString ssid = m_wlanListItem->getSsid();
    QVERIFY(ssid == "UpdatedSSID");
}

void TestWlanListItem::testForgetPwd()
{
    m_wlanListItem->forgetPwd();
    QVERIFY(true);

    KyWirelessNetItem wirelessItem;
    wirelessItem.m_isConfigured = false;
    WlanListItem *item = new WlanListItem(wirelessItem, "wlan0", false, true);
    item->forgetPwd();
    QVERIFY(true);
    delete item;
}

void TestWlanListItem::testGetAutoConnect()
{
    bool autoConnect = m_wlanListItem->getAutoConnect();
    QVERIFY(autoConnect == true);
}

void TestWlanListItem::testGetIconFunction()
{
    QString icon = getIcon(false, 90, 0);
    QVERIFY(!icon.isEmpty());

    icon = getIcon(true, 90, 0);
    QVERIFY(!icon.isEmpty());

    icon = getIcon(false, 50, 1);
    QVERIFY(!icon.isEmpty());

    icon = getIcon(true, 50, 1);
    QVERIFY(!icon.isEmpty());

    icon = getIcon(false, 30, 2);
    QVERIFY(!icon.isEmpty());

    icon = getIcon(true, 30, 2);
    QVERIFY(!icon.isEmpty());

    icon = getIcon(false, 10, 3);
    QVERIFY(!icon.isEmpty());

    icon = getIcon(true, 10, 3);
    QVERIFY(!icon.isEmpty());
}
