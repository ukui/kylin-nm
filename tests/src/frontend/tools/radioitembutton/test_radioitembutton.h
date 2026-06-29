#ifndef TEST_RADIOITEMBUTTON_H
#define TEST_RADIOITEMBUTTON_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QIcon>
#include <QPixmap>
#include "radioitembutton.h"

class TestRadioItemButton : public QObject
{
    Q_OBJECT
public:
    explicit TestRadioItemButton(QObject *parent = nullptr);
    ~TestRadioItemButton() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases for RadioItemButton functions
    void test_constructor();
    void test_destructor();
    void test_startLoading();
    void test_stopLoading();
    void test_setButtonIcon();
    void test_setButtonIcon_NullIcon();
    void test_setDefaultPixmap();
    void test_setActive();
    void test_setActive_True();
    void test_setActive_False();
    void test_loadSvg_White();
    void test_loadSvg_Black();
    void test_loadSvg_Gray();
    void test_loadSvg_Blue();
    void test_loadSvg_Default();
    void test_signal_requestStartLoading();
    void test_signal_requestStopLoading();
    void test_signal_animationStoped();

private:
    RadioItemButton *m_radioButton;
};

#endif // TEST_RADIOITEMBUTTON_H
