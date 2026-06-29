#include <QApplication>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>
#include "itemframe.h"

class TestItemFrame : public QObject
{
    Q_OBJECT
public:
    TestItemFrame() {}

private slots:
    void testConstructor();
    void testUpdateCornerStyleWithNoItems();
    void testUpdateCornerStyleWithItems();
    void testFilletStyleChange();
};

void TestItemFrame::testConstructor()
{
    ItemFrame *frame = new ItemFrame(nullptr);
    QVERIFY2(frame != nullptr, "Failed to create ItemFrame instance");
    QVERIFY(frame->m_mainVLayout != nullptr);
    QVERIFY(frame->m_vpnFrame != nullptr);
    QVERIFY(frame->m_vpnVLayout != nullptr);
    QVERIFY(frame->m_addVpnWidget != nullptr);
    QVERIFY(frame->m_itemMap.isEmpty());
    delete frame;
}

void TestItemFrame::testUpdateCornerStyleWithNoItems()
{
    ItemFrame *frame = new ItemFrame(nullptr);

    QVERIFY(frame->m_vpnVLayout->count() == 0);

    frame->updateCornerStyle();

    delete frame;
}

void TestItemFrame::testUpdateCornerStyleWithItems()
{
    ItemFrame *frame = new ItemFrame(nullptr);

    VpnItem *item1 = new VpnItem(false, frame);
    frame->m_vpnVLayout->addWidget(item1);
    frame->m_itemMap.insert("item1", item1);

    QVERIFY(frame->m_vpnVLayout->count() > 0);

    frame->updateCornerStyle();

    delete frame;
}

void TestItemFrame::testFilletStyleChange()
{
    ItemFrame *frame = new ItemFrame(nullptr);

    VpnItem *item1 = new VpnItem(false, frame);
    frame->m_vpnVLayout->addWidget(item1);
    frame->m_itemMap.insert("item1", item1);

    VpnItem *item2 = new VpnItem(false, frame);
    frame->m_vpnVLayout->addWidget(item2);
    frame->m_itemMap.insert("item2", item2);

    VpnItem *item3 = new VpnItem(false, frame);
    frame->m_vpnVLayout->addWidget(item3);
    frame->m_itemMap.insert("item3", item3);

    frame->filletStyleChange();

    delete frame;
}

QTEST_MAIN(TestItemFrame)
#include "test_itemframe.moc"
