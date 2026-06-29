#ifndef TEST_KYLINNETWORKRESOURCEMANAGER_H
#define TEST_KYLINNETWORKRESOURCEMANAGER_H

#include <gtest/gtest.h>
#include <QObject>
#include <QString>

class TestKyNetworkResourceManager : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetInstance : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetActiveConnectList : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetConnectList : public ::testing::Test
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

class TestFindActiveConnection : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestFindDeviceUni : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestFindDeviceInterface : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestFindWifiNetwork : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestIsActiveConnection : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestIsActivatingConnection : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetActiveConnectionState : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetConnectivity : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestNetworkManagerIsInited : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestSetWirelessNetworkEnabled : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetNetworkDevice : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetActiveConnect : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetConnect : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetWifiNetwork : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestEnumToQstring : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

#endif
