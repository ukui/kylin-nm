#include <QTest>
#include <QCoreApplication>
#include <QSignalSpy>
#include "itemframe.h"

class TestItemFrame : public QObject
{
    Q_OBJECT
public:
    explicit TestItemFrame(QObject *parent = nullptr) : QObject(parent) {}

private slots:
    void initTestCase();
    void testConstructor();
    void testDestructor();
    void testOnDrownLabelClicked_data();
    void testOnDrownLabelClicked();
    void testFilletStyleChange_data();
    void testFilletStyleChange();

private:
    ItemFrame *m_itemFrame = nullptr;
};

void TestItemFrame::initTestCase()
{
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi, true);
}

void TestItemFrame::testConstructor()
{
    QString devName = "wlan0";
    ItemFrame frame(devName);

    QVERIFY(frame.deviceLanLayout != nullptr);
    QVERIFY(frame.lanItemFrame != nullptr);
    QVERIFY(frame.lanItemLayout != nullptr);
    QVERIFY(frame.deviceFrame != nullptr);
    QVERIFY(frame.addWlanWidget != nullptr);
    QVERIFY(frame.uuid == "");
}

void TestItemFrame::testDestructor()
{
    QString devName = "wlan0";
    ItemFrame *frame = new ItemFrame(devName);
    QVERIFY(frame != nullptr);
    delete frame;
}

void TestItemFrame::testOnDrownLabelClicked_data()
{
    QTest::addColumn<bool>("initialChecked");

    QTest::newRow("dropdown initially unchecked - should show") << false;
    QTest::newRow("dropdown initially checked - should hide") << true;
}

void TestItemFrame::testOnDrownLabelClicked()
{
    QString devName = "wlan0";
    ItemFrame frame(devName);

    QFETCH(bool, initialChecked);
    frame.deviceFrame->dropDownLabel->isChecked = initialChecked;

    bool wasVisible = frame.lanItemFrame->isVisible();

    // Emit the signal that triggers onDrownLabelClicked
    emit frame.deviceFrame->dropDownLabel->labelClicked();

    bool dropDownStatus = frame.deviceFrame->dropDownLabel->isChecked;

    if (!initialChecked) {
        QVERIFY(dropDownStatus == true);
    } else {
        QVERIFY(dropDownStatus == false);
    }
}

void TestItemFrame::testFilletStyleChange_data()
{
    QTest::addColumn<int>("itemCount");

    QTest::newRow("empty layout") << 0;
    QTest::newRow("single item") << 1;
    QTest::newRow("multiple items") << 3;
}

void TestItemFrame::testFilletStyleChange()
{
    QString devName = "wlan0";
    ItemFrame frame(devName);

    QFETCH(int, itemCount);

    for (int i = 0; i < itemCount; ++i) {
        WlanItem *item = new WlanItem(false, false, &frame);
        frame.lanItemLayout->addWidget(item);
    }

    frame.filletStyleChange();
}

QTEST_MAIN(TestItemFrame)
#include "test_itemframe.moc"
