#include "test_drownlabel.h"
#include "drownlabel.h"
#include <QDebug>
#include <QMouseEvent>

TestDrownLabel::TestDrownLabel(QObject *parent)
    : QObject(parent)
{
}

TestDrownLabel::~TestDrownLabel()
{
}

void TestDrownLabel::initTestCase()
{
}

void TestDrownLabel::cleanupTestCase()
{
}

void TestDrownLabel::init()
{
}

void TestDrownLabel::cleanup()
{
}

void TestDrownLabel::test_constructor()
{
    QString devName = "TestDevice";
    
    DrownLabel *label = new DrownLabel(devName, nullptr);
    
    QVERIFY(label != nullptr);
    QVERIFY(label->m_devName == devName);
    QVERIFY(label->isChecked == true);
    
    QVERIFY(label->width() == 36);
    QVERIFY(label->height() == 36);
    
    QVariant useIconHighlightEffect = label->property("useIconHighlightEffect");
    QVERIFY(useIconHighlightEffect.isValid());
    QVERIFY(useIconHighlightEffect.toInt() == 0x2);
    
    delete label;
    
    label = new DrownLabel("AnotherDevice");
    QVERIFY(label != nullptr);
    QVERIFY(label->m_devName == "AnotherDevice");
    delete label;
}

void TestDrownLabel::test_setDropDownStatus()
{
    DrownLabel *label = new DrownLabel("TestDevice", nullptr);
    
    QVERIFY(label->isChecked == true);
    
    label->setDropDownStatus(false);
    QVERIFY(label->isChecked == false);
    
    label->setDropDownStatus(true);
    QVERIFY(label->isChecked == true);
    
    label->setDropDownStatus(false);
    QVERIFY(label->isChecked == false);
    
    delete label;
}

void TestDrownLabel::test_mouseReleaseEvent()
{
    DrownLabel *label = new DrownLabel("TestDevice", nullptr);
    
    QSignalSpy spy(label, SIGNAL(labelClicked()));
    
    QMouseEvent *event = new QMouseEvent(
        QEvent::MouseButtonRelease,
        QPointF(10, 10),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );
    
    QCoreApplication::postEvent(label, event);
    
    QVERIFY(spy.wait(1000));
    
    QVERIFY(spy.count() == 1);
    
    QList<QVariant> args = spy.takeFirst();
    QVERIFY(args.count() == 0);
    
    spy.clear();
    
    QMouseEvent *event2 = new QMouseEvent(
        QEvent::MouseButtonRelease,
        QPointF(20, 20),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier
    );
    
    QCoreApplication::postEvent(label, event2);
    
    QVERIFY(spy.wait(1000));
    QVERIFY(spy.count() == 1);
    
    delete label;
}

void TestDrownLabel::test_memberVariables()
{
    DrownLabel *label = new DrownLabel("TestDevice", nullptr);
    
    label->m_devName = "NewDeviceName";
    QVERIFY(label->m_devName == "NewDeviceName");
    
    label->isChecked = false;
    QVERIFY(label->isChecked == false);
    
    label->isChecked = true;
    QVERIFY(label->isChecked == true);
    
    delete label;
}
