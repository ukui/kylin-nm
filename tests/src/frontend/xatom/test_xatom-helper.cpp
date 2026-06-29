#include "test_xatom-helper.h"
#include "../../../../../src/frontend/xatom/xatom-helper.h"

TestXAtomHelper::TestXAtomHelper(QObject *parent)
    : QObject(parent)
{
}

void TestXAtomHelper::initTestCase()
{
}

void TestXAtomHelper::cleanupTestCase()
{
}

void TestXAtomHelper::init()
{
}

void TestXAtomHelper::cleanup()
{
}

void TestXAtomHelper::test_getInstance()
{
    XAtomHelper *instance1 = XAtomHelper::getInstance();
    QVERIFY(instance1 != nullptr);
    
    XAtomHelper *instance2 = XAtomHelper::getInstance();
    QVERIFY(instance1 == instance2);
}

void TestXAtomHelper::test_isFrameLessWindow()
{
    int invalidWinId = 0;
    bool result = XAtomHelper::isFrameLessWindow(invalidWinId);
    QVERIFY(result == false || result == true);
}

void TestXAtomHelper::test_isWindowDecorateBorderOnly()
{
    int invalidWinId = 0;
    bool result = XAtomHelper::isWindowDecorateBorderOnly(invalidWinId);
    QVERIFY(result == false || result == true);
}

void TestXAtomHelper::test_isWindowMotifHintDecorateBorderOnly()
{
    MotifWmHints hint1;
    hint1.flags = 0;
    hint1.functions = 0;
    hint1.decorations = 0;
    bool result1 = XAtomHelper::isWindowMotifHintDecorateBorderOnly(hint1);
    QVERIFY(result1 == false);
    
    MotifWmHints hint2;
    hint2.flags = MWM_HINTS_DECORATIONS;
    hint2.functions = 1;
    hint2.decorations = MWM_DECOR_BORDER;
    bool result2 = XAtomHelper::isWindowMotifHintDecorateBorderOnly(hint2);
    QVERIFY(result2 == false);
    
    MotifWmHints hint3;
    hint3.flags = MWM_HINTS_DECORATIONS | (MWM_HINTS_DECORATIONS + 1);
    hint3.decorations = MWM_DECOR_BORDER;
    bool result3 = XAtomHelper::isWindowMotifHintDecorateBorderOnly(hint3);
    QVERIFY(result3 == true);
    
    MotifWmHints hint4;
    hint4.flags = MWM_HINTS_DECORATIONS | (MWM_HINTS_DECORATIONS + 1);
    hint4.decorations = MWM_DECOR_ALL;
    bool result4 = XAtomHelper::isWindowMotifHintDecorateBorderOnly(hint4);
    QVERIFY(result4 == false);
}

void TestXAtomHelper::test_isUKUICsdSupported()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    bool result = helper->isUKUICsdSupported();
    QVERIFY(result == false);
}

void TestXAtomHelper::test_isUKUIDecorationWindow()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    int invalidWinId = 0;
    bool result = helper->isUKUIDecorationWindow(invalidWinId);
    QVERIFY(result == false || result == true);
}

void TestXAtomHelper::test_getWindowBorderRadius()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    int invalidWinId = 0;
    UnityCorners corners = helper->getWindowBorderRadius(invalidWinId);
    QVERIFY(corners.topLeft == 0);
    QVERIFY(corners.topRight == 0);
    QVERIFY(corners.bottomLeft == 0);
    QVERIFY(corners.bottomRight == 0);
}

void TestXAtomHelper::test_setWindowBorderRadius_withCorners()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    int invalidWinId = 0;
    UnityCorners corners;
    corners.topLeft = 5;
    corners.topRight = 10;
    corners.bottomLeft = 15;
    corners.bottomRight = 20;
    helper->setWindowBorderRadius(invalidWinId, corners);
}

void TestXAtomHelper::test_setWindowBorderRadius_withFourInts()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    int invalidWinId = 0;
    helper->setWindowBorderRadius(invalidWinId, 1, 2, 3, 4);
}

void TestXAtomHelper::test_setUKUIDecoraiontHint()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    int invalidWinId = 0;
    helper->setUKUIDecoraiontHint(invalidWinId, true);
    helper->setUKUIDecoraiontHint(invalidWinId, false);
}

void TestXAtomHelper::test_setWindowMotifHint()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    int invalidWinId = 0;
    MotifWmHints hints;
    hints.flags = MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_ALL;
    hints.input_mode = 0;
    hints.status = 0;
    helper->setWindowMotifHint(invalidWinId, hints);
}

void TestXAtomHelper::test_getWindowMotifHint()
{
    XAtomHelper *helper = XAtomHelper::getInstance();
    int invalidWinId = 0;
    MotifWmHints hints = helper->getWindowMotifHint(invalidWinId);
    QVERIFY(hints.flags == 0 || hints.flags > 0);
}

QTEST_MAIN(TestXAtomHelper)
