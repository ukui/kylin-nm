#include "test_netdevice.h"

TestNetDevice::TestNetDevice(QObject *parent)
    : QObject(parent)
    , m_device(nullptr)
    , m_testDevName("eth0")
    , m_testDevType(WIRED_DEVICE)
{
}

TestNetDevice::~TestNetDevice()
{
}

void TestNetDevice::initTestCase()
{
}

void TestNetDevice::cleanupTestCase()
{
}

void TestNetDevice::init()
{
    m_device = new NetDevice(m_testDevName, m_testDevType);
}

void TestNetDevice::cleanup()
{
    delete m_device;
    m_device = nullptr;
}

QMap<QString, QVariant> TestNetDevice::createConnection(const QString &name, const QString &uuid,
                                                          const QString &path, int state, bool configured, int signal)
{
    QMap<QString, QVariant> conn;
    conn.insert("Name", name);
    conn.insert("Uuid", uuid);
    conn.insert("Path", path);
    conn.insert("State", state);
    conn.insert("Configured", configured ? 1 : 0);
    conn.insert("Signal", QString::number(signal));
    return conn;
}

void TestNetDevice::test_constructor()
{
    NetDevice device("wlan0", WIRELESS_DEVICE);
    QString name = device.devName();
    DeviceType type = device.getDevType();
    bool available = device.isAvailable();
    
    QVERIFY(name == "wlan0");
    QVERIFY(type == WIRELESS_DEVICE);
    QVERIFY(available == false);
}

void TestNetDevice::test_destructor()
{
    NetDevice *device = new NetDevice("eth0", WIRED_DEVICE);
    delete device;
    QVERIFY(true);
}

void TestNetDevice::test_isAvailable()
{
    QVERIFY(m_device->isAvailable() == false);
    m_device->setAvailable(true);
    QVERIFY(m_device->isAvailable() == true);
}

void TestNetDevice::test_devName()
{
    QString name = m_device->devName();
    QVERIFY(name == m_testDevName);
}

void TestNetDevice::test_getDevType()
{
    DeviceType type = m_device->getDevType();
    QVERIFY(type == m_testDevType);
}

void TestNetDevice::test_setAvailable()
{
    m_device->setAvailable(true);
    QVERIFY(m_device->isAvailable() == true);
    
    m_device->setAvailable(false);
    QVERIFY(m_device->isAvailable() == false);
}

void TestNetDevice::test_containsConnection()
{
    QMap<QString, QVariant> conn = createConnection("TestConn", "uuid-123", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    QVERIFY(m_device->containsConnection("uuid-123") == true);
    QVERIFY(m_device->containsConnection("nonexistent") == false);
}

void TestNetDevice::test_containsConnectionName()
{
    QMap<QString, QVariant> conn = createConnection("TestConn", "uuid-123", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    QVERIFY(m_device->containsConnectionName("TestConn") == true);
    QVERIFY(m_device->containsConnectionName("NonExistent") == false);
}

void TestNetDevice::test_addConnection()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED, true, 50);
    m_device->addConnection(conn);
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.count() == 1);
}

void TestNetDevice::test_addConnection_activated()
{
    QMap<QString, QVariant> conn1 = createConnection("Conn1", "uuid-1", "/path/1", ACTIVATED);
    QMap<QString, QVariant> conn2 = createConnection("Conn2", "uuid-2", "/path/2", DEACTIVATED);
    
    m_device->addConnection(conn2);
    m_device->addConnection(conn1);
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.count() == 2);
    
    QMap<QString, QVariant> first = conns.at(0).toMap();
    QVERIFY(first.value("Uuid").toString() == "uuid-1");
}

void TestNetDevice::test_addConnection_sorted()
{
    QMap<QString, QVariant> conn1 = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED, true, 80);
    QMap<QString, QVariant> conn2 = createConnection("Conn2", "uuid-2", "/path/2", DEACTIVATED, true, 60);
    QMap<QString, QVariant> conn3 = createConnection("Conn3", "uuid-3", "/path/3", DEACTIVATED, false, 50);
    
    m_device->addConnection(conn1);
    m_device->addConnection(conn2);
    m_device->addConnection(conn3);
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.count() == 3);
    
    QMap<QString, QVariant> first = conns.at(0).toMap();
    QVERIFY(first.value("Uuid").toString() == "uuid-1");
}

