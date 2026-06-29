#include "test_switchbutton.h"
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QCoreApplication>

TestSwitchButton::TestSwitchButton(QObject *parent)
    : QObject(parent)
{
}

void TestSwitchButton::initTestCase()
{
}

void TestSwitchButton::cleanupTestCase()
{
}

void TestSwitchButton::init()
{
}

void TestSwitchButton::cleanup()
{
}

void TestSwitchButton::test_constructor()
{
    SwitchButton *btn = new SwitchButton();
    QVERIFY(btn != nullptr);
    QVERIFY(btn->isChecked() == false);
    QVERIFY(btn->getDisabledFlag() == false);
    delete btn;
}

void TestSwitchButton::test_constructorWithParams()
{
    SwitchButton *btn = new SwitchButton(nullptr, false);
    QVERIFY(btn != nullptr);
    QVERIFY(btn->isChecked() == false);
    QVERIFY(btn->getDisabledFlag() == false);
    delete btn;

    btn = new SwitchButton(nullptr, true);
    QVERIFY(btn != nullptr);
    delete btn;
}

void TestSwitchButton::test_setChecked()
{
    SwitchButton *btn = new SwitchButton();
    QVERIFY(btn->isChecked() == false);

    btn->setChecked(true);
    QTRY_VERIFY(btn->isChecked() == true);

    btn->setChecked(false);
    QTRY_VERIFY(btn->isChecked() == false);

    delete btn;
}

void TestSwitchButton::test_setCheckedToSameValue()
{
    SwitchButton *btn = new SwitchButton();
    btn->setChecked(true);
    QCoreApplication::processEvents();
    QTRY_VERIFY(btn->isChecked() == true);

    QSignalSpy spy(btn, SIGNAL(checkedChanged(bool)));
    btn->setChecked(true);
    QCoreApplication::processEvents();
    QVERIFY(spy.count() == 0);

    delete btn;
}

void TestSwitchButton::test_setCheckedEmitsSignal()
{
    SwitchButton *btn = new SwitchButton();
    QSignalSpy spy(btn, SIGNAL(checkedChanged(bool)));

    btn->setChecked(true);
    QCoreApplication::processEvents();
    QTRY_VERIFY(spy.count() >= 1);

    delete btn;
}

void TestSwitchButton::test_setAnimation()
{
    SwitchButton *btn = new SwitchButton();
    btn->setAnimation(false);
    btn->setAnimation(true);
    delete btn;
}

void TestSwitchButton::test_isChecked()
{
    SwitchButton *btn = new SwitchButton();
    QVERIFY(btn->isChecked() == false);

    btn->setChecked(true);
    QTRY_VERIFY(btn->isChecked() == true);

    btn->setChecked(false);
    QTRY_VERIFY(btn->isChecked() == false);

    delete btn;
}

void TestSwitchButton::test_setDisabledFlag()
{
    SwitchButton *btn = new SwitchButton();
    QVERIFY(btn->getDisabledFlag() == false);

    btn->setDisabledFlag(true);
    QVERIFY(btn->getDisabledFlag() == true);

    btn->setDisabledFlag(false);
    QVERIFY(btn->getDisabledFlag() == false);

    delete btn;
}

void TestSwitchButton::test_getDisabledFlag()
{
    SwitchButton *btn = new SwitchButton();
    QVERIFY(btn->getDisabledFlag() == false);

    btn->setDisabledFlag(true);
    QVERIFY(btn->getDisabledFlag() == true);

    delete btn;
}

void TestSwitchButton::test_mousePressEvent_data()
{
    QTest::addColumn<bool>("disabled");
    QTest::addColumn<bool>("isMoving");
    QTest::addColumn<bool>("expectedChecked");

    QTest::newRow("enabled and not moving") << false << false << true;
    QTest::newRow("disabled") << true << false << false;
}

void TestSwitchButton::test_mousePressEvent()
{
    QFETCH(bool, disabled);
    QFETCH(bool, isMoving);
    QFETCH(bool, expectedChecked);

    SwitchButton *btn = new SwitchButton();
    btn->setDisabledFlag(disabled);

    QSignalSpy spy(btn, SIGNAL(checkedChanged(bool)));

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(btn, &pressEvent);

    QCoreApplication::processEvents();

    if (disabled) {
        QSignalSpy disabledSpy(btn, SIGNAL(disabledClick()));
        QVERIFY(disabledSpy.count() >= 1);
    } else {
        if (!isMoving && !disabled) {
            QTRY_VERIFY(spy.count() >= 1);
        }
    }

    delete btn;
}

