#ifndef TEST_KYLINIPV6ARPING_H
#define TEST_KYLINIPV6ARPING_H

#include <QObject>
#include <QString>
#include <QTest>

class KyIpv6Arping;

class TestKyIpv6Arping : public QObject
{
    Q_OBJECT

public:
    explicit TestKyIpv6Arping(QObject *parent = nullptr);
    ~TestKyIpv6Arping();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testIpv6IsConflictInitialState();
    void testSetIpv6ConflictFlag();
    void testGetConflictMacAddressInitialState();
    void testGetConflictMacAddressAfterSet();
    void testIpv6ConflictCheckWithInvalidInterface();
    void testIpv6ConflictCheckWithInvalidIp();
    void testSaveMacAddress();
    void testGetLocalMacAddress();

private:
    KyIpv6Arping *m_arping;
};

#endif // TEST_KYLINIPV6ARPING_H
