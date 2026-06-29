#ifndef TEST_KYLINAPCONNECTITEM_H
#define TEST_KYLINAPCONNECTITEM_H

#include <gtest/gtest.h>
#include <QObject>
#include <QString>

class KyApConnectItem;

class TestKyApConnectItem : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    KyApConnectItem *m_apConnectItem;
};

class TestKyApConnectItemConstructor : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    KyApConnectItem *m_apConnectItem;
};

class TestKyApConnectItemDestructor : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

#endif // TEST_KYLINAPCONNECTITEM_H
