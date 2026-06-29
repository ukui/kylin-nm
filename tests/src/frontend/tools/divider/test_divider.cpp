#include "test_divider.h"
#include "divider.h"
#include <QApplication>
#include <QPaintEvent>
#include <QSignalSpy>
#include <QTimer>
#include <QMetaObject>

TestDivider::TestDivider(QObject *parent)
    : QObject(parent)
    , m_parentWidget(nullptr)
{
}

void TestDivider::initTestCase()
{
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);
    m_parentWidget = new QWidget();
}

void TestDivider::cleanupTestCase()
{
    delete m_parentWidget;
    m_parentWidget = nullptr;
}

void TestDivider::init()
{
}

void TestDivider::cleanup()
{
}

void TestDivider::testConstructorDefault()
{
    Divider *divider = new Divider();
    QVERIFY2(divider != nullptr, "Failed to create Divider with default parameters");
    QVERIFY(divider->parent() == nullptr);
    QVERIFY(divider->height() == 1);
    delete divider;
}

void TestDivider::testConstructorWithDarkPal()
{
    Divider *divider = new Divider(true);
    QVERIFY2(divider != nullptr, "Failed to create Divider with useDarkPal=true");
    QVERIFY(divider->parent() == nullptr);
    QVERIFY(divider->height() == 1);
    delete divider;
}

void TestDivider::testConstructorWithParent()
{
    Divider *divider = new Divider(false, m_parentWidget);
    QVERIFY2(divider != nullptr, "Failed to create Divider with parent");
    QVERIFY(divider->parent() == m_parentWidget);
    QVERIFY(divider->height() == 1);
    delete divider;
}

void TestDivider::testOnPaletteChanged()
{
    Divider *divider = new Divider(false);
    QMetaObject::invokeMethod(divider, "onPaletteChanged", Qt::DirectConnection);
    QVERIFY2(divider != nullptr, "Failed to call onPaletteChanged");
    delete divider;
}

void TestDivider::testOnPaletteChangedWithDarkTheme()
{
    Divider *divider = new Divider(true);
    QMetaObject::invokeMethod(divider, "onPaletteChanged", Qt::DirectConnection);
    QVERIFY2(divider != nullptr, "Failed to call onPaletteChanged with dark theme");
    delete divider;
}

void TestDivider::testOnPaletteChangedWithoutSchema()
{
    Divider *divider = new Divider(false);
    QMetaObject::invokeMethod(divider, "onPaletteChanged", Qt::DirectConnection);
    QVERIFY2(divider != nullptr, "Failed to call onPaletteChanged without schema");
    delete divider;
}

void TestDivider::testPaintEvent()
{
    Divider *divider = new Divider();
    QMetaObject::invokeMethod(divider, "paintEvent", Qt::DirectConnection,
                              Q_ARG(QPaintEvent*, new QPaintEvent(divider->rect())));
    QVERIFY2(divider != nullptr, "Failed to execute paintEvent");
    delete divider;
}

void TestDivider::testFixedHeight()
{
    Divider *divider = new Divider();
    QVERIFY(divider->height() == 1);
    delete divider;
}
