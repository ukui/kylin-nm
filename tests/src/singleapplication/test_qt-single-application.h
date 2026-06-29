#ifndef TEST_QT_SINGLE_APPLICATION_H
#define TEST_QT_SINGLE_APPLICATION_H

#include <QObject>
#include <QString>
#include "qt-single-application.h"

class TestQtSingleApplication : public QObject
{
    Q_OBJECT
public:
    explicit TestQtSingleApplication(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructorWithGuiEnabled();
    void testConstructorWithAppId();
    void testIsRunning();
    void testId();
    void testSetActivationWindow();
    void testActivationWindow();
    void testSendMessage();
    void testActivateWindow();
    void testSendMessageWithTimeout();
    void testSetActivationWindowNoAutoActivate();

private:
    QString m_appId;
};

#endif // TEST_QT_SINGLE_APPLICATION_H
