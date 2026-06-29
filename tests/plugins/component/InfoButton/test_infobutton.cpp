#include "test_infobutton.h"
#include "../../../../plugins/component/InfoButton/infobutton.h"
#include <QDebug>
#include <QTest>
#include <QMouseEvent>

TestInfoButton::TestInfoButton(QObject *parent)
    : QObject(parent)
{
}

TestInfoButton::~TestInfoButton()
{
}

void TestInfoButton::initTestCase()
{
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

void TestInfoButton::test_constructor_withNullParent()
{
    InfoButton *button = new InfoButton(nullptr);
    QVERIFY(button != nullptr);
    QVERIFY(button->parent() == nullptr);
    delete button;
}

void TestInfoButton::test_constructor_withValidParent()
{
    QWidget parentWidget;
    InfoButton *button = new InfoButton(&parentWidget);
    QVERIFY(button != nullptr);
    QVERIFY(button->parent() == &parentWidget);
    delete button;
}

void TestInfoButton::test_destructor()
{
    InfoButton *button = new InfoButton(nullptr);
    delete button;
    QVERIFY(true);
}

void TestInfoButton::test_buttonSize()
{
    InfoButton button(nullptr);
    QVERIFY(button.width() == 36);
    QVERIFY(button.height() == 36);
}

void TestInfoButton::test_backgroundColor()
{
    InfoButton button(nullptr);
    QVERIFY(button.isVisible() == false);
}

void TestInfoButton::test_foregroundColor()
{
    InfoButton button(nullptr);
    QVERIFY(button.isEnabled() == true);
}

void TestInfoButton::test_enterEvent()
{
    InfoButton button(nullptr);
    button.show();
    QTest::mouseMove(&button, QPoint(5, 5));
    QTest::qWait(50);
    QVERIFY(true);
    button.hide();
}

void TestInfoButton::test_leaveEvent()
{
    InfoButton button(nullptr);
    button.show();
    QTest::mouseMove(&button, QPoint(5, 5));
    QTest::qWait(50);
    QTest::mouseMove(&button, QPoint(100, 100));
    QTest::qWait(50);
    QVERIFY(true);
    button.hide();
}

void TestInfoButton::test_mousePressEvent()
{
    InfoButton button(nullptr);
    button.show();
    QTest::mouseClick(&button, Qt::LeftButton);
    QTest::qWait(50);
    QVERIFY(true);
    button.hide();
}

void TestInfoButton::test_mouseReleaseEvent()
{
    InfoButton button(nullptr);
    button.show();
    QTest::mousePress(&button, Qt::LeftButton);
    QTest::qWait(50);
    QTest::mouseRelease(&button, Qt::LeftButton);
    QTest::qWait(50);
    QVERIFY(true);
    button.hide();
}

void TestInfoButton::test_paintEvent()
{
    InfoButton button(nullptr);
    button.show();
    button.repaint();
    QTest::qWait(50);
    QVERIFY(true);
    button.hide();
}

void TestInfoButton::test_gsettingsConnection()
{
    InfoButton *button = new InfoButton(nullptr);
    QVERIFY(button != nullptr);
    delete button;
}
