#ifndef TEST_SWITCHBUTTON_H
#define TEST_SWITCHBUTTON_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include "../../../../../../plugins/component/SwitchButton/switchbutton.h"

class TestSwitchButton : public QObject
{
    Q_OBJECT
public:
    explicit TestSwitchButton(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test constructors
    void test_constructor();
    void test_constructorWithParams();

    // Test setChecked
    void test_setChecked();
    void test_setCheckedToSameValue();
    void test_setCheckedEmitsSignal();

    // Test setAnimation
    void test_setAnimation();

    // Test isChecked
    void test_isChecked();

    // Test setDisabledFlag
    void test_setDisabledFlag();

    // Test getDisabledFlag
    void test_getDisabledFlag();

    // Test mousePressEvent
    void test_mousePressEvent_data();
    void test_mousePressEvent();
    void test_mousePressEventWhenDisabled();
    void test_mousePressEventEmitsSignal();

    // Test resizeEvent
    void test_resizeEvent();
    void test_resizeEventWhenChecked();

    // Test paintEvent
    void test_paintEvent();

    // Test enterEvent and leaveEvent
    void test_enterEvent();
    void test_leaveEvent();

    // Test changeColor
    void test_changeColor_data();
    void test_changeColor();

    // Test animation and draw functions (indirectly via other tests)
    void test_animationThroughSetChecked();
    void test_drawThroughPaint();
};

#endif // TEST_SWITCHBUTTON_H
