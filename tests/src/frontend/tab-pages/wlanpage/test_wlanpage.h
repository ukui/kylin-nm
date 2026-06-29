#ifndef TEST_WLANPAGE_H
#define TEST_WLANPAGE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class WlanPage;

class TestWlanPage : public QObject
{
    Q_OBJECT
public:
    explicit TestWlanPage(QObject *parent = nullptr);
    ~TestWlanPage();

private:
    WlanPage *m_wlanPage = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_wlanPageConstructionAndDestruction();
    void test_getWirelessList_emptyDevice();
    void test_getWirelessList_withDevice();
    void test_getStoredApInfo();
    void test_getApConnectionPath();
    void test_getActiveConnectionPath();
    void test_activateWirelessConnection();
    void test_deactivateWirelessConnection();
    void test_deactivateWirelessConnectionWithUuid();
    void test_deleteWirelessConnect();
    void test_setWirelessConnectAutoConnectState();
    void test_checkWlanStatus();
    void test_checkInternetLoading();
    void test_getApInfoBySsid();
    void test_setWirelessSwitchEnable();
    void test_getWirelessDeviceCap();
    void test_getConnectivity();
    void test_getWirelessSwitchBtnState();
    void test_getActivateWifiSignal();
    void test_getWirelssDeviceConnectState();
    void test_getCurrentDisplayDevice();
    void test_getWirelssDeviceConnect();
    void test_getWirelessDefaultDeviceName();
    void test_getActivateWifiCategory();
    void test_signals_wlanAdd();
    void test_signals_wlanRemove();
    void test_signals_wlanActiveConnectionStateChanged();
    void test_signals_hotspotDeactivated();
    void test_signals_hotspotActivated();
    void test_signals_signalStrengthChange();
    void test_signals_secuTypeChange();
    void test_signals_connectivityChanged();
};

#endif // TEST_WLANPAGE_H
