#ifndef TEST_KYWIRELESSCONNECTOPERATION_H
#define TEST_KYWIRELESSCONNECTOPERATION_H

#include <gtest/gtest.h>
#include <QObject>
#include <QSignalSpy>
#include "kywirelessconnectoperation.h"

class TestKyWirelessConnectOperation : public QObject, public ::testing::Test
{
    Q_OBJECT
public:
    TestKyWirelessConnectOperation();
    ~TestKyWirelessConnectOperation();

protected:
    virtual void SetUp() override;
    virtual void TearDown() override;

    KyWirelessConnectOperation* m_operation;
    KyWirelessConnectSetting* m_connectSetting;
};

#endif // TEST_KYWIRELESSCONNECTOPERATION_H
