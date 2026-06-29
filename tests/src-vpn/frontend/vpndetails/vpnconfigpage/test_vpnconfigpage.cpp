#include "test_vpnconfigpage.h"
#include "../../../../../../src/backend/dbus-interface/kyvpnconnectoperation.h"

TestVpnConfigPage::TestVpnConfigPage(QObject *parent)
    : QObject(parent)
    , m_vpnConfigPage(nullptr)
{
}

void TestVpnConfigPage::initTestCase()
{
}

void TestVpnConfigPage::cleanupTestCase()
{
}

void TestVpnConfigPage::init()
{
    m_vpnConfigPage = new VpnConfigPage();
}

void TestVpnConfigPage::cleanup()
{
    if (m_vpnConfigPage) {
        delete m_vpnConfigPage;
        m_vpnConfigPage = nullptr;
    }
}

void TestVpnConfigPage::testConstructor()
{
    QVERIFY2(m_vpnConfigPage != nullptr, "Failed to create VpnConfigPage instance");
    QVERIFY2(m_vpnConfigPage->isVisible(), "VpnConfigPage should be visible after construction");
    QSignalSpy spy(m_vpnConfigPage, &VpnConfigPage::vpnTypeChanged);
    QVERIFY2(spy.isValid(), "vpnTypeChanged signal should be valid");
}

void TestVpnConfigPage::testGetVpnType()
{
    VpnType type = m_vpnConfigPage->getVpnType();
    QVERIFY2(type == L2TP, "Default VPN type should be L2TP");
    
    QComboBox *comboBox = m_vpnConfigPage->findChild<QComboBox *>();
    QVERIFY2(comboBox != nullptr, "VPN type comboBox should exist");
    
    if (comboBox) {
        comboBox->setCurrentIndex(OPEN_VPN);
        type = m_vpnConfigPage->getVpnType();
        QVERIFY2(type == OPEN_VPN, "VPN type should be OPEN_VPN after change");
        
        comboBox->setCurrentIndex(PPTP);
        type = m_vpnConfigPage->getVpnType();
        QVERIFY2(type == PPTP, "VPN type should be PPTP after change");
        
        comboBox->setCurrentIndex(STRONG_SWAN);
        type = m_vpnConfigPage->getVpnType();
        QVERIFY2(type == STRONG_SWAN, "VPN type should be STRONG_SWAN after change");
    }
}

void TestVpnConfigPage::testSetVpnConfigInfo()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_vpnType = KYVPNTYPE_L2TP;
    vpnInfo.m_vpnName = "TestVPN";
    vpnInfo.m_gateway = "192.168.1.1";
    
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    
    QLineEdit *nameEdit = m_vpnConfigPage->findChild<QLineEdit *>();
    QVERIFY2(nameEdit != nullptr, "VPN name edit should exist");
    
    if (nameEdit) {
        QString name = nameEdit->text();
        QVERIFY2(name == "TestVPN", "VPN name should match");
    }
    
    QComboBox *comboBox = m_vpnConfigPage->findChild<QComboBox *>();
    QVERIFY2(comboBox != nullptr, "VPN type comboBox should exist");
    
    vpnInfo.m_vpnType = KYVPNTYPE_OPENVPN;
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    
    vpnInfo.m_vpnType = KYVPNTYPE_PPTP;
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    
    vpnInfo.m_vpnType = KYVPNTYPE_STRONGSWAN;
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    
    vpnInfo.m_vpnType = static_cast<KyVpnType>(999);
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
}

void TestVpnConfigPage::testCheckIsChanged()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_vpnType = KYVPNTYPE_L2TP;
    vpnInfo.m_vpnName = "TestVPN";
    vpnInfo.m_gateway = "192.168.1.1";
    
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    
    bool isChanged = m_vpnConfigPage->checkIsChanged(vpnInfo);
    QVERIFY2(isChanged == false, "Should not detect change when values match");
    
    vpnInfo.m_vpnName = "DifferentVPN";
    isChanged = m_vpnConfigPage->checkIsChanged(vpnInfo);
    QVERIFY2(isChanged == true, "Should detect change when name differs");
    
    vpnInfo.m_vpnName = "TestVPN";
    vpnInfo.m_gateway = "10.0.0.1";
    isChanged = m_vpnConfigPage->checkIsChanged(vpnInfo);
    QVERIFY2(isChanged == true, "Should detect change when gateway differs");
    
    vpnInfo.m_vpnType = KYVPNTYPE_OPENVPN;
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    vpnInfo.m_vpnType = KYVPNTYPE_L2TP;
    isChanged = m_vpnConfigPage->checkIsChanged(vpnInfo);
    QVERIFY2(isChanged == true, "Should detect change when type differs");
    
    vpnInfo.m_vpnType = KYVPNTYPE_L2TP;
    vpnInfo.m_vpnName = "TestVPN";
    vpnInfo.m_gateway = "192.168.1.1";
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    
    QComboBox *comboBox = m_vpnConfigPage->findChild<QComboBox *>();
    if (comboBox) {
        comboBox->setCurrentIndex(OPEN_VPN);
        isChanged = m_vpnConfigPage->checkIsChanged(vpnInfo);
        QVERIFY2(isChanged == true, "Should detect change when combo box changes");
    }
}

void TestVpnConfigPage::testUpdateVpnConfigInfo()
{
    KyVpnConfig vpnInfo;
    vpnInfo.m_vpnType = KYVPNTYPE_L2TP;
    vpnInfo.m_vpnName = "";
    vpnInfo.m_gateway = "";
    
    m_vpnConfigPage->setVpnConfigInfo(vpnInfo);
    
    QLineEdit *nameEdit = m_vpnConfigPage->findChild<QLineEdit *>();
    QLineEdit *gatewayEdit = m_vpnConfigPage->findChild<QLineEdit *>();
    QComboBox *comboBox = m_vpnConfigPage->findChild<QComboBox *>();
    
    if (nameEdit && gatewayEdit && comboBox) {
        nameEdit->setText("UpdatedVPN");
        gatewayEdit->setText("10.0.0.1");
        comboBox->setCurrentIndex(OPEN_VPN);
        
        KyVpnConfig updatedInfo;
        m_vpnConfigPage->updateVpnConfigInfo(updatedInfo);
        
        QVERIFY2(updatedInfo.m_vpnName == "UpdatedVPN", "VPN name should be updated");
        QVERIFY2(updatedInfo.m_gateway == "10.0.0.1", "Gateway should be updated");
        QVERIFY2(updatedInfo.m_vpnType == KYVPNTYPE_OPENVPN, "VPN type should be updated");
    }
    
    comboBox->setCurrentIndex(PPTP);
    {
        KyVpnConfig updatedInfo;
        m_vpnConfigPage->updateVpnConfigInfo(updatedInfo);
        QVERIFY2(updatedInfo.m_vpnType == KYVPNTYPE_PPTP, "VPN type should be PPTP");
    }
    
    comboBox->setCurrentIndex(STRONG_SWAN);
    {
        KyVpnConfig updatedInfo;
        m_vpnConfigPage->updateVpnConfigInfo(updatedInfo);
        QVERIFY2(updatedInfo.m_vpnType == KYVPNTYPE_STRONGSWAN, "VPN type should be STRONG_SWAN");
    }
    
    comboBox->setCurrentIndex(100);
    {
        KyVpnConfig updatedInfo;
        m_vpnConfigPage->updateVpnConfigInfo(updatedInfo);
    }
}
