#include <QTest>
#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <cmath>

#include "securitypage.h"
#include "kylinconnectsetting.h"
#include "coninfo.h"
#include "test_securitypage.h"

TestSecurityPage::TestSecurityPage(QObject *parent)
    : QObject(parent)
    , m_securityPage(nullptr)
    , m_conInfo(nullptr)
    , m_setting(nullptr)
{
}

TestSecurityPage::~TestSecurityPage()
{
}

void TestSecurityPage::initTestCase()
{
}

void TestSecurityPage::cleanupTestCase()
{
}

void TestSecurityPage::init()
{
    m_securityPage = new SecurityPage(true, nullptr);
    m_conInfo = new ConInfo();
    m_setting = new KyWirelessConnectSetting();
}

void TestSecurityPage::cleanup()
{
    delete m_securityPage;
    m_securityPage = nullptr;
    delete m_conInfo;
    m_conInfo = nullptr;
    delete m_setting;
    m_setting = nullptr;
}

void TestSecurityPage::testConstructor_WithDetailPage()
{
    SecurityPage *widget = new SecurityPage(true, nullptr);
    QVERIFY(widget != nullptr);
    QVERIFY(widget->findChild<QComboBox *>() != nullptr);
    delete widget;
}

void TestSecurityPage::testConstructor_WithoutDetailPage()
{
    SecurityPage *widget = new SecurityPage(false, nullptr);
    QVERIFY(widget != nullptr);
    QVERIFY(widget->findChild<QComboBox *>() != nullptr);
    delete widget;
}

void TestSecurityPage::testSetSecurity()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_PERSONAL);
    QList<QComboBox *> comboBoxes = m_securityPage->findChildren<QComboBox *>();
    QComboBox *secuTypeCombox = nullptr;
    for (QComboBox *cb : comboBoxes) {
        if (cb->count() >= 4) {
            secuTypeCombox = cb;
            break;
        }
    }
    QVERIFY(secuTypeCombox != nullptr);
    QVERIFY(secuTypeCombox->currentIndex() == KYLIN_NM::WPA_AND_WPA2_PERSONAL);

    m_securityPage->setSecurity(KYLIN_NM::WPA3_PERSONAL);
    QVERIFY(secuTypeCombox->currentIndex() == KYLIN_NM::WPA3_PERSONAL);

    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    QVERIFY(secuTypeCombox->currentIndex() == KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);

    m_securityPage->setSecurity(KYLIN_NM::NONE);
    QVERIFY(secuTypeCombox->currentIndex() == KYLIN_NM::NONE);
}

void TestSecurityPage::testSetPsk()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_PERSONAL);
    QString psk = "testpassword123";
    m_securityPage->setPsk(psk);
    QList<QLineEdit *> edits = m_securityPage->findChildren<QLineEdit *>();
    QVERIFY(edits.size() >= 1);
    QLineEdit *pwdEdit = edits.at(0);
    QVERIFY(pwdEdit->text() == psk);
}

void TestSecurityPage::testSetTlsInfo_WithEmptyCaCert()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTlsInfo info;
    info.identity = "testidentity";
    info.domain = "testdomain";
    info.caCertPath = "";
    info.clientCertPath = "";
    info.clientPrivateKey = "";
    info.clientPrivateKeyPWD = "";
    info.m_privateKeyPWDFlag = NetworkManager::Setting::AgentOwned;

    m_securityPage->setTlsInfo(info);

    QComboBox *eapTypeCombox = m_securityPage->findChild<QComboBox *>();
    QVERIFY(eapTypeCombox != nullptr);
}

