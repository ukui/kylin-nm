#ifndef TEST_DBUS_INTERFACE_H
#define TEST_DBUS_INTERFACE_H

#include <gtest/gtest.h>
#include <QObject>
#include <QString>
#include <QDBusConnection>
#include "dbus_interface.h"

class TestComKylinNetworkInterface : public QObject, public ::testing::Test
{
    Q_OBJECT

public:
    TestComKylinNetworkInterface();
    ~TestComKylinNetworkInterface() override;

protected:
    void SetUp() override;
    void TearDown() override;

    QString m_service;
    QString m_path;
    QDBusConnection m_connection;
    ComKylinNetworkInterface* m_interface;
};

#endif // TEST_DBUS_INTERFACE_H
