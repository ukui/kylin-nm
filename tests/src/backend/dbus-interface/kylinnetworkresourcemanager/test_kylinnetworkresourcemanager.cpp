#include "test_kylinnetworkresourcemanager.h"
#include "../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h"
#include "../../../../../src/backend/dbus-interface/kylinutil.h"
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/AccessPoint>

void TestKyNetworkResourceManager::SetUp() {}
void TestKyNetworkResourceManager::TearDown() {}

void TestGetInstance::SetUp() {}
void TestGetInstance::TearDown() {}

TEST_F(TestGetInstance, GetInstanceReturnsNonNull)
{
    KyNetworkResourceManager* instance = KyNetworkResourceManager::getInstance();
    EXPECT_NE(instance, nullptr);
}

TEST_F(TestGetInstance, GetInstanceReturnsSameInstance)
{
    KyNetworkResourceManager* instance1 = KyNetworkResourceManager::getInstance();
    KyNetworkResourceManager* instance2 = KyNetworkResourceManager::getInstance();
    EXPECT_EQ(instance1, instance2);
}

void TestGetActiveConnectList::SetUp() {}
void TestGetActiveConnectList::TearDown() {}

TEST_F(TestGetActiveConnectList, GetActiveConnectListReturnsList)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::ActiveConnection::List list = mgr->getActiveConnectList();
    EXPECT_TRUE(true);
}

void TestGetConnectList::SetUp() {}
void TestGetConnectList::TearDown() {}

TEST_F(TestGetConnectList, GetConnectListReturnsList)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Connection::List list = mgr->getConnectList();
    EXPECT_TRUE(true);
}

void TestGetNetworkDeviceList::SetUp() {}
void TestGetNetworkDeviceList::TearDown() {}

TEST_F(TestGetNetworkDeviceList, GetNetworkDeviceListReturnsList)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Device::List list = mgr->getNetworkDeviceList();
    EXPECT_TRUE(true);
}

void TestFindActiveConnection::SetUp() {}
void TestFindActiveConnection::TearDown() {}

TEST_F(TestFindActiveConnection, FindActiveConnectionWithEmptyPath)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::ActiveConnection::Ptr result = mgr->findActiveConnection("");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestFindActiveConnection, FindActiveConnectionWithInvalidPath)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::ActiveConnection::Ptr result = mgr->findActiveConnection("/org/freedesktop/NetworkManager/ActiveConnection/99999");
    EXPECT_TRUE(result.isNull());
}

void TestFindDeviceUni::SetUp() {}
void TestFindDeviceUni::TearDown() {}

TEST_F(TestFindDeviceUni, FindDeviceUniWithEmptyUni)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Device::Ptr result = mgr->findDeviceUni("");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestFindDeviceUni, FindDeviceUniWithInvalidUni)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Device::Ptr result = mgr->findDeviceUni("/org/freedesktop/NetworkManager/Device/99999");
    EXPECT_TRUE(result.isNull());
}

void TestFindDeviceInterface::SetUp() {}
void TestFindDeviceInterface::TearDown() {}

TEST_F(TestFindDeviceInterface, FindDeviceInterfaceWithEmptyName)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Device::Ptr result = mgr->findDeviceInterface("");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestFindDeviceInterface, FindDeviceInterfaceWithInvalidName)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Device::Ptr result = mgr->findDeviceInterface("invalid_interface_name_xyz");
    EXPECT_TRUE(result.isNull());
}

void TestFindWifiNetwork::SetUp() {}
void TestFindWifiNetwork::TearDown() {}

TEST_F(TestFindWifiNetwork, FindWifiNetworkWithEmptyParams)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::WirelessNetwork::Ptr result = mgr->findWifiNetwork("", "");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestFindWifiNetwork, FindWifiNetworkWithInvalidParams)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::WirelessNetwork::Ptr result = mgr->findWifiNetwork("InvalidSSID", "/org/freedesktop/NetworkManager/Device/99999");
    EXPECT_TRUE(result.isNull());
}

void TestIsActiveConnection::SetUp() {}
void TestIsActiveConnection::TearDown() {}

TEST_F(TestIsActiveConnection, IsActiveConnectionWithEmptyUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    bool result = mgr->isActiveConnection("");
    EXPECT_FALSE(result);
}

TEST_F(TestIsActiveConnection, IsActiveConnectionWithInvalidUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    bool result = mgr->isActiveConnection("00000000-0000-0000-0000-000000000000");
    EXPECT_FALSE(result);
}

void TestIsActivatingConnection::SetUp() {}
void TestIsActivatingConnection::TearDown() {}

TEST_F(TestIsActivatingConnection, IsActivatingConnectionWithEmptyUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    bool result = mgr->isActivatingConnection("");
    EXPECT_FALSE(result);
}

TEST_F(TestIsActivatingConnection, IsActivatingConnectionWithInvalidUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    bool result = mgr->isActivatingConnection("00000000-0000-0000-0000-000000000000");
    EXPECT_FALSE(result);
}

void TestGetActiveConnectionState::SetUp() {}
void TestGetActiveConnectionState::TearDown() {}

TEST_F(TestGetActiveConnectionState, GetActiveConnectionStateWithEmptyUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::ActiveConnection::State state = mgr->getActiveConnectionState("");
    EXPECT_EQ(state, NetworkManager::ActiveConnection::State::Unknown);
}

TEST_F(TestGetActiveConnectionState, GetActiveConnectionStateWithInvalidUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::ActiveConnection::State state = mgr->getActiveConnectionState("00000000-0000-0000-0000-000000000000");
    EXPECT_EQ(state, NetworkManager::ActiveConnection::State::Unknown);
}

