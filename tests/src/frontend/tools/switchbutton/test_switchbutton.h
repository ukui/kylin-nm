#ifndef TEST_SWITCHBUTTON_H
#define TEST_SWITCHBUTTON_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QMouseEvent>
#include "switchbutton.h"

class TestSwitchButton : public QObject
{
    Q_OBJECT
public:
    explicit TestSwitchButton(QObject *parent = nullptr);
    ~TestSwitchButton() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases for SwitchButton functions
    void test_constructor();
    void test_constructor_defaultParent();
    void test_constructor_withParent();
    void test_destructor();
    void test_setSwitchStatus_on();
    void test_setSwitchStatus_off();
    void test_setSwitchStatus_whenDisabled();
    void test_getSwitchStatus_on();
    void test_getSwitchStatus_off();
    void test_setEnabled_true();
    void test_setEnabled_false();
    void test_getEnabled_default();
    void test_getEnabled_afterSetEnabled();
    void test_startAnimation_animationOn();
    void test_startAnimation_animationOff();
    void test_signal_clicked();
    void test_signal_switchStatusChanged();
    void test_mousePressEvent_enabled();
    void test_mousePressEvent_disabled();
    void test_enterEvent_enabled_on();
    void test_enterEvent_enabled_off();
    void test_enterEvent_disabled();
    void test_leaveEvent_enabled_on();
    void test_leaveEvent_enabled_off();
    void test_leaveEvent_disabled();

private:
    SwitchButton *m_switchButton;
};

#endif // TEST_SWITCHBUTTON_H
