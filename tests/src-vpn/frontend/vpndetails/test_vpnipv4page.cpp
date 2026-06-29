#include "test_vpnipv4page.h"
#include "../../../../src-vpn/frontend/vpndetails/vpnipv4page.h"
#include "../../../../src/backend/dbus-interface/kyvpnconnectoperation.h"
#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

TestVpnIpv4Page::TestVpnIpv4Page(QObject *parent)
    : QObject(parent), m_vpnIpv4Page(nullptr)
{
}

TestVpnIpv4Page::~TestVpnIpv4Page()
{
    if (m_vpnIpv4Page) {
        delete m_vpnIpv4Page;
        m_vpnIpv4Page = nullptr;
    }
}

void TestVpnIpv4Page::initTestCase()
{
}

void TestVpnIpv4Page::cleanupTestCase()
{
}

void TestVpnIpv4Page::init()
{
    m_vpnIpv4Page = new VpnIpv4Page();
}

void TestVpnIpv4Page::cleanup()
{
    if (m_vpnIpv4Page) {
        delete m_vpnIpv4Page;
        m_vpnIpv4Page = nullptr;
    }
}

void TestVpnIpv4Page::test_constructor()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    QVERIFY(m_vpnIpv4Page->parent() == nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    QVERIFY(comboBox->count() == 2);
    
    QLineEdit *addressEdit = m_vpnIpv4Page->findChild<QLineEdit *>();
    QVERIFY(addressEdit != nullptr);
}

void TestVpnIpv4Page::test_setVpnIpv4Info_dhcp()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    vpnInfo.m_ipv4Address.clear();
    vpnInfo.m_ipv4Dns.clear();
    
    m_vpnIpv4Page->setVpnIpv4Info(vpnInfo);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    QVERIFY(comboBox->currentIndex() == 0);
}

void TestVpnIpv4Page::test_setVpnIpv4Info_manual()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("192.168.1.100"));
    addr.setNetmask(QHostAddress("255.255.255.0"));
    addr.setGateway(QHostAddress("192.168.1.1"));
    vpnInfo.m_ipv4Address.append(addr);
    
    m_vpnIpv4Page->setVpnIpv4Info(vpnInfo);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    QVERIFY(comboBox->currentIndex() == 1);
    
    QList<QLineEdit*> edits = m_vpnIpv4Page->findChildren<QLineEdit *>();
    QVERIFY(edits.count() >= 3);
}

void TestVpnIpv4Page::test_setVpnIpv4Info_empty()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    vpnInfo.m_ipv4Address.clear();
    
    m_vpnIpv4Page->setVpnIpv4Info(vpnInfo);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    QVERIFY(comboBox->currentIndex() == 1);
}

void TestVpnIpv4Page::test_setDns()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QString dns = "8.8.8.8";
    m_vpnIpv4Page->setDns(dns);
    
    QList<QLineEdit*> edits = m_vpnIpv4Page->findChildren<QLineEdit *>();
    bool found = false;
    for (QLineEdit *edit : edits) {
        if (edit->text() == dns) {
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestVpnIpv4Page::test_setSearchDomain()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QString domain = "example.com";
    m_vpnIpv4Page->setSearchDomain(domain);
    
    QList<QLineEdit*> edits = m_vpnIpv4Page->findChildren<QLineEdit *>();
    bool found = false;
    for (QLineEdit *edit : edits) {
        if (edit->text() == domain) {
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestVpnIpv4Page::test_setDhcpClientId()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QString dhcpId = "client-id-001";
    m_vpnIpv4Page->setDhcpClientId(dhcpId);
    
    QList<QLineEdit*> edits = m_vpnIpv4Page->findChildren<QLineEdit *>();
    bool found = false;
    for (QLineEdit *edit : edits) {
        if (edit->text() == dhcpId) {
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestVpnIpv4Page::test_checkIsChanged_autoConfigNoChange()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(0);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    vpnInfo.m_ipv4Address.clear();
    vpnInfo.m_ipv4Dns.clear();
    vpnInfo.m_ipv4DnsSearch.clear();
    vpnInfo.m_ipv4DhcpClientId = "";
    
    bool isChanged = m_vpnIpv4Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == false);
}

void TestVpnIpv4Page::test_checkIsChanged_autoConfigChanged()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(0);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    vpnInfo.m_ipv4Address.clear();
    vpnInfo.m_ipv4Dns.clear();
    vpnInfo.m_ipv4DnsSearch.clear();
    vpnInfo.m_ipv4DhcpClientId = "";
    
    bool isChanged = m_vpnIpv4Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == true);
}

void TestVpnIpv4Page::test_checkIsChanged_manualConfigNoChange()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(1);
    
    QList<QLineEdit*> edits = m_vpnIpv4Page->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = nullptr;
    QLineEdit *netmaskEdit = nullptr;
    QLineEdit *gatewayEdit = nullptr;
    
    for (QLineEdit *edit : edits) {
        if (edit->placeholderText() == "Required" || addressEdit == nullptr) {
            if (addressEdit == nullptr) addressEdit = edit;
            else if (netmaskEdit == nullptr) netmaskEdit = edit;
            else if (gatewayEdit == nullptr) gatewayEdit = edit;
        }
    }
    
    if (addressEdit) addressEdit->setText("192.168.1.100");
    if (netmaskEdit) netmaskEdit->setText("255.255.255.0");
    if (gatewayEdit) gatewayEdit->setText("192.168.1.1");
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_MANUAL;
    NetworkManager::IpAddress addr;
    addr.setIp(QHostAddress("192.168.1.100"));
    addr.setNetmask(QHostAddress("255.255.255.0"));
    addr.setGateway(QHostAddress("192.168.1.1"));
    vpnInfo.m_ipv4Address.append(addr);
    vpnInfo.m_ipv4Dns.clear();
    vpnInfo.m_ipv4DnsSearch.clear();
    vpnInfo.m_ipv4DhcpClientId = "";
    
    bool isChanged = m_vpnIpv4Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == false);
}

