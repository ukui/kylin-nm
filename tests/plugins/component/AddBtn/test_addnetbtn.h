#ifndef TEST_ADDNETBTN_H
#define TEST_ADDNETBTN_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QWidget>

class TestAddNetBtn : public QObject
{
    Q_OBJECT
public:
    explicit TestAddNetBtn(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // test_constructor_isWlan
    void test_constructor_isWlan();
    // test_constructor_notWlan
    void test_constructor_notWlan();
    // test_destructor
    void test_destructor();
    // test_setTextLabel
    void test_setTextLabel();
    // test_setUseRoundedCorners
    void test_setUseRoundedCorners();
    // test_setCornerType_All
    void test_setCornerType_All();
    // test_setCornerType_BottomRight
    void test_setCornerType_BottomRight();
    // test_enterEvent
    void test_enterEvent();
    // test_leaveEvent
    void test_leaveEvent();
    // test_paintEvent
    void test_paintEvent();
};

#endif // TEST_ADDNETBTN_H
