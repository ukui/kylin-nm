#include "test_wifi-auth-thread.h"
#include "wifi-auth-thread.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>

TestWifiAuthThread::TestWifiAuthThread(QObject *parent)
    : QObject(parent)
{
}

void TestWifiAuthThread::initTestCase()
{
    QVERIFY2(QCoreApplication::instance(), "QCoreApplication instance required");
}

void TestWifiAuthThread::cleanupTestCase()
{
}

void TestWifiAuthThread::init()
{
}

void TestWifiAuthThread::cleanup()
{
}

void TestWifiAuthThread::testConstructor()
{
    WifiAuthThread *thread = new WifiAuthThread();
    QVERIFY(thread != nullptr);
    QVERIFY(thread->isRunning() == false);
    delete thread;
}

void TestWifiAuthThread::testDestructor()
{
    WifiAuthThread *thread = new WifiAuthThread();
    QVERIFY(thread != nullptr);
    delete thread;
}

void TestWifiAuthThread::testRun()
{
    WifiAuthThread *thread = new WifiAuthThread();
    QVERIFY(thread != nullptr);
    
    thread->start();
    QVERIFY(thread->wait(5000));
    
    QVERIFY(thread->isFinished() || thread->isRunning());
    
    if (thread->isRunning()) {
        thread->quit();
        thread->wait();
    }
    
    delete thread;
}

void TestWifiAuthThread::waitForThreadStart(void *thread, int timeoutMs)
{
    Q_UNUSED(thread);
    Q_UNUSED(timeoutMs);
}
