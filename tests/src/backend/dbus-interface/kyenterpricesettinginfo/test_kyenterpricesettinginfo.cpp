#include "test_kyenterpricesettinginfo.h"
#include "../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h"
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Security8021xSetting>
#include <NetworkManagerQt/WirelessSecuritySetting>

void TestAssembleEapMethodTlsSettings::SetUp() {}
void TestAssembleEapMethodTlsSettings::TearDown() {}

TEST_F(TestAssembleEapMethodTlsSettings, WithDomainAndCa)
{
    KyEapMethodTlsInfo info;
    info.identity = "testuser";
    info.domain = "testdomain.com";
    info.bNeedCa = true;
    info.caCertPath = "/path/to/ca.crt";
    info.clientCertPath = "/path/to/client.crt";
    info.clientPrivateKey = "/path/to/client.key";
    info.clientPrivateKeyPWD = "password";
    info.m_privateKeyPWDFlag = NetworkManager::Setting::None;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodTlsSettings(connSettingPtr, info));
}

TEST_F(TestAssembleEapMethodTlsSettings, WithoutDomainAndCa)
{
    KyEapMethodTlsInfo info;
    info.identity = "testuser";
    info.domain = "";
    info.bNeedCa = false;
    info.caCertPath = "";
    info.clientCertPath = "/path/to/client.crt";
    info.clientPrivateKey = "/path/to/client.key";
    info.clientPrivateKeyPWD = "password";
    info.m_privateKeyPWDFlag = NetworkManager::Setting::None;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodTlsSettings(connSettingPtr, info));
}

void TestAssembleEapMethodPeapSettings::SetUp() {}
void TestAssembleEapMethodPeapSettings::TearDown() {}

TEST_F(TestAssembleEapMethodPeapSettings, BasicPeapSettings)
{
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodPeapSettings(connSettingPtr, info));
}

void TestAssembleEapMethodTtlsSettings::SetUp() {}
void TestAssembleEapMethodTtlsSettings::TearDown() {}

TEST_F(TestAssembleEapMethodTtlsSettings, AuthEapType)
{
    KyEapMethodTtlsInfo info;
    info.authType = AUTH_EAP;
    info.authEapMethod = KyAuthEapMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodTtlsSettings(connSettingPtr, info));
}

TEST_F(TestAssembleEapMethodTtlsSettings, AuthNoEapType)
{
    KyEapMethodTtlsInfo info;
    info.authType = AUTH_NO_EAP;
    info.authNoEapMethod = KyAuthMethodChap;
    info.userName = "testuser";
    info.userPWD = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodTtlsSettings(connSettingPtr, info));
}

void TestAssembleEapMethodLeapSettings::SetUp() {}
void TestAssembleEapMethodLeapSettings::TearDown() {}

TEST_F(TestAssembleEapMethodLeapSettings, BasicLeapSettings)
{
    KyEapMethodLeapInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodLeapSettings(connSettingPtr, info));
}

void TestAssembleEapMethodPwdSettings::SetUp() {}
void TestAssembleEapMethodPwdSettings::TearDown() {}

TEST_F(TestAssembleEapMethodPwdSettings, BasicPwdSettings)
{
    KyEapMethodPwdInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodPwdSettings(connSettingPtr, info));
}

void TestAssembleEapMethodFastSettings::SetUp() {}
void TestAssembleEapMethodFastSettings::TearDown() {}

TEST_F(TestAssembleEapMethodFastSettings, WithAutoPac)
{
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anonymous";
    info.m_pacProvisioning = KyFastProvisioningAllowAuthenticated;
    info.m_allowAutoPacFlag = true;
    info.m_pacFilePath = "/path/to/pacfile";
    info.m_authMethod = KyAuthMethodMschapv2;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodFastSettings(connSettingPtr, info));
}

TEST_F(TestAssembleEapMethodFastSettings, WithoutAutoPac)
{
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anonymous";
    info.m_pacProvisioning = KyFastProvisioningDisabled;
    info.m_allowAutoPacFlag = false;
    info.m_pacFilePath = "";
    info.m_authMethod = KyAuthMethodMschapv2;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(assembleEapMethodFastSettings(connSettingPtr, info));
}

void TestModifyEapMethodTlsSettings::SetUp() {}
void TestModifyEapMethodTlsSettings::TearDown() {}

