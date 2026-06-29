#ifndef TEST_WIFI_AUTH_THREAD_H
#define TEST_WIFI_AUTH_THREAD_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QElapsedTimer>

class WifiAuthThread;

class TestWifiAuthThread : public QObject
{
    Q_OBJECT
public:
    explicit TestWifiAuthThread(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testRun();

private:
    void waitForThreadStart(void *thread, int timeoutMs = 100);
};

#endif // TEST_WIFI_AUTH_THREAD_H
