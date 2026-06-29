#ifndef TEST_WLANMOREITEM_H
#define TEST_WLANMOREITEM_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include "wlanmoreitem.h"

class TestWlanMoreItem : public QObject
{
    Q_OBJECT
public:
    explicit TestWlanMoreItem(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_onNetButtonClicked();
    void test_onNetButtonReleased();
    void test_onMenuTriggered();

private:
    WlanMoreItem *m_wlanMoreItem = nullptr;
};

#endif // TEST_WLANMOREITEM_H
