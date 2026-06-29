#include "test_itemframe.h"
#include "../../../../plugins/netconnect/deviceframe.h"
#include "../../../../plugins/netconnect/lanitem.h"

TestItemFrame::TestItemFrame(QObject *parent)
    : QObject(parent)
    , m_itemFrame(nullptr)
{}

void TestItemFrame::initTestCase()
{
    setupTestEnvironment();
}

void TestItemFrame::cleanupTestCase()
{
    if (m_itemFrame) {
        delete m_itemFrame;
        m_itemFrame = nullptr;
    }
}

void TestItemFrame::init()
{}

void TestItemFrame::cleanup()
{}

void TestItemFrame::setupTestEnvironment()
{
    m_itemFrame = new ItemFrame("eth0", nullptr);
}

void TestItemFrame::test_constructor()
{
    ItemFrame *frame = new ItemFrame("eth0", nullptr);
    QVERIFY(frame != nullptr);
    QVERIFY(frame->deviceLanLayout != nullptr);
    QVERIFY(frame->deviceFrame != nullptr);
    QVERIFY(frame->lanItemFrame != nullptr);
    QVERIFY(frame->lanItemLayout != nullptr);
    QVERIFY(frame->addLanWidget != nullptr);
    delete frame;
}

void TestItemFrame::test_updateCornerStyle_data()
{
    QTest::addColumn<bool>("hasItems");
    QTest::addColumn<bool>("deviceEnabled");

    QTest::newRow("No items, device disabled") << false << false;
    QTest::newRow("No items, device enabled") << false << true;
    QTest::newRow("Has items, device disabled") << true << false;
    QTest::newRow("Has items, device enabled") << true << true;
}

void TestItemFrame::test_updateCornerStyle()
{
    QFETCH(bool, hasItems);
    QFETCH(bool, deviceEnabled);

    ItemFrame *frame = new ItemFrame("eth0", nullptr);

    if (hasItems) {
        LanItem *item = new LanItem(false, frame);
        frame->lanItemLayout->addWidget(item);
    }

    if (deviceEnabled && frame->deviceFrame && frame->deviceFrame->deviceSwitch) {
        frame->deviceFrame->deviceSwitch->setChecked(true);
    } else if (frame->deviceFrame && frame->deviceFrame->deviceSwitch) {
        frame->deviceFrame->deviceSwitch->setChecked(false);
    }

    frame->updateCornerStyle();

    QVERIFY(frame->m_deviceSeparator != nullptr);
    QVERIFY(frame->m_addSeparator != nullptr);

    delete frame;
}

void TestItemFrame::test_filletStyleChange_data()
{
    QTest::addColumn<int>("itemCount");

    QTest::newRow("Zero items") << 0;
    QTest::newRow("One item") << 1;
    QTest::newRow("Multiple items") << 3;
}

void TestItemFrame::test_filletStyleChange()
{
    QFETCH(int, itemCount);

    ItemFrame *frame = new ItemFrame("eth0", nullptr);

    for (int i = 0; i < itemCount; i++) {
        LanItem *item = new LanItem(false, frame);
        frame->lanItemLayout->addWidget(item);
    }

    frame->filletStyleChange();

    QVERIFY(frame->lanItemLayout->count() == itemCount);

    delete frame;
}
