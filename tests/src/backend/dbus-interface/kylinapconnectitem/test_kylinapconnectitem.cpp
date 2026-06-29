#include "test_kylinapconnectitem.h"
#include "../../../../../src/backend/dbus-interface/kylinapconnectitem.h"

void TestKyApConnectItem::SetUp()
{
    m_apConnectItem = nullptr;
}

void TestKyApConnectItem::TearDown()
{
    if (m_apConnectItem) {
        delete m_apConnectItem;
        m_apConnectItem = nullptr;
    }
}

void TestKyApConnectItemConstructor::SetUp()
{
    m_apConnectItem = nullptr;
}

void TestKyApConnectItemConstructor::TearDown()
{
    if (m_apConnectItem) {
        delete m_apConnectItem;
        m_apConnectItem = nullptr;
    }
}

TEST_F(TestKyApConnectItemConstructor, DefaultConstructor)
{
    m_apConnectItem = new KyApConnectItem();
    EXPECT_NE(m_apConnectItem, nullptr);
    EXPECT_EQ(m_apConnectItem->m_connectName, "");
    EXPECT_EQ(m_apConnectItem->m_connectSsid, "");
    EXPECT_EQ(m_apConnectItem->m_connectUuid, "");
    EXPECT_EQ(m_apConnectItem->m_ifaceName, "");
    EXPECT_EQ(m_apConnectItem->m_password, "");
    EXPECT_EQ(m_apConnectItem->m_band, "");
    EXPECT_EQ(m_apConnectItem->m_isActivated, false);
}

TEST_F(TestKyApConnectItemConstructor, ConstructorWithNullParent)
{
    m_apConnectItem = new KyApConnectItem(nullptr);
    EXPECT_NE(m_apConnectItem, nullptr);
    EXPECT_EQ(m_apConnectItem->m_connectName, "");
    EXPECT_EQ(m_apConnectItem->m_connectSsid, "");
    EXPECT_EQ(m_apConnectItem->m_connectUuid, "");
    EXPECT_EQ(m_apConnectItem->m_ifaceName, "");
    EXPECT_EQ(m_apConnectItem->m_password, "");
    EXPECT_EQ(m_apConnectItem->m_band, "");
    EXPECT_EQ(m_apConnectItem->m_isActivated, false);
}

TEST_F(TestKyApConnectItem, SetMemberVariables)
{
    m_apConnectItem = new KyApConnectItem();
    m_apConnectItem->m_connectName = "TestConnection";
    m_apConnectItem->m_connectSsid = "TestSSID";
    m_apConnectItem->m_connectUuid = "test-uuid-1234";
    m_apConnectItem->m_ifaceName = "wlan0";
    m_apConnectItem->m_password = "testpassword";
    m_apConnectItem->m_band = "2.4GHz";
    m_apConnectItem->m_isActivated = true;

    EXPECT_EQ(m_apConnectItem->m_connectName, "TestConnection");
    EXPECT_EQ(m_apConnectItem->m_connectSsid, "TestSSID");
    EXPECT_EQ(m_apConnectItem->m_connectUuid, "test-uuid-1234");
    EXPECT_EQ(m_apConnectItem->m_ifaceName, "wlan0");
    EXPECT_EQ(m_apConnectItem->m_password, "testpassword");
    EXPECT_EQ(m_apConnectItem->m_band, "2.4GHz");
    EXPECT_EQ(m_apConnectItem->m_isActivated, true);
}

void TestKyApConnectItemDestructor::SetUp()
{}

void TestKyApConnectItemDestructor::TearDown()
{}

TEST_F(TestKyApConnectItemDestructor, DestructorNoCrash)
{
    KyApConnectItem *item = new KyApConnectItem();
    EXPECT_NE(item, nullptr);
    EXPECT_NO_THROW(delete item);
}

TEST_F(TestKyApConnectItemDestructor, DestructorWithDataNoCrash)
{
    KyApConnectItem *item = new KyApConnectItem();
    item->m_connectName = "Test";
    item->m_connectSsid = "TestSSID";
    item->m_password = "password";
    EXPECT_NE(item, nullptr);
    EXPECT_NO_THROW(delete item);
}
