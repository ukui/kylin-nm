#ifndef TEST_XATOM_HELPER_H
#define TEST_XATOM_HELPER_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class TestXAtomHelper : public QObject
{
    Q_OBJECT
public:
    explicit TestXAtomHelper(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_getInstance();
    void test_isFrameLessWindow();
    void test_isWindowDecorateBorderOnly();
    void test_isWindowMotifHintDecorateBorderOnly();
    void test_isUKUICsdSupported();
    void test_isUKUIDecorationWindow();
    void test_getWindowBorderRadius();
    void test_setWindowBorderRadius_withCorners();
    void test_setWindowBorderRadius_withFourInts();
    void test_setUKUIDecoraiontHint();
    void test_setWindowMotifHint();
    void test_getWindowMotifHint();
};

#endif // TEST_XATOM_HELPER_H
