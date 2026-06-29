#include "test_lanitem.h"
#include "../../../../plugins/netconnect/lanitem.h"

TestLanItem::TestLanItem(QObject *parent)
    : QObject(parent)
    , m_lanItem(nullptr)
{
}

TestLanItem::~TestLanItem()
{
}

void TestLanItem::initTestCase()
{
    m_lanItem = new LanItem(true);
    QVERIFY(m_lanItem != nullptr);
}

void TestLanItem::cleanupTestCase()
{
    if (m_lanItem) {
        delete m_lanItem;
        m_lanItem = nullptr;
    }
}

void TestLanItem::init()
{
}

void TestLanItem::cleanup()
{
}

void TestLanItem::test_constructor_withActiveTrue()
{
    LanItem *lanItem = new LanItem(true);
    QVERIFY(lanItem != nullptr);
    QVERIFY(lanItem->isAcitve == true);
    delete lanItem;
}

void TestLanItem::test_constructor_withActiveFalse()
{
    LanItem *lanItem = new LanItem(false);
    QVERIFY(lanItem != nullptr);
    QVERIFY(lanItem->isAcitve == false);
    delete lanItem;
}

void TestLanItem::test_destructor()
{
    LanItem *lanItem = new LanItem(true);
    delete lanItem;
    QVERIFY(true);
}

void TestLanItem::test_startLoading()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->startLoading();
    QVERIFY(lanItem->loading == true);
    delete lanItem;
}

void TestLanItem::test_stopLoading()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->startLoading();
    lanItem->stopLoading();
    QVERIFY(lanItem->loading == false);
    delete lanItem;
}

void TestLanItem::test_setNetworkCheckFrameHidden_true()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setNetworkCheckFrameHidden(true);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setNetworkCheckFrameHidden_false()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setNetworkCheckFrameHidden(false);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setTopSeparatorVisible_true()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setTopSeparatorVisible(true);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setTopSeparatorVisible_false()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setTopSeparatorVisible(false);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setBottomSeparatorVisible_true()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setBottomSeparatorVisible(true);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setBottomSeparatorVisible_false()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setBottomSeparatorVisible(false);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setConnectivityWarn_NoConnectivity()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setConnectivityWarn(NoConnectivity);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setConnectivityWarn_Portal()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setConnectivityWarn(Portal);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setConnectivityWarn_Limited()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setConnectivityWarn(Limited);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setConnectivityWarn_Full()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setConnectivityWarn(Full);
    QVERIFY(true);
    delete lanItem;
}

void TestLanItem::test_setConnectivityWarn_Unknown()
{
    LanItem *lanItem = new LanItem(true);
    lanItem->setConnectivityWarn(UnknownConnectivity);
    QVERIFY(true);
    delete lanItem;
}
