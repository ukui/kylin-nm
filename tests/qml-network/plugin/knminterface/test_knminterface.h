#ifndef TEST_KNMINTERFACE_H
#define TEST_KNMINTERFACE_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QSignalSpy>

class KnmInterface;

class TestKnmInterface : public QObject
{
    Q_OBJECT
public:
    explicit TestKnmInterface(QObject *parent = nullptr);
    ~TestKnmInterface();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_wiredDeviceList();
    void test_destructor();
    void test_wirelessConLists();
    void test_getIconData();
    void test_getIconData_withDifferentSizes();
    void test_getIconData_withThemeIcons();
    void test_getWiFiIcon_variousParameters();
    void test_getLoadingIcon_multipleCalls();
    void test_setUpwareRateData_multipleTimes();
    void test_setDownwareRateData_multipleTimes();
    void test_changeSelectDevice_variousParameters();
    void test_updateNetCtrl_multipleTypes();
    void test_getUiCtlData_afterUpdates();
    void test_getIconData_variousSizes();
    void test_getIconData_invalidName();
    void test_wirelessDeviceList();
    void test_wiredDevConnList();
    void test_wirelessDevConnList();
    void test_wirelessSwitchState();
    void test_wiredMainSwitchState();
    void test_upwareRateDate();
    void test_downwareRateDate();
    void test_getNetMacConnectStatus();
    void test_getLoadingIcon();
    void test_getWiFiIcon();
    void test_fontSize();
    void test_getConnectionStatus();
    void test_getUiCtlData();
    void test_activateConnect();
    void test_deActivateConnect();
    void test_setWiredMainSwitch();
    void test_setWirelessSwitch();
    void test_setWirelessScanState();
    void test_setUpwareRateData();
    void test_setDownwareRateData();
    void test_getWiredDevConnListWithParam();
    void test_getWirelessDevConnListWithParam();
    void test_changeSelectDevice();
    void test_getCableStatus();
    void test_getCableStatusByDev();

    void test_getLoadingIcon_cycle();
    void test_initNetCtrl_safe();
    void test_componentSettings();
    void test_updateNetCtrl();
    void test_updateNetCtrl_wireless();
    void test_rebuildCurrentWirelessList_safe();
    void test_slotRefreshTimeout();
    void test_rescanWirelessConn();
    void test_showAddOtherWlanPage();
    void test_showPropertyWidget();
    void test_passwordConnect_safe();
    void test_deleteConnect_safe();
    void test_openNetworkSetting();
    void test_openwLanNetworkSetting();
    void test_submitWirelessPassword();
    void test_cancelWirelessPassword();
    void test_wirelessDevConnListPropUpdate();
    void test_setNetworkConnectAutoConnectState();
    void test_onRequestInputPasswdAgent();
    void test_passwdAgentChangeSelectSsid();
    void test_showPasswordError();
    
    // 新增测试用例
    void test_deleteConnect_safe_various();
    void test_getCableStatusByDev_various();
    void test_updateNetCtrl_allTypes();
    void test_getUiCtlData_various();
    void test_getConnectionStatus_various();
    void test_wiredDeviceList_empty();
    void test_wirelessDeviceList_empty();
    void test_getIconData_withEmptyName();
    void test_getIconData_withInvalidSize();
    void test_fontSize_empty();
    void test_activateConnect_safe();
    void test_deActivateConnect_safe();
    void test_setWiredMainSwitch_safe();
    void test_setWirelessSwitch_safe();
    void test_setWirelessScanState_true();
    void test_setWirelessScanState_false();
    void test_changeSelectDevice_empty();
    void test_changeSelectDevice_notFound();
    void test_getNetMacConnectStatus_safe();
    void test_getWiFiIcon_safe();
    void test_passwordConnect_safe_various();
    void test_showAddOtherWlanPage_safe();
    void test_showPropertyWidget_safe();
    void test_deleteConnect_safe_various2();
    void test_wiredDevConnList_safe();
    void test_wirelessDevConnList_safe();
    
    // 新增测试用例 - 提高覆盖率
    void test_getConnectionStatus_allStates();
    void test_getConnectionStatus_wireOnly();
    void test_getConnectionStatus_wirelessOnly();
    void test_wiredDeviceList_withConnectedDevice();
    void test_getWiredDevConnList_withDevice();
    void test_getWiredDevConnList_noDevice();
    void test_getWirelessDevConnList_withDevice();
    void test_getWirelessDevConnList_noDevice();
    void test_rebuildCurrentWirelessList_withDevice();
    void test_rebuildCurrentWirelessList_noDevice();
    void test_changeSelectDevice_found();
    void test_onRequestInputPasswdAgent_withParams();
    void test_onRequestInputPasswdAgent_noSsid();
    void test_onRequestInputPasswdAgent_noDevice();
    void test_passwdAgentChangeSelectSsid_extended();
    void test_wirelessDevConnListPropUpdate_differentDevice();
    void test_wirelessDevConnListPropUpdate_emptyList();
    void test_wirelessDevConnListPropUpdate_updateExisting();
    void test_setNetworkConnectAutoConnectState_extended();
    void test_showPasswordError_differentDevice();
    void test_showPasswordError_sameDevice();
    void test_submitWirelessPassword_extended();
    void test_cancelWirelessPassword_extended();
    void test_activateConnect_withTimer();
    void test_deActivateConnect_withTimer();
    void test_setWiredMainSwitch_extended();
    void test_setWirelessSwitch_extended();
    void test_setWirelessScanState_true_extended();
    void test_passwordConnect_extended();
    void test_showPropertyWidget_extended();
    void test_deleteConnect_extended();
    void test_showAddOtherWlanPage_extended();
    void test_rescanWirelessConn_extended();
    void test_getNetMacConnectStatus_connected();
    void test_getNetMacConnectStatus_notConnected();
    void test_getCableStatus_extended();
    void test_getCableStatusByDev_extended();
    void test_openNetworkSetting_wireless();
    void test_openNetworkSetting_wired();
    void test_openwLanNetworkSetting_extended();
    void test_slotRefreshTimeout_extended();
    void test_getWiFiIcon_variousCategories();
    void test_getWiFiIcon_differentSecurity();
    void test_getWiFiIcon_apConnection();
    void test_getWiFiIcon_signalLevels();
    
private:
    KnmInterface *m_interface;
};

#endif // TEST_KNMINTERFACE_H
