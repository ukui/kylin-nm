#include "test_kylinutil.h"
#include "../../../../../src/backend/dbus-interface/kylinutil.h"

void TestKylinUtil::SetUp()
{
}

void TestKylinUtil::TearDown()
{
}

void TestGetConnectTypeByDbus::SetUp()
{
}

void TestGetConnectTypeByDbus::TearDown()
{
}

TEST_F(TestGetConnectTypeByDbus, EmptyPath)
{
    QString emptyPath = "";
    QString result = getConnectTypeByDbus(emptyPath);
    EXPECT_EQ(result, "");
}

void TestGetSsidFromByteArray::SetUp()
{
}

void TestGetSsidFromByteArray::TearDown()
{
}

TEST_F(TestGetSsidFromByteArray, EmptyByteArray)
{
    QByteArray emptyArray;
    QString result = getSsidFromByteArray(emptyArray);
    EXPECT_EQ(result, "");
}

TEST_F(TestGetSsidFromByteArray, ValidUtf8Ssid)
{
    QByteArray ssid("TestWiFi");
    QString result = getSsidFromByteArray(ssid);
    EXPECT_EQ(result, "TestWiFi");
}

void TestSetWiredEnabledByGDbus::SetUp()
{
}

void TestSetWiredEnabledByGDbus::TearDown()
{
}

TEST_F(TestSetWiredEnabledByGDbus, SetEnabledTrue)
{
    EXPECT_NO_THROW(setWiredEnabledByGDbus(true));
}

TEST_F(TestSetWiredEnabledByGDbus, SetEnabledFalse)
{
    EXPECT_NO_THROW(setWiredEnabledByGDbus(false));
}

void TestGetWiredEnabledByGDbus::SetUp()
{
}

void TestGetWiredEnabledByGDbus::TearDown()
{
}

TEST_F(TestGetWiredEnabledByGDbus, GetWiredEnabled)
{
    bool result = getWiredEnabledByGDbus();
    (void)result;
}

void TestSetDeviceManagedByGDbus::SetUp()
{
}

void TestSetDeviceManagedByGDbus::TearDown()
{
}

TEST_F(TestSetDeviceManagedByGDbus, SetDeviceManagedTrue)
{
    QString dbusPath = "/org/freedesktop/NetworkManager/Devices/0";
    EXPECT_NO_THROW(setDeviceManagedByGDbus(dbusPath, true));
}

TEST_F(TestSetDeviceManagedByGDbus, SetDeviceManagedFalse)
{
    QString dbusPath = "/org/freedesktop/NetworkManager/Devices/0";
    EXPECT_NO_THROW(setDeviceManagedByGDbus(dbusPath, false));
}

void TestGetConnectivityCheckSpareUriByGDbus::SetUp()
{
}

void TestGetConnectivityCheckSpareUriByGDbus::TearDown()
{
}

TEST_F(TestGetConnectivityCheckSpareUriByGDbus, GetConnectivityCheckUri)
{
    QString result = getConnectivityCheckSpareUriByGDbus();
    (void)result;
}

void TestSetConnectivityCheckSpareUriByGDbus::SetUp()
{
}

void TestSetConnectivityCheckSpareUriByGDbus::TearDown()
{
}

TEST_F(TestSetConnectivityCheckSpareUriByGDbus, SetConnectivityCheckUri)
{
    QString uri = "http://www.example.com/connectivity";
    EXPECT_NO_THROW(setConnectivityCheckSpareUriByGDbus(uri));
}
