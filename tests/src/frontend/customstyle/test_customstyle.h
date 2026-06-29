#ifndef TEST_CUSTOMSTYLE_H
#define TEST_CUSTOMSTYLE_H

#include <QObject>
#include <QTest>
#include <QStyleOption>
#include <QPainter>
#include <QSize>
#include <QTabBar>

class TestCustomStyle : public QObject
{
    Q_OBJECT
public:
    explicit TestCustomStyle(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testCustomStyleConstructorDefault();
    void testCustomStyleConstructorWithParam();
    void testSizeFromContentsCT_TabBarTabRoundedNorth();
    void testSizeFromContentsCT_TabBarTabRoundedSouth();
    void testSizeFromContentsCT_TabBarTabRoundedWest();
    void testSizeFromContentsCT_TabBarTabRoundedEast();
    void testSizeFromContentsCT_TabBarTabDefaultShape();
    void testSizeFromContentsDefaultType();
    void testDrawControl();
};

#endif // TEST_CUSTOMSTYLE_H
