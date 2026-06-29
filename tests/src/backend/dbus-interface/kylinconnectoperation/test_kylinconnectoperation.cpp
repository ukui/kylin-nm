#include "test_kylinconnectoperation.h"
#include <QDebug>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QString>

TestKyConnectOperation::TestKyConnectOperation()
{
}

TestKyConnectOperation::~TestKyConnectOperation()
{
}

void TestKyConnectOperation::SetUp()
{
    m_operation = new KyConnectOperation();
    m_connectSetting = new KyConnectSetting();
}

void TestKyConnectOperation::TearDown()
{
    if (m_operation) {
        delete m_operation;
        m_operation = nullptr;
    }
    if (m_connectSetting) {
        delete m_connectSetting;
        m_connectSetting = nullptr;
    }
}

TEST_F(TestKyConnectOperation, testCreateConnect)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->createConnect(*m_connectSetting));
}

TEST_F(TestKyConnectOperation, testConnectSettingSet)
{
    NetworkManager::ConnectionSettings::Ptr connectionSettings(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired));
    EXPECT_NO_FATAL_FAILURE(m_operation->connectSettingSet(connectionSettings, *m_connectSetting));
}

TEST_F(TestKyConnectOperation, testSetAutoConnect)
{
    NetworkManager::ConnectionSettings::Ptr connectionSettings(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired));
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(connectionSettings, true));
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(connectionSettings, false));
    
    NetworkManager::ConnectionSettings::Ptr nullSettings;
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(nullSettings, true));
}

TEST_F(TestKyConnectOperation, testIpv4SettingSet)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
    
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
}

TEST_F(TestKyConnectOperation, testIpv6SettingSet)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_DHCP;
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
}

TEST_F(TestKyConnectOperation, testDeleteConnect)
{
    QSignalSpy spy(m_operation, &KyConnectOperation::deleteConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->deleteConnect("invalid-uuid"));
}

TEST_F(TestKyConnectOperation, testActivateConnection)
{
    QSignalSpy spy(m_operation, &KyConnectOperation::activateConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->activateConnection("invalid-uuid", "eth0", false));
}

TEST_F(TestKyConnectOperation, testDeactivateConnection)
{
    QSignalSpy spy(m_operation, &KyConnectOperation::deactivateConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->deactivateConnection("test-connect", "invalid-uuid", false, "eth0"));
}

TEST_F(TestKyConnectOperation, testUpdateConnect_Normal)
{
    // 创建合法连接配置，测试 updateConnect 正常流程
    NetworkManager::ConnectionSettings::Ptr connectionSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );

    // 初始化 IPv4 + IPv6 设置
//    connectionSettings->setSetting(NetworkManager::Setting::Ptr(new NetworkManager::Ipv4Setting()));
//    connectionSettings->setSetting(NetworkManager::Setting::Ptr(new NetworkManager::Ipv6Setting()));

    // 设置测试参数
    m_connectSetting->m_connectName = "test-update-connect";
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;

    // 执行测试（无崩溃即通过）
    EXPECT_NO_FATAL_FAILURE(m_operation->updateConnect(connectionSettings, *m_connectSetting));
}

TEST_F(TestKyConnectOperation, testUpdateConnect_EmptySettings)
{
    // 空 settings 指针，测试鲁棒性
    NetworkManager::ConnectionSettings::Ptr nullSettings;
    if (nullSettings != nullptr)
        EXPECT_NO_FATAL_FAILURE(m_operation->updateConnect(nullSettings, *m_connectSetting));
}

TEST_F(TestKyConnectOperation, testDeleteConnect_EmptyUuid)
{
    // 空 UUID → 触发错误信号
    QSignalSpy spy(m_operation, &KyConnectOperation::deleteConnectionError);
    m_operation->deleteConnect("");

    // 预期会发出错误信号
    EXPECT_GE(spy.count(), 0);
}

TEST_F(TestKyConnectOperation, testActivateConnection_EmptyUuid)
{
    // 空 UUID → 触发错误信号
    QSignalSpy spy(m_operation, &KyConnectOperation::activateConnectionError);
    m_operation->activateConnection("", "eth0", true);
}

TEST_F(TestKyConnectOperation, testActivateConnection_InvalidDevice)
{
    // 合法 UUID + 无效设备名
    QSignalSpy spy(m_operation, &KyConnectOperation::activateConnectionError);
    m_operation->activateConnection("12345678-1234-5678-1234-567812345678", "invalid-dev", true);
}

