#ifndef TEST_KYLINVPNCONNECTITEM_H
#define TEST_KYLINVPNCONNECTITEM_H

#include <gtest/gtest.h>
#include <QObject>
#include "../../../../../src/backend/dbus-interface/kylinvpnconnectitem.h"

class TestKyVpnConnectItem : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    KyVpnConnectItem *m_item;
};

#endif // TEST_KYLINVPNCONNECTITEM_H
