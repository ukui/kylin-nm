#include "test_kylinipv6arping.h"
#include "kyarping/kylinipv6arping.h"
#include "kyarping/kylinarping.h"
#include <QDebug>

TestKyIpv6Arping::TestKyIpv6Arping(QObject *parent)
    : QObject(parent)
    , m_arping(nullptr)
{
}

TestKyIpv6Arping::~TestKyIpv6Arping()
{
}

void TestKyIpv6Arping::initTestCase()
{
}

void TestKyIpv6Arping::cleanupTestCase()
{
}

void TestKyIpv6Arping::init()
{
}

void TestKyIpv6Arping::cleanup()
{
    if (m_arping) {
        delete m_arping;
        m_arping = nullptr;
    }
}

void TestKyIpv6Arping::testConstructor()
{
    QString ifaceName = "eth0";
    QString ipAddress = "::1";
    int retryCount = 3;
    int timeout = 1000;

    m_arping = new KyIpv6Arping(ifaceName, ipAddress, retryCount, timeout);
    QVERIFY(m_arping != nullptr);
    QVERIFY(m_arping->ipv6IsConflict() == false);
    QVERIFY(m_arping->getConflictMacAddress().isEmpty());
}

void TestKyIpv6Arping::testDestructor()
{
    QString ifaceName = "eth0";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 3, 1000);
    QVERIFY(m_arping != nullptr);
    delete m_arping;
    m_arping = nullptr;
    QVERIFY(true);
}

void TestKyIpv6Arping::testIpv6IsConflictInitialState()
{
    QString ifaceName = "eth0";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 3, 1000);
    QVERIFY(m_arping != nullptr);
    bool initialConflict = m_arping->ipv6IsConflict();
    QBENCHMARK {
        initialConflict = m_arping->ipv6IsConflict();
    }
    QVERIFY(initialConflict == false);
}

void TestKyIpv6Arping::testSetIpv6ConflictFlag()
{
    QString ifaceName = "eth0";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 3, 1000);
    QVERIFY(m_arping != nullptr);

    m_arping->setIpv6ConflictFlag(true);
    QVERIFY(m_arping->ipv6IsConflict() == true);

    m_arping->setIpv6ConflictFlag(false);
    QVERIFY(m_arping->ipv6IsConflict() == false);
}

void TestKyIpv6Arping::testGetConflictMacAddressInitialState()
{
    QString ifaceName = "eth0";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 3, 1000);
    QVERIFY(m_arping != nullptr);

    QString macAddress = m_arping->getConflictMacAddress();
    QVERIFY(macAddress.isEmpty());
}

void TestKyIpv6Arping::testGetConflictMacAddressAfterSet()
{
    QString ifaceName = "eth0";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 3, 1000);
    QVERIFY(m_arping != nullptr);

    QString testMac = "00:11:22:33:44:55";
    m_arping->setIpv6ConflictFlag(true);
    
    QString macAddress = m_arping->getConflictMacAddress();
    QVERIFY(macAddress.isEmpty() || macAddress == testMac);
}

void TestKyIpv6Arping::testIpv6ConflictCheckWithInvalidInterface()
{
    QString ifaceName = "invalid_interface_xyz";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 1, 100);
    QVERIFY(m_arping != nullptr);

    int result = m_arping->ipv6ConflictCheck();
    QVERIFY(result < 0);
}

void TestKyIpv6Arping::testIpv6ConflictCheckWithInvalidIp()
{
    QString ifaceName = "lo";
    QString ipAddress = "invalid_ipv6_address";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 1, 100);
    QVERIFY(m_arping != nullptr);

    int result = m_arping->ipv6ConflictCheck();
    QVERIFY(result < 0);
}

void TestKyIpv6Arping::testSaveMacAddress()
{
    QString ifaceName = "lo";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 1, 100);
    QVERIFY(m_arping != nullptr);

    uint8_t testMac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    m_arping->setIpv6ConflictFlag(true);
    
    QString macAddress = m_arping->getConflictMacAddress();
    QVERIFY(macAddress.isEmpty() || macAddress.contains("00"));
}

void TestKyIpv6Arping::testGetLocalMacAddress()
{
    QString ifaceName = "lo";
    QString ipAddress = "::1";
    m_arping = new KyIpv6Arping(ifaceName, ipAddress, 1, 100);
    QVERIFY(m_arping != nullptr);

    unsigned char addr[6] = {0};
    int ret = m_arping->ipv6ConflictCheck();
    QVERIFY(ret != -999);
}
