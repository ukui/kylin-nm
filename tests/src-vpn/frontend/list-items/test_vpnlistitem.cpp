#include "test_vpnlistitem.h"

TestVpnListItem::TestVpnListItem(QObject *parent)
    : QObject(parent)
{
}

TestVpnListItem::~TestVpnListItem()
{
}

void TestVpnListItem::initTestCase()
{
    m_testConnectItem = new KyConnectItem();
    m_testConnectItem->m_connectName = "TestVpnConnection";
    m_testConnectItem->m_connectUuid = "test-vpn-uuid-1234";
    m_testConnectItem->m_connectPath = "/org/freedesktop/NetworkManager/ActiveConnection/1";
    m_testConnectItem->m_connectState = NetworkManager::ActiveConnection::State::Activated;
    m_testConnectItem->m_ifaceName = "vpn0";
    m_testConnectItem->m_itemType = NetworkManager::ConnectionSettings::ConnectionType::Vpn;
}

void TestVpnListItem::cleanupTestCase()
{
    if (m_testConnectItem) {
        delete m_testConnectItem;
        m_testConnectItem = nullptr;
    }
}

void TestVpnListItem::init()
{
    m_vpnListItem = new VpnListItem(m_testConnectItem);
}

void TestVpnListItem::cleanup()
{
    if (m_vpnListItem) {
        delete m_vpnListItem;
        m_vpnListItem = nullptr;
    }
}

void TestVpnListItem::test_constructorWithParams()
{
    QVERIFY(m_vpnListItem != nullptr);
    QString name = m_vpnListItem->getConnectionName();
    QVERIFY(name == "TestVpnConnection");
    QString path = m_vpnListItem->getConnectionPath();
    QVERIFY(!path.isEmpty());
}

void TestVpnListItem::test_constructorDefault()
{
    VpnListItem *defaultItem = new VpnListItem();
    QVERIFY(defaultItem != nullptr);
    QString name = defaultItem->getConnectionName();
    QVERIFY(name.isEmpty() || name == tr("Not connected"));
    delete defaultItem;
}

void TestVpnListItem::test_updateConnectionState()
{
    m_vpnListItem->updateConnectionState(Activated);
    QTest::qWait(50);
    m_vpnListItem->updateConnectionState(Deactivated);
    QTest::qWait(50);
    m_vpnListItem->updateConnectionState(Activating);
    QTest::qWait(50);
    m_vpnListItem->updateConnectionState(Deactivating);
    QTest::qWait(50);
    m_vpnListItem->updateConnectionState(UnknownState);
    QTest::qWait(50);
}

void TestVpnListItem::test_getConnectionName()
{
    QString name = m_vpnListItem->getConnectionName();
    QVERIFY(name == "TestVpnConnection");
}

void TestVpnListItem::test_updateConnectionName()
{
    QString newName = "UpdatedVpnConnection";
    m_vpnListItem->updateConnectionName(newName);
    QString name = m_vpnListItem->getConnectionName();
    QVERIFY(name == newName);
}

void TestVpnListItem::test_getConnectionPath()
{
    QString path = m_vpnListItem->getConnectionPath();
    QVERIFY(!path.isEmpty());
}

void TestVpnListItem::test_updateConnectionPath()
{
    QString newPath = "/org/freedesktop/NetworkManager/ActiveConnection/3";
    m_vpnListItem->updateConnectionPath(newPath);
    QString path = m_vpnListItem->getConnectionPath();
    QVERIFY(path == newPath);
}

void TestVpnListItem::test_activeConnection()
{
    m_vpnListItem->updateConnectionState(Deactivated);
    QTest::qWait(50);
    m_vpnListItem->activeConnection();
    QTest::qWait(50);

    m_vpnListItem->updateConnectionState(Activated);
    QTest::qWait(50);
    m_vpnListItem->activeConnection();
    QTest::qWait(50);
}

void TestVpnListItem::test_setIcon()
{
    m_vpnListItem->updateConnectionState(Activated);
    QTest::qWait(50);
    m_vpnListItem->updateConnectionState(Deactivated);
    QTest::qWait(50);
}

void TestVpnListItem::test_onInfoButtonClicked()
{
    m_vpnListItem->onInfoButtonClicked();
    QTest::qWait(50);
}

QTEST_MAIN(TestVpnListItem)

#include "test_vpnlistitem.moc"