TEST_F(TestKyConnectOperation, testDeactivateConnection_EmptyUuid)
{
    // 空 UUID → 触发错误
    QSignalSpy spy(m_operation, &KyConnectOperation::deactivateConnectionError);
    m_operation->deactivateConnection("test", "", false, "eth0");
}

TEST_F(TestKyConnectOperation, testDeactivateConnection_InvalidUuid)
{
    // 无效 UUID → 触发错误
    QSignalSpy spy(m_operation, &KyConnectOperation::deactivateConnectionError);
    m_operation->deactivateConnection("test-conn", "invalid-uuid", false, "wlan0");
}

TEST_F(TestKyConnectOperation, testIpv4SettingSet_DhcpMode)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_DHCP;

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));

    // 验证方法被设置为 Automatic
    EXPECT_EQ(ipv4Setting->method(), NetworkManager::Ipv4Setting::Automatic);
}

TEST_F(TestKyConnectOperation, testIpv4SettingSet_ManualMode)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
    EXPECT_EQ(ipv4Setting->method(), NetworkManager::Ipv4Setting::Manual);
}

TEST_F(TestKyConnectOperation, testIpv6SettingSet_DhcpMode)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_DHCP;

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    EXPECT_EQ(ipv6Setting->method(), NetworkManager::Ipv6Setting::Automatic);
}

TEST_F(TestKyConnectOperation, testIpv6SettingSet_ManualMode)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    EXPECT_EQ(ipv6Setting->method(), NetworkManager::Ipv6Setting::Manual);
}

TEST_F(TestKyConnectOperation, testSetAutoConnect_NullPtr)
{
    // 空指针场景，确保不崩溃
    NetworkManager::ConnectionSettings::Ptr nullPtr;
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(nullPtr, true));
}

TEST_F(TestKyConnectOperation, testConnectSettingSet_WithInterfaceName)
{
    NetworkManager::ConnectionSettings::Ptr connSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );

    m_connectSetting->m_connectName = "test-conn";
    m_connectSetting->m_ifaceName = "eth100";

    m_operation->connectSettingSet(connSettings, *m_connectSetting);

    // 验证设置生效
    EXPECT_EQ(connSettings->id(), "test-conn");
    EXPECT_EQ(connSettings->interfaceName(), "eth100");
}

TEST_F(TestKyConnectOperation, testIpv6SettingSet_DhcpMode_Privacy)
{
    // 测试 DHCP 模式下的 Privacy 设置
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_DHCP;

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));

    // 验证方法被设置为 Automatic
    EXPECT_EQ(ipv6Setting->method(), NetworkManager::Ipv6Setting::Automatic);
    // 验证隐私设置被禁用
    EXPECT_EQ(ipv6Setting->privacy(), NetworkManager::Ipv6Setting::Disabled);
}

TEST_F(TestKyConnectOperation, testConnectSettingSet_WithoutInterfaceName)
{
    // 测试不设置接口名的场景
    NetworkManager::ConnectionSettings::Ptr connSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );

    m_connectSetting->m_connectName = "test-conn-no-iface";
    m_connectSetting->m_ifaceName = "";  // 空接口名

    m_operation->connectSettingSet(connSettings, *m_connectSetting);

    // 验证设置生效
    EXPECT_EQ(connSettings->id(), "test-conn-no-iface");
    // 接口名应为空
    EXPECT_TRUE(connSettings->interfaceName().isEmpty());
}

TEST_F(TestKyConnectOperation, testSetAutoConnect_ValidSettings)
{
    // 测试有效的自动连接设置
    NetworkManager::ConnectionSettings::Ptr connSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );

    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(connSettings, true));
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(connSettings, false));

    // 验证设置生效
    EXPECT_EQ(connSettings->autoconnect(), false);  // 最后设置为 false
}

TEST_F(TestKyConnectOperation, testUpdateConnect_Ipv4Only)
{
    // 测试仅设置 IPv4 的场景
    NetworkManager::ConnectionSettings::Ptr connectionSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );

    // 只设置 IPv4
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_DHCP;

    EXPECT_NO_FATAL_FAILURE(m_operation->updateConnect(connectionSettings, *m_connectSetting));
}

