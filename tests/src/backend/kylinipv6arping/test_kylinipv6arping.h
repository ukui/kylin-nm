#ifndef TEST_KYLINIPV6ARPING_H
#define TEST_KYLINIPV6ARPING_H

#include <QObject>
#include <QTest>
#include <QString>

class KyIpv6Arping;

class TestKyIpv6Arping : public QObject
{
    Q_OBJECT
public:
    explicit TestKyIpv6Arping(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testConstructorWithParameters();
    void testIpv6IsConflict();
    void testSetIpv6ConflictFlag();
    void testGetConflictMacAddress();
    void testSetAndGetConflictMacAddress();
    void testIpv6ConflictCheck();
};

#endif // TEST_KYLINIPV6ARPING_H
