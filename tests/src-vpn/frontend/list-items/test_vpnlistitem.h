#ifndef TEST_VPNLISTITEM_H
#define TEST_VPNLISTITEM_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>
#include "vpnlistitem.h"
#include "kylinconnectitem.h"

class TestVpnListItem : public QObject
{
    Q_OBJECT

public:
    TestVpnListItem(QObject *parent = nullptr);
    ~TestVpnListItem();

private:
    KyConnectItem *m_testConnectItem = nullptr;
    VpnListItem *m_vpnListItem = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructorWithParams();
    void test_constructorDefault();
    void test_updateConnectionState();
    void test_getConnectionName();
    void test_updateConnectionName();
    void test_getConnectionPath();
    void test_updateConnectionPath();
    void test_activeConnection();
    void test_setIcon();
    void test_onInfoButtonClicked();
};

#endif
