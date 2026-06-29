#ifndef TEST_KYLINIPV4ARPING_H
#define TEST_KYLINIPV4ARPING_H

#include <QObject>
#include <QTest>
#include <QString>

class KyIpv4Arping;

class TestKyIpv4Arping : public QObject
{
    Q_OBJECT
public:
    explicit TestKyIpv4Arping(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testConstructorWithParameters();
    void testIpv4IsConflict();
    void testSetIpv4ConflictFlag();
    void testGetConflictMacAddress();
    void testSetAndGetConflictMacAddress();
    void testIpv4ConflictCheck();
};

#endif // TEST_KYLINIPV4ARPING_H
