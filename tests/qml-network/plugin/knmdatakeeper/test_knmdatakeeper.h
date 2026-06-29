#ifndef TEST_KNMDATAKEEPER_H
#define TEST_KNMDATAKEEPER_H

#include <QObject>
#include <QString>
#include <QSignalSpy>

class KnmDataKeeper;

class TestKnmDataKeeper : public QObject
{
    Q_OBJECT
public:
    explicit TestKnmDataKeeper(QObject *parent = nullptr);
    ~TestKnmDataKeeper();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_addDevice();
    void test_addDevice_notAvailable();
    void test_clearDevice();
    void test_getDeviceList();
    void test_getWiredMainSwitchState();
    void test_setWiredMainSwitchState();
    void test_getSwitchState();
    void test_setSwitchState();
    void test_getDevConnections();
    void test_getDevConnections_emptyList();
    void test_getDevConnections_deviceNotExist();
    void test_clearConnectionList();
    void test_clearConnectionList_deviceNotExist();
    void test_setUpwardRate();
    void test_getUpwardRate();
    void test_setDownwardRate();
    void test_getDownwardRate();
    void test_getCurrentLoadRate_nullParams();
    void test_getCurrentLoadRate_nullDevice();
    void test_getCurrentLoadRate_success();
    
    // 新增测试用例
    void test_addDevice_wirelessType();
    void test_addDevice_duplicateName();
    void test_addDevice_emptyName();
    void test_removeDevice();
    void test_multipleDevices();
    void test_getDevConnections_multipleConnections();
    void test_clearConnectionList_preservesDevice();
    void test_switchState_toggle();
    void test_wiredMainSwitchState_toggle();
    void test_rate_format();
    void test_rate_emptyString();
    void test_getCurrentLoadRate_emptyDeviceName();
    void test_deviceList_returnCopy();
    void test_addDevice_mixedAvailability();

private:
    KnmDataKeeper *m_keeper;
};

#endif // TEST_KNMDATAKEEPER_H
