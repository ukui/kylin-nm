#include "test_qt-single-application.h"
#include <QTest>
#include <QWidget>

TestQtSingleApplication::TestQtSingleApplication()
    : m_app(nullptr), m_argc(0), m_argv(nullptr)
{
}

TestQtSingleApplication::~TestQtSingleApplication()
{
}

void TestQtSingleApplication::initTestCase()
{
    m_argc = 1;
    m_argv = new char*[1];
    m_argv[0] = const_cast<char*>("test_app");
    m_app = new QtSingleApplication(m_argc, m_argv, true);
    QVERIFY(m_app != nullptr);
}

void TestQtSingleApplication::cleanupTestCase()
{
    if (m_app) {
        delete m_app;
        m_app = nullptr;
    }
    if (m_argv) {
        delete[] m_argv;
        m_argv = nullptr;
    }
}

void TestQtSingleApplication::init()
{
}

void TestQtSingleApplication::cleanup()
{
}

void TestQtSingleApplication::test_constructor_with_GUIenabled()
{
    QtSingleApplication app1(m_argc, m_argv, true);
    QVERIFY(app1.isRunning() == false);
    QVERIFY(!app1.id().isEmpty());
}

void TestQtSingleApplication::test_constructor_with_appId()
{
    QString appId = "test_app_id";
    QtSingleApplication app2(appId, m_argc, m_argv);
    QVERIFY(app2.id() == appId);
    QVERIFY(app2.isRunning() == false);
}

void TestQtSingleApplication::test_isRunning()
{
    QVERIFY(!m_app->isRunning());
}

void TestQtSingleApplication::test_sendMessage()
{
    bool result = m_app->sendMessage("test message", 500);
}

void TestQtSingleApplication::test_id()
{
    QString appId = m_app->id();
    QVERIFY(!appId.isEmpty());
}

void TestQtSingleApplication::test_setActivationWindow()
{
    QWidget *window = new QWidget();
    m_app->setActivationWindow(window, true);
    QVERIFY(m_app->activationWindow() == window);
    
    m_app->setActivationWindow(window, false);
    QVERIFY(m_app->activationWindow() == window);
    
    delete window;
}

void TestQtSingleApplication::test_activationWindow()
{
    QWidget *window = new QWidget();
    m_app->setActivationWindow(window, true);
    QVERIFY(m_app->activationWindow() == window);
    delete window;
}

void TestQtSingleApplication::test_activateWindow_with_activationWindow()
{
    QWidget *window = new QWidget();
    m_app->setActivationWindow(window, true);
    m_app->activateWindow();
    delete window;
}

void TestQtSingleApplication::test_activateWindow_without_activationWindow()
{
    m_app->activateWindow();
}

void TestQtSingleApplication::test_messageReceived_signal()
{
}

QTEST_MAIN(QtSingleApplication)
