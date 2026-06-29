#include "test_kywirelessconnectoperation.h"
#include <QDebug>

TestKyWirelessConnectOperation::TestKyWirelessConnectOperation()
{
}

TestKyWirelessConnectOperation::~TestKyWirelessConnectOperation()
{
}

void TestKyWirelessConnectOperation::SetUp()
{
    m_operation = new KyWirelessConnectOperation();
    m_connectSetting = new KyWirelessConnectSetting();
}

void TestKyWirelessConnectOperation::TearDown()
{
    if (m_operation) {
        delete m_operation;
        m_operation = nullptr;
    }
    if (m_connectSetting) {
        delete m_connectSetting;
        m_connectSetting = nullptr;
    }
}

TEST_F(TestKyWirelessConnectOperation, testConstructor)
{
    EXPECT_NE(m_operation, nullptr);
}

TEST_F(TestKyWirelessConnectOperation, testSetWirelessEnabled)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->setWirelessEnabled(true));
    EXPECT_NO_FATAL_FAILURE(m_operation->setWirelessEnabled(false));
}

TEST_F(TestKyWirelessConnectOperation, testGetWirelessEnabled)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->getWirelessEnabled());
}

TEST_F(TestKyWirelessConnectOperation, testActiveWirelessConnect)
{
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::activateConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->activeWirelessConnect("wlan0", "invalid-uuid"));
}

TEST_F(TestKyWirelessConnectOperation, testDeactivateWirelessConnection)
{
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::deactivateConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->deActivateWirelessConnection("test-connect", "invalid-uuid"));
}

TEST_F(TestKyWirelessConnectOperation, testSetWirelessAutoConnect)
{
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::updateConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->setWirelessAutoConnect("invalid-uuid", true));
    EXPECT_NO_FATAL_FAILURE(m_operation->setWirelessAutoConnect("invalid-uuid", false));
}

TEST_F(TestKyWirelessConnectOperation, testDeleteWirelessConnect)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->deleteWirelessConnect("invalid-uuid"));
}

TEST_F(TestKyWirelessConnectOperation, testGetPsk)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->getPsk("invalid-uuid"));
}

TEST_F(TestKyWirelessConnectOperation, testGetPrivateKeyPassword)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->getPrivateKeyPassword("invalid-uuid"));
}

TEST_F(TestKyWirelessConnectOperation, testGet8021xPassword)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->get8021xPassword("invalid-uuid"));
}

TEST_F(TestKyWirelessConnectOperation, testUpdateIpv4AndIpv6SettingInfo)
{
    KyConnectSetting setting;
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::updateConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->updateIpv4AndIpv6SettingInfo("invalid-uuid", setting));
}

TEST_F(TestKyWirelessConnectOperation, testGetConnectKeyMgmt)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->getConnectKeyMgmt("invalid-uuid"));
}

TEST_F(TestKyWirelessConnectOperation, testGetEnterpiseEapMethod)
{
    KyEapMethodType type;
    EXPECT_NO_FATAL_FAILURE(m_operation->getEnterpiseEapMethod("invalid-uuid", type));
}

TEST_F(TestKyWirelessConnectOperation, testGetConnSecretFlags)
{
    QString uuid = "invalid-uuid";
    NetworkManager::Setting::SecretFlags flag;
    EXPECT_NO_FATAL_FAILURE(m_operation->getConnSecretFlags(uuid, flag));
}

TEST_F(TestKyWirelessConnectOperation, testAddConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_ifaceName = "wlan0";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::createConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->addConnect(*m_connectSetting));
}

TEST_F(TestKyWirelessConnectOperation, testAddTlsConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_ifaceName = "wlan0";
    KyEapMethodTlsInfo tlsInfo;
    tlsInfo.identity = "test";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::createConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->addTlsConnect(*m_connectSetting, tlsInfo));
}

TEST_F(TestKyWirelessConnectOperation, testAddPeapConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_ifaceName = "wlan0";
    KyEapMethodPeapInfo peapInfo;
    peapInfo.userName = "test";
    peapInfo.userPWD = "test";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::createConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->addPeapConnect(*m_connectSetting, peapInfo));
}

TEST_F(TestKyWirelessConnectOperation, testAddTtlsConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_ifaceName = "wlan0";
    KyEapMethodTtlsInfo ttlsInfo;
    ttlsInfo.userName = "test";
    ttlsInfo.userPWD = "test";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::createConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->addTtlsConnect(*m_connectSetting, ttlsInfo));
}

TEST_F(TestKyWirelessConnectOperation, testAddLeapConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_ifaceName = "wlan0";
    KyEapMethodLeapInfo leapInfo;
    leapInfo.m_userName = "test";
    leapInfo.m_userPwd = "test";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::createConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->addLeapConnect(*m_connectSetting, leapInfo));
}

TEST_F(TestKyWirelessConnectOperation, testAddPwdConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_ifaceName = "wlan0";
    KyEapMethodPwdInfo pwdInfo;
    pwdInfo.m_userName = "test";
    pwdInfo.m_userPwd = "test";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::createConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->addPwdConnect(*m_connectSetting, pwdInfo));
}

TEST_F(TestKyWirelessConnectOperation, testAddFastConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_ifaceName = "wlan0";
    KyEapMethodFastInfo fastInfo;
    fastInfo.m_userName = "test";
    fastInfo.m_userPwd = "test";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::createConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->addFastConnect(*m_connectSetting, fastInfo));
}

TEST_F(TestKyWirelessConnectOperation, testUpdateWirelessPersonalConnect)
{
    m_connectSetting->m_ssid = "TestSSID";
    m_connectSetting->m_type = KyKeyMgmt::WpaPsk;
    m_connectSetting->m_psk = "testpassword";
    QSignalSpy spy(m_operation, &KyWirelessConnectOperation::updateConnectionError);
    EXPECT_NO_FATAL_FAILURE(m_operation->updateWirelessPersonalConnect("invalid-uuid", *m_connectSetting, false));
    EXPECT_NO_FATAL_FAILURE(m_operation->updateWirelessPersonalConnect("invalid-uuid", *m_connectSetting, true));
}

TEST_F(TestKyWirelessConnectOperation, testRequestWirelessScan)
{
    EXPECT_NO_FATAL_FAILURE(m_operation->requestWirelessScan());
}
