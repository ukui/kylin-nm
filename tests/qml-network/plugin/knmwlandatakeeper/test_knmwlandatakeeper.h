#ifndef TEST_KNMWLANDATAKEEPER_H
#define TEST_KNMWLANDATAKEEPER_H

#include <QObject>
#include <QTest>
#include <QDebug>

class KnmWlanDataKeeper;

class TestKnmWlanDataKeeper : public QObject
{
    Q_OBJECT
public:
    explicit TestKnmWlanDataKeeper(QObject *parent = nullptr);
    ~TestKnmWlanDataKeeper();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_setSignal();
    void test_getWiFiIcon();
    void test_addDevConnection();
    
    // 新增测试用例
    void test_getWiFiIcon_edgeCases();
    void test_getWiFiIcon_differentSecurity();
    void test_getWiFiIcon_category();
    void test_getWiFiIcon_signalThresholds();
    void test_inheritedMethods();
    void test_multipleDevices();
    void test_clearDevice();
    void test_getDeviceList();
    
    // 新增测试用例
    void test_getWiFiIcon_hotspotConnection();
    void test_addDevice_invalidTypes();
    void test_connectionOperations();
    void test_deviceNameOperations();
    void test_rateOperations();
    void test_switchState_operations();
    
    // 新增覆盖率提升测试用例
    void test_makeConnectionMap();
    void test_getWiFiIconString();
    void test_addDevConnection_various();
    void test_onActiveConnectionChanged();
    void test_onNetworkAdd();
    void test_onNetworkRemove();
    void test_onDeviceNameChanged();
    void test_onSwitchBtnChanged();
    void test_onNetworkPropChanged();
    void test_onPasswordError();
    void test_updateList();
    
    // 新增分支覆盖测试用例
    void test_makeConnectionMap_activated();
    void test_makeConnectionMap_deactivated();
    void test_addDevConnection_withDash();
    void test_addDevConnection_empty();
    void test_onActiveConnectionChanged_switchOff();
    void test_onActiveConnectionChanged_emptyUuid();
    void test_onNetworkAdd_switchOff();
    void test_onNetworkAdd_emptyDeviceName();
    void test_onNetworkAdd_10Items();
    void test_onNetworkAdd_11Items();
    void test_onNetworkRemove_emptyDeviceName();
    void test_onNetworkRemove_deviceNotExists();
    void test_onDeviceNameChanged_notWireless();
    void test_onDeviceNameChanged_oldNotExists();
    void test_onDeviceNameChanged_newExists();
    void test_onNetworkPropChanged_deviceNotExists();
    void test_getWiFiIconString_category0();
    void test_getWiFiIconString_category1();
    void test_getWiFiIconString_category3();
    void test_getWiFiIconString_categoryOther();
    void test_setSignal_boundaries();
    void test_updateList_switchOff();
    void test_updateList_switchOn();
    void test_makeConnectionMap_direct();

private:
    KnmWlanDataKeeper *m_keeper;
};

#endif // TEST_KNMWLANDATAKEEPER_H