void TestNetDevice::test_removeConnection_byMap()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    QVariantList conns1 = m_device->getConnections();
    QVERIFY(conns1.count() == 1);
    
    m_device->removeConnection(conn);
    
    QVariantList conns2 = m_device->getConnections();
    QVERIFY(conns2.count() == 0);
}

void TestNetDevice::test_removeConnection_byPath()
{
    QMap<QString, QVariant> conn1 = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    QMap<QString, QVariant> conn2 = createConnection("Conn2", "uuid-2", "/path/2", DEACTIVATED);
    
    m_device->addConnection(conn1);
    m_device->addConnection(conn2);
    
    m_device->removeConnection("/path/1");
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.count() == 1);
    
    QMap<QString, QVariant> remaining = conns.at(0).toMap();
    QVERIFY(remaining.value("Uuid").toString() == "uuid-2");
}

void TestNetDevice::test_removeConnectionByName()
{
    QMap<QString, QVariant> conn1 = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    QMap<QString, QVariant> conn2 = createConnection("Conn2", "uuid-2", "/path/2", DEACTIVATED);
    
    m_device->addConnection(conn1);
    m_device->addConnection(conn2);
    
    m_device->removeConnectionByName("Conn1");
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.count() == 1);
    
    QMap<QString, QVariant> remaining = conns.at(0).toMap();
    QVERIFY(remaining.value("Name").toString() == "Conn2");
}

void TestNetDevice::test_updateConnection()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    QMap<QString, QVariant> updated = m_device->updateConnection("uuid-1", ACTIVATING);
    
    QVERIFY(updated.value("Uuid").toString() == "uuid-1");
    QVERIFY(updated.value("State").toInt() == ACTIVATING);
}

void TestNetDevice::test_updateConnection_activated()
{
    QMap<QString, QVariant> conn1 = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    QMap<QString, QVariant> conn2 = createConnection("Conn2", "uuid-2", "/path/2", DEACTIVATED);
    
    m_device->addConnection(conn1);
    m_device->addConnection(conn2);
    
    m_device->updateConnection("uuid-1", ACTIVATED);
    
    QVariantList conns = m_device->getConnections();
    QMap<QString, QVariant> first = conns.at(0).toMap();
    QVERIFY(first.value("Uuid").toString() == "uuid-1");
}

void TestNetDevice::test_getConnectionByName()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    QMap<QString, QVariant> result = m_device->getConnectionByName("Conn1");
    QVERIFY(result.value("Uuid").toString() == "uuid-1");
    
    QMap<QString, QVariant> notFound = m_device->getConnectionByName("NonExistent");
    QVERIFY(notFound.isEmpty());
}

void TestNetDevice::test_getConnections()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.count() == 1);
}

void TestNetDevice::test_getConnections_empty()
{
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.isEmpty());
}

void TestNetDevice::test_clearConnections()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    m_device->clearConnections();
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.isEmpty());
}

void TestNetDevice::test_updateConnectivity()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    m_device->addConnection(conn);
    
    QMap<QString, QVariant> updated = m_device->updateConnectivity("uuid-1", DEACTIVATED, 2);
    
    QVERIFY(updated.value("Connectivity").toInt() == 2);
}

void TestNetDevice::test_updateConnectivity_activated()
{
    QMap<QString, QVariant> conn1 = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    QMap<QString, QVariant> conn2 = createConnection("Conn2", "uuid-2", "/path/2", DEACTIVATED);
    
    m_device->addConnection(conn1);
    m_device->addConnection(conn2);
    
    m_device->updateConnectivity("uuid-1", ACTIVATED, 2);
    
    QVariantList conns = m_device->getConnections();
    QMap<QString, QVariant> first = conns.at(0).toMap();
    QVERIFY(first.value("Uuid").toString() == "uuid-1");
    QVERIFY(first.value("Connectivity").toInt() == 2);
}

void TestNetDevice::test_updateConnectionProp()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED, true, 50);
    m_device->addConnection(conn);
    
    QVariantMap props;
    props.insert("Name", QString("Conn1"));
    props.insert("Signal", QString("80"));
    props.insert("Uuid", QString("uuid-1"));
    props.insert("Security", QString("WPA2"));
    props.insert("Configured", 1);
    
    m_device->updateConnectionProp(props);
    
    QMap<QString, QVariant> result = m_device->getConnectionByName("Conn1");
    QVERIFY(result.value("Signal").toString() == "80");
}