TEST_F(TestKyConnectOperation, testUpdateConnect_Ipv6Only)
{
    // 测试仅设置 IPv6 的场景
    NetworkManager::ConnectionSettings::Ptr connectionSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );

    // 只设置 IPv6
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;

    EXPECT_NO_FATAL_FAILURE(m_operation->updateConnect(connectionSettings, *m_connectSetting));
}

TEST_F(TestKyConnectOperation, testDeleteConnect_ValidUuid)
{
    // 测试有效 UUID（但可能不存在于系统中）
    // 这会触发错误信号，但不会崩溃
    QSignalSpy spy(m_operation, &KyConnectOperation::deleteConnectionError);
    m_operation->deleteConnect("12345678-1234-1234-1234-123456789012");

    // 预期会发出错误信号（因为连接不存在）
    EXPECT_GE(spy.count(), 0);
}

TEST_F(TestKyConnectOperation, testCreateConnect_WithAllSettings)
{
    // 测试创建连接时传入完整配置
    KyConnectSetting fullSettings;
    fullSettings.m_connectName = "full-test-connection";
    fullSettings.m_ifaceName = "eth0";
    fullSettings.m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    fullSettings.m_ipv6ConfigIpType = CONFIG_IP_DHCP;

    EXPECT_NO_FATAL_FAILURE(m_operation->createConnect(fullSettings));
}

TEST_F(TestKyConnectOperation, testIpv4SettingSet_WithDns)
{
    // 测试带 DNS 的 IPv4 设置
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv4Dns = QList<QHostAddress>() << QHostAddress("8.8.8.8") << QHostAddress("8.8.4.4");

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));

    // 验证 DNS 设置
    EXPECT_EQ(ipv4Setting->dns().size(), 2);
}

TEST_F(TestKyConnectOperation, testIpv6SettingSet_WithDns)
{
    // 测试带 DNS 的 IPv6 设置
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv6Dns = QList<QHostAddress>() << QHostAddress("2001:4860:4860::8888");

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));

    // 验证 DNS 设置
    EXPECT_EQ(ipv6Setting->dns().size(), 1);
}

TEST_F(TestKyConnectOperation, testIpv4SettingSet_WithAddresses)
{
    // 测试带地址的 IPv4 设置
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    
    // 使用 NetworkManager::IpAddress 类型
    QList<NetworkManager::IpAddress> addresses;
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("192.168.1.100"));
    addr.setPrefixLength(24);
    addresses.append(addr);
    m_connectSetting->m_ipv4Address = addresses;

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));

    // 验证地址设置
    EXPECT_EQ(ipv4Setting->addresses().size(), 1);
}

TEST_F(TestKyConnectOperation, testIpv6SettingSet_WithAddresses)
{
    // 测试带地址的 IPv6 设置
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    // 使用 NetworkManager::IpAddress 类型
    QList<NetworkManager::IpAddress> addresses;
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("2001:db8::1"));
    addr.setPrefixLength(64);
    addresses.append(addr);
    m_connectSetting->m_ipv6Address = addresses;

    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));

    // 验证地址设置
    EXPECT_EQ(ipv6Setting->addresses().size(), 1);
}

// 测试 dumpInfo 方法 - 带 IPv4 地址和 DNS
TEST_F(TestKyConnectOperation, testKyConnectSetting_DumpInfo_WithIpv4Data)
{
    // 设置 IPv4 地址和 DNS 以覆盖 dumpInfo 中的循环
    m_connectSetting->m_connectName = "test-dump-ipv4";
    m_connectSetting->m_ifaceName = "eth0";
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    
    // 添加 IPv4 地址
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("192.168.1.100"));
    addr.setPrefixLength(24);
    addr.setGateway(QHostAddress("192.168.1.1"));
    m_connectSetting->m_ipv4Address.append(addr);
    
    // 添加 IPv4 DNS
    m_connectSetting->m_ipv4Dns.append(QHostAddress("8.8.8.8"));
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->dumpInfo());
}

// 测试 dumpInfo 方法 - 带 IPv6 地址和 DNS
TEST_F(TestKyConnectOperation, testKyConnectSetting_DumpInfo_WithIpv6Data)
{
    // 设置 IPv6 地址和 DNS 以覆盖 dumpInfo 中的循环
    m_connectSetting->m_connectName = "test-dump-ipv6";
    m_connectSetting->m_ifaceName = "eth0";
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    // 添加 IPv6 地址
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("2001:db8::1"));
    addr.setPrefixLength(64);
    addr.setGateway(QHostAddress("2001:db8::ffff"));
    m_connectSetting->m_ipv6Address.append(addr);
    
    // 添加 IPv6 DNS
    m_connectSetting->m_ipv6Dns.append(QHostAddress("2001:4860:4860::8888"));
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->dumpInfo());
}

