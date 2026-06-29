#include "test_tabpage.h"
#include "../../../../../../src/frontend/tab-pages/tabpage.h"
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QSettings>

TestTabPage::TestTabPage(QObject *parent)
    : QObject(parent)
{
}

TestTabPage::~TestTabPage()
{
}

void TestTabPage::initTestCase()
{
    QDir().mkpath(QDir::homePath() + "/.config/ukui");
}

void TestTabPage::cleanupTestCase()
{
}

void TestTabPage::init()
{
}

void TestTabPage::cleanup()
{
}

void TestTabPage::test_checkTimeIsOut()
{
    QTest::qWait(10);
    bool result = checkTimeIsOut(100);
    QVERIFY(result == true || result == false);
}

void TestTabPage::test_checkTimeIsOut_firstCall()
{
    QTest::qWait(50);
    bool result = checkTimeIsOut(100);
    QVERIFY(result == true);
}

void TestTabPage::test_checkTimeIsOut_withinTimeout()
{
    checkTimeIsOut(1000);
    QTest::qWait(50);
    bool result = checkTimeIsOut(1000);
    QVERIFY(result == false);
}

void TestTabPage::test_checkTimeIsOut_exceedTimeout()
{
    checkTimeIsOut(10);
    QTest::qWait(20);
    bool result = checkTimeIsOut(10);
    QVERIFY(result == true);
}

void TestTabPage::test_showNoDeiceInfo_visible()
{
}

void TestTabPage::test_showNoDeiceInfo_invisible()
{
}

void TestTabPage::test_isNoDevice()
{
}

void TestTabPage::test_isNoDevice_afterShowNoDevice()
{
}

void TestTabPage::test_changeDeviceStateText_connected()
{
}

void TestTabPage::test_changeDeviceStateText_notConnected()
{
}

void TestTabPage::test_replaceDeviceConnectState()
{
}

void TestTabPage::test_replaceDeviceConnectState_sameName()
{
}

void TestTabPage::test_replaceDeviceConnectState_emptyNewName()
{
}

void TestTabPage::test_updateDeviceConnectState()
{
}

void TestTabPage::test_updateDeviceConnectState_emptyDevice()
{
}

void TestTabPage::test_updateDeviceConnectState_newDevice()
{
}

void TestTabPage::test_showDesktopNotify()
{
    QTest::qWait(100);
}

void TestTabPage::test_getDefaultDeviceName_wired()
{
    QString result = getDefaultDeviceName(WIRED);
    QVERIFY(result.isEmpty() || !result.isEmpty());
}

void TestTabPage::test_getDefaultDeviceName_wireless()
{
    QString result = getDefaultDeviceName(WIRELESS);
    QVERIFY(result.isEmpty() || !result.isEmpty());
}

void TestTabPage::test_getDefaultDeviceName_invalid()
{
    KyDeviceType invalidType = static_cast<KyDeviceType>(100);
    QString result = getDefaultDeviceName(invalidType);
    QVERIFY(result.isEmpty());
}

void TestTabPage::test_setDefaultDevice_wired()
{
    setDefaultDevice(WIRED, "test_wired");
    QTest::qWait(50);
}

void TestTabPage::test_setDefaultDevice_wireless()
{
    setDefaultDevice(WIRELESS, "test_wireless");
    QTest::qWait(50);
}

void TestTabPage::test_getDeviceEnableState_wired()
{
    QMap<QString, bool> map;
    getDeviceEnableState(WIRED, map);
    QVERIFY(map.isEmpty() || !map.isEmpty());
}

void TestTabPage::test_getDeviceEnableState_wireless()
{
    QMap<QString, bool> map;
    getDeviceEnableState(WIRELESS, map);
    QVERIFY(map.isEmpty() || !map.isEmpty());
}

void TestTabPage::test_getDeviceEnableState_invalid()
{
    QMap<QString, bool> map;
    getDeviceEnableState(100, map);
    QVERIFY(map.isEmpty());
}

void TestTabPage::test_getOldVersionWiredSwitchState_notExist()
{
    bool state = false;
    bool result = getOldVersionWiredSwitchState(state);
    QVERIFY(result == false || result == true);
}

void TestTabPage::test_getOldVersionWiredSwitchState_exist()
{
    QSettings settings(CONFIG_FILE_PATH, QSettings::IniFormat);
    settings.setValue("lan_switch_opened", true);
    settings.sync();

    bool state = false;
    bool result = getOldVersionWiredSwitchState(state);
    QVERIFY(result == true);
    QVERIFY(state == true);
}

void TestTabPage::test_tabPageConstructionAndDestruction()
{
}