#ifndef TEST_KYLINUTIL_H
#define TEST_KYLINUTIL_H

#include <gtest/gtest.h>
#include <QString>
#include <QByteArray>

class TestKylinUtil : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetConnectTypeByDbus : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetSsidFromByteArray : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestSetWiredEnabledByGDbus : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetWiredEnabledByGDbus : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestSetDeviceManagedByGDbus : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestGetConnectivityCheckSpareUriByGDbus : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestSetConnectivityCheckSpareUriByGDbus : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

#endif // TEST_KYLINUTIL_H
