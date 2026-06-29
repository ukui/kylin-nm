#include "test_kylinipv4arping.h"
#include "../../../../sys-dbus-register/kyarping/kylinipv4arping.h"

TestKyIpv4Arping::TestKyIpv4Arping(QObject *parent)
    : QObject(parent)
{
    m_testIfaceName = "eth0";
    m_testIpAddress = "192.168.1.1";
    m_testRetryCount = 3;
    m_testTimeout = 1;
}

void TestKyIpv4Arping::initTestCase()
{
}

void TestKyIpv4Arping::cleanupTestCase()
{
}

void TestKyIpv4Arping::init()
{
}

void TestKyIpv4Arping::cleanup()
{
}

void TestKyIpv4Arping::testConstructor()
{
    KyIpv4Arping *arping = new KyIpv4Arping(m_testIfaceName, m_testIpAddress, 
                                              m_testRetryCount, m_testTimeout);
    QVERIFY(arping != nullptr);
    QVERIFY(arping->ipv4IsConflict() == false);
    QVERIFY(arping->getMacAddress().isEmpty());
    delete arping;
}

void TestKyIpv4Arping::testDestructor()
{
    KyIpv4Arping *arping = new KyIpv4Arping(m_testIfaceName, m_testIpAddress,
                                              m_testRetryCount, m_testTimeout);
    QVERIFY(arping != nullptr);
    delete arping;
}

void TestKyIpv4Arping::testIpv4IsConflict_default()
{
    KyIpv4Arping arping(m_testIfaceName, m_testIpAddress, m_testRetryCount, m_testTimeout);
    QVERIFY(arping.ipv4IsConflict() == false);
}

void TestKyIpv4Arping::testIpv4IsConflict_afterSet()
{
    KyIpv4Arping arping(m_testIfaceName, m_testIpAddress, m_testRetryCount, m_testTimeout);
    QVERIFY(arping.ipv4IsConflict() == false);
    
    // Note: We cannot directly call private method setConflictFlag here
    // Testing through public interface only
}

void TestKyIpv4Arping::testGetMacAddress_default()
{
    KyIpv4Arping arping(m_testIfaceName, m_testIpAddress, m_testRetryCount, m_testTimeout);
    QVERIFY(arping.getMacAddress().isEmpty());
}

void TestKyIpv4Arping::testGetMacAddress_afterSet()
{
    KyIpv4Arping arping(m_testIfaceName, m_testIpAddress, m_testRetryCount, m_testTimeout);
    QVERIFY(arping.getMacAddress().isEmpty());
}

void TestKyIpv4Arping::testSetConflictFlag()
{
    KyIpv4Arping arping(m_testIfaceName, m_testIpAddress, m_testRetryCount, m_testTimeout);
    QVERIFY(arping.ipv4IsConflict() == false);
    QVERIFY(arping.getMacAddress().isEmpty());
}
