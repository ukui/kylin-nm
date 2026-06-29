#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusError>
#include <QMap>
#include <QVariant>
#include <QTemporaryDir>
#include "../../../pub/uisecurityconfig.h"

class TestUiSecurityConfig : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_singleton();
    void test_getModuleHideStatus_data();
    void test_getModuleHideStatus();
    void test_getModuleHideStatus_invalidInterface();
    void test_getProjectIdentity_data();
    void test_getProjectIdentity();
    void test_getProjectIdentity_nullScene();
    void test_getConnectSettingsData_data();
    void test_getConnectSettingsData();
    void test_getConnectSettingsData_emptyRuleName();
    void test_sysSleepMonitoring();
    void test_getSysSleepState();
    void test_isSeewoOrMaxhub();
    void test_initSysSleepMonistor();
};
