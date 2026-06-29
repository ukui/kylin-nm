#include "test_kylinvpnconnectitem.h"

void TestKyVpnConnectItem::SetUp() {
    m_item = new KyVpnConnectItem();
}

void TestKyVpnConnectItem::TearDown() {
    delete m_item;
    m_item = nullptr;
}

TEST_F(TestKyVpnConnectItem, TestConstructor)
{
    EXPECT_NE(m_item, nullptr);
    EXPECT_EQ(m_item->m_vpnName, "");
    EXPECT_EQ(m_item->m_vpnUuid, "");
    EXPECT_EQ(m_item->m_vpnUser, "");
    EXPECT_EQ(m_item->m_vpnGateWay, "");
    EXPECT_EQ(m_item->m_vpnIpv4Address, "");
    EXPECT_EQ(m_item->m_vpnIpv6Address, "");
    EXPECT_EQ(m_item->m_vpnMppe, false);
    EXPECT_EQ(m_item->m_vpnState, NetworkManager::VpnConnection::State::Disconnected);
}

TEST_F(TestKyVpnConnectItem, TestConstructorWithParent)
{
    QObject parent;
    KyVpnConnectItem *itemWithParent = new KyVpnConnectItem(&parent);
    EXPECT_NE(itemWithParent, nullptr);
    EXPECT_EQ(itemWithParent->parent(), &parent);
    delete itemWithParent;
}

TEST_F(TestKyVpnConnectItem, TestDestructor)
{
    KyVpnConnectItem *item = new KyVpnConnectItem();
    EXPECT_NO_THROW(delete item);
}

TEST_F(TestKyVpnConnectItem, TestSetMembers)
{
    m_item->m_vpnName = "TestVPN";
    m_item->m_vpnUuid = "test-uuid-12345";
    m_item->m_vpnUser = "testuser";
    m_item->m_vpnGateWay = "192.168.1.1";
    m_item->m_vpnIpv4Address = "192.168.1.100";
    m_item->m_vpnIpv6Address = "::1";
    m_item->m_vpnMppe = true;
    m_item->m_vpnState = NetworkManager::VpnConnection::State::Activated;

    EXPECT_EQ(m_item->m_vpnName, "TestVPN");
    EXPECT_EQ(m_item->m_vpnUuid, "test-uuid-12345");
    EXPECT_EQ(m_item->m_vpnUser, "testuser");
    EXPECT_EQ(m_item->m_vpnGateWay, "192.168.1.1");
    EXPECT_EQ(m_item->m_vpnIpv4Address, "192.168.1.100");
    EXPECT_EQ(m_item->m_vpnIpv6Address, "::1");
    EXPECT_EQ(m_item->m_vpnMppe, true);
    EXPECT_EQ(m_item->m_vpnState, NetworkManager::VpnConnection::State::Activated);
}
