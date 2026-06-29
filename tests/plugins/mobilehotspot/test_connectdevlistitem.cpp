#include <QTest>
#include <QSignalSpy>
#include <QMouseEvent>
#include "connectdevlistitem.h"
#include "kborderlessbutton.h"

class TestConnectDevListItem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConstructor();
    void testConstructorWithParent();
    void testConstructorEmptyName();
    void testSignalEmission();

private:
};

void TestConnectDevListItem::initTestCase()
{
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi, true);
}

void TestConnectDevListItem::testConstructor()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    ConnectDevListItem item(staMac, staName, nullptr);

    QVERIFY(item.layout() != nullptr);
    QVERIFY(item.minimumWidth() == 550);
    QVERIFY(item.minimumHeight() == 60);
    QVERIFY(item.maximumWidth() == 16777215);
    QVERIFY(item.height() == 60);
}

void TestConnectDevListItem::testConstructorWithParent()
{
    QString staMac = "11:22:33:44:55:66";
    QString staName = "Device1";
    
    QWidget parentWidget;
    ConnectDevListItem item(staMac, staName, &parentWidget);

    QVERIFY(item.layout() != nullptr);
    QVERIFY(item.parent() == &parentWidget);
    QVERIFY(item.minimumWidth() == 550);
    QVERIFY(item.minimumHeight() == 60);
}

void TestConnectDevListItem::testConstructorEmptyName()
{
    QString staMac = "00:11:22:33:44:55";
    QString staName = "";

    ConnectDevListItem item(staMac, staName, nullptr);

    QVERIFY(item.layout() != nullptr);
    QVERIFY(item.minimumWidth() == 550);
    QVERIFY(item.minimumHeight() == 60);
}

void TestConnectDevListItem::testSignalEmission()
{
    QString staMac = "AA:BB:CC:DD:EE:FF";
    QString staName = "TestDevice";

    ConnectDevListItem item(staMac, staName, nullptr);

    QSignalSpy spy(&item, SIGNAL(onBtnClicked(QString, QString)));
    QVERIFY(spy.isValid());

    QMouseEvent *releaseEvent = new QMouseEvent(
        QEvent::MouseButtonRelease,
        QPointF(0, 0),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );

    QCoreApplication::postEvent(&item, releaseEvent);
    QTest::qWait(100);

    QVERIFY(spy.count() == 1);

    QList<QVariant> args = spy.takeFirst();
    QVERIFY(args.at(0).toString() == staMac);
    QVERIFY(args.at(1).toString() == staName);
}

QTEST_MAIN(TestConnectDevListItem)

#include "test_connectdevlistitem.moc"
