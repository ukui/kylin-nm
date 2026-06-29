#ifndef TEST_KYLINCONNECTOPERATION_H
#define TEST_KYLINCONNECTOPERATION_H

#include <gtest/gtest.h>
#include <QObject>
#include <QSignalSpy>
#include "../../../../../src/backend/dbus-interface/kylinconnectoperation.h"
#include "../../../../../src/backend/dbus-interface/kylinconnectsetting.h"

class TestKyConnectOperation : public QObject, public ::testing::Test
{
    Q_OBJECT
public:
    TestKyConnectOperation();
    ~TestKyConnectOperation();

protected:
    virtual void SetUp() override;
    virtual void TearDown() override;

    KyConnectOperation* m_operation;
    KyConnectSetting* m_connectSetting;
};

#endif // TEST_KYLINCONNECTOPERATION_H
