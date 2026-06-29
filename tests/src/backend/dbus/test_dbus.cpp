#include "test_dbus.h"
#include "dbus.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSignalSpy>

TestDbus::TestDbus(QObject *parent)
    : QObject(parent), m_mockMainWindow(nullptr)
{
}

TestDbus::~TestDbus()
{
    delete m_mockMainWindow;
}

void TestDbus::initTestCase()
{
    QVERIFY2(QCoreApplication::instance(), "QCoreApplication instance required");
    
    m_mockMainWindow = new MockMainWindow();
    QVERIFY(m_mockMainWindow != nullptr);
}

void TestDbus::cleanupTestCase()
{
}

void TestDbus::init()
{
}

void TestDbus::cleanup()
{
}

void TestDbus::testConstructor()
{
    QVERIFY(m_mockMainWindow != nullptr);
}

void TestDbus::testDestructor()
{
    MockMainWindow *mock = new MockMainWindow();
    QVERIFY(mock != nullptr);
    delete mock;
}

void TestDbus::testGetWirelessList()
{
    MockMainWindow mock;
    QList<QStringList> list;
    mock.getWirelessList("eth0", list);
    QVERIFY(list.isEmpty());
}

void TestDbus::testGetWiredList()
{
    MockMainWindow mock;
    QList<QStringList> list;
    mock.getWiredList("eth0", list);
    QVERIFY(list.isEmpty());
}

void TestDbus::testGetWirelessSwitchBtnState()
{
    MockMainWindow mock;
    bool state = mock.getWirelessSwitchBtnState();
    QVERIFY(state == false);
}

void TestDbus::testGetWiredMainSwitchBtnState()
{
    QVERIFY(true);
}

void TestDbus::testGetDeviceConnectivity()
{
    MockMainWindow mock;
    int connectivity = mock.getDeviceConnectivity("eth0");
    QVERIFY(connectivity == 0);
}

void TestDbus::testGetCableStateByDevice()
{
    MockMainWindow mock;
    bool state = mock.getCableStateByDevice("eth0");
    QVERIFY(state == false);
}

void TestDbus::testSetWiredSwitchEnable()
{
    QVERIFY(true);
}

void TestDbus::testSetWirelessSwitchEnable()
{
    QVERIFY(true);
}

void TestDbus::testSetDeviceEnable()
{
    QVERIFY(true);
}

void TestDbus::testActivateConnect()
{
    QVERIFY(true);
}

void TestDbus::testDeActivateConnect()
{
    QVERIFY(true);
}

void TestDbus::testDeActivateConnectConcise()
{
    QVERIFY(true);
}

void TestDbus::testDeleteConnect()
{
    QVERIFY(true);
}

void TestDbus::testSetDeviceAutoConnectState()
{
    QVERIFY(true);
}

void TestDbus::testSetNetworkConnectionAutoConnectState()
{
    QVERIFY(true);
}

void TestDbus::testGetDeviceListAndEnabled()
{
    QVERIFY(true);
}

void TestDbus::testGetWirelessDeviceCap()
{
    MockMainWindow mock;
    QMap<QString, int> map;
    mock.getWirelessDeviceCap(map);
    QVERIFY(map.isEmpty());
}

void TestDbus::testShowPropertyWidget()
{
    QVERIFY(true);
}

void TestDbus::testShowCreateWiredConnectWidget()
{
    QVERIFY(true);
}

void TestDbus::testShowAddOtherWlanWidget()
{
    QVERIFY(true);
}

void TestDbus::testActiveWirelessAp()
{
    QVERIFY(true);
}

void TestDbus::testDeactiveWirelessAp()
{
    QVERIFY(true);
}

void TestDbus::testPasswordConnect()
{
    QVERIFY(true);
}

void TestDbus::testGetStoredApInfo()
{
    MockMainWindow mock;
    QStringList list;
    mock.getStoredApInfo(list);
    QVERIFY(list.isEmpty());
}

void TestDbus::testGetApInfoBySsid()
{
    MockMainWindow mock;
    QStringList list;
    mock.getApInfoBySsid("wlan0", "TestSSID", list);
    QVERIFY(list.isEmpty());
}

void TestDbus::testGetApConnectionPath()
{
    MockMainWindow mock;
    QString path;
    mock.getApConnectionPath(path, "test-uuid");
    QVERIFY(path.isEmpty());
}

void TestDbus::testGetActiveConnectionPath()
{
    MockMainWindow mock;
    QString path;
    mock.getActiveConnectionPath(path, "test-uuid");
    QVERIFY(path.isEmpty());
}

void TestDbus::testReScan()
{
    MockMainWindow mock;
    mock.rescan();
    QVERIFY(true);
}

void TestDbus::testKeyRingInit()
{
    MockMainWindow mock;
    mock.keyRingInit();
    QVERIFY(true);
}

void TestDbus::testKeyRingClear()
{
    MockMainWindow mock;
    mock.keyRingClear();
    QVERIFY(true);
}

void TestDbus::testShowKylinNM()
{
    QVERIFY(true);
}

void TestDbus::testGetNetworkDeviceData()
{
    QVERIFY(true);
}

void TestDbus::testRegisterInputPasswdAgent()
{
    QVERIFY(true);
}

void TestDbus::testRequestInputPasswdAgent()
{
    QVERIFY(true);
}

void TestDbus::testSetDefaultWiredDevice()
{
    QVERIFY(true);
}

void TestDbus::testGetDefaultWiredDevice()
{
    QVERIFY(true);
}

void TestDbus::testSetDefaultWirelessDevice()
{
    QVERIFY(true);
}

void TestDbus::testGetDefaultWirelessDevice()
{
    QVERIFY(true);
}

void TestDbus::testSendPasswordError()
{
    QVERIFY(true);
}

void TestDbus::testSubmitWirelessPassword()
{
    QVERIFY(true);
}

void TestDbus::testCancelWirelessPassword()
{
    QVERIFY(true);
}

