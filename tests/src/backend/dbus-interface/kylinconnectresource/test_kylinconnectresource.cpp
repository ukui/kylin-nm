#include "test_kylinconnectresource.h"
#include <QDebug>

void TestKyConnectResourse::SetUp()
{
    m_connectResource = nullptr;
}

void TestKyConnectResourse::TearDown()
{
    if (m_connectResource) {
        delete m_connectResource;
        m_connectResource = nullptr;
    }
}

TEST_F(TestKyConnectResourse, testConstructor)
{
    m_connectResource = new KyConnectResourse();
    ASSERT_NE(m_connectResource, nullptr);
    EXPECT_NE(m_connectResource, nullptr);
}

TEST_F(TestKyConnectResourse, testGetConnectivity)
{
    m_connectResource = new KyConnectResourse();
    NetworkManager::Connectivity connectivity = NetworkManager::Connectivity(0);
    m_connectResource->getConnectivity(connectivity);
}

TEST_F(TestKyConnectResourse, testGetActiveConnectionState)
{
    m_connectResource = new KyConnectResourse();
    QString uuid = "test-uuid-123";
    NetworkManager::ActiveConnection::State state = m_connectResource->getActiveConnectionState(uuid);
    Q_UNUSED(state);
}

TEST_F(TestKyConnectResourse, testGetInterfaceByUuid)
{
    m_connectResource = new KyConnectResourse();
    QString deviceName = "";
    QString connUuid = "test-uuid-456";
    bool result = m_connectResource->getInterfaceByUuid(deviceName, connUuid);
    EXPECT_FALSE(result);
}

TEST_F(TestKyConnectResourse, testIsVirtualConnection)
{
    m_connectResource = new KyConnectResourse();
    QString uuid = "test-vpn-uuid";
    bool result = m_connectResource->isVirtualConncection(uuid);
    EXPECT_FALSE(result);
}

TEST_F(TestKyConnectResourse, testIsWiredConnection)
{
    m_connectResource = new KyConnectResourse();
    QString uuid = "test-wired-uuid";
    bool result = m_connectResource->isWiredConnection(uuid);
    EXPECT_FALSE(result);
}

TEST_F(TestKyConnectResourse, testIsWirelessConnection)
{
    m_connectResource = new KyConnectResourse();
    QString uuid = "test-wireless-uuid";
    bool result = m_connectResource->isWirelessConnection(uuid);
    EXPECT_FALSE(result);
}

TEST_F(TestKyConnectResourse, testIsPppoeConnection)
{
    m_connectResource = new KyConnectResourse();
    QString uuid = "test-pppoe-uuid";
    bool result = m_connectResource->isPppoeConnection(uuid);
    EXPECT_FALSE(result);
}

TEST_F(TestKyConnectResourse, testIsActivatedConnection)
{
    m_connectResource = new KyConnectResourse();
    QString uuid = "test-active-uuid";
    bool result = m_connectResource->isActivatedConnection(uuid);
    EXPECT_FALSE(result);
}

TEST_F(TestKyConnectResourse, testIsApConnection)
{
    m_connectResource = new KyConnectResourse();
    QString uuid = "test-ap-uuid";
    bool result = m_connectResource->isApConnection(uuid);
    EXPECT_FALSE(result);
}

TEST_F(TestKyConnectResourse, testGetConnectionSetting)
{
    m_connectResource = new KyConnectResourse();
    KyConnectSetting connectSetting;
    m_connectResource->getConnectionSetting("test-uuid-setting", connectSetting);
}

TEST_F(TestKyConnectResourse, testGetApConnectionByUuid)
{
    m_connectResource = new KyConnectResourse();
    KyApConnectItem *item = m_connectResource->getApConnectionByUuid("test-ap-uuid");
    EXPECT_EQ(item, nullptr);
}

TEST_F(TestKyConnectResourse, testGetApConnectionPathByUuid)
{
    m_connectResource = new KyConnectResourse();
    QString path = m_connectResource->getApConnectionPathByUuid("test-ap-path-uuid");
    EXPECT_EQ(path, "");
}

TEST_F(TestKyConnectResourse, testGetVpnConnections)
{
    m_connectResource = new KyConnectResourse();
    QList<KyVpnConnectItem *> vpnList;
    m_connectResource->getVpnConnections(vpnList);
    EXPECT_TRUE(vpnList.isEmpty());
}

TEST_F(TestKyConnectResourse, testGetBluetoothConnections)
{
    m_connectResource = new KyConnectResourse();
    QList<KyBluetoothConnectItem *> btList;
    m_connectResource->getBluetoothConnections(btList);
    EXPECT_TRUE(btList.isEmpty());
}

TEST_F(TestKyConnectResourse, testGetApConnections)
{
    m_connectResource = new KyConnectResourse();
    QList<KyApConnectItem *> apList;
    m_connectResource->getApConnections(apList);
    EXPECT_TRUE(apList.isEmpty());
}

TEST_F(TestKyConnectResourse, testGetVpnAndVirtualConnections)
{
    m_connectResource = new KyConnectResourse();
    QList<KyConnectItem *> connList;
    m_connectResource->getVpnAndVirtualConnections(connList);
    EXPECT_TRUE(connList.isEmpty());
}
