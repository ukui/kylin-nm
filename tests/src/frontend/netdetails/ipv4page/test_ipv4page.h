#ifndef TEST_IPV4PAGE_H
#define TEST_IPV4PAGE_H

#include <QObject>
#include <QHostAddress>
#include <QSignalSpy>

class Ipv4Page;
class KyConnectSetting;
class ConInfo;

class TestIpv4Page : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testSetIpv4Config_Auto();
    void testSetIpv4Config_Manual();
    void testSetIpv4();
    void testSetNetMask();
    void testSetMulDns();
    void testSetGateWay();
    void testSetUuid();
    void testCheckIsChanged_AutoConfig_NoChange();
    void testCheckIsChanged_AutoConfig_Changed();
    void testCheckIsChanged_ManualConfig_NoChange();
    void testCheckIsChanged_ManualConfig_Changed();
    void testCheckDnsSettingsIsChanged();
    void testStartLoading();
    void testStopLoading();
    void testShowIpv4AddressConflict_True();
    void testShowIpv4AddressConflict_False();
    void testGetErrorMessage_Empty();
    void testGetErrorMessage_InvalidIp();
    void testGetErrorMessage_InvalidMask();
    void testGetErrorMessage_InvalidGateway();
    void testGetErrorMessage_Valid();
    void testGetTextEditState_Empty();
    void testGetTextEditState_ValidIp();
    void testGetTextEditState_InvalidIp();
    void testNetMaskIsValide_ValidIp();
    void testNetMaskIsValide_ValidPrefix();
    void testNetMaskIsValide_Invalid();
    void testGetNetMaskText_FullFormat();
    void testGetNetMaskText_PrefixFormat();
    void testSetEnableOfSaveBtnSignal();

private:
    Ipv4Page *m_widget;
    ConInfo *m_conInfo;
    KyConnectSetting *m_setting;
};

#endif // TEST_IPV4PAGE_H
