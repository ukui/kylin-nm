#ifndef TEST_TABPAGE_H
#define TEST_TABPAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class TestTabPage : public QObject
{
    Q_OBJECT
public:
    explicit TestTabPage(QObject *parent = nullptr);
    ~TestTabPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_checkTimeIsOut();
    void test_checkTimeIsOut_firstCall();
    void test_checkTimeIsOut_withinTimeout();
    void test_checkTimeIsOut_exceedTimeout();

    void test_showNoDeiceInfo_visible();
    void test_showNoDeiceInfo_invisible();

    void test_isNoDevice();
    void test_isNoDevice_afterShowNoDevice();

    void test_changeDeviceStateText_connected();
    void test_changeDeviceStateText_notConnected();

    void test_replaceDeviceConnectState();
    void test_replaceDeviceConnectState_sameName();
    void test_replaceDeviceConnectState_emptyNewName();

    void test_updateDeviceConnectState();
    void test_updateDeviceConnectState_emptyDevice();
    void test_updateDeviceConnectState_newDevice();

    void test_showDesktopNotify();

    void test_getDefaultDeviceName_wired();
    void test_getDefaultDeviceName_wireless();
    void test_getDefaultDeviceName_invalid();

    void test_setDefaultDevice_wired();
    void test_setDefaultDevice_wireless();

    void test_getDeviceEnableState_wired();
    void test_getDeviceEnableState_wireless();
    void test_getDeviceEnableState_invalid();

    void test_getOldVersionWiredSwitchState_notExist();
    void test_getOldVersionWiredSwitchState_exist();

    void test_tabPageConstructionAndDestruction();
};

#endif // TEST_TABPAGE_H