TEST_F(TestModifyEapMethodTlsSettings, WithDomainAndCaAndChanged)
{
    KyEapMethodTlsInfo info;
    info.identity = "testuser";
    info.domain = "testdomain.com";
    info.bNeedCa = true;
    info.caCertPath = "/path/to/ca.crt";
    info.clientCertPath = "/path/to/client.crt";
    info.clientPrivateKey = "/path/to/client.key";
    info.clientPrivateKeyPWD = "password";
    info.m_privateKeyPWDFlag = NetworkManager::Setting::None;
    info.bChanged = true;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodTlsSettings(connSettingPtr, info));
}

TEST_F(TestModifyEapMethodTlsSettings, WithoutDomainAndCaAndNotChanged)
{
    KyEapMethodTlsInfo info;
    info.identity = "testuser";
    info.domain = "";
    info.bNeedCa = false;
    info.caCertPath = "";
    info.clientCertPath = "/path/to/client.crt";
    info.clientPrivateKey = "/path/to/client.key";
    info.clientPrivateKeyPWD = "password";
    info.m_privateKeyPWDFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodTlsSettings(connSettingPtr, info));
}

void TestModifyEapMethodPeapSettings::SetUp() {}
void TestModifyEapMethodPeapSettings::TearDown() {}

TEST_F(TestModifyEapMethodPeapSettings, WithChanged)
{
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = true;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodPeapSettings(connSettingPtr, info));
}

TEST_F(TestModifyEapMethodPeapSettings, WithoutChanged)
{
    KyEapMethodPeapInfo info;
    info.phase2AuthMethod = KyAuthMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodPeapSettings(connSettingPtr, info));
}

void TestModifyEapMethodTtlsSettings::SetUp() {}
void TestModifyEapMethodTtlsSettings::TearDown() {}

TEST_F(TestModifyEapMethodTtlsSettings, AuthEapTypeWithChanged)
{
    KyEapMethodTtlsInfo info;
    info.authType = AUTH_EAP;
    info.authEapMethod = KyAuthEapMethodMschapv2;
    info.userName = "testuser";
    info.userPWD = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = true;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodTtlsSettings(connSettingPtr, info));
}

TEST_F(TestModifyEapMethodTtlsSettings, AuthNoEapTypeWithChanged)
{
    KyEapMethodTtlsInfo info;
    info.authType = AUTH_NO_EAP;
    info.authNoEapMethod = KyAuthMethodChap;
    info.userName = "testuser";
    info.userPWD = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = true;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodTtlsSettings(connSettingPtr, info));
}

void TestModifyEapMethodLeapSettings::SetUp() {}
void TestModifyEapMethodLeapSettings::TearDown() {}

TEST_F(TestModifyEapMethodLeapSettings, WithChanged)
{
    KyEapMethodLeapInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = true;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodLeapSettings(connSettingPtr, info));
}

TEST_F(TestModifyEapMethodLeapSettings, WithoutChanged)
{
    KyEapMethodLeapInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodLeapSettings(connSettingPtr, info));
}

void TestModifyEapMethodPwdSettings::SetUp() {}
void TestModifyEapMethodPwdSettings::TearDown() {}

TEST_F(TestModifyEapMethodPwdSettings, WithChanged)
{
    KyEapMethodPwdInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = true;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodPwdSettings(connSettingPtr, info));
}

TEST_F(TestModifyEapMethodPwdSettings, WithoutChanged)
{
    KyEapMethodPwdInfo info;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodPwdSettings(connSettingPtr, info));
}

void TestModifyEapMethodFastSettings::SetUp() {}
void TestModifyEapMethodFastSettings::TearDown() {}

TEST_F(TestModifyEapMethodFastSettings, WithAutoPacAndChanged)
{
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anonymous";
    info.m_pacProvisioning = KyFastProvisioningAllowAuthenticated;
    info.m_allowAutoPacFlag = true;
    info.m_pacFilePath = "/path/to/pacfile";
    info.m_authMethod = KyAuthMethodMschapv2;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = true;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodFastSettings(connSettingPtr, info));
}

TEST_F(TestModifyEapMethodFastSettings, WithoutAutoPacAndChanged)
{
    KyEapMethodFastInfo info;
    info.m_anonIdentity = "anonymous";
    info.m_pacProvisioning = KyFastProvisioningDisabled;
    info.m_allowAutoPacFlag = false;
    info.m_pacFilePath = "";
    info.m_authMethod = KyAuthMethodMschapv2;
    info.m_userName = "testuser";
    info.m_userPwd = "password";
    info.m_passwdFlag = NetworkManager::Setting::None;
    info.bChanged = false;
    
    NetworkManager::ConnectionSettings::Ptr connSettingPtr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wireless));
    EXPECT_NO_THROW(modifyEapMethodFastSettings(connSettingPtr, info));
}