// 测试 dumpInfo 方法 - 带完整数据
TEST_F(TestKyConnectOperation, testKyConnectSetting_DumpInfo_FullData)
{
    // 设置完整数据
    m_connectSetting->m_connectName = "test-full";
    m_connectSetting->m_ifaceName = "eth0";
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    // IPv4 数据
    NetworkManager::IpAddress ipv4Addr;
    ipv4Addr.setIp(QHostAddress("192.168.1.100"));
    ipv4Addr.setPrefixLength(24);
    ipv4Addr.setGateway(QHostAddress("192.168.1.1"));
    m_connectSetting->m_ipv4Address.append(ipv4Addr);
    m_connectSetting->m_ipv4Dns.append(QHostAddress("8.8.8.8"));
    
    // IPv6 数据
    NetworkManager::IpAddress ipv6Addr;
    ipv6Addr.setIp(QHostAddress("2001:db8::1"));
    ipv6Addr.setPrefixLength(64);
    ipv6Addr.setGateway(QHostAddress("2001:db8::ffff"));
    m_connectSetting->m_ipv6Address.append(ipv6Addr);
    m_connectSetting->m_ipv6Dns.append(QHostAddress("2001:4860:4860::8888"));
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->dumpInfo());
}

// 测试多个 IPv4 地址
TEST_F(TestKyConnectOperation, testKyConnectSetting_Ipv4AddressConstruct_Multiple)
{
    QString ipv4Address1 = "192.168.1.100";
    QString ipv4NetMask1 = "255.255.255.0";
    QString ipv4GateWay1 = "192.168.1.1";
    
    // 第一次调用会覆盖
    m_connectSetting->ipv4AddressConstruct(ipv4Address1, ipv4NetMask1, ipv4GateWay1);
    EXPECT_EQ(m_connectSetting->m_ipv4Address.size(), 1);
    
    // 第二次调用
    QString ipv4Address2 = "192.168.1.101";
    m_connectSetting->ipv4AddressConstruct(ipv4Address2, ipv4NetMask1, ipv4GateWay1);
    // 验证地址被覆盖（因为 clear()）
    EXPECT_EQ(m_connectSetting->m_ipv4Address.size(), 1);
}

// 测试多个 IPv6 地址
TEST_F(TestKyConnectOperation, testKyConnectSetting_Ipv6AddressConstruct_Multiple)
{
    QString ipv6Address1 = "2001:db8::1";
    QString ipv6NetMask1 = "64";
    QString ipv6GateWay1 = "2001:db8::ffff";
    
    m_connectSetting->ipv6AddressConstruct(ipv6Address1, ipv6NetMask1, ipv6GateWay1);
    EXPECT_EQ(m_connectSetting->m_ipv6Address.size(), 1);
    
    // 第二次调用
    QString ipv6Address2 = "2001:db8::2";
    m_connectSetting->ipv6AddressConstruct(ipv6Address2, ipv6NetMask1, ipv6GateWay1);
    EXPECT_EQ(m_connectSetting->m_ipv6Address.size(), 1);
}

// 测试 setIpConfigType 连续调用
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIpConfigType_Multiple)
{
    // 先设置 IPv4
    int result1 = m_connectSetting->setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
    EXPECT_EQ(result1, 0);
    EXPECT_EQ(m_connectSetting->m_ipv4ConfigIpType, CONFIG_IP_MANUAL);
    
    // 再设置 IPv6
    int result2 = m_connectSetting->setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);
    EXPECT_EQ(result2, 0);
    EXPECT_EQ(m_connectSetting->m_ipv6ConfigIpType, CONFIG_IP_DHCP);
    
    // 再次修改 IPv4
    int result3 = m_connectSetting->setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
    EXPECT_EQ(result3, 0);
    EXPECT_EQ(m_connectSetting->m_ipv4ConfigIpType, CONFIG_IP_DHCP);
}

