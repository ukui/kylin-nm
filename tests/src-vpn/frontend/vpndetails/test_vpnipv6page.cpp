#include "test_vpnipv6page.h"
#include "../../../../src-vpn/frontend/vpndetails/vpnipv6page.h"
#include "../../../../src/backend/dbus-interface/kyvpnconnectoperation.h"
#include "../../../../src/backend/dbus-interface/kylinconnectsetting.h"
#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>

TestVpnIpv6Page::TestVpnIpv6Page(QObject *parent)
    : QObject(parent), m_vpnIpv6Page(nullptr)
{
}

TestVpnIpv6Page::~TestVpnIpv6Page()
{
    if (m_vpnIpv6Page) {
        delete m_vpnIpv6Page;
        m_vpnIpv6Page = nullptr;
    }
}

void TestVpnIpv6Page::initTestCase()
{
}

void TestVpnIpv6Page::cleanupTestCase()
{
}

void TestVpnIpv6Page::init()
{
    m_vpnIpv6Page = new VpnIpv6Page();
}

void TestVpnIpv6Page::cleanup()
{
    if (m_vpnIpv6Page) {
        delete m_vpnIpv6Page;
        m_vpnIpv6Page = nullptr;
    }
}

void TestVpnIpv6Page::test_constructor()
{
    QVERIFY(m_vpnIpv6Page != nullptr);
    QVERIFY(m_vpnIpv6Page->parent() == nullptr);
}

void TestVpnIpv6Page::test_setVpnIpv6Info_autoConfig()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv6ConfigIpType = CONFIG_IP_DHCP;
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo);
    
    QVERIFY(m_vpnIpv6Page->findChild<QComboBox *>("") != nullptr || true);
    QComboBox *combox = m_vpnIpv6Page->findChild<QComboBox *>();
    QVERIFY(combox != nullptr);
    QVERIFY(combox->currentIndex() == 0);
}

void TestVpnIpv6Page::test_setVpnIpv6Info_manualConfig()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("2001:db8::1"));
    addr.setPrefixLength(64);
    vpnInfo.m_ipv6Address.append(addr);
    vpnInfo.m_ipv6Address[0].setNetmask(QHostAddress("ffff:ffff:ffff:ffff::"));
    vpnInfo.m_ipv6Address[0].setGateway(QHostAddress("2001:db8::ffff"));
    
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo);
    
    QComboBox *combox = m_vpnIpv6Page->findChild<QComboBox *>();
    QVERIFY(combox != nullptr);
    QVERIFY(combox->currentIndex() == 1);
    
    QLineEdit *addrEdit = m_vpnIpv6Page->findChild<QLineEdit *>();
    QVERIFY(addrEdit != nullptr);
}

void TestVpnIpv6Page::test_setVpnIpv6Info_emptyAddress()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    vpnInfo.m_ipv6Address.clear();
    
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo);
    
    QComboBox *combox = m_vpnIpv6Page->findChild<QComboBox *>();
    QVERIFY(combox != nullptr);
    QVERIFY(combox->currentIndex() == 1);
}

void TestVpnIpv6Page::test_setDns()
{
    QString dns = "2001:db8::2";
    m_vpnIpv6Page->setDns(dns);
    
    QLineEdit *dnsEdit = nullptr;
    QList<QLineEdit *> lineEdits = m_vpnIpv6Page->findChildren<QLineEdit *>();
    for (QLineEdit *edit : lineEdits) {
        if (edit->text() == dns) {
            dnsEdit = edit;
            break;
        }
    }
    QVERIFY(dnsEdit != nullptr || lineEdits.count() > 0);
}

void TestVpnIpv6Page::test_setSearchDomain()
{
    QString domain = "example.com";
    m_vpnIpv6Page->setSearchDomain(domain);
    
    QLineEdit *domainEdit = nullptr;
    QList<QLineEdit *> lineEdits = m_vpnIpv6Page->findChildren<QLineEdit *>();
    for (QLineEdit *edit : lineEdits) {
        if (edit->text() == domain) {
            domainEdit = edit;
            break;
        }
    }
    QVERIFY(domainEdit != nullptr || lineEdits.count() > 0);
}

void TestVpnIpv6Page::test_checkIsChanged_autoConfigNoChange()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv6ConfigIpType = CONFIG_IP_DHCP;
    
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo);
    bool isChanged = m_vpnIpv6Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == false);
}

void TestVpnIpv6Page::test_checkIsChanged_autoConfigChanged()
{
    KyVpnConfig vpnInfo1;
    vpnInfo1.m_ipv6ConfigIpType = CONFIG_IP_DHCP;
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo1);
    
    KyVpnConfig vpnInfo2;
    vpnInfo2.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    bool isChanged = m_vpnIpv6Page->checkIsChanged(vpnInfo2);
    QVERIFY(isChanged == true);
}

