#ifndef TEST_KYLINCONNECTITEM_H
#define TEST_KYLINCONNECTITEM_H

#include <QObject>
#include <QString>
#include <QTest>

class TestKyConnectItem : public QObject
{
    Q_OBJECT
public:
    explicit TestKyConnectItem(QObject *parent = nullptr);

private Q_SLOTS:
    void testDefaultConstructor();
    void testSetConnectUuid();
    void testSetConnectUuidWithEmptyString();
    void testSetConnectUuidWithValidUuid();
    void testDumpInfo();
};

#endif // TEST_KYLINCONNECTITEM_H
