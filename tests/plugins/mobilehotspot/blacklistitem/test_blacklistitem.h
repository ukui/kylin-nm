#ifndef TEST_BLACKLISTITEM_H
#define TEST_BLACKLISTITEM_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class TestBlacklistItem : public QObject
{
    Q_OBJECT
public:
    explicit TestBlacklistItem(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_constructor_withParent();
    void test_eventFilter_clickButton();
    void test_eventFilter_clickButton_disabled();
    void test_eventFilter_clickOtherObject();
    void test_eventFilter_mousePressEvent();
    void test_eventFilter_mouseMoveEvent();
    void test_signal_onBtnClicked();
};

#endif // TEST_BLACKLISTITEM_H
