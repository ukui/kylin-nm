#ifndef TEST_VPNIPV4PAGE_H
#define TEST_VPNIPV4PAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class VpnIpv4Page;

class TestVpnIpv4Page : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnIpv4Page(QObject *parent = nullptr);
    ~TestVpnIpv4Page();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_setVpnIpv4Info_dhcp();
    void test_setVpnIpv4Info_manual();
    void test_setVpnIpv4Info_empty();
    void test_setDns();
    void test_setSearchDomain();
    void test_setDhcpClientId();
    void test_checkIsChanged_autoConfigNoChange();
    void test_checkIsChanged_autoConfigChanged();
    void test_checkIsChanged_manualConfigNoChange();
    void test_checkIsChanged_manualConfigChanged();
    void test_checkIsChanged_dnsChanged();
    void test_checkIsChanged_searchDomainChanged();
    void test_checkIsChanged_dhcpClientIdChanged();
    void test_updateVpnIpv4Info_autoConfig();
    void test_updateVpnIpv4Info_manualConfig();
    void test_setEnableOfConfirmBtn_signal();
    void test_comboBoxIndexChanged_signal();

private:
    VpnIpv4Page *m_vpnIpv4Page;
};

#endif // TEST_VPNIPV4PAGE_H
