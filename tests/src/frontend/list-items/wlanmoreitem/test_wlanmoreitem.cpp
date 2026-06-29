#include "test_wlanmoreitem.h"

TestWlanMoreItem::TestWlanMoreItem(QObject *parent)
    : QObject(parent)
{
}

void TestWlanMoreItem::initTestCase()
{
}

void TestWlanMoreItem::cleanupTestCase()
{
}

void TestWlanMoreItem::init()
{
    m_wlanMoreItem = new WlanMoreItem();
}

void TestWlanMoreItem::cleanup()
{
    if (m_wlanMoreItem) {
        delete m_wlanMoreItem;
        m_wlanMoreItem = nullptr;
    }
}

void TestWlanMoreItem::test_constructor()
{
    QVERIFY(m_wlanMoreItem != nullptr);
    QVERIFY(m_wlanMoreItem->objectName() == WMI_OB_NAME);
}

void TestWlanMoreItem::test_destructor()
{
    delete m_wlanMoreItem;
    m_wlanMoreItem = nullptr;
    QVERIFY(true);
}

void TestWlanMoreItem::test_onNetButtonClicked()
{
    QSignalSpy spy(m_wlanMoreItem, &WlanMoreItem::hiddenWlanClicked);
    m_wlanMoreItem->onNetButtonClicked();
    QVERIFY(spy.count() == 1);
}

void TestWlanMoreItem::test_onNetButtonReleased()
{
    m_wlanMoreItem->onNetButtonReleased();
    QVERIFY(true);
}

void TestWlanMoreItem::test_onMenuTriggered()
{
    QAction *action = new QAction("test", m_wlanMoreItem);
    m_wlanMoreItem->onMenuTriggered(action);
    QVERIFY(true);
    delete action;
}

QTEST_MAIN(TestWlanMoreItem)

#include "test_wlanmoreitem.moc"
