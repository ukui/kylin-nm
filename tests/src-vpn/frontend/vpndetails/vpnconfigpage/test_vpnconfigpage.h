#ifndef TEST_VPNCONFIGPAGE_H
#define TEST_VPNCONFIGPAGE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include "vpnconfigpage.h"

class TestVpnConfigPage : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnConfigPage(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testGetVpnType();
    void testSetVpnConfigInfo();
    void testCheckIsChanged();
    void testUpdateVpnConfigInfo();

private:
    VpnConfigPage *m_vpnConfigPage;
};

#endif // TEST_VPNCONFIGPAGE_H
