#include "test_grayinfobutton.h"
#include "../../../../plugins/component/AddBtn/grayinfobutton.h"
#include <QDebug>

TestGrayInfoButton::TestGrayInfoButton(QObject *parent)
    : QObject(parent)
{
}

TestGrayInfoButton::~TestGrayInfoButton()
{
}

void TestGrayInfoButton::initTestCase()
{
}

void TestGrayInfoButton::cleanupTestCase()
{
}

void TestGrayInfoButton::init()
{
}

void TestGrayInfoButton::cleanup()
{
}

void TestGrayInfoButton::test_constructor_withNullParent()
{
    GrayInfoButton *button = new GrayInfoButton(nullptr);
    QVERIFY(button != nullptr);
    QVERIFY(button->parent() == nullptr);
    delete button;
}

void TestGrayInfoButton::test_constructor_withValidParent()
{
    QWidget parentWidget;
    GrayInfoButton *button = new GrayInfoButton(&parentWidget);
    QVERIFY(button != nullptr);
    QVERIFY(button->parent() == &parentWidget);
    delete button;
}

void TestGrayInfoButton::test_destructor()
{
    GrayInfoButton *button = new GrayInfoButton(nullptr);
    delete button;
    QVERIFY(true);
}

void TestGrayInfoButton::test_buttonSize()
{
    GrayInfoButton button(nullptr);
    QVERIFY(button.width() == 36);
    QVERIFY(button.height() == 36);
}

void TestGrayInfoButton::test_iconSetting()
{
    GrayInfoButton button(nullptr);
    QVERIFY(!button.icon().isNull());
}

void TestGrayInfoButton::test_propertyUseButtonPalette()
{
    GrayInfoButton button(nullptr);
    QVariant useButtonPalette = button.property("useButtonPalette");
    QVERIFY(useButtonPalette.isValid());
    QVERIFY(useButtonPalette.toBool() == true);
}

void TestGrayInfoButton::test_flatProperty()
{
    GrayInfoButton button(nullptr);
    QVERIFY(button.isFlat() == true);
}