void TestVpnIpv4Page::test_checkIsChanged_manualConfigChanged()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(1);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    vpnInfo.m_ipv4Address.clear();
    vpnInfo.m_ipv4Dns.clear();
    vpnInfo.m_ipv4DnsSearch.clear();
    vpnInfo.m_ipv4DhcpClientId = "";
    
    bool isChanged = m_vpnIpv4Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == true);
}

void TestVpnIpv4Page::test_checkIsChanged_dnsChanged()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(0);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    vpnInfo.m_ipv4Address.clear();
    vpnInfo.m_ipv4Dns.append(QHostAddress("8.8.8.8"));
    vpnInfo.m_ipv4DnsSearch.clear();
    vpnInfo.m_ipv4DhcpClientId = "";
    
    bool isChanged = m_vpnIpv4Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == true);
}

void TestVpnIpv4Page::test_checkIsChanged_searchDomainChanged()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(0);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    vpnInfo.m_ipv4Address.clear();
    vpnInfo.m_ipv4Dns.clear();
    vpnInfo.m_ipv4DnsSearch.append("example.com");
    vpnInfo.m_ipv4DhcpClientId = "";
    
    bool isChanged = m_vpnIpv4Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == true);
}

void TestVpnIpv4Page::test_checkIsChanged_dhcpClientIdChanged()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(0);
    
    KyVpnConfig vpnInfo;
    vpnInfo.m_ipv4ConfigIpType = CONFIG_IP_DHCP;
    vpnInfo.m_ipv4Address.clear();
    vpnInfo.m_ipv4Dns.clear();
    vpnInfo.m_ipv4DnsSearch.clear();
    vpnInfo.m_ipv4DhcpClientId = "new-client-id";
    
    bool isChanged = m_vpnIpv4Page->checkIsChanged(vpnInfo);
    QVERIFY(isChanged == true);
}

void TestVpnIpv4Page::test_updateVpnIpv4Info_autoConfig()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(0);
    
    KyVpnConfig vpnInfo;
    m_vpnIpv4Page->updateVpnIpv4Info(vpnInfo);
    
    QVERIFY(vpnInfo.m_ipv4ConfigIpType == CONFIG_IP_DHCP);
}

void TestVpnIpv4Page::test_updateVpnIpv4Info_manualConfig()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(1);
    
    QList<QLineEdit*> edits = m_vpnIpv4Page->findChildren<QLineEdit *>();
    QLineEdit *addressEdit = nullptr;
    QLineEdit *netmaskEdit = nullptr;
    QLineEdit *gatewayEdit = nullptr;
    
    for (QLineEdit *edit : edits) {
        if (edit->placeholderText() == "Required" || addressEdit == nullptr) {
            if (addressEdit == nullptr) addressEdit = edit;
            else if (netmaskEdit == nullptr) netmaskEdit = edit;
            else if (gatewayEdit == nullptr) gatewayEdit = edit;
        }
    }
    
    if (addressEdit) addressEdit->setText("192.168.1.100");
    if (netmaskEdit) netmaskEdit->setText("255.255.255.0");
    if (gatewayEdit) gatewayEdit->setText("192.168.1.1");
    
    KyVpnConfig vpnInfo;
    m_vpnIpv4Page->updateVpnIpv4Info(vpnInfo);
    
    QVERIFY(vpnInfo.m_ipv4ConfigIpType == CONFIG_IP_MANUAL);
}

void TestVpnIpv4Page::test_setEnableOfConfirmBtn_signal()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QSignalSpy spy(m_vpnIpv4Page, &VpnIpv4Page::setIpv4PageState);
    QVERIFY(spy.isValid());
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    comboBox->setCurrentIndex(0);
    QTest::qWait(50);
    
    QVERIFY(spy.count() >= 1);
}

void TestVpnIpv4Page::test_comboBoxIndexChanged_signal()
{
    QVERIFY(m_vpnIpv4Page != nullptr);
    
    QComboBox *comboBox = m_vpnIpv4Page->findChild<QComboBox *>();
    QVERIFY(comboBox != nullptr);
    
    QSignalSpy spy(comboBox, SIGNAL(currentIndexChanged(int)));
    QVERIFY(spy.isValid());
    
    comboBox->setCurrentIndex(1);
    QTest::qWait(50);
    
    QVERIFY(spy.count() >= 1);
}
