#ifndef TEST_KYLINSECRETAGENT_H
#define TEST_KYLINSECRETAGENT_H

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusMessage>
#include "../../../../../src/backend/dbus-interface/kylinsecretagent.h"

class TestKylinSecretAgent : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    KylinSecretAgent *m_secretAgent;
};

#endif
