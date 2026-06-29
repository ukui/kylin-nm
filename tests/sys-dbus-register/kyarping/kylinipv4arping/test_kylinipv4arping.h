#ifndef TEST_KYLINIPV4ARPING_H
#define TEST_KYLINIPV4ARPING_H

#include <QString>
#include <QTest>
#include <QObject>

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
    void testIpv4IsConflict_default();
    void testIpv4IsConflict_afterSet();
    void testGetMacAddress_default();
    void testGetMacAddress_afterSet();
    void testSetConflictFlag();

private:
    QString m_testIfaceName;
    QString m_testIpAddress;
    int m_testRetryCount;
    int m_testTimeout;
};

#endif // TEST_KYLINIPV4ARPING_H
