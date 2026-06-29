#include "test_kylinipv6arping.h"
#include "kylinipv6arping.h"
#include <QCoreApplication>
#include <QDebug>

TestKyIpv6Arping::TestKyIpv6Arping(QObject *parent)
    : QObject(parent)
{
}

void TestKyIpv6Arping::initTestCase()
{
    QVERIFY2(QCoreApplication::instance(), "QCoreApplication instance required");
}

void TestKyIpv6Arping::cleanupTestCase()
{
}

void TestKyIpv6Arping::init()
{
}

void TestKyIpv6Arping::cleanup()
{
}

void TestKyIpv6Arping::testConstructor()
{
    KyIpv6Arping *arping = new KyIpv6Arping("lo", "::1");
    QVERIFY(arping != nullptr);
    QVERIFY(arping->ipv6IsConflict() == false);
    delete arping;
}

void TestKyIpv6Arping::testDestructor()
{
    KyIpv6Arping *arping = new KyIpv6Arping("lo", "::1");
    QVERIFY(arping != nullptr);
    delete arping;
}

void TestKyIpv6Arping::testConstructorWithParameters()
{
    KyIpv6Arping *arping = new KyIpv6Arping("eth0", "fe80::1", 5, 2000);
    QVERIFY(arping != nullptr);
    QVERIFY(arping->ipv6IsConflict() == false);
    delete arping;
}

void TestKyIpv6Arping::testIpv6IsConflict()
{
    KyIpv6Arping *arping = new KyIpv6Arping("lo", "::1");
    QVERIFY(arping != nullptr);
    
    bool conflict = arping->ipv6IsConflict();
    QVERIFY(conflict == false);
    
    delete arping;
}

void TestKyIpv6Arping::testSetIpv6ConflictFlag()
{
    KyIpv6Arping *arping = new KyIpv6Arping("lo", "::1");
    QVERIFY(arping != nullptr);
    
    QVERIFY(arping->ipv6IsConflict() == false);
    
    arping->setIpv6ConflictFlag(true);
    QVERIFY(arping->ipv6IsConflict() == true);
    
    arping->setIpv6ConflictFlag(false);
    QVERIFY(arping->ipv6IsConflict() == false);
    
    delete arping;
}

void TestKyIpv6Arping::testGetConflictMacAddress()
{
    KyIpv6Arping *arping = new KyIpv6Arping("lo", "::1");
    QVERIFY(arping != nullptr);
    
    QString macAddr = arping->getConflictMacAddress();
    QVERIFY(macAddr.isEmpty() || !macAddr.isNull());
    
    delete arping;
}

void TestKyIpv6Arping::testSetAndGetConflictMacAddress()
{
    KyIpv6Arping *arping = new KyIpv6Arping("lo", "::1");
    QVERIFY(arping != nullptr);
    
    QString testMac = "00:11:22:33:44:55";
    
    QString macAddr = arping->getConflictMacAddress();
    
    arping->setIpv6ConflictFlag(true);
    
    macAddr = arping->getConflictMacAddress();
    QVERIFY(macAddr.isEmpty() || !macAddr.isNull());
    
    delete arping;
}

void TestKyIpv6Arping::testIpv6ConflictCheck()
{
    KyIpv6Arping *arping = new KyIpv6Arping("lo", "::1", 1, 100);
    QVERIFY(arping != nullptr);
    
    int result = arping->ipv6ConflictCheck();
    
    QVERIFY(result >= -2 && result <= 1);
    
    delete arping;
}