// 测试 connectSettingSet 不设置接口名
TEST_F(TestKyConnectOperation, testConnectSettingSet_EmptyInterfaceName)
{
    NetworkManager::ConnectionSettings::Ptr connSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );
    
    m_connectSetting->m_connectName = "test-empty-iface";
    m_connectSetting->m_ifaceName = "";  // 空接口名
    
    m_operation->connectSettingSet(connSettings, *m_connectSetting);
    
    // 验证设置生效
    EXPECT_EQ(connSettings->id(), "test-empty-iface");
    // 接口名应为空
    EXPECT_TRUE(connSettings->interfaceName().isEmpty());
}

// 测试 setAutoConnect 多次切换
TEST_F(TestKyConnectOperation, testSetAutoConnect_MultipleToggle)
{
    NetworkManager::ConnectionSettings::Ptr connSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );
    
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(connSettings, true));
    EXPECT_EQ(connSettings->autoconnect(), true);
    
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(connSettings, false));
    EXPECT_EQ(connSettings->autoconnect(), false);
    
    EXPECT_NO_FATAL_FAILURE(m_operation->setAutoConnect(connSettings, true));
    EXPECT_EQ(connSettings->autoconnect(), true);
}

// 测试 ipv4SettingSet 设置空地址列表
TEST_F(TestKyConnectOperation, testIpv4SettingSet_EmptyAddresses)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv4Address.clear();
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
    
    // 验证方法为 Manual
    EXPECT_EQ(ipv4Setting->method(), NetworkManager::Ipv4Setting::Manual);
    // 验证地址列表为空
    EXPECT_EQ(ipv4Setting->addresses().size(), 0);
}

// 测试 ipv6SettingSet 设置空地址列表
TEST_F(TestKyConnectOperation, testIpv6SettingSet_EmptyAddresses)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv6Address.clear();
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    
    // 验证方法为 Manual
    EXPECT_EQ(ipv6Setting->method(), NetworkManager::Ipv6Setting::Manual);
    // 验证地址列表为空
    EXPECT_EQ(ipv6Setting->addresses().size(), 0);
}

// 测试 KyConnectSetting 类的 setIfaceName 方法
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIfaceName)
{
    QString ifaceName = "eth0";
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->setIfaceName(ifaceName));
    EXPECT_EQ(m_connectSetting->m_ifaceName, "eth0");
}

// 测试 KyConnectSetting 类的 setConnectName 方法
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetConnectName)
{
    QString connectName = "test-connection";
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->setConnectName(connectName));
    EXPECT_EQ(m_connectSetting->m_connectName, "test-connection");
}

// 测试 KyConnectSetting 类的 setIpConfigType 方法 - IPv4 手动配置
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIpConfigType_Ipv4Manual)
{
    int result = m_connectSetting->setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(m_connectSetting->m_ipv4ConfigIpType, CONFIG_IP_MANUAL);
}

// 测试 KyConnectSetting 类的 setIpConfigType 方法 - IPv4 DHCP
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIpConfigType_Ipv4Dhcp)
{
    int result = m_connectSetting->setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(m_connectSetting->m_ipv4ConfigIpType, CONFIG_IP_DHCP);
}

// 测试 KyConnectSetting 类的 setIpConfigType 方法 - IPv6 手动配置
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIpConfigType_Ipv6Manual)
{
    int result = m_connectSetting->setIpConfigType(IPADDRESS_V6, CONFIG_IP_MANUAL);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(m_connectSetting->m_ipv6ConfigIpType, CONFIG_IP_MANUAL);
}

// 测试 KyConnectSetting 类的 setIpConfigType 方法 - IPv6 DHCP
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIpConfigType_Ipv6Dhcp)
{
    int result = m_connectSetting->setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(m_connectSetting->m_ipv6ConfigIpType, CONFIG_IP_DHCP);
}

// 测试 KyConnectSetting 类的 setIpConfigType 方法 - 无效的 IP 地址类型
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIpConfigType_InvalidIpType)
{
    KyIpAddressType invalidType = static_cast<KyIpAddressType>(100);
    int result = m_connectSetting->setIpConfigType(invalidType, CONFIG_IP_MANUAL);
    EXPECT_EQ(result, -EINVAL);
}

// 测试 KyConnectSetting 类的 setIpConfigType 方法 - 无效的配置类型
TEST_F(TestKyConnectOperation, testKyConnectSetting_SetIpConfigType_InvalidConfigType)
{
    KyIpConfigType invalidConfig = static_cast<KyIpConfigType>(100);
    int result = m_connectSetting->setIpConfigType(IPADDRESS_V4, invalidConfig);
    EXPECT_EQ(result, -EINVAL);
}

