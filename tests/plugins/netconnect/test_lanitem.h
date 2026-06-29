#ifndef TEST_LANITEM_H
#define TEST_LANITEM_H

#include <QObject>
#include <QTest>
#include <QWidget>
#include <QTimer>

class LanItem;

class TestLanItem : public QObject
{
    Q_OBJECT
public:
    explicit TestLanItem(QObject *parent = nullptr);
    ~TestLanItem();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor_withActiveTrue();
    void test_constructor_withActiveFalse();
    void test_destructor();
    void test_startLoading();
    void test_stopLoading();
    void test_setNetworkCheckFrameHidden_true();
    void test_setNetworkCheckFrameHidden_false();
    void test_setTopSeparatorVisible_true();
    void test_setTopSeparatorVisible_false();
    void test_setBottomSeparatorVisible_true();
    void test_setBottomSeparatorVisible_false();
    void test_setConnectivityWarn_NoConnectivity();
    void test_setConnectivityWarn_Portal();
    void test_setConnectivityWarn_Limited();
    void test_setConnectivityWarn_Full();
    void test_setConnectivityWarn_Unknown();

private:
    LanItem *m_lanItem;
};

#endif // TEST_LANITEM_H
