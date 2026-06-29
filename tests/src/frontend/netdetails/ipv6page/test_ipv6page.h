#ifndef TEST_IPV6PAGE_H
#define TEST_IPV6PAGE_H

#include <QObject>
#include <QHostAddress>
#include <QSignalSpy>

class Ipv6Page;
class KyConnectSetting;
class ConInfo;

class TestIpv6Page : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testSetIpv6Config_Auto();
    void testSetIpv6Config_Manual();
    void testSetIpv6();
    void testSetIpv6Perfix();
    void testSetMulDns();
    void testSetGateWay();
    void testCheckIsChanged_AutoConfig_NoChange();
    void testCheckIsChanged_AutoConfig_Changed();
    void testCheckIsChanged_ManualConfig_NoChange();
    void testCheckIsChanged_ManualConfig_Changed();
    void testGetPerfixLength_Empty();
    void testGetPerfixLength_Valid();
    void testStartLoading();
    void testStopLoading();
    void testShowIpv6AddressConflict_True();
    void testShowIpv6AddressConflict_False();
    void testGetErrorMessage_Empty();
    void testGetErrorMessage_Invalid();
    void testGetErrorMessage_Valid();

private:
    Ipv6Page *m_widget;
    ConInfo *m_conInfo;
    KyConnectSetting *m_setting;
};

#endif // TEST_IPV6PAGE_H
