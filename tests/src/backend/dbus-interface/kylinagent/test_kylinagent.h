#ifndef TEST_KYLINAGENT_H
#define TEST_KYLINAGENT_H

#include <gtest/gtest.h>
#include <QCoreApplication>
#include "../../../../../src/backend/dbus-interface/kylinagent.h"

class TestKylinAgent : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    KylinAgent *m_agent;
};

#endif
