#ifndef TEST_KYLINNETWORKDEVICERESOURCE_H
#define TEST_KYLINNETWORKDEVICERESOURCE_H

#include <gtest/gtest.h>
#include <QString>
#include <QStringList>
#include <QSignalSpy>

class TestKyNetworkDeviceResourse : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetNetworkDeviceList : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetHardwareInfo : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetDeviceActiveAPInfo : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetWirelessDeviceCapability : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetDeviceState : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetActiveConnectionInfo : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestWiredDeviceIsCarriered : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestWirelessDeviceIsExist : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestCheckDeviceType : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestSetDeviceRefreshRate : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestSetDeviceManaged : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetDeviceManaged : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetDeviceRxRefreshRate : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetDeviceTxRefreshRate : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetDeviceConnectivity : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestOnDeviceAdd : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestOnDeviceRemove : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestOnDeviceUpdate : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

#endif // TEST_KYLINNETWORKDEVICERESOURCE_H
