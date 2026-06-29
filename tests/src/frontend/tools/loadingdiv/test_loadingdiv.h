#ifndef TEST_LOADINGDIV_H
#define TEST_LOADINGDIV_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QWidget>
#include <QTimer>

class TestLoadingDiv : public QObject
{
    Q_OBJECT
public:
    explicit TestLoadingDiv(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testStartLoading();
    void testStopLoading();
    void testSwitchAnimStep();
    void testSwitchAnimStepLoop();
    void testToStopLoadingSignal();

private:
    void simulateTimerTimeout(QTimer *timer);
};

#endif // TEST_LOADINGDIV_H
