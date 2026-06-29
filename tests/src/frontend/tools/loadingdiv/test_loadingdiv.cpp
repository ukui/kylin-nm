#include "test_loadingdiv.h"
#include "../../../../../../src/frontend/tools/loadingdiv.h"
#include <QApplication>
#include <QDebug>

static QApplication *g_app = nullptr;

TestLoadingDiv::TestLoadingDiv(QObject *parent)
    : QObject(parent)
{
}

void TestLoadingDiv::initTestCase()
{
    int argc = 0;
    char **argv = nullptr;
    g_app = new QApplication(argc, argv);
}

void TestLoadingDiv::cleanupTestCase()
{
    if (g_app) {
        delete g_app;
        g_app = nullptr;
    }
}

void TestLoadingDiv::init()
{
}

void TestLoadingDiv::cleanup()
{
}

void TestLoadingDiv::testConstructor()
{
    LoadingDiv *loading = new LoadingDiv(nullptr);
    QVERIFY(loading != nullptr);
    QVERIFY(loading->isHidden() == true);
    QVERIFY(loading->width() == 480);
    QVERIFY(loading->height() == 538);
    delete loading;
}

void TestLoadingDiv::testStartLoading()
{
    LoadingDiv *loading = new LoadingDiv(nullptr);
    loading->startLoading();
    QVERIFY(loading->isVisible() == true);
    QVERIFY(loading->isHidden() == false);
    loading->stopLoading();
    delete loading;
}

void TestLoadingDiv::testStopLoading()
{
    LoadingDiv *loading = new LoadingDiv(nullptr);
    loading->startLoading();
    QVERIFY(loading->isVisible() == true);
    loading->stopLoading();
    QVERIFY(loading->isHidden() == true);
    delete loading;
}

void TestLoadingDiv::testSwitchAnimStep()
{
    LoadingDiv *loading = new LoadingDiv(nullptr);
    loading->startLoading();
    QVERIFY(loading->isVisible() == true);
    loading->stopLoading();
    delete loading;
}

void TestLoadingDiv::testSwitchAnimStepLoop()
{
    LoadingDiv *loading = new LoadingDiv(nullptr);
    loading->startLoading();

    simulateTimerTimeout(loading->findChild<QTimer *>());
    simulateTimerTimeout(loading->findChild<QTimer *>());
    simulateTimerTimeout(loading->findChild<QTimer *>());

    loading->stopLoading();
    delete loading;
}

void TestLoadingDiv::testToStopLoadingSignal()
{
    LoadingDiv *loading = new LoadingDiv(nullptr);
    QSignalSpy spy(loading, SIGNAL(toStopLoading()));
    QVERIFY(spy.isValid());
    loading->startLoading();
    loading->stopLoading();
    QVERIFY(spy.count() == 0);
    delete loading;
}

void TestLoadingDiv::simulateTimerTimeout(QTimer *timer)
{
    if (timer) {
        QMetaObject::invokeMethod(timer, "timeout", Qt::DirectConnection);
    }
}
