#ifndef TEST_ITEMFRAME_H
#define TEST_ITEMFRAME_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include "../../../../plugins/netconnect/itemframe.h"

class TestItemFrame : public QObject
{
    Q_OBJECT
public:
    explicit TestItemFrame(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_updateCornerStyle_data();
    void test_updateCornerStyle();
    void test_filletStyleChange_data();
    void test_filletStyleChange();

private:
    void setupTestEnvironment();
    ItemFrame *m_itemFrame;
};

#endif // TEST_ITEMFRAME_H
