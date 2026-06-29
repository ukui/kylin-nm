#ifndef TEST_KYENTERPRICESETTINGINFO_H
#define TEST_KYENTERPRICESETTINGINFO_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QList>
#include <QDebug>

class TestAssembleEapMethodTlsSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestAssembleEapMethodPeapSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestAssembleEapMethodTtlsSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestAssembleEapMethodLeapSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestAssembleEapMethodPwdSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestAssembleEapMethodFastSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestModifyEapMethodTlsSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestModifyEapMethodPeapSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestModifyEapMethodTtlsSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestModifyEapMethodLeapSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestModifyEapMethodPwdSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class TestModifyEapMethodFastSettings : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

#endif // TEST_KYENTERPRICESETTINGINFO_H