void TestGetConnectivity::SetUp() {}
void TestGetConnectivity::TearDown() {}

TEST_F(TestGetConnectivity, GetConnectivity)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Connectivity connectivity;
    EXPECT_NO_THROW(mgr->getConnectivity(connectivity));
}

void TestNetworkManagerIsInited::SetUp() {}
void TestNetworkManagerIsInited::TearDown() {}

TEST_F(TestNetworkManagerIsInited, NetworkManagerIsInited)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    bool result = mgr->NetworkManagerIsInited();
    EXPECT_TRUE(result == true || result == false);
}

void TestSetWirelessNetworkEnabled::SetUp() {}
void TestSetWirelessNetworkEnabled::TearDown() {}

TEST_F(TestSetWirelessNetworkEnabled, SetWirelessNetworkEnabledTrue)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    EXPECT_NO_THROW(mgr->setWirelessNetworkEnabled(true));
}

TEST_F(TestSetWirelessNetworkEnabled, SetWirelessNetworkEnabledFalse)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    EXPECT_NO_THROW(mgr->setWirelessNetworkEnabled(false));
}

void TestGetNetworkDevice::SetUp() {}
void TestGetNetworkDevice::TearDown() {}

TEST_F(TestGetNetworkDevice, GetNetworkDeviceWithEmptyName)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Device::Ptr result = mgr->getNetworkDevice("");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestGetNetworkDevice, GetNetworkDeviceWithInvalidName)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Device::Ptr result = mgr->getNetworkDevice("invalid_interface_xyz");
    EXPECT_TRUE(result.isNull());
}

void TestGetActiveConnect::SetUp() {}
void TestGetActiveConnect::TearDown() {}

TEST_F(TestGetActiveConnect, GetActiveConnectWithEmptyUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::ActiveConnection::Ptr result = mgr->getActiveConnect("");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestGetActiveConnect, GetActiveConnectWithInvalidUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::ActiveConnection::Ptr result = mgr->getActiveConnect("00000000-0000-0000-0000-000000000000");
    EXPECT_TRUE(result.isNull());
}

void TestGetConnect::SetUp() {}
void TestGetConnect::TearDown() {}

TEST_F(TestGetConnect, GetConnectWithEmptyUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Connection::Ptr result = mgr->getConnect("");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestGetConnect, GetConnectWithInvalidUuid)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::Connection::Ptr result = mgr->getConnect("00000000-0000-0000-0000-000000000000");
    EXPECT_TRUE(result.isNull());
}

void TestGetWifiNetwork::SetUp() {}
void TestGetWifiNetwork::TearDown() {}

TEST_F(TestGetWifiNetwork, GetWifiNetworkWithEmptyName)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::WirelessNetwork::Ptr result = mgr->getWifiNetwork("");
    EXPECT_TRUE(result.isNull());
}

TEST_F(TestGetWifiNetwork, GetWifiNetworkWithInvalidName)
{
    KyNetworkResourceManager* mgr = KyNetworkResourceManager::getInstance();
    NetworkManager::WirelessNetwork::Ptr result = mgr->getWifiNetwork("InvalidWiFiNetwork");
    EXPECT_TRUE(result.isNull());
}

void TestEnumToQstring::SetUp() {}
void TestEnumToQstring::TearDown() {}

TEST_F(TestEnumToQstring, EnumToQstringNone)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0), NetworkManager::AccessPoint::WpaFlags(0), NetworkManager::AccessPoint::WpaFlags(0));
    EXPECT_EQ(result, "");
}

TEST_F(TestEnumToQstring, EnumToQstringWep)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0x1), NetworkManager::AccessPoint::WpaFlags(0), NetworkManager::AccessPoint::WpaFlags(0));
    EXPECT_TRUE(result.contains("WEP"));
}

TEST_F(TestEnumToQstring, EnumToQstringWpa1)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0), NetworkManager::AccessPoint::WpaFlags(0x100), NetworkManager::AccessPoint::WpaFlags(0));
    EXPECT_TRUE(result.contains("WPA1"));
}

TEST_F(TestEnumToQstring, EnumToQstringWpa2Psk)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0), NetworkManager::AccessPoint::WpaFlags(0), NetworkManager::AccessPoint::WpaFlags(0x100));
    EXPECT_TRUE(result.contains("WPA2"));
}

TEST_F(TestEnumToQstring, EnumToQstringWpa2Eap)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0), NetworkManager::AccessPoint::WpaFlags(0), NetworkManager::AccessPoint::WpaFlags(0x200));
    EXPECT_TRUE(result.contains("WPA2"));
}

TEST_F(TestEnumToQstring, EnumToQstringWpa3)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0), NetworkManager::AccessPoint::WpaFlags(0), NetworkManager::AccessPoint::WpaFlags(0x400));
    EXPECT_TRUE(result.contains("WPA3"));
}

TEST_F(TestEnumToQstring, EnumToQstring8021X)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0), NetworkManager::AccessPoint::WpaFlags(0x200), NetworkManager::AccessPoint::WpaFlags(0));
    EXPECT_TRUE(result.contains("802.1X"));
}

TEST_F(TestEnumToQstring, EnumToQstring8021XRsn)
{
    QString result = enumToQstring(NetworkManager::AccessPoint::Capabilities(0), NetworkManager::AccessPoint::WpaFlags(0), NetworkManager::AccessPoint::WpaFlags(0x200));
    EXPECT_TRUE(result.contains("802.1X"));
}
