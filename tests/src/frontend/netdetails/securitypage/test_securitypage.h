#ifndef TEST_SECURITYPAGE_H
#define TEST_SECURITYPAGE_H

#include <QObject>
#include <QTest>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QSignalSpy>

class SecurityPage;
class KyWirelessConnectSetting;
class ConInfo;

class TestSecurityPage : public QObject
{
    Q_OBJECT
public:
    explicit TestSecurityPage(QObject *parent = nullptr);
    ~TestSecurityPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Constructor tests
    void testConstructor_WithDetailPage();
    void testConstructor_WithoutDetailPage();

    // Basic setter tests
    void testSetSecurity();
    void testSetPsk();

    // TLS info tests
    void testSetTlsInfo_WithEmptyCaCert();
    void testSetTlsInfo_WithCaCert();
    void testUpdateTlsChange();

    // PEAP info tests
    void testSetPeapInfo_Mschapv2();
    void testSetPeapInfo_Md5();
    void testSetPeapInfo_Gtc();
    void testUpdatePeapChange();

    // TTLS info tests
    void testSetTtlsInfo_AuthEap();
    void testSetTtlsInfo_AuthNoEap();
    void testUpdateTtlsChange();

    // LEAP info tests
    void testSetLeapInfo();
    void testUpdateLeapChange();

    // PWD info tests
    void testSetPwdInfo();
    void testUpdatePwdChange();

    // FAST info tests
    void testSetFastInfo_Disabled();
    void testSetFastInfo_Unauthenticated();
    void testSetFastInfo_Authenticated();
    void testSetFastInfo_Both();
    void testUpdateFastChange();

    // Visibility tests
    void testSetSecurityVisible_True();
    void testSetSecurityVisible_False();
    void testSetAutoConnectVisible();

    // Getter tests
    void testGetSecuType();
    void testGetAutoConnectState();

    // Change detection tests
    void testCheckIsChanged_None();
    void testCheckIsChanged_Psk_Same();
    void testCheckIsChanged_Psk_Different();
    void testCheckIsChanged_Enterprise_Tls_Same();
    void testCheckIsChanged_Enterprise_Tls_Different();
    void testCheckIsChanged_Enterprise_Peap_Same();
    void testCheckIsChanged_Enterprise_Peap_Different();

    // Error message test
    void testGetErrorMessage();

    // Update security change
    void testUpdateSecurityChange();

private:
    SecurityPage *m_securityPage;
    ConInfo *m_conInfo;
    KyWirelessConnectSetting *m_setting;
};

#endif // TEST_SECURITYPAGE_H
