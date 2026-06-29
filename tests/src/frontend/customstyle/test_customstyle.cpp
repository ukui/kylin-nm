#include "test_customstyle.h"
#include "../../../../src/frontend/customstyle.h"
#include <QPainter>
#include <QStyleOptionTab>

TestCustomStyle::TestCustomStyle(QObject *parent)
    : QObject(parent)
{}

void TestCustomStyle::initTestCase()
{
}

void TestCustomStyle::cleanupTestCase()
{
}

void TestCustomStyle::init()
{
}

void TestCustomStyle::cleanup()
{
}

void TestCustomStyle::testCustomStyleConstructorDefault()
{
    CustomStyle style;
    QVERIFY(true);
}

void TestCustomStyle::testCustomStyleConstructorWithParam()
{
    CustomStyle style("fusion");
    QVERIFY(true);
}

void TestCustomStyle::testSizeFromContentsCT_TabBarTabRoundedNorth()
{
    CustomStyle style;
    QStyleOptionTab tabOption;
    tabOption.shape = QTabBar::RoundedNorth;
    QSize contentsSize(100, 40);
    QSize result = style.sizeFromContents(QStyle::CT_TabBarTab, &tabOption, contentsSize, nullptr);
    // size = (0, 40), add (50, 0) = (50, 40)
    QVERIFY(result.width() == 50);
    QVERIFY(result.height() == 40);
}

void TestCustomStyle::testSizeFromContentsCT_TabBarTabRoundedSouth()
{
    CustomStyle style;
    QStyleOptionTab tabOption;
    tabOption.shape = QTabBar::RoundedSouth;
    QSize contentsSize(100, 40);
    QSize result = style.sizeFromContents(QStyle::CT_TabBarTab, &tabOption, contentsSize, nullptr);
    // size = (0, 40), add (50, 0) = (50, 40)
    QVERIFY(result.width() == 50);
    QVERIFY(result.height() == 40);
}

void TestCustomStyle::testSizeFromContentsCT_TabBarTabRoundedWest()
{
    CustomStyle style;
    QStyleOptionTab tabOption;
    tabOption.shape = QTabBar::RoundedWest;
    QSize contentsSize(100, 40);
    QSize result = style.sizeFromContents(QStyle::CT_TabBarTab, &tabOption, contentsSize, nullptr);
    // size = (0, 40), add (0, 50) = (0, 90)
    QVERIFY(result.width() == 0);
    QVERIFY(result.height() == 90);
}

void TestCustomStyle::testSizeFromContentsCT_TabBarTabRoundedEast()
{
    CustomStyle style;
    QStyleOptionTab tabOption;
    tabOption.shape = QTabBar::RoundedEast;
    QSize contentsSize(100, 40);
    QSize result = style.sizeFromContents(QStyle::CT_TabBarTab, &tabOption, contentsSize, nullptr);
    // size = (0, 40), add (0, 50) = (0, 90)
    QVERIFY(result.width() == 0);
    QVERIFY(result.height() == 90);
}

void TestCustomStyle::testSizeFromContentsCT_TabBarTabDefaultShape()
{
    CustomStyle style;
    QStyleOptionTab tabOption;
    tabOption.shape = QTabBar::TriangularNorth;
    QSize contentsSize(100, 40);
    QSize result = style.sizeFromContents(QStyle::CT_TabBarTab, &tabOption, contentsSize, nullptr);
    // default case: returns size = (0, 40)
    QVERIFY(result.width() == 0);
    QVERIFY(result.height() == 40);
}

void TestCustomStyle::testSizeFromContentsDefaultType()
{
    CustomStyle style;
    QStyleOption opt;
    QSize contentsSize(100, 40);
    QSize result = style.sizeFromContents(QStyle::CT_PushButton, &opt, contentsSize, nullptr);
    QVERIFY(result.width() >= 0);
    QVERIFY(result.height() >= 0);
}

void TestCustomStyle::testDrawControl()
{
    CustomStyle style;
    QStyleOptionButton opt;
    QPainter painter;
    style.drawControl(QStyle::CE_PushButton, &opt, &painter, nullptr);
    QVERIFY(true);
}
