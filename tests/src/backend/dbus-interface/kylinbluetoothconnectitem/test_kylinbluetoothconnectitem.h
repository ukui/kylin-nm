#ifndef TEST_KYLINBLUETOOTHCONNECTITEM_H
#define TEST_KYLINBLUETOOTHCONNECTITEM_H

#include <QObject>
#include <QString>
#include "kylinbluetoothconnectitem.h"

class TestKylinBluetoothConnectItem : public QObject
{
    Q_OBJECT
public:
    TestKylinBluetoothConnectItem(QObject *parent = nullptr);
    ~TestKylinBluetoothConnectItem();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test constructor and destructor
    void test_constructor();
    void test_destructor();

    // Test public members
    void test_memberVariables();
    void test_dumpInfo();
};

#endif // TEST_KYLINBLUETOOTHCONNECTITEM_H