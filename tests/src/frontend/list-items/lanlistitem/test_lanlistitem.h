#ifndef TEST_LANLISTITEM_H
#define TEST_LANLISTITEM_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include "lanlistitem.h"
#include "kylinconnectitem.h"

class TestLanListItem : public QObject
{
    Q_OBJECT
public:
    explicit TestLanListItem(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructorWithParams();
    void test_constructorDefault();
    void test_updateConnectionState();
    void test_getConnectionName();
    void test_updateConnectionName();
    void test_getConnectionPath();
    void test_updateConnectionPath();
    void test_updateConnectivityText();
    void test_itemHeight();

private:
    LanListItem *m_lanListItem = nullptr;
    KyConnectItem *m_testConnectItem = nullptr;
};

#endif // TEST_LANLISTITEM_H