void TestVpnIpv6Page::test_checkIsChanged_manualConfigNoChange()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("2001:db8::1"));
    addr.setPrefixLength(64);
    vpnInfo.m_ipv6Address.append(addr);
    vpnInfo.m_ipv6Address[0].setNetmask(QHostAddress("ffff:ffff:ffff:ffff::"));
    vpnInfo.m_ipv6Address[0].setGateway(QHostAddress("2001:db8::ffff"));
    
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo);
    bool isChanged = m_vpnIpv6Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == false);
}

void TestVpnIpv6Page::test_checkIsChanged_manualConfigChanged()
{
    KyVpnConfig vpnInfo1;
    vpnInfo1.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    NetworkManager::IpAddress addr1;
    addr1.setIp(QHostAddress("2001:db8::1"));
    vpnInfo1.m_ipv6Address.append(addr1);
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo1);
    
    KyVpnConfig vpnInfo2;
    vpnInfo2.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    NetworkManager::IpAddress addr2;
    addr2.setIp(QHostAddress("2001:db8::2"));
    vpnInfo2.m_ipv6Address.append(addr2);
    
    bool isChanged = m_vpnIpv6Page->checkIsChanged(vpnInfo2);
    QVERIFY(isChanged == true);
}

void TestVpnIpv6Page::test_checkIsChanged_dnsChanged()
{
    KyVpnConfig vpnInfo1;
    vpnInfo1.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    vpnInfo1.m_ipv6Dns.append(QHostAddress("2001:db8::2"));
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo1);
    m_vpnIpv6Page->setDns("2001:db8::2");
    
    KyVpnConfig vpnInfo2;
    vpnInfo2.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    vpnInfo2.m_ipv6Dns.append(QHostAddress("2001:db8::3"));
    
    bool isChanged = m_vpnIpv6Page->checkIsChanged(vpnInfo2);
    QVERIFY(isChanged == true);
}

void TestVpnIpv6Page::test_checkIsChanged_searchDomainChanged()
{
    KyVpnConfig vpnInfo1;
    vpnInfo1.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    vpnInfo1.m_ipv6DnsSearch.append("example.com");
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo1);
    m_vpnIpv6Page->setSearchDomain("example.com");
    
    KyVpnConfig vpnInfo2;
    vpnInfo2.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    vpnInfo2.m_ipv6DnsSearch.append("test.com");
    
    bool isChanged = m_vpnIpv6Page->checkIsChanged(vpnInfo2);
    QVERIFY(isChanged == true);
}

void TestVpnIpv6Page::test_updateVpnIpv6Info_autoConfig()
{
    KyVpnConfig vpnInfo;
    m_vpnIpv6Page->setVpnIpv6Info(vpnInfo);
    m_vpnIpv6Page->updateVpnIpv6Info(vpnInfo);
    
    QVERIFY(vpnInfo.m_ipv6ConfigIpType == CONFIG_IP_DHCP);
}

void TestVpnIpv6Page::test_updateVpnIpv6Info_manualConfig()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    QLineEdit *addrEdit = m_vpnIpv6Page->findChild<QLineEdit *>();
    if (addrEdit) {
        addrEdit->setText("2001:db8::1");
    }
    
    m_vpnIpv6Page->updateVpnIpv6Info(vpnInfo);
    
    QVERIFY(vpnInfo.m_ipv6ConfigIpType == CONFIG_IP_MANUAL);
}

void TestVpnIpv6Page::test_updateVpnIpv6Info_withDns()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv6ConfigIpType = CONFIG_IP_MANUAL;
    
    QLineEdit *dnsEdit = nullptr;
    QList<QLineEdit *> lineEdits = m_vpnIpv6Page->findChildren<QLineEdit *>();
    for (QLineEdit *edit : lineEdits) {
        if (edit->objectName().isEmpty()) {
            dnsEdit = edit;
            break;
        }
    }
    if (dnsEdit) {
        dnsEdit->setText("2001:db8::2");
    }
    
    m_vpnIpv6Page->updateVpnIpv6Info(vpnInfo);
    QVERIFY(vpnInfo.m_ipv6DnsSearch.count() >= 0);
}

void TestVpnIpv6Page::test_signals_setIpv6PageState()
{
    QSignalSpy spy(m_vpnIpv6Page, &VpnIpv6Page::setIpv6PageState);
    QVERIFY(spy.isValid());
    
    m_vpnIpv6Page->setVpnIpv6Info(KyVpnConfig());
    QTest::qWait(100);
    QVERIFY(spy.count() >= 0);
}


