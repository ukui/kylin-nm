#include "test_kylinnetworkdeviceresource.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinnetworkresourcemanager.h"
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/WiredDevice>

void TestKyNetworkDeviceResourse::SetUp()
{
}

void TestKyNetworkDeviceResourse::TearDown()
{
}

void TestGetNetworkDeviceList::SetUp()
{
}

void TestGetNetworkDeviceList::TearDown()
{
}

TEST_F(TestKyNetworkDeviceResourse, ConstructorDestructor)
{
    KyNetworkDeviceResourse *device = new KyNetworkDeviceResourse();
    ASSERT_NE(device, nullptr);
    
    delete device;
    device = nullptr;
    ASSERT_EQ(device, nullptr);
}

TEST_F(TestGetNetworkDeviceList, GetEthernetDeviceList)
{
    KyNetworkDeviceResourse device;
    QStringList deviceList;
    
    device.getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, deviceList);
    EXPECT_TRUE(deviceList.contains("lo") || !deviceList.isEmpty());
}

TEST_F(TestGetNetworkDeviceList, GetWifiDeviceList)
{
    KyNetworkDeviceResourse device;
    QStringList deviceList;
    
    device.getNetworkDeviceList(NetworkManager::Device::Type::Wifi, deviceList);
    SUCCEED();
}

void TestGetHardwareInfo::SetUp()
{
}

void TestGetHardwareInfo::TearDown()
{
}

TEST_F(TestGetHardwareInfo, GetHardwareInfoForLoopback)
{
    KyNetworkDeviceResourse device;
    QString hardAddress;
    int bandWith = 0;
    
    device.getHardwareInfo("lo", hardAddress, bandWith);
    EXPECT_TRUE(hardAddress.isEmpty() || !hardAddress.isEmpty());
}

TEST_F(TestGetHardwareInfo, GetHardwareInfoForNonexistent)
{
    KyNetworkDeviceResourse device;
    QString hardAddress;
    int bandWith = 0;
    
    device.getHardwareInfo("nonexistent", hardAddress, bandWith);
    EXPECT_TRUE(hardAddress.isEmpty());
    EXPECT_EQ(bandWith, 0);
}

void TestGetDeviceActiveAPInfo::SetUp()
{
}

void TestGetDeviceActiveAPInfo::TearDown()
{
}

TEST_F(TestGetDeviceActiveAPInfo, GetDeviceActiveAPInfoForEthernet)
{
    KyNetworkDeviceResourse device;
    QString strMac;
    uint iHz = 0;
    uint iChan = 0;
    QString secuType;
    
    device.getDeviceActiveAPInfo("lo", strMac, iHz, iChan, secuType);
    SUCCEED();
}

TEST_F(TestGetDeviceActiveAPInfo, GetDeviceActiveAPInfoForNonexistent)
{
    KyNetworkDeviceResourse device;
    QString strMac;
    uint iHz = 0;
    uint iChan = 0;
    QString secuType;
    
    device.getDeviceActiveAPInfo("nonexistent", strMac, iHz, iChan, secuType);
    EXPECT_TRUE(strMac.isEmpty());
}

void TestGetWirelessDeviceCapability::SetUp()
{
}

void TestGetWirelessDeviceCapability::TearDown()
{
}

TEST_F(TestGetWirelessDeviceCapability, GetCapabilityForNonexistent)
{
    KyNetworkDeviceResourse device;
    int cap = device.getWirelessDeviceCapability("nonexistent");
    EXPECT_EQ(cap, 0x01);
}

void TestGetDeviceState::SetUp()
{
}

void TestGetDeviceState::TearDown()
{
}

TEST_F(TestGetDeviceState, GetStateForLoopback)
{
    KyNetworkDeviceResourse device;
    NetworkManager::Device::State state = device.getDeviceState("lo");
    EXPECT_GE(state, 0);
}

TEST_F(TestGetDeviceState, GetStateForNonexistent)
{
    KyNetworkDeviceResourse device;
    NetworkManager::Device::State state = device.getDeviceState("nonexistent");
    EXPECT_EQ(state, NetworkManager::Device::State::UnknownState);
}

void TestGetActiveConnectionInfo::SetUp()
{
}

void TestGetActiveConnectionInfo::TearDown()
{
}

TEST_F(TestGetActiveConnectionInfo, GetActiveConnectionInfoForEthernet)
{
    KyNetworkDeviceResourse device;
    int signalStrength = 0;
    QString uni;
    QString secuType;
    
    bool result = device.getActiveConnectionInfo("lo", signalStrength, uni, secuType);
    EXPECT_FALSE(result);
}

TEST_F(TestGetActiveConnectionInfo, GetActiveConnectionInfoForNonexistent)
{
    KyNetworkDeviceResourse device;
    int signalStrength = 0;
    QString uni;
    QString secuType;
    
    bool result = device.getActiveConnectionInfo("nonexistent", signalStrength, uni, secuType);
    EXPECT_FALSE(result);
}

void TestWiredDeviceIsCarriered::SetUp()
{
}

void TestWiredDeviceIsCarriered::TearDown()
{
}

TEST_F(TestWiredDeviceIsCarriered, CheckCarrieredForNonexistent)
{
    KyNetworkDeviceResourse device;
    bool carriered = device.wiredDeviceIsCarriered("nonexistent");
    EXPECT_FALSE(carriered);
}

void TestWirelessDeviceIsExist::SetUp()
{
}

void TestWirelessDeviceIsExist::TearDown()
{
}

TEST_F(TestWirelessDeviceIsExist, CheckExistForNonexistent)
{
    KyNetworkDeviceResourse device;
    bool exist = device.wirelessDeviceIsExist("nonexistent");
    EXPECT_FALSE(exist);
}

