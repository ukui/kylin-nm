#ifndef TEST_KNMLANDATAKEEPER_H
#define TEST_KNMLANDATAKEEPER_H

#include <QObject>
#include <QTest>
#include <QMap>
#include <QStringList>
#include <QSharedPointer>

#include "../../../../qml-network/plugin/knmlandatakeeper.h"
#include "../../../../qml-network/plugin/netdevice.h"

class KnmLanDataKeeper;
class NetDevice;

// NetDevicePtr 类型定义（与 knmdatakeeper.h 保持一致）
typedef QSharedPointer<NetDevice> NetDevicePtr;

class TestKnmLanDataKeeper : public QObject
{
    Q_OBJECT

public:
    explicit TestKnmLanDataKeeper(QObject *parent = nullptr);
    ~TestKnmLanDataKeeper();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 构造/析构测试
    void test_constructor();
    void test_destructor();

    // 基础功能测试
    void test_makeConnectionMap_withActivatedStatus();
    void test_makeConnectionMap_withDeactivatedStatus();
    void test_makeConnectionMap_withEmptyPath();

    // 设备连接管理测试
    void test_addDevConnection_withValidData();
    void test_addDevConnection_withEmptyList();
    void test_addDevConnection_withNonExistentDevice();

    // 信号槽测试
    void test_onActiveConnectionChanged_withValidData();
    void test_onActiveConnectionChanged_withEmptyUuid();
    void test_onActiveConnectionChanged_withEmptyDeviceName();
    void test_onActiveConnectionChanged_withNonExistentDevice();
    void test_onLanAdd_withValidData();
    void test_onLanAdd_withEmptyDeviceName();
    void test_onLanAdd_withNonExistentDevice();
    void test_onLanRemove_withValidPath();
    void test_onLanRemove_withEmptyPath();
    void test_updateLanInfo_withEmptyDeviceName();
    void test_updateLanInfo_withValidDeviceName();
    void test_updateLanInfo_modifyConnectionName();
    void test_onDeviceNameChanged_withValidData();
    void test_onDeviceNameChanged_withInvalidType();
    void test_onDeviceNameChanged_withNonExistentDevice();
    void test_onDeviceNameChanged_withDuplicateName();
    void test_onSwitchBtnChanged_withTrue();
    void test_onSwitchBtnChanged_withFalse();

    // 工具函数测试
    void test_isDslConnection_withPppoeType();
    void test_isDslConnection_withNonPppoeType();

    // 测试模式控制测试
    void test_isTestMode_defaultValue();
    void test_isTestMode_afterSetTestMode();

private:
    KnmLanDataKeeper *m_keeper;
    QMap<QString, NetDevicePtr> m_deviceList;
};

#endif // TEST_KNMLANDATAKEEPER_H