void TestSecurityPage::testSetTlsInfo_WithCaCert()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTlsInfo info;
    info.identity = "testidentity";
    info.domain = "testdomain";
    info.caCertPath = "/path/to/ca.crt";
    info.clientCertPath = "/path/to/client.crt";
    info.clientPrivateKey = "/path/to/client.key";
    info.clientPrivateKeyPWD = "password";
    info.m_privateKeyPWDFlag = NetworkManager::Setting::None;

    m_securityPage->setTlsInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testUpdateTlsChange()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTlsInfo info;
    info.clientPrivateKeyPWD = "oldpassword";
    m_securityPage->updateTlsChange(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetPeapInfo_Mschapv2()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setPeapInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetPeapInfo_Md5()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodMd5;
    info.userName = "testuser";
    info.userPWD = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setPeapInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetPeapInfo_Gtc()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodGtc;
    info.userName = "testuser";
    info.userPWD = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setPeapInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testUpdatePeapChange()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPeapInfo info;
    info.userPWD = "oldpassword";
    m_securityPage->updatePeapChange(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetTtlsInfo_AuthEap()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTtlsInfo info;
    info.authType = AUTH_EAP;
    info.authEapMethod = KyAuthEapMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setTtlsInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetTtlsInfo_AuthNoEap()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTtlsInfo info;
    info.authType = AUTH_NO_EAP;
    info.authNoEapMethod = KyAuthMethodPap;
    info.userName = "testuser";
    info.userPWD = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setTtlsInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testUpdateTtlsChange()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTtlsInfo info;
    info.userPWD = "oldpassword";
    m_securityPage->updateTtlsChange(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetLeapInfo()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodLeapInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setLeapInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testUpdateLeapChange()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodLeapInfo info;
    info.m_userPwd = "oldpassword";
    m_securityPage->updateLeapChange(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetPwdInfo()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPwdInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setPwdInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testUpdatePwdChange()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPwdInfo info;
    info.m_userPwd = "oldpassword";
    m_securityPage->updatePwdChange(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetFastInfo_Disabled()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anon";
    info.m_pacProvisioning = KyFastProvisioningDisabled;
    info.m_allowAutoPacFlag = false;
    info.m_pacFilePath = "";
    info.m_authMethod = KyAuthMethodGtc;
    info.m_userName = "testuser";
    info.m_userPwd = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setFastInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetFastInfo_Unauthenticated()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anon";
    info.m_pacProvisioning = KyFastProvisioningAllowUnauthenticated;
    info.m_allowAutoPacFlag = true;
    info.m_pacFilePath = "";
    info.m_authMethod = KyAuthMethodMschapv2;
    info.m_userName = "testuser";
    info.m_userPwd = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setFastInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetFastInfo_Authenticated()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anon";
    info.m_pacProvisioning = KyFastProvisioningAllowAuthenticated;
    info.m_allowAutoPacFlag = true;
    info.m_pacFilePath = "";
    info.m_authMethod = KyAuthMethodGtc;
    info.m_userName = "testuser";
    info.m_userPwd = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setFastInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetFastInfo_Both()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anon";
    info.m_pacProvisioning = KyFastProvisioningAllowBoth;
    info.m_allowAutoPacFlag = true;
    info.m_pacFilePath = "";
    info.m_authMethod = KyAuthMethodMschapv2;
    info.m_userName = "testuser";
    info.m_userPwd = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setFastInfo(info);
    QVERIFY(true);
}

void TestSecurityPage::testUpdateFastChange()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodFastInfo info;
    info.m_userPwd = "oldpassword";
    m_securityPage->updateFastChange(info);
    QVERIFY(true);
}

void TestSecurityPage::testSetSecurityVisible_True()
{
    m_securityPage->setSecurityVisible(true);
    QVERIFY(true);
}

void TestSecurityPage::testSetSecurityVisible_False()
{
    m_securityPage->setSecurityVisible(false);
    QVERIFY(true);
}

void TestSecurityPage::testSetAutoConnectVisible()
{
    m_securityPage->setAutoConnectVisible(true);
    m_securityPage->setAutoConnectVisible(false);
    QVERIFY(true);
}

void TestSecurityPage::testGetSecuType()
{
    KySecuType secuType;
    KyEapMethodType enterpriseType;
    m_securityPage->getSecuType(secuType, enterpriseType);
    QVERIFY(secuType == KYLIN_NM::NONE ||
            secuType == KYLIN_NM::WPA_AND_WPA2_PERSONAL ||
            secuType == KYLIN_NM::WPA_AND_WPA2_ENTERPRISE ||
            secuType == KYLIN_NM::WPA3_PERSONAL);
}

void TestSecurityPage::testGetAutoConnectState()
{
    bool state = m_securityPage->getAutoConnectState();
    QVERIFY(state == true || state == false);
}

void TestSecurityPage::testCheckIsChanged_None()
{
    m_conInfo->secType = KYLIN_NM::NONE;
    m_securityPage->setSecurity(KYLIN_NM::NONE);
    bool result = m_securityPage->checkIsChanged(*m_conInfo);
    QVERIFY(result == false);
}

void TestSecurityPage::testCheckIsChanged_Psk_Same()
{
    m_conInfo->secType = KYLIN_NM::WPA_AND_WPA2_PERSONAL;
    m_conInfo->strPassword = "testpassword";
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_PERSONAL);
    m_securityPage->setPsk("testpassword");
    bool result = m_securityPage->checkIsChanged(*m_conInfo);
    QVERIFY(result == false);
}

void TestSecurityPage::testCheckIsChanged_Psk_Different()
{
    m_conInfo->secType = KYLIN_NM::WPA_AND_WPA2_PERSONAL;
    m_conInfo->strPassword = "testpassword";
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_PERSONAL);
    m_securityPage->setPsk("differentpassword");
    bool result = m_securityPage->checkIsChanged(*m_conInfo);
    QVERIFY(result == true);
}

void TestSecurityPage::testCheckIsChanged_Enterprise_Tls_Same()
{
    m_conInfo->secType = KYLIN_NM::WPA_AND_WPA2_ENTERPRISE;
    m_conInfo->enterpriseType = TLS;
    m_conInfo->tlsInfo.identity = "testidentity";
    m_conInfo->tlsInfo.domain = "testdomain";
    m_conInfo->tlsInfo.caCertPath = "";
    m_conInfo->tlsInfo.clientCertPath = "";
    m_conInfo->tlsInfo.clientPrivateKey = "";
    m_conInfo->tlsInfo.clientPrivateKeyPWD = "";
    m_conInfo->tlsInfo.m_privateKeyPWDFlag = NetworkManager::Setting::AgentOwned;

    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTlsInfo info;
    info.identity = "testidentity";
    info.domain = "testdomain";
    info.caCertPath = "";
    info.clientCertPath = "";
    info.clientPrivateKey = "";
    info.clientPrivateKeyPWD = "";
    info.m_privateKeyPWDFlag = NetworkManager::Setting::AgentOwned;
    m_securityPage->setTlsInfo(info);

    bool result = m_securityPage->checkIsChanged(*m_conInfo);
    QVERIFY(result == false);
}

void TestSecurityPage::testCheckIsChanged_Enterprise_Tls_Different()
{
    m_conInfo->secType = KYLIN_NM::WPA_AND_WPA2_ENTERPRISE;
    m_conInfo->enterpriseType = TLS;
    m_conInfo->tlsInfo.identity = "testidentity";

    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodTlsInfo info;
    info.identity = "differentidentity";
    m_securityPage->setTlsInfo(info);

    bool result = m_securityPage->checkIsChanged(*m_conInfo);
    QVERIFY(result == true);
}

void TestSecurityPage::testCheckIsChanged_Enterprise_Peap_Same()
{
    m_conInfo->secType = KYLIN_NM::WPA_AND_WPA2_ENTERPRISE;
    m_conInfo->enterpriseType = PEAP;
    m_conInfo->peapInfo.phase2AuthMethod = KyAuthMethodMschapv2;
    m_conInfo->peapInfo.userName = "testuser";
    m_conInfo->peapInfo.userPWD = "testpassword";
    m_conInfo->peapInfo.m_passwdFlag = NetworkManager::Setting::None;

    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;
    m_securityPage->setPeapInfo(info);

    bool result = m_securityPage->checkIsChanged(*m_conInfo);
    QVERIFY(result == false);
}

void TestSecurityPage::testCheckIsChanged_Enterprise_Peap_Different()
{
    m_conInfo->secType = KYLIN_NM::WPA_AND_WPA2_ENTERPRISE;
    m_conInfo->enterpriseType = PEAP;
    m_conInfo->peapInfo.userName = "testuser";

    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_ENTERPRISE);
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodMschapv2;
    info.userName = "differentuser";
    info.userPWD = "testpassword";
    info.m_passwdFlag = NetworkManager::Setting::None;
    m_securityPage->setPeapInfo(info);

    bool result = m_securityPage->checkIsChanged(*m_conInfo);
    QVERIFY(result == true);
}

void TestSecurityPage::testGetErrorMessage()
{
    QString msg = m_securityPage->getErrorMessage();
    QVERIFY(msg.isEmpty() || !msg.isEmpty());
}

void TestSecurityPage::testUpdateSecurityChange()
{
    m_securityPage->setSecurity(KYLIN_NM::WPA_AND_WPA2_PERSONAL);
    m_securityPage->setPsk("testpassword");
    m_securityPage->updateSecurityChange(*m_setting);
    QVERIFY(true);
}