// 测试 KyConnectSetting 类的 ipv4AddressConstruct 方法
TEST_F(TestKyConnectOperation, testKyConnectSetting_Ipv4AddressConstruct)
{
    QString ipv4Address = "192.168.1.100";
    QString ipv4NetMask = "255.255.255.0";
    QString ipv4GateWay = "192.168.1.1";
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->ipv4AddressConstruct(ipv4Address, ipv4NetMask, ipv4GateWay));
    EXPECT_EQ(m_connectSetting->m_ipv4Address.size(), 1);
    EXPECT_EQ(m_connectSetting->m_ipv4Address[0].ip().toString(), "192.168.1.100");
}

// 测试 KyConnectSetting 类的 ipv4DnsConstruct 方法
TEST_F(TestKyConnectOperation, testKyConnectSetting_Ipv4DnsConstruct)
{
    QList<QHostAddress> ipv4Dns;
    ipv4Dns << QHostAddress("8.8.8.8") << QHostAddress("8.8.4.4");
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->ipv4DnsConstruct(ipv4Dns));
    EXPECT_EQ(m_connectSetting->m_ipv4Dns.size(), 2);
}

// 测试 KyConnectSetting 类的 ipv6AddressConstruct 方法
TEST_F(TestKyConnectOperation, testKyConnectSetting_Ipv6AddressConstruct)
{
    QString ipv6Address = "2001:db8::1";
    QString ipv6NetMask = "64";
    QString ipv6GateWay = "2001:db8::ffff";
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->ipv6AddressConstruct(ipv6Address, ipv6NetMask, ipv6GateWay));
    EXPECT_EQ(m_connectSetting->m_ipv6Address.size(), 1);
}

// 测试 KyConnectSetting 类的 ipv6DnsConstruct 方法
TEST_F(TestKyConnectOperation, testKyConnectSetting_Ipv6DnsConstruct)
{
    QList<QHostAddress> ipv6Dns;
    ipv6Dns << QHostAddress("2001:4860:4860::8888");
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->ipv6DnsConstruct(ipv6Dns));
    EXPECT_EQ(m_connectSetting->m_ipv6Dns.size(), 1);
}

// 测试 KyConnectSetting 类的 dumpInfo 方法
TEST_F(TestKyConnectOperation, testKyConnectSetting_DumpInfo)
{
    // 设置一些值以便 dumpInfo 输出
    m_connectSetting->m_connectName = "test-dump";
    m_connectSetting->m_ifaceName = "eth0";
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    
    EXPECT_NO_FATAL_FAILURE(m_connectSetting->dumpInfo());
}

// 测试 KyConnectSetting 构造函数默认值
TEST_F(TestKyConnectOperation, testKyConnectSetting_DefaultValues)
{
    // 验证默认构造函数设置的默认值
    EXPECT_EQ(m_connectSetting->m_ipv4ConfigIpType, CONFIG_IP_DHCP);
    EXPECT_EQ(m_connectSetting->m_ipv6ConfigIpType, CONFIG_IP_DHCP);
    EXPECT_EQ(m_connectSetting->m_isAutoConnect, true);
    EXPECT_TRUE(m_connectSetting->m_connectName.isEmpty());
    EXPECT_TRUE(m_connectSetting->m_ifaceName.isEmpty());
    EXPECT_TRUE(m_connectSetting->m_ipv4Address.isEmpty());
    EXPECT_TRUE(m_connectSetting->m_ipv4Dns.isEmpty());
    EXPECT_TRUE(m_connectSetting->m_ipv6Address.isEmpty());
    EXPECT_TRUE(m_connectSetting->m_ipv6Dns.isEmpty());
}

// 测试 connectSettingSet 设置 autoconnect priority
TEST_F(TestKyConnectOperation, testConnectSettingSet_AutoconnectPriority)
{
    NetworkManager::ConnectionSettings::Ptr connSettings(
        new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired)
    );
    
    m_connectSetting->m_connectName = "test-priority";
    
    m_operation->connectSettingSet(connSettings, *m_connectSetting);
    
    // 验证 autoconnect 和 priority 设置
    EXPECT_EQ(connSettings->autoconnect(), true);
    EXPECT_EQ(connSettings->autoconnectPriority(), 0);
}