void TestSwitchButton::test_mousePressEventWhenDisabled()
{
    SwitchButton *btn = new SwitchButton();
    btn->setDisabledFlag(true);

    QSignalSpy disabledClickSpy(btn, SIGNAL(disabledClick()));
    QSignalSpy checkedChangedSpy(btn, SIGNAL(checkedChanged(bool)));

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(btn, &pressEvent);
    QCoreApplication::processEvents();

    QVERIFY(disabledClickSpy.count() >= 1);
    QVERIFY(checkedChangedSpy.count() == 0);
    QVERIFY(btn->isChecked() == false);

    delete btn;
}

void TestSwitchButton::test_mousePressEventEmitsSignal()
{
    SwitchButton *btn = new SwitchButton();
    QSignalSpy spy(btn, SIGNAL(checkedChanged(bool)));

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(btn, &pressEvent);
    QCoreApplication::processEvents();

    QTRY_VERIFY(spy.count() >= 1);

    delete btn;
}

void TestSwitchButton::test_resizeEvent()
{
    SwitchButton *btn = new SwitchButton();
    btn->setChecked(false);

    QResizeEvent resizeEvent(QSize(60, 28), QSize(50, 24));
    QApplication::sendEvent(btn, &resizeEvent);
    QCoreApplication::processEvents();

    delete btn;
}

void TestSwitchButton::test_resizeEventWhenChecked()
{
    SwitchButton *btn = new SwitchButton();
    btn->setChecked(true);
    QCoreApplication::processEvents();

    QResizeEvent resizeEvent(QSize(60, 28), QSize(50, 24));
    QApplication::sendEvent(btn, &resizeEvent);
    QCoreApplication::processEvents();

    delete btn;
}

void TestSwitchButton::test_paintEvent()
{
    SwitchButton *btn = new SwitchButton();

    QPaintEvent paintEvent(btn->rect());
    QApplication::sendEvent(btn, &paintEvent);
    QCoreApplication::processEvents();

    delete btn;
}

void TestSwitchButton::test_enterEvent()
{
    SwitchButton *btn = new SwitchButton();

    QEnterEvent enterEvent(QPoint(10, 10), QPoint(10, 10), QPoint(10, 10));
    QApplication::sendEvent(btn, &enterEvent);
    QCoreApplication::processEvents();

    delete btn;
}

void TestSwitchButton::test_leaveEvent()
{
    SwitchButton *btn = new SwitchButton();

    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(btn, &leaveEvent);
    QCoreApplication::processEvents();

    delete btn;
}

void TestSwitchButton::test_changeColor_data()
{
    QTest::addColumn<QString>("theme");

    QTest::newRow("ukui-dark") << "ukui-dark";
    QTest::newRow("ukui-black") << "ukui-black";
    QTest::newRow("ukui-light") << "ukui-light";
    QTest::newRow("default") << "default";
}

void TestSwitchButton::test_changeColor()
{
    QFETCH(QString, theme);

    SwitchButton *btn = new SwitchButton();

    QEnterEvent enterEvent(QPoint(10, 10), QPoint(10, 10), QPoint(10, 10));
    QApplication::sendEvent(btn, &enterEvent);
    QCoreApplication::processEvents();

    btn->changeColor(theme);

    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(btn, &leaveEvent);
    QCoreApplication::processEvents();

    btn->changeColor(theme);

    delete btn;
}

void TestSwitchButton::test_animationThroughSetChecked()
{
    SwitchButton *btn = new SwitchButton();
    btn->setAnimation(true);

    btn->setChecked(true);
    QCoreApplication::processEvents();
    QTRY_VERIFY(btn->isChecked() == true);

    btn->setChecked(false);
    QCoreApplication::processEvents();
    QTRY_VERIFY(btn->isChecked() == false);

    delete btn;
}

void TestSwitchButton::test_drawThroughPaint()
{
    SwitchButton *btn = new SwitchButton();

    btn->setChecked(false);
    QCoreApplication::processEvents();
    QPaintEvent paintEvent1(btn->rect());
    QApplication::sendEvent(btn, &paintEvent1);
    QCoreApplication::processEvents();

    btn->setChecked(true);
    QCoreApplication::processEvents();
    QPaintEvent paintEvent2(btn->rect());
    QApplication::sendEvent(btn, &paintEvent2);
    QCoreApplication::processEvents();

    btn->setDisabledFlag(true);
    QCoreApplication::processEvents();
    QPaintEvent paintEvent3(btn->rect());
    QApplication::sendEvent(btn, &paintEvent3);
    QCoreApplication::processEvents();

    btn->setChecked(false);
    QCoreApplication::processEvents();
    QPaintEvent paintEvent4(btn->rect());
    QApplication::sendEvent(btn, &paintEvent4);
    QCoreApplication::processEvents();

    delete btn;
}

QTEST_MAIN(TestSwitchButton)
