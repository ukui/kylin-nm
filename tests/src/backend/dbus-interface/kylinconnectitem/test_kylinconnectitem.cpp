#include "test_kylinconnectitem.h"
#include "kylinconnectitem.h"
#include <QDebug>

TestKyConnectItem::TestKyConnectItem(QObject *parent)
    : QObject(parent)
{
}

void TestKyConnectItem::testDefaultConstructor()
{
    KyConnectItem item;
    // Verify default values
    QVERIFY(item.m_connectName.isEmpty());
    QVERIFY(item.m_connectUuid.isEmpty());
    QVERIFY(item.m_connectPath.isEmpty());
    QVERIFY(item.m_ifaceName.isEmpty());
    // State should be Unknown initially
    QVERIFY(item.m_connectState == NetworkManager::ActiveConnection::State::Unknown);
    // Type should be Unknown initially
    QVERIFY(item.m_itemType == NetworkManager::ConnectionSettings::ConnectionType::Unknown);
}

void TestKyConnectItem::testSetConnectUuid()
{
    KyConnectItem item;
    // Test setting UUID to empty string
    item.setConnectUuid("");
    QVERIFY(item.m_connectUuid.isEmpty());
}

void TestKyConnectItem::testSetConnectUuidWithEmptyString()
{
    KyConnectItem item;
    QString uuid = "";
    item.setConnectUuid(uuid);
    QVERIFY(item.m_connectUuid == "");
}

void TestKyConnectItem::testSetConnectUuidWithValidUuid()
{
    KyConnectItem item;
    QString uuid = "12345678-1234-1234-1234-123456789012";
    item.setConnectUuid(uuid);
    QVERIFY(item.m_connectUuid == uuid);
}

void TestKyConnectItem::testDumpInfo()
{
    KyConnectItem item;
    // set some values first
    item.m_connectName = "TestConnection";
    item.m_connectUuid = "test-uuid-123";
    item.m_ifaceName = "eth0";
    item.m_connectPath = "/org/freedesktop/NetworkManager/ActiveConnection/1";

    // dumpInfo should not crash
    item.dumpInfo();
    // If we get here, the test passes
    QVERIFY(true);
}

QTEST_MAIN(TestKyConnectItem)
