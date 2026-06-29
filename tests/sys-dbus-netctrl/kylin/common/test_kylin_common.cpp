#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QDBusReply>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDebug>

#include "../../../../sys-dbus-netctrl/kylin/common/kylin_common.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

class MockQDBusInterface : public QDBusInterface {
public:
    MockQDBusInterface(const QString &service, const QString &path,
                       const QString &interface, QObject *parent = nullptr)
        : QDBusInterface(service, path, interface, QDBusConnection::systemBus(), parent) {}

    // 严格匹配 Qt 基类虚函数：const + QDBusMessage 返回值
    MOCK_METHOD(QDBusMessage, call,
                (const QString &method),
                (const, override));
    MOCK_METHOD(QDBusMessage, call,
                (const QString &method, const QVariant &arg1),
                (const, override));
    MOCK_METHOD(QDBusMessage, call,
                (const QString &method, const QVariant &arg1, const QVariant &arg2),
                (const, override));

    MOCK_METHOD(QVariant, property, (const char *name), (const, override));
};

class KylinCommonTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(KylinCommonTest, CheckIpAddressValid_ValidIPv4) {
    EXPECT_TRUE(check_ipaddress_valid("192.168.1.1"));
    EXPECT_TRUE(check_ipaddress_valid("10.0.0.1"));
    EXPECT_TRUE(check_ipaddress_valid("255.255.255.255"));
    EXPECT_TRUE(check_ipaddress_valid("0.0.0.0"));
    EXPECT_TRUE(check_ipaddress_valid("172.16.0.1"));
}

TEST_F(KylinCommonTest, CheckIpAddressValid_ValidIPv6) {
    EXPECT_TRUE(check_ipaddress_valid("::1"));
    EXPECT_TRUE(check_ipaddress_valid("fe80::1"));
    EXPECT_TRUE(check_ipaddress_valid("2001:db8::1"));
    EXPECT_TRUE(check_ipaddress_valid("::ffff:192.168.1.1"));
    EXPECT_TRUE(check_ipaddress_valid("fe80:0000:0000:0000:0000:0000:0000:0001"));
}

TEST_F(KylinCommonTest, CheckIpAddressValid_InvalidIP) {
    EXPECT_FALSE(check_ipaddress_valid(""));
    EXPECT_FALSE(check_ipaddress_valid("256.1.1.1"));
    EXPECT_FALSE(check_ipaddress_valid("192.168.1"));
    EXPECT_FALSE(check_ipaddress_valid("192.168.1.1.1"));
    EXPECT_FALSE(check_ipaddress_valid("abc.def.ghi.jkl"));
    EXPECT_FALSE(check_ipaddress_valid("192.168.1.256"));
    EXPECT_FALSE(check_ipaddress_valid("192.168.1.-1"));
    EXPECT_FALSE(check_ipaddress_valid("192.168.1.1.1"));
    EXPECT_FALSE(check_ipaddress_valid("192.168.1"));
    EXPECT_FALSE(check_ipaddress_valid("aaaa::zzzz"));
}

TEST_F(KylinCommonTest, GetVpnGatewayFromRoutes_ValidRoute) {
    QString route = "10.8.0.1/32 via 10.8.0.2 dev tun0";
    EXPECT_EQ(get_vpn_gateway_from_routes(route), "10.8.0.1");

    route = "192.168.100.0/24 via 192.168.1.1 dev eth0";
    EXPECT_EQ(get_vpn_gateway_from_routes(route), "192.168.100.0");

    route = "0.0.0.0/0 via 192.168.1.1";
    EXPECT_EQ(get_vpn_gateway_from_routes(route), "0.0.0.0");

    route = "10.0.0.0/8 via 10.0.0.1";
    EXPECT_EQ(get_vpn_gateway_from_routes(route), "10.0.0.0");
}

TEST_F(KylinCommonTest, GetVpnGatewayFromRoutes_InvalidRoute) {
    EXPECT_EQ(get_vpn_gateway_from_routes(""), "");
    EXPECT_EQ(get_vpn_gateway_from_routes("invalid route"), "");
    EXPECT_EQ(get_vpn_gateway_from_routes("no via here"), "");
    EXPECT_EQ(get_vpn_gateway_from_routes("10.0.0.1"), "");
}

TEST_F(KylinCommonTest, GetWifiConnectedSsid_ValidPath) {
    QString path = "/org/freedesktop/NetworkManager/ActiveConnection/1";
    QString expectedSsid = "TestWiFi";

    MockQDBusInterface *mockInterface = new MockQDBusInterface(
        "org.freedesktop.NetworkManager",
        path,
        "org.freedesktop.NetworkManager.Connection.Active"
    );

    EXPECT_CALL(*mockInterface, call(StrEq("Get")))
        .WillOnce(Return(QDBusReply<QString>(expectedSsid)));

    QString result = get_wifi_connected_ssid(path);
    EXPECT_EQ(result, expectedSsid);

    delete mockInterface;
}

TEST_F(KylinCommonTest, GetWifiConnectedSsid_InvalidPath) {
    EXPECT_EQ(get_wifi_connected_ssid(""), "");
    EXPECT_EQ(get_wifi_connected_ssid("/invalid/path"), "");
}

TEST_F(KylinCommonTest, KylinNmGetDbusPid_ValidPid) {
    QString path = "/org/freedesktop/NetworkManager/ActiveConnection/1";

    MockQDBusInterface *mockInterface = new MockQDBusInterface(
        "org.freedesktop.NetworkManager",
        path,
        "org.freedesktop.NetworkManager.Connection.Active"
    );

    EXPECT_CALL(*mockInterface, call(StrEq("Get")))
        .WillOnce(Return(QDBusReply<QString>("12345")));

    int pid = kylin_nm_get_dbus_pid(*mockInterface);
    EXPECT_EQ(pid, 12345);

    delete mockInterface;
}

TEST_F(KylinCommonTest, KylinNmGetDbusPid_InvalidPid) {
    QString path = "/org/freedesktop/NetworkManager/ActiveConnection/999";

    MockQDBusInterface *mockInterface = new MockQDBusInterface(
        "org.freedesktop.NetworkManager",
        path,
        "org.freedesktop.NetworkManager.Connection.Active"
    );

    EXPECT_CALL(*mockInterface, call(StrEq("Get")))
        .WillOnce(Return(QDBusReply<QString>("")));

    int pid = kylin_nm_get_dbus_pid(*mockInterface);
    EXPECT_EQ(pid, -1);

    delete mockInterface;
}

TEST_F(KylinCommonTest, GetDeviceDriverVersion_ValidDevice) {
    QString device = "/sys/class/net/eth0";
    QString expectedVersion = "1.2.3";

    QString result = get_device_driver_version(device);
    EXPECT_EQ(result, expectedVersion);
}

TEST_F(KylinCommonTest, GetDeviceDriverVersion_InvalidDevice) {
    EXPECT_EQ(get_device_driver_version(""), "");
    EXPECT_EQ(get_device_driver_version("/invalid/device/path"), "");
}
