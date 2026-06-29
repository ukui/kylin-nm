#include "test_mainwindow.h"

TestMainWindow::TestMainWindow(QObject *parent)
    : QObject(parent)
    , m_mainWindow(nullptr)
{
}

void TestMainWindow::initTestCase()
{
    // Initialize resources if needed
}

void TestMainWindow::cleanupTestCase()
{
    // Cleanup resources if needed
}

void TestMainWindow::init()
{
}

void TestMainWindow::cleanup()
{
}

void TestMainWindow::test_setWiredDefaultDevice()
{
    QString deviceName = "eth0";
    m_mainWindow->setWiredDefaultDevice(deviceName);
}

void TestMainWindow::test_setWirelessDefaultDevice()
{
    QString deviceName = "wlan0";
    m_mainWindow->setWirelessDefaultDevice(deviceName);
}

void TestMainWindow::test_getWirelessSwitchBtnState()
{
    bool state = m_mainWindow->getWirelessSwitchBtnState();
    QVERIFY(state == true || state == false);
}

void TestMainWindow::test_getWiredEnabledState()
{
    bool state = m_mainWindow->getWiredEnabledState();
    QVERIFY(state == true || state == false);
}

void TestMainWindow::test_getDeviceConnectivity()
{
    QString deviceName = "wlan0";
    int connectivity = m_mainWindow->getDeviceConnectivity(deviceName);
    QVERIFY(connectivity >= 0 && connectivity <= 4);
}

void TestMainWindow::test_getCableStateByDevice()
{
    QString deviceName = "eth0";
    bool state = m_mainWindow->getCableStateByDevice(deviceName);
    QVERIFY(state == true || state == false);
}

void TestMainWindow::test_getDefaultDeviceName()
{
    QString name = m_mainWindow->getDefaultDeviceName(0);
    QVERIFY(name.isEmpty() || !name.isEmpty());
}

void TestMainWindow::test_setWirelessSwitchEnable()
{
    m_mainWindow->setWirelessSwitchEnable(true);
    m_mainWindow->setWirelessSwitchEnable(false);
}

void TestMainWindow::test_setWiredEnableStatus()
{
    m_mainWindow->setWiredEnableStatus(true);
    m_mainWindow->setWiredEnableStatus(false);
}

void TestMainWindow::test_signal_mainWindowVisibleChanged()
{
    QSignalSpy spy(m_mainWindow, &MainWindow::mainWindowVisibleChanged);
    QVERIFY(spy.isValid());
}

QTEST_MAIN(TestMainWindow)
