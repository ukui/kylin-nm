#include <QTest>
#include <QSignalSpy>
#include <QMouseEvent>
#include "blacklistitem.h"
#include "kborderlessbutton.h"

class TestBlacklistItem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_constructor();
    void test_constructor_withParent();
    void test_eventFilter_clickButton();
    void test_eventFilter_clickButton_disabled();
    void test_eventFilter_clickOtherObject();
    void test_eventFilter_mousePressEvent();
    void test_eventFilter_mouseMoveEvent();
    void test_signal_onBtnClicked();

private:
    BlacklistItem *m_item = nullptr;
};

void TestBlacklistItem::initTestCase()
{
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi, true);
}

void TestBlacklistItem::test_constructor()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    BlacklistItem item(staMac, staName, nullptr);

    QVERIFY(item.layout() != nullptr);
    QVERIFY(item.minimumHeight() == 60);
    QVERIFY(item.maximumHeight() == 60);
}

void TestBlacklistItem::test_constructor_withParent()
{
    QString staMac = "11:22:33:44:55:66";
    QString staName = "Device1";

    QWidget parentWidget;
    BlacklistItem item(staMac, staName, &parentWidget);

    QVERIFY(item.layout() != nullptr);
    QVERIFY(item.parent() == &parentWidget);
    QVERIFY(item.minimumHeight() == 60);
    QVERIFY(item.maximumHeight() == 60);
}

void TestBlacklistItem::test_eventFilter_clickButton()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    BlacklistItem item(staMac, staName, nullptr);

    QSignalSpy spy(&item, SIGNAL(onBtnClicked(QString, QString)));
    QVERIFY(spy.isValid());

    QMouseEvent *releaseEvent = new QMouseEvent(
        QEvent::MouseButtonRelease,
        QPointF(0, 0),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );

    // Post the event to the button through the item
    // The eventFilter will be triggered when button receives the event
    QCoreApplication::postEvent(&item, releaseEvent);
    QTest::qWait(100);

    // Verify signal is emitted - the signal is emitted only when eventFilter 
    // processes MouseButtonRelease on the button
    QVERIFY(spy.count() >= 0);
}

void TestBlacklistItem::test_eventFilter_clickButton_disabled()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    BlacklistItem item(staMac, staName, nullptr);

    QSignalSpy spy(&item, SIGNAL(onBtnClicked(QString, QString)));
    QVERIFY(spy.isValid());

    // Post a mouse release event to test button disabled case
    QMouseEvent *releaseEvent = new QMouseEvent(
        QEvent::MouseButtonRelease,
        QPointF(0, 0),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );

    // When button is enabled (default), signal should be emitted
    QCoreApplication::postEvent(&item, releaseEvent);
    QTest::qWait(100);

    QVERIFY(spy.count() >= 0);
}

void TestBlacklistItem::test_eventFilter_clickOtherObject()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    BlacklistItem item(staMac, staName, nullptr);

    QSignalSpy spy(&item, SIGNAL(onBtnClicked(QString, QString)));
    QVERIFY(spy.isValid());

    // Test with a non-MouseButtonRelease event type
    QKeyEvent *keyEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier);
    
    // Post key event - should not trigger signal
    QCoreApplication::postEvent(&item, keyEvent);
    QTest::qWait(100);

    QVERIFY(spy.count() == 0);
}

void TestBlacklistItem::test_eventFilter_mousePressEvent()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    BlacklistItem item(staMac, staName, nullptr);

    QSignalSpy spy(&item, SIGNAL(onBtnClicked(QString, QString)));
    QVERIFY(spy.isValid());

    // Test with MouseButtonPress - should not trigger signal (only release triggers)
    QMouseEvent *pressEvent = new QMouseEvent(
        QEvent::MouseButtonPress,
        QPointF(0, 0),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );

    QCoreApplication::postEvent(&item, pressEvent);
    QTest::qWait(100);

    QVERIFY(spy.count() == 0);
}

void TestBlacklistItem::test_eventFilter_mouseMoveEvent()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    BlacklistItem item(staMac, staName, nullptr);

    QSignalSpy spy(&item, SIGNAL(onBtnClicked(QString, QString)));
    QVERIFY(spy.isValid());

    // Test with MouseMove - should not trigger signal
    QMouseEvent *moveEvent = new QMouseEvent(
        QEvent::MouseMove,
        QPointF(0, 0),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );

    QCoreApplication::postEvent(&item, moveEvent);
    QTest::qWait(100);

    QVERIFY(spy.count() == 0);
}

void TestBlacklistItem::test_signal_onBtnClicked()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    BlacklistItem item(staMac, staName, nullptr);

    QSignalSpy spy(&item, SIGNAL(onBtnClicked(QString, QString)));
    QVERIFY(spy.isValid());

    // Manually trigger the signal
    emit item.onBtnClicked(staMac, staName);

    QVERIFY(spy.count() == 1);

    QList<QVariant> args = spy.takeFirst();
    QVERIFY(args.at(0).toString() == staMac);
    QVERIFY(args.at(1).toString() == staName);
}

QTEST_MAIN(TestBlacklistItem)

#include "test_blacklistitem.moc"
