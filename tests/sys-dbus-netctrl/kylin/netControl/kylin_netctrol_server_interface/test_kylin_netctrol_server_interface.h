#ifndef TEST_KYLIN_NETCTROL_SERVER_INTERFACE_H
#define TEST_KYLIN_NETCTROL_SERVER_INTERFACE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QVariantMap>

#include "kylin_netctrol_server_interface.h"

class TestKylinNetCtrlServerInterface : public QObject
{
    Q_OBJECT

public:
    explicit TestKylinNetCtrlServerInterface(QObject *parent = nullptr);
    ~TestKylinNetCtrlServerInterface();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_getNetContrlInstance();
    void test_initServer();
    void test_setNetContrlRule();
    void test_getNetContrlRule();
    void test_sendNetCtrLog();
    void test_sendSysProxyNetCtlLog();
    void test_clearBackMessage();
    void test_waitNetCtrlBackResult();
    void test_transQvariantToGvariant();
    void test_transGvariantToQvariant();
    void test_signal_sigNetContrlRuleChanged();
    void test_setNetContrlRule_valueNotChanged();
    void test_setNetContrlRule_differentModules();
    void test_getNetContrlRule_differentModules();
    void test_transGvariantToQvariant_additionalTypes();
    void test_setNetContrlRule_errorHandling();
    void test_getNetContrlRule_errorHandling();
    void test_transQvariantToGvariant_additionalTypes();
    void test_showDesktopNotify();
    void test_setNetContrlRule_multipleItems();
    void test_getNetContrlRule_withData();

private:
    void waitAsyncTask();
};

#endif // TEST_KYLIN_NETCTROL_SERVER_INTERFACE_H
