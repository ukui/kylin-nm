#ifndef TEST_WLANITEM_H
#define TEST_WLANITEM_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class TestWlanItem : public QObject
{
    Q_OBJECT
public:
    explicit TestWlanItem(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_setHalfFillet();
    void test_startLoading();
    void test_stopLoading();
    void test_updateIcon();
    void test_paintEvent();

private:
    QWidget *m_parentWidget;
};

#endif // TEST_WLANITEM_H
