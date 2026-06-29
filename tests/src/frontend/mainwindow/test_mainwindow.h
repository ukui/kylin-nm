#ifndef TEST_MAINWINDOW_H
#define TEST_MAINWINDOW_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "mainwindow.h"

class TestMainWindow : public QObject
{
    Q_OBJECT
public:
    explicit TestMainWindow(QObject *parent = nullptr);
    ~TestMainWindow() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases for MainWindow functions
    void test_setWiredDefaultDevice();
    void test_setWirelessDefaultDevice();
    void test_getWirelessSwitchBtnState();
    void test_getWiredEnabledState();
    void test_getDeviceConnectivity();
    void test_getCableStateByDevice();
    void test_getDefaultDeviceName();
    void test_setWirelessSwitchEnable();
    void test_setWiredEnableStatus();
    void test_signal_mainWindowVisibleChanged();

private:
    MainWindow *m_mainWindow;
};

#endif // TEST_MAINWINDOW_H
