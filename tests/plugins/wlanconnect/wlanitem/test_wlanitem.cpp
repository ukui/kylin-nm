#include "test_wlanitem.h"
#include "wlanitem.h"

TestWlanItem::TestWlanItem(QObject *parent)
    : QObject(parent)
    , m_parentWidget(nullptr)
{
}

void TestWlanItem::initTestCase()
{
    m_parentWidget = new QWidget();
}

void TestWlanItem::cleanupTestCase()
{
    delete m_parentWidget;
    m_parentWidget = nullptr;
}

void TestWlanItem::init()
{
}

void TestWlanItem::cleanup()
{
}

void TestWlanItem::test_constructor()
{
    WlanItem *item1 = new WlanItem(true, false, m_parentWidget);
    QVERIFY(item1 != nullptr);
    QVERIFY(item1->isAcitve == true);
    QVERIFY(item1->isLock == false);
    QVERIFY(item1->loading == false);
    QVERIFY(item1->iconLabel != nullptr);
    QVERIFY(item1->titileLabel != nullptr);
    QVERIFY(item1->statusLabel != nullptr);
    QVERIFY(item1->infoLabel != nullptr);
    delete item1;

    WlanItem *item2 = new WlanItem(false, true, m_parentWidget);
    QVERIFY(item2->isAcitve == false);
    QVERIFY(item2->isLock == true);
    delete item2;

    WlanItem *item3 = new WlanItem(false, false, nullptr);
    QVERIFY(item3->parent() == nullptr);
    delete item3;
}

void TestWlanItem::test_destructor()
{
    WlanItem *item = new WlanItem(false, false, m_parentWidget);
    delete item;
    QVERIFY(true);
}

void TestWlanItem::test_setHalfFillet()
{
    WlanItem *item = new WlanItem(false, false, m_parentWidget);

    item->setHalfFillet(true);
    item->repaint();

    item->setHalfFillet(false);
    item->repaint();

    delete item;
}

void TestWlanItem::test_startLoading()
{
    WlanItem *item = new WlanItem(false, false, m_parentWidget);

    QVERIFY(item->loading == false);
    item->startLoading();
    QVERIFY(item->loading == true);

    delete item;
}

void TestWlanItem::test_stopLoading()
{
    WlanItem *item = new WlanItem(false, false, m_parentWidget);

    item->startLoading();
    QVERIFY(item->loading == true);

    item->stopLoading();
    QVERIFY(item->loading == false);

    delete item;
}

void TestWlanItem::test_updateIcon()
{
    WlanItem *item = new WlanItem(false, false, m_parentWidget);

    item->startLoading();

    QTest::qWait(200);

    QVERIFY(item->loading == true);

    item->stopLoading();

    delete item;
}

void TestWlanItem::test_paintEvent()
{
    WlanItem *item = new WlanItem(false, false, m_parentWidget);
    item->show();

    QTest::qWait(100);

    delete item;
}
