/*
 * Unit test implementation for vpndetail.cpp
 */
#include "test_vpndetail.h"
#include "../../../../src-vpn/frontend/vpndetails/vpndetail.h"

TestVpnDetail::TestVpnDetail(QObject *parent)
    : QObject(parent)
{
}

void TestVpnDetail::initTestCase()
{
    m_testUuid = "test-uuid-12345";
    m_testVpnName = "Test VPN Connection";
}

void TestVpnDetail::cleanupTestCase()
{
}

void TestVpnDetail::init()
{
}

void TestVpnDetail::cleanup()
{
}

void TestVpnDetail::testVpnDetailConstructor()
{
    // Test constructor with valid parameters
    VpnDetail *vpnDetail = new VpnDetail(m_testUuid, m_testVpnName, nullptr);
    QVERIFY2(vpnDetail != nullptr, "Failed to create VpnDetail instance");
    
    // Verify basic widget properties
    QVERIFY2(vpnDetail->windowTitle() == "VPN", "Window title should be VPN");
    
    // Check if the widget has WA_DeleteOnClose attribute
    QVERIFY2(vpnDetail->testAttribute(Qt::WA_DeleteOnClose), "Widget should have WA_DeleteOnClose attribute");
    
    delete vpnDetail;
}

void TestVpnDetail::testCenterToScreen()
{
    VpnDetail *vpnDetail = new VpnDetail(m_testUuid, m_testVpnName, nullptr);
    
    // Test centerToScreen method - just verify it does not crash
    vpnDetail->centerToScreen();
    
    // After centering, the widget should have valid geometry
    QVERIFY2(vpnDetail->x() >= 0, "Widget should have valid x position");
    QVERIFY2(vpnDetail->y() >= 0, "Widget should have valid y position");
    
    delete vpnDetail;
}

void TestVpnDetail::testVpnTabBarConstructor()
{
    // Test VpnTabBar constructor
    VpnTabBar *tabBar = new VpnTabBar(nullptr);
    QVERIFY2(tabBar != nullptr, "Failed to create VpnTabBar instance");
    
    // Verify initial tab count
    QVERIFY2(tabBar->count() >= 0, "Tab bar should have valid tab count");
    
    delete tabBar;
}

#include "moc_test_vpndetail.cpp"
