#ifndef TEST_KNMDBUSCALLER_H
#define TEST_KNMDBUSCALLER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariantList>
#include <QtTest/QtTest>
#include <QDBusConnection>
#include <QDBusInterface>

#include "knmdbuscaller.h"
#include "netdevice.h"
#include "knmlandatakeeper.h"
#include "knmwlandatakeeper.h"
#include "knminterface.h"

class TestKnmDBusCaller : public QObject
{
    Q_OBJECT
public:
    explicit TestKnmDBusCaller(QObject *parent = nullptr);
    ~TestKnmDBusCaller();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_wiredDeviceList();
    void test_wirelessDeviceList();
    void test_wiredDeviceConnList();
    void test_wirelessDeviceConnList();
    void test_getWiFiIcon();
    void test_wirelessSwitchState();
    void test_wiredMainSwitchState();
    void test_upwardRateDate();
    void test_downwardRateDate();
    void test_rescanWirelessConn();
    void test_activateConnect();
    void test_deActivateConnect();
    void test_setWirelessSwitchEnable();
    void test_passwordConnect();
    void test_setWiredMainSwitchEnable();
    void test_setUpwareRateData();
    void test_setDownwareRateData();
    void test_showPropertyWidget();
    void test_deleteConnect();
    void test_showAddOtherWlanPage();
    void test_getWirelessConList();
    void test_updateWirelessConListSync();
    void test_getCableStateByDevice();
    void test_setNetworkConnectionAutoConnectState();
    void test_setDefaultWiredDevice();
    void test_setDefaultWirelessDevice();
    void test_getDefaultWiredDevice();
    void test_getDefaultWirelessDevice();
    
    // 新增测试用例
    void test_registerInputPasswdAgent();
    void test_getWiFiIcon_variousParameters();
    void test_wiredDeviceList_multipleCalls();
    void test_wirelessDeviceList_multipleCalls();
    void test_wiredDeviceConnList_differentDevices();
    void test_wirelessDeviceConnList_differentDevices();
    void test_switchState_consistency();
    void test_rateData_persistence();
    void test_activateConnect_differentTypes();
    void test_deActivateConnect_differentTypes();
    void test_deleteConnect_differentTypes();
    void test_setNetworkConnectionAutoConnectState_differentTypes();
    
    // 新增测试用例 - 提高覆盖率
    void test_wiredDeviceList_empty();
    void test_wirelessDeviceList_empty();
    void test_getWiFiIcon_edgeCases();
    void test_wiredDeviceConnList_multiple();
    void test_wirelessDeviceConnList_multiple();
    void test_upwardRateDate_various();
    void test_downwardRateDate_various();
    void test_wirelessSwitchState_various();
    void test_wiredMainSwitchState_various();
    
    // 新增测试用例 - 进一步提高覆盖率
    void test_reinitTestDevices();
    void test_reinitTestDevices_nullKeepers();
    // test_asyncCall_nullInterface 已存在
    // test_asyncCall_invalidInterface 已存在
    void test_constructor_testMode();
    void test_constructor_normalMode();
    void test_destructor();
    void test_wiredDeviceList_withData();
    void test_wirelessDeviceList_withData();
    void test_wiredDeviceConnList_withConnections();
    void test_wirelessDeviceConnList_withConnections();
    void test_getWiFiIcon_allCategories();
    void test_getWiFiIcon_allSecurityTypes();
    void test_getWiFiIcon_signalRange();
    void test_getWiFiIcon_apModes();
    void test_setUpwareRateData_various();
    void test_setDownwareRateData_various();
    void test_rescanWirelessConn_testMode();
    void test_activateConnect_testMode();
    void test_deActivateConnect_testMode();
    void test_setWirelessSwitchEnable_testMode();
    void test_setWiredMainSwitchEnable_testMode();
    void test_passwordConnect_testMode();
    void test_showPropertyWidget_testMode();
    void test_deleteConnect_testMode();
    void test_showAddOtherWlanPage_testMode();
    void test_getWirelessConList_testMode();
    void test_updateWirelessConListSync_testMode();
    void test_getCableStateByDevice_testMode();
    void test_setNetworkConnectionAutoConnectState_testMode();
    void test_setDefaultWiredDevice_testMode();
    void test_registerInputPasswdAgent_testMode();
    void test_getWiredConList_testMode();
    void test_getWirelessSwitchState_testMode();
    void test_getWiredMainSwitchState_testMode();
    void test_getWiredDeviceMap_testMode();
    void test_getWirelessDeviceMap_testMode();
    void test_updateWiredDeviceMap_testMode();
    void test_updateWirelessDeviceMap_testMode();
    void test_updateWirelessDevice_testMode();
    void test_isTestMode();
    void test_reinitTestDevices_notInTestMode();
    void test_asyncCall_testMode();
    // test_asyncCall_nullInterface 已存在
    // test_asyncCall_invalidInterface 已存在
    void test_destructor_coverage();
    void test_constructor_testModeBranch();
    void test_constructor_normalModeBranch();
    void test_getWiFiIcon_detailed();
    void test_setTestMode();
    void testDbusBranches();
    void testDbusBranches_nonTestMode();
    void test_showAddOtherWlanPage_coverage();
    void test_getWiredConList_coverage();
    void test_getWirelessConList_coverage();
    void test_updateWirelessConListSync_coverage();
    void test_getCableStateByDevice_coverage();
    void test_setNetworkConnectionAutoConnectState_coverage();
    void test_setDefaultWiredDevice_coverage();
    void test_setDefaultWirelessDevice_coverage();
    void test_getDefaultWiredDevice_coverage();
    void test_getDefaultWirelessDevice_coverage();
    void test_registerInputPasswdAgent_coverage();
    void test_wiredDeviceList_edgeCases();
    void test_wirelessDeviceList_edgeCases();
    void test_getWiFiIcon_extensive();
    void test_activateConnect_variousParams();
    void test_deActivateConnect_variousParams();
    void test_passwordConnect_variousParams();
    void test_deleteConnect_variousParams();
    void test_setWirelessSwitchEnable_variousParams();
    void test_setWiredMainSwitchEnable_variousParams();
    void test_showPropertyWidget_variousParams();
    void test_getWirelessSwitchState_coverage();
    void test_getWiredMainSwitchState_coverage();
    void test_getWiredDeviceMap_coverage();
    void test_getWirelessDeviceMap_coverage();
    void test_updateWiredDeviceMap_coverage();
    void test_updateWirelessDeviceMap_coverage();
    void test_updateWirelessDevice_coverage();
    void test_asyncCall_direct();

private:
    KnmDBusCaller *m_dbusCaller;
};

#endif // TEST_KNMDBUSCALLER_H
