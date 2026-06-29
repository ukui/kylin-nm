#ifndef TEST_QT_SINGLE_APPLICATION_H
#define TEST_QT_SINGLE_APPLICATION_H

#include <QObject>
#include <QtSingleApplication>

class TestQtSingleApplication : public QObject {
    Q_OBJECT

public:
    TestQtSingleApplication();
    ~TestQtSingleApplication();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor_with_GUIenabled();
    void test_constructor_with_appId();
    void test_isRunning();
    void test_sendMessage();
    void test_id();
    void test_setActivationWindow();
    void test_activationWindow();
    void test_activateWindow_with_activationWindow();
    void test_activateWindow_without_activationWindow();
    void test_messageReceived_signal();

private:
    QtSingleApplication *m_app;
    int m_argc;
    char **m_argv;
};

#endif // TEST_QT_SINGLE_APPLICATION_H
