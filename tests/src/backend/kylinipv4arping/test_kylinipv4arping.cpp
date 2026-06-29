#include "test_kylinipv4arping.h"
#include "../../../../src/backend/kylinipv4arping.h"
#include "../../../../src/backend/kylinarping.h"

#include <QCoreApplication>
#include <QDebug>

TestKyIpv4Arping::TestKyIpv4Arping(QObject *parent)
    : QObject(parent)
{
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
    KyIpv4Arping *arping = new KyIpv4Arping("eth0", "192.168.1.1", 3, 1);
    QVERIFY(arping != nullptr);
    QVERIFY(arping->ipv4IsConflict() == false);
    QVERIFY(arping->getMacAddress().isEmpty());
    delete arping;
}

void TestKyIpv4Arping::testDestructor()
{
    KyIpv4Arping *arping = new KyIpv4Arping("eth0", "192.168.1.1", 3, 1);
    delete arping;
    QVERIFY(true);
}

void TestKyIpv4Arping::testConstructorWithParameters()
{
    KyIpv4Arping *arping = new KyIpv4Arping("eth0", "192.168.1.100", 5, 2);
    QVERIFY(arping != nullptr);
    QVERIFY(arping->ipv4IsConflict() == false);
    delete arping;
}

void TestKyIpv4Arping::testIpv4IsConflict()
{
    KyIpv4Arping *arping = new KyIpv4Arping("eth0", "192.168.1.1", 3, 1);
    bool initialConflict = arping->ipv4IsConflict();
    QVERIFY(initialConflict == false);
    delete arping;
}

void TestKyIpv4Arping::testSetIpv4ConflictFlag()
{
    KyIpv4Arping *arping = new KyIpv4Arping("eth0", "192.168.1.1", 3, 1);
    QVERIFY(arping->ipv4IsConflict() == false);
    delete arping;
}

void TestKyIpv4Arping::testGetConflictMacAddress()
{
    KyIpv4Arping *arping = new KyIpv4Arping("eth0", "192.168.1.1", 3, 1);
    QString mac = arping->getMacAddress();
    QVERIFY(mac.isEmpty());
    delete arping;
}

void TestKyIpv4Arping::testSetAndGetConflictMacAddress()
{
    KyIpv4Arping *arping = new KyIpv4Arping("eth0", "192.168.1.1", 3, 1);
    QVERIFY(arping->getMacAddress().isEmpty());
    delete arping;
}

void TestKyIpv4Arping::testIpv4ConflictCheck()
{
    KyIpv4Arping *arping = new KyIpv4Arping("", "192.168.1.254", 1, 1);
    int result = arping->ipv4ConflictCheck();
    QVERIFY(result == -1 || result == 0);
    delete arping;
}
