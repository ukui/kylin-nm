#include "test_lanlistitem.h"

TestLanListItem::TestLanListItem(QObject *parent)
    : QObject(parent)
{
}

void TestLanListItem::initTestCase()
{
    m_testConnectItem = new KyConnectItem();
    m_testConnectItem->m_connectName = "TestConnection";
    m_testConnectItem->m_connectUuid = "test-uuid-1234";
    m_testConnectItem->m_connectPath = "/org/freedesktop/NetworkManager/ActiveConnection/1";
    m_testConnectItem->m_connectState = NetworkManager::ActiveConnection::State::Activated;
    m_testConnectItem->m_ifaceName = "eth0";
    m_testConnectItem->m_itemType = NetworkManager::ConnectionSettings::ConnectionType::Wired;
}

void TestLanListItem::cleanupTestCase()
{
    if (m_testConnectItem) {
        delete m_testConnectItem;
        m_testConnectItem = nullptr;
    }
}

void TestLanListItem::init()
{
    m_lanListItem = new LanListItem(m_testConnectItem, "eth0");
}

void TestLanListItem::cleanup()
{
    if (m_lanListItem) {
        delete m_lanListItem;
        m_lanListItem = nullptr;
    }
}

void TestLanListItem::test_constructorWithParams()
{
    QVERIFY(m_lanListItem != nullptr);
    QString name = m_lanListItem->getConnectionName();
    QVERIFY(name == "TestConnection");
    QString path = m_lanListItem->getConnectionPath();
    QVERIFY(!path.isEmpty());
}

void TestLanListItem::test_constructorDefault()
{
    LanListItem *defaultItem = new LanListItem();
    QVERIFY(defaultItem != nullptr);
    QString name = defaultItem->getConnectionName();
    QVERIFY(name.isEmpty() || name == tr("Not connected"));
    delete defaultItem;
}

void TestLanListItem::test_updateConnectionState()
{
    m_lanListItem->updateConnectionState(Activated);
    QTest::qWait(50);
    m_lanListItem->updateConnectionState(Deactivated);
    QTest::qWait(50);
    m_lanListItem->updateConnectionState(Activating);
    QTest::qWait(50);
    m_lanListItem->updateConnectionState(Deactivating);
    QTest::qWait(50);
    m_lanListItem->updateConnectionState(UnknownState);
    QTest::qWait(50);
}

void TestLanListItem::test_getConnectionName()
{
    QString name = m_lanListItem->getConnectionName();
    QVERIFY(name == "TestConnection");
}

void TestLanListItem::test_updateConnectionName()
{
    QString newName = "UpdatedConnection";
    m_lanListItem->updateConnectionName(newName);
    QString name = m_lanListItem->getConnectionName();
    QVERIFY(name == newName);
}

void TestLanListItem::test_getConnectionPath()
{
    QString path = m_lanListItem->getConnectionPath();
    QVERIFY(!path.isEmpty());
}

void TestLanListItem::test_updateConnectionPath()
{
    QString newPath = "/org/freedesktop/NetworkManager/ActiveConnection/3";
    m_lanListItem->updateConnectionPath(newPath);
    QString path = m_lanListItem->getConnectionPath();
    QVERIFY(path == newPath);
}

void TestLanListItem::test_updateConnectivityText()
{
    m_lanListItem->updateConnectivityText("Connected");
    QTest::qWait(50);
    m_lanListItem->updateConnectivityText("");
    QTest::qWait(50);
}

void TestLanListItem::test_itemHeight()
{
    int height = m_lanListItem->itemHeight();
    QVERIFY(height >= 0);
}

QTEST_MAIN(TestLanListItem)

#include "test_lanlistitem.moc"
