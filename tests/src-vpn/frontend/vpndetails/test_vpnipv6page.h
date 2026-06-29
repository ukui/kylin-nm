#ifndef TEST_VPNIPV6PAGE_H
#define TEST_VPNIPV6PAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class VpnIpv6Page;

class TestVpnIpv6Page : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnIpv6Page(QObject *parent = nullptr);
    ~TestVpnIpv6Page();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_setVpnIpv6Info_autoConfig();
    void test_setVpnIpv6Info_manualConfig();
    void test_setVpnIpv6Info_emptyAddress();
    void test_setDns();
    void test_setSearchDomain();
    void test_checkIsChanged_autoConfigNoChange();
    void test_checkIsChanged_autoConfigChanged();
    void test_checkIsChanged_manualConfigNoChange();
    void test_checkIsChanged_manualConfigChanged();
    void test_checkIsChanged_dnsChanged();
    void test_checkIsChanged_searchDomainChanged();
    void test_updateVpnIpv6Info_autoConfig();
    void test_updateVpnIpv6Info_manualConfig();
    void test_updateVpnIpv6Info_withDns();
    void test_signals_setIpv6PageState();

private:
    VpnIpv6Page *m_vpnIpv6Page;
};

#endif // TEST_VPNIPV6PAGE_H
