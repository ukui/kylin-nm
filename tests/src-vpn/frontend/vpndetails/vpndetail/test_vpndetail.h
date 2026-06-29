/*
 * Unit test for vpndetail.cpp
 */
#ifndef TEST_VPNDETAIL_H
#define TEST_VPNDETAIL_H

#include <QObject>
#include <QString>
#include <QTest>
#include <QSignalSpy>
#include <QWidget>

class TestVpnDetail : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnDetail(QObject *parent = nullptr);
    ~TestVpnDetail() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test VpnDetail class public methods
    void testVpnDetailConstructor();
    void testCenterToScreen();

    // Test VpnTabBar class public methods
    void testVpnTabBarConstructor();

private:
    QString m_testUuid;
    QString m_testVpnName;
};

#endif // TEST_VPNDETAIL_H
