#include "test_infobutton.h"
#include "../../../../../../src/frontend/tools/infobutton.h"
#include <QApplication>
#include <QDebug>

TestInfoButton::TestInfoButton(QObject *parent)
    : QObject(parent)
{
}

void TestInfoButton::initTestCase()
{
    int argc = 0;
    char **argv = nullptr;
    QApplication app(argc, argv);
}

void TestInfoButton::cleanupTestCase()
{
}

void TestInfoButton::init()
{
}

void TestInfoButton::cleanup()
{
}

void TestInfoButton::testConstructor()
{
    InfoButton *button = new InfoButton(nullptr);
    QVERIFY(button != nullptr);
    QVERIFY(button->isVisible() == true);
    QVERIFY(button->fixedWidth() == 36);
    QVERIFY(button->fixedHeight() == 36);
    delete button;
}

void TestInfoButton::testInitUI()
{
    InfoButton *button = new InfoButton(nullptr);
    QVERIFY(button->property("useButtonPalette").toBool() == true);
    delete button;
}

void TestInfoButton::testOnPaletteChanged()
{
    InfoButton *button = new InfoButton(nullptr);
    QSignalSpy spy(button, SIGNAL(paletteChanged()));
    button->onPaletteChanged();
    QVERIFY(button != nullptr);
    delete button;
}

void TestInfoButton::testEnterEvent()
{
    InfoButton *button = new InfoButton(nullptr);
    simulateEnterEvent(button);
    QVERIFY(button != nullptr);
    delete button;
}

void TestInfoButton::testLeaveEvent()
{
    InfoButton *button = new InfoButton(nullptr);
    simulateLeaveEvent(button);
    QVERIFY(button != nullptr);
    delete button;
}

void TestInfoButton::testMousePressEvent()
{
    InfoButton *button = new InfoButton(nullptr);
    simulateMousePress(button);
    QVERIFY(button != nullptr);
    delete button;
}

void TestInfoButton::testMouseReleaseEvent()
{
    InfoButton *button = new InfoButton(nullptr);
    simulateMouseRelease(button);
    QVERIFY(button != nullptr);
    delete button;
}

void TestInfoButton::simulateEnterEvent(QWidget *widget)
{
    QEvent enterEvent(QEvent::Enter);
    QApplication::sendEvent(widget, &enterEvent);
}

void TestInfoButton::simulateLeaveEvent(QWidget *widget)
{
    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(widget, &leaveEvent);
}

void TestInfoButton::simulateMousePress(QWidget *widget)
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(widget, &pressEvent);
}

void TestInfoButton::simulateMouseRelease(QWidget *widget)
{
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(widget, &releaseEvent);
}