void TestNetDevice::test_updateConnectionProp_frequency()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED, true, 50);
    m_device->addConnection(conn);
    
    QVariantMap props;
    props.insert("Name", QString("Conn1"));
    props.insert("frequency", QString("2400"));
    
    m_device->updateConnectionProp(props);
    
    QMap<QString, QVariant> result = m_device->getConnectionByName("Conn1");
    QVERIFY(result.value("frequency").toString() == "2400");
}

void TestNetDevice::test_updateConnectionProp_isMix()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED, true, 50);
    m_device->addConnection(conn);
    
    QVariantMap props;
    props.insert("Name", QString("Conn1"));
    props.insert("isMix", true);
    
    m_device->updateConnectionProp(props);
    
    QMap<QString, QVariant> result = m_device->getConnectionByName("Conn1");
    QVERIFY(result.value("isMix").toBool() == true);
}

void TestNetDevice::test_updateConnectionProp_State()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED, true, 50);
    m_device->addConnection(conn);
    
    QVariantMap props;
    props.insert("Name", QString("Conn1"));
    props.insert("State", ACTIVATING);
    
    m_device->updateConnectionProp(props);
    
    QMap<QString, QVariant> result = m_device->getConnectionByName("Conn1");
    QVERIFY(result.value("State").toInt() == ACTIVATING);
}

void TestNetDevice::test_updateConnectionProp_autoConnect()
{
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED, true, 50);
    m_device->addConnection(conn);
    
    QVariantMap props;
    props.insert("Name", QString("Conn1"));
    props.insert("autoConnect", false);
    
    m_device->updateConnectionProp(props);
    
    QMap<QString, QVariant> result = m_device->getConnectionByName("Conn1");
    QVERIFY(result.value("autoConnect").toBool() == false);
}

void TestNetDevice::test_updateConnectionProp_notFound()
{
    // Test updating a non-existent connection
    QVariantMap props;
    props.insert("Name", QString("NonExistent"));
    props.insert("Signal", QString("80"));
    
    m_device->updateConnectionProp(props);
    
    // Should not crash, just do nothing
    QVERIFY(true);
}

void TestNetDevice::test_addConnection_multipleActivated()
{
    // Test with multiple activated connections to cover the insertIndex++ branch
    QMap<QString, QVariant> conn1 = createConnection("Conn1", "uuid-1", "/path/1", ACTIVATED, true, 80);
    QMap<QString, QVariant> conn2 = createConnection("Conn2", "uuid-2", "/path/2", ACTIVATED, true, 60);
    QMap<QString, QVariant> conn3 = createConnection("Conn3", "uuid-3", "/path/3", ACTIVATED, true, 70);
    
    m_device->addConnection(conn1);
    m_device->addConnection(conn2);
    m_device->addConnection(conn3);
    
    QVariantList conns = m_device->getConnections();
    QVERIFY(conns.count() == 3);
}

void TestNetDevice::test_containsConnection_wiredDevice()
{
    // Test containsConnection for WIRED_DEVICE (should check Path)
    NetDevice device("eth0", WIRED_DEVICE);
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    device.addConnection(conn);
    
    // Test by Uuid
    QVERIFY(device.containsConnection("uuid-1") == true);
    // Test by Path (wired device)
    QVERIFY(device.containsConnection("/path/1") == true);
    QVERIFY(device.containsConnection("nonexistent") == false);
}

void TestNetDevice::test_containsConnection_wirelessDevice()
{
    // Test containsConnection for WIRELESS_DEVICE (should NOT check Path)
    NetDevice device("wlan0", WIRELESS_DEVICE);
    QMap<QString, QVariant> conn = createConnection("Conn1", "uuid-1", "/path/1", DEACTIVATED);
    device.addConnection(conn);
    
    // Test by Uuid
    QVERIFY(device.containsConnection("uuid-1") == true);
    // For wireless device, Path should NOT be checked
    QVERIFY(device.containsConnection("/path/1") == false);
    QVERIFY(device.containsConnection("nonexistent") == false);
}

void TestNetDevice::test_updateConnection_notFound()
{
    // Test updating a non-existent connection
    QMap<QString, QVariant> result = m_device->updateConnection("non-existent-uuid", ACTIVATED);
    QVERIFY(result.isEmpty());
}

void TestNetDevice::test_updateConnectivity_notFound()
{
    // Test updating connectivity for a non-existent connection
    QMap<QString, QVariant> result = m_device->updateConnectivity("non-existent-uuid", DEACTIVATED, 3);
    QVERIFY(result.isEmpty());
}