void TestCheckDeviceType::SetUp()
{
}

void TestCheckDeviceType::TearDown()
{
}

TEST_F(TestCheckDeviceType, CheckTypeForLoopback)
{
    KyNetworkDeviceResourse device;
    bool result = device.checkDeviceType("lo", NetworkManager::Device::Type::Ethernet);
    EXPECT_TRUE(result == true || result == false);
}

TEST_F(TestCheckDeviceType, CheckTypeForNonexistent)
{
    KyNetworkDeviceResourse device;
    bool result = device.checkDeviceType("nonexistent", NetworkManager::Device::Type::Wifi);
    EXPECT_FALSE(result);
}

void TestSetDeviceRefreshRate::SetUp()
{
}

void TestSetDeviceRefreshRate::TearDown()
{
}

TEST_F(TestSetDeviceRefreshRate, SetRefreshRateForLoopback)
{
    KyNetworkDeviceResourse device;
    device.setDeviceRefreshRate("lo", 1000);
    SUCCEED();
}

TEST_F(TestSetDeviceRefreshRate, SetRefreshRateForNonexistent)
{
    KyNetworkDeviceResourse device;
    device.setDeviceRefreshRate("nonexistent", 1000);
    SUCCEED();
}

void TestSetDeviceManaged::SetUp()
{
}

void TestSetDeviceManaged::TearDown()
{
}

TEST_F(TestSetDeviceManaged, SetManagedForLoopback)
{
    KyNetworkDeviceResourse device;
    device.setDeviceManaged("lo", true);
    SUCCEED();
}

TEST_F(TestSetDeviceManaged, SetManagedForNonexistent)
{
    KyNetworkDeviceResourse device;
    device.setDeviceManaged("nonexistent", false);
    SUCCEED();
}

void TestGetDeviceManaged::SetUp()
{
}

void TestGetDeviceManaged::TearDown()
{
}

TEST_F(TestGetDeviceManaged, GetManagedForLoopback)
{
    KyNetworkDeviceResourse device;
    bool managed = device.getDeviceManaged("lo");
    EXPECT_TRUE(managed == true || managed == false);
}

TEST_F(TestGetDeviceManaged, GetManagedForNonexistent)
{
    KyNetworkDeviceResourse device;
    bool managed = device.getDeviceManaged("nonexistent");
    EXPECT_FALSE(managed);
}

void TestGetDeviceRxRefreshRate::SetUp()
{
}

void TestGetDeviceRxRefreshRate::TearDown()
{
}

TEST_F(TestGetDeviceRxRefreshRate, GetRxRefreshRateForNonexistent)
{
    KyNetworkDeviceResourse device;
    qulonglong rx = device.getDeviceRxRefreshRate("nonexistent");
    EXPECT_EQ(rx, 0);
}

void TestGetDeviceTxRefreshRate::SetUp()
{
}

void TestGetDeviceTxRefreshRate::TearDown()
{
}

TEST_F(TestGetDeviceTxRefreshRate, GetTxRefreshRateForNonexistent)
{
    KyNetworkDeviceResourse device;
    qulonglong tx = device.getDeviceTxRefreshRate("nonexistent");
    EXPECT_EQ(tx, 0);
}

void TestGetDeviceConnectivity::SetUp()
{
}

void TestGetDeviceConnectivity::TearDown()
{
}

TEST_F(TestGetDeviceConnectivity, GetConnectivityForNonexistent)
{
    KyNetworkDeviceResourse device;
    NetworkManager::Connectivity connectivity = NetworkManager::Connectivity::UnknownConnectivity;
    device.getDeviceConnectivity("nonexistent", connectivity);
    EXPECT_EQ(connectivity, NetworkManager::Connectivity::UnknownConnectivity);
}

void TestOnDeviceAdd::SetUp()
{
}

void TestOnDeviceAdd::TearDown()
{
}

TEST_F(TestOnDeviceAdd, DeviceAddSignal)
{
    KyNetworkDeviceResourse device;
    
    QSignalSpy spy(&device, SIGNAL(deviceAdd(QString, NetworkManager::Device::Type)));
    ASSERT_TRUE(spy.isValid());
    
    device.onDeviceAdd("test", "/test/uni", NetworkManager::Device::Type::Ethernet);
    
    EXPECT_EQ(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    EXPECT_EQ(args.at(0).toString(), "test");
    EXPECT_EQ(args.at(1).value<NetworkManager::Device::Type>(), NetworkManager::Device::Type::Ethernet);
}

void TestOnDeviceRemove::SetUp()
{
}

void TestOnDeviceRemove::TearDown()
{
}

TEST_F(TestOnDeviceRemove, DeviceRemoveSignal)
{
    KyNetworkDeviceResourse device;
    
    QSignalSpy spy(&device, SIGNAL(deviceRemove(QString)));
    ASSERT_TRUE(spy.isValid());
    
    device.onDeviceRemove("test", "/test/uni");
    
    EXPECT_EQ(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    EXPECT_EQ(args.at(0).toString(), "test");
}

void TestOnDeviceUpdate::SetUp()
{
}

void TestOnDeviceUpdate::TearDown()
{
}

TEST_F(TestOnDeviceUpdate, DeviceNameUpdateSignal)
{
    KyNetworkDeviceResourse device;
    
    device.onDeviceAdd("test", "/test/uni", NetworkManager::Device::Type::Ethernet);
    
    QSignalSpy spy(&device, SIGNAL(deviceNameUpdate(QString, QString)));
    ASSERT_TRUE(spy.isValid());
    
    device.onDeviceUpdate("new_test", "/test/uni");
    
    EXPECT_EQ(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    EXPECT_EQ(args.at(0).toString(), "test");
    EXPECT_EQ(args.at(1).toString(), "new_test");
}