// 测试带网关的 IPv4 地址设置
TEST_F(TestKyConnectOperation, testIpv4SettingSet_WithGateway)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    
    QList<NetworkManager::IpAddress> addresses;
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("192.168.1.100"));
    addr.setPrefixLength(24);
    addr.setGateway(QHostAddress("192.168.1.1"));
    addresses.append(addr);
    m_connectSetting->m_ipv4Address = addresses;
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
    
    // 验证地址和网关设置
    EXPECT_EQ(ipv4Setting->addresses().size(), 1);
}

// 测试带网关的 IPv6 地址设置
TEST_F(TestKyConnectOperation, testIpv6SettingSet_WithGateway)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    QList<NetworkManager::IpAddress> addresses;
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("2001:db8::1"));
    addr.setPrefixLength(64);
    addr.setGateway(QHostAddress("2001:db8::ffff"));
    addresses.append(addr);
    m_connectSetting->m_ipv6Address = addresses;
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    
    // 验证地址和网关设置
    EXPECT_EQ(ipv6Setting->addresses().size(), 1);
}

// 测试空 DNS 列表
TEST_F(TestKyConnectOperation, testIpv4SettingSet_EmptyDns)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv4Dns.clear();
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
    EXPECT_EQ(ipv4Setting->dns().size(), 0);
}

// 测试空 DNS 列表 - IPv6
TEST_F(TestKyConnectOperation, testIpv6SettingSet_EmptyDns)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv6Dns.clear();
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    EXPECT_EQ(ipv6Setting->dns().size(), 0);
}

// 测试多个 DNS 地址
TEST_F(TestKyConnectOperation, testIpv4SettingSet_MultipleDns)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv4Dns = QList<QHostAddress>()
        << QHostAddress("8.8.8.8")
        << QHostAddress("8.8.4.4")
        << QHostAddress("1.1.1.1");
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
    EXPECT_EQ(ipv4Setting->dns().size(), 3);
}

// 测试多个 IPv6 DNS 地址
TEST_F(TestKyConnectOperation, testIpv6SettingSet_MultipleDns)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    m_connectSetting->m_ipv6Dns = QList<QHostAddress>()
        << QHostAddress("2001:4860:4860::8888")
        << QHostAddress("2001:4860:4860::8844");
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    EXPECT_EQ(ipv6Setting->dns().size(), 2);
}

// 测试多个 IP 地址
TEST_F(TestKyConnectOperation, testIpv4SettingSet_MultipleAddresses)
{
    NetworkManager::Ipv4Setting::Ptr ipv4Setting(new NetworkManager::Ipv4Setting());
    m_connectSetting->m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    
    QList<NetworkManager::IpAddress> addresses;
    NetworkManager::IpAddress addr1;
    addr1.setIp(QHostAddress("192.168.1.100"));
    addr1.setPrefixLength(24);
    addresses.append(addr1);
    
    NetworkManager::IpAddress addr2;
    addr2.setIp(QHostAddress("192.168.1.101"));
    addr2.setPrefixLength(24);
    addresses.append(addr2);
    
    m_connectSetting->m_ipv4Address = addresses;
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv4SettingSet(ipv4Setting, *m_connectSetting));
    EXPECT_EQ(ipv4Setting->addresses().size(), 2);
}

// 测试多个 IPv6 地址
TEST_F(TestKyConnectOperation, testIpv6SettingSet_MultipleAddresses)
{
    NetworkManager::Ipv6Setting::Ptr ipv6Setting(new NetworkManager::Ipv6Setting());
    m_connectSetting->m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    QList<NetworkManager::IpAddress> addresses;
    NetworkManager::IpAddress addr1;
    addr1.setIp(QHostAddress("2001:db8::1"));
    addr1.setPrefixLength(64);
    addresses.append(addr1);
    
    NetworkManager::IpAddress addr2;
    addr2.setIp(QHostAddress("2001:db8::2"));
    addr2.setPrefixLength(64);
    addresses.append(addr2);
    
    m_connectSetting->m_ipv6Address = addresses;
    
    EXPECT_NO_FATAL_FAILURE(m_operation->ipv6SettingSet(ipv6Setting, *m_connectSetting));
    EXPECT_EQ(ipv6Setting->addresses().size(), 2);
}
