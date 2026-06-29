#ifndef TEST_KYLINCONNECTRESOURCE_H
#define TEST_KYLINCONNECTRESOURCE_H

#include <gtest/gtest.h>
#include <QCoreApplication>
#include "../../../../../../src/backend/dbus-interface/kylinconnectresource.h"

class TestKyConnectResourse : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    KyConnectResourse *m_connectResource;
};

#endif
