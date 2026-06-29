#ifndef TEST_VPNPAGE_H
#define TEST_VPNPAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class VpnPage;

class TestVpnPage : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnPage(QObject *parent = nullptr);
    ~TestVpnPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_vpnPageConstructionAndDestruction();

    void test_getVirtualList_empty();
    void test_getVirtualList_withData();

    void test_deleteVpn();
    void test_deleteVpn_withEmptyUuid();

    void test_activateVpn();
    void test_activateVpn_notInMap();

    void test_deactivateVpn();

    void test_showDetailPage();

    void test_showUI();

    void test_signals_vpnAdd();
    void test_signals_vpnRemove();
    void test_signals_vpnUpdate();
    void test_signals_vpnActiveConnectionStateChanged();
    void test_signals_vpnConnectChanged();

private:
    VpnPage *m_vpnPage;
};

#endif // TEST_VPNPAGE_H
