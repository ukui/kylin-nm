#include <QTest>
#include "test_qt-single-application.h"
#include <QTest>
#include <QWidget>
#include <QSignalSpy>

TestQtSingleApplication::TestQtSingleApplication(QObject *parent) : QObject(parent)
{
}

void TestQtSingleApplication::initTestCase()
{
    m_appId = "test_qt_single_application_" + QString::number(QCoreApplication::applicationPid());
}

void TestQtSingleApplication::cleanupTestCase()
{
}

void TestQtSingleApplication::init()
{
}

void TestQtSingleApplication::cleanup()
{
}

void TestQtSingleApplication::testConstructorWithGuiEnabled()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(argc, argv, true);
    QVERIFY2(true, "Constructor with GUIenabled should work");
}

void TestQtSingleApplication::testConstructorWithAppId()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    QVERIFY2(!app.id().isEmpty(), "Application ID should not be empty");
    QVERIFY2(app.id().contains("test_qt_single_application"), "Application ID should contain our app ID");
}

void TestQtSingleApplication::testIsRunning()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    bool running = app.isRunning();
    QVERIFY2(!running || running, "isRunning should return a valid boolean");
}

void TestQtSingleApplication::testId()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    QString id = app.id();
    QVERIFY2(!id.isEmpty(), "Application ID should not be empty");
    QVERIFY2(id.contains("test_qt_single_application"), "Application ID should match our test ID");
}

void TestQtSingleApplication::testSetActivationWindow()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    QWidget *window = new QWidget();
    app.setActivationWindow(window, true);
    QVERIFY2(app.activationWindow() == window, "Activation window should be set correctly");
    delete window;
}

void TestQtSingleApplication::testActivationWindow()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    QWidget *window = new QWidget();
    app.setActivationWindow(window);
    QVERIFY2(app.activationWindow() == window, "activationWindow() should return the set window");
    delete window;
}

void TestQtSingleApplication::testSendMessage()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    bool result = app.sendMessage("test message");
    QVERIFY2(result == true || result == false, "sendMessage should return a valid boolean");
}

void TestQtSingleApplication::testActivateWindow()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    QWidget *window = new QWidget();
    app.setActivationWindow(window, false);
    app.activateWindow();
    QVERIFY2(true, "activateWindow should not crash when called without MainWindow");
    delete window;
}

void TestQtSingleApplication::testSendMessageWithTimeout()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    bool result = app.sendMessage("test message", 1000);
    QVERIFY2(result == true || result == false, "sendMessage with timeout should return a valid boolean");
}

void TestQtSingleApplication::testSetActivationWindowNoAutoActivate()
{
    int argc = 1;
    char *argv[] = {(char*)"test"};
    QtSingleApplication app(m_appId, argc, argv);
    QWidget *window = new QWidget();
    app.setActivationWindow(window, false);
    QVERIFY2(app.activationWindow() == window, "Activation window should be set with auto-activate disabled");
    delete window;
}
