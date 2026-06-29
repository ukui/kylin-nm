#ifndef TEST_CUSTOMTABSTYLE_H
#define TEST_CUSTOMTABSTYLE_H

#include <QObject>

class TestCustomTabStyle : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testConstructor();
    void testSizeFromContents();
    void testSizeFromContents_TabBarTab();
    void testSizeFromContents_OtherType();
    void testDrawControl();
    void testDrawControl_TabBarTab_Selected();
    void testDrawControl_TabBarTab_MouseOver();
    void testDrawControl_TabBarTab_Normal();
    void testDrawControl_OtherElement();
};

#endif // TEST_CUSTOMTABSTYLE_H
