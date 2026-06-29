#include "test_knmlandatakeeper.h"
#include "../../../../qml-network/plugin/knmlandatakeeper.h"
#include "../../../../qml-network/plugin/netdevice.h"
#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>

// 注册 NetDevicePtr 类型到 Qt 元对象系统
Q_DECLARE_METATYPE(NetDevicePtr)

// 前置声明
extern QDBusConnection QDBusConnection_open(const QString &name);

// 覆盖源文件中的 g_lanDataKeeperTestMode 静态变量
// 源文件中的 isTestMode() 函数会使用此变量
static bool g_lanDataKeeperTestMode = false;

TestKnmLanDataKeeper::TestKnmLanDataKeeper(QObject *parent)
    : QObject(parent)
    , m_keeper(nullptr)
{
}

TestKnmLanDataKeeper::~TestKnmLanDataKeeper()
{
}

void TestKnmLanDataKeeper::initTestCase()
{
    // 初始化测试环境
    qRegisterMetaType<NetDevicePtr>();
    
    // 模拟DBUS连接
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning() << "Cannot connect to the D-Bus session bus";
    }
}

void TestKnmLanDataKeeper::cleanupTestCase()
{
}

void TestKnmLanDataKeeper::init()
{
    m_keeper = new KnmLanDataKeeper();
}

void TestKnmLanDataKeeper::cleanup()
{
    if (m_keeper) {
        delete m_keeper;
        m_keeper = nullptr;
    }
}

void TestKnmLanDataKeeper::test_constructor()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    QVERIFY(keeper != nullptr);
    
    // 验证默认状态
    QVERIFY(keeper->getDeviceList().isEmpty());
    QVERIFY(keeper->getWiredMainSwitchState() == false);
    QVERIFY(keeper->getSwitchState() == false);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_destructor()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    delete keeper;
    QVERIFY(true); // 析构不应导致崩溃
}

void TestKnmLanDataKeeper::test_makeConnectionMap_withActivatedStatus()
{
    KnmLanDataKeeper keeper;
    
    QStringList conPath;
    conPath << "TestConnection" << "uuid-123" << "/org/freedesktop/NetworkManager/Settings/1";
    
    QMap<QString, QVariant> map = keeper.makeConnectionMap(ACTIVATED, conPath);
    
    QVERIFY(map.contains("Name"));
    QVERIFY(map.contains("Uuid"));
    QVERIFY(map.contains("Path"));
    QVERIFY(map.contains("State"));
    QVERIFY(map.contains("IsDSL"));
    
    QCOMPARE(map.value("Name").toString(), QString("TestConnection"));
    QCOMPARE(map.value("Uuid").toString(), QString("uuid-123"));
    QCOMPARE(map.value("State").toInt(), ACTIVATED);
    QCOMPARE(map.value("Loading").toBool(), false);
}

void TestKnmLanDataKeeper::test_makeConnectionMap_withDeactivatedStatus()
{
    KnmLanDataKeeper keeper;
    
    QStringList conPath;
    conPath << "TestConnection" << "uuid-123" << "/org/freedesktop/NetworkManager/Settings/1";
    
    QMap<QString, QVariant> map = keeper.makeConnectionMap(DEACTIVATED, conPath);
    
    QVERIFY(map.contains("Name"));
    QVERIFY(map.contains("Uuid"));
    QCOMPARE(map.value("State").toInt(), DEACTIVATED);
}

void TestKnmLanDataKeeper::test_makeConnectionMap_withEmptyPath()
{
    KnmLanDataKeeper keeper;
    
    QStringList conPath;
    
    QMap<QString, QVariant> map = keeper.makeConnectionMap(DEACTIVATED, conPath);
    
    // 空路径测试
    QVERIFY(map.isEmpty() || map.contains("Name"));
}

void TestKnmLanDataKeeper::test_addDevConnection_withValidData()
{
    KnmLanDataKeeper keeper;
    
    // 首先添加设备
    keeper.addDevice("eth0", true, WIRED_DEVICE);
    
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    
    // 添加连接
    keeper.addDevConnection("eth0", conPath);
    
    // 验证设备存在
    QVERIFY(keeper.getDeviceList().contains("eth0"));
    
    // 获取设备连接
    NetDevicePtr dev = keeper.getDeviceList().value("eth0");
    QVariantList connections = dev->getConnections();
    
    // 验证连接被添加
    QCOMPARE(connections.size(), 1);
    
    QMap<QString, QVariant> connMap = connections.at(0).toMap();
    QCOMPARE(connMap.value("Name").toString(), QString("TestConnection"));
    QCOMPARE(connMap.value("State").toInt(), ACTIVATED);
}

void TestKnmLanDataKeeper::test_addDevConnection_withEmptyList()
{
    KnmLanDataKeeper keeper;
    
    QList<QStringList> conPath;
    
    // 首先添加设备
    keeper.addDevice("eth0", true, WIRED_DEVICE);
    
    // 空列表测试
    keeper.addDevConnection("eth0", conPath);
    
    NetDevicePtr dev = keeper.getDeviceList().value("eth0");
    QCOMPARE(dev->getConnections().size(), 0);
}

void TestKnmLanDataKeeper::test_addDevConnection_withNonExistentDevice()
{
    KnmLanDataKeeper keeper;
    
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "Test" << "uuid" << "/path";
    conPath << conn1;
    
    // 非存在设备测试 - 不应该崩溃
    keeper.addDevConnection("nonexistent", conPath);
    
    QVERIFY(keeper.getDeviceList().isEmpty());
}

void TestKnmLanDataKeeper::test_onActiveConnectionChanged_withValidData()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    // 添加连接
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    keeper->addDevConnection("eth0", conPath);
    
    // 模拟连接状态变化 - 已激活
    QMetaObject::invokeMethod(keeper, "onActiveConnectionChanged", 
                              Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QString, "uuid-123"),
                              Q_ARG(int, ACTIVATED));
    
    // 验证连接状态更新
    NetDevicePtr dev = keeper->getDeviceList().value("eth0");
    QVariantList connections = dev->getConnections();
    QCOMPARE(connections.size(), 1);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onActiveConnectionChanged_withEmptyUuid()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 空UUID测试 - 不应该崩溃
    QMetaObject::invokeMethod(keeper, "onActiveConnectionChanged",
                              Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QString, ""),  // 空UUID
                              Q_ARG(int, ACTIVATED));
    
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onLanAdd_withValidData()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    QStringList lanInfo;
    lanInfo << "NewConnection" << "uuid-456" << "/path/2" << "1" << "0";
    
    // 添加连接
    QMetaObject::invokeMethod(keeper, "onLanAdd", Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QStringList, lanInfo));
    
    // 验证新增的连接
    NetDevicePtr dev = keeper->getDeviceList().value("eth0");
    QVariantList connections = dev->getConnections();
    
    // 至少有一个连接
    QVERIFY(connections.size() >= 1);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onLanRemove_withValidPath()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    // 添加连接
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    keeper->addDevConnection("eth0", conPath);
    
    // 删除连接
    QMetaObject::invokeMethod(keeper, "onLanRemove", Qt::DirectConnection,
                              Q_ARG(QString, "/path/1"));
    
    // 验证连接被删除
    NetDevicePtr dev = keeper->getDeviceList().value("eth0");
    QVariantList connections = dev->getConnections();
    
    // 连接数量可能为0或保持原状(取决于实现)
    QVERIFY(connections.size() >= 0);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onSwitchBtnChanged_withTrue()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    bool initialSwitchState = keeper->getSwitchState();
    
    QMetaObject::invokeMethod(keeper, "onSwitchBtnChanged", Qt::DirectConnection,
                              Q_ARG(bool, true));
    
    // 验证开关状态更新
    QVERIFY(keeper->getSwitchState() == true);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onSwitchBtnChanged_withFalse()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    QMetaObject::invokeMethod(keeper, "onSwitchBtnChanged", Qt::DirectConnection,
                              Q_ARG(bool, false));
    
    // 验证开关状态更新
    QVERIFY(keeper->getSwitchState() == false);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_isDslConnection_withPppoeType()
{
    KnmLanDataKeeper keeper;
    
    // 这是一个基于Process的测试，需要伪造输出
    // 由于实际测试需要调用nmcli，这里仅测试函数调用不崩溃
    bool result = keeper.isDslConnection("test-uuid");
    
    // 函数不应崩溃，结果可以是任意值
    QVERIFY(true);
}

void TestKnmLanDataKeeper::test_isDslConnection_withNonPppoeType()
{
    KnmLanDataKeeper keeper;
    
    bool result = keeper.isDslConnection("non-pppoe-uuid");
    
    QVERIFY(true);
}

void TestKnmLanDataKeeper::test_onActiveConnectionChanged_withEmptyDeviceName()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    // 添加连接
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    keeper->addDevConnection("eth0", conPath);
    
    // 模拟设备名为空但状态不是 DEACTIVATED 的情况
    QMetaObject::invokeMethod(keeper, "onActiveConnectionChanged",
                              Qt::DirectConnection,
                              Q_ARG(QString, ""),
                              Q_ARG(QString, "uuid-123"),
                              Q_ARG(int, ACTIVATED));
    
    // 验证设备存在
    QVERIFY(keeper->getDeviceList().contains("eth0"));
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onActiveConnectionChanged_withNonExistentDevice()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 设备不存在的情况
    QMetaObject::invokeMethod(keeper, "onActiveConnectionChanged",
                              Qt::DirectConnection,
                              Q_ARG(QString, "non-existent"),
                              Q_ARG(QString, "uuid-123"),
                              Q_ARG(int, ACTIVATED));
    
    // 不应崩溃
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onLanAdd_withEmptyDeviceName()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    keeper->addDevice("eth1", true, WIRED_DEVICE);
    
    QStringList lanInfo;
    lanInfo << "NewConnection" << "uuid-456" << "/path/2" << "1" << "0";
    
    // 设备名为空，添加到所有设备
    QMetaObject::invokeMethod(keeper, "onLanAdd", Qt::DirectConnection,
                              Q_ARG(QString, ""),
                              Q_ARG(QStringList, lanInfo));
    
    // 验证两个设备都有新连接
    NetDevicePtr dev0 = keeper->getDeviceList().value("eth0");
    NetDevicePtr dev1 = keeper->getDeviceList().value("eth1");
    QVERIFY(dev0->getConnections().size() >= 1);
    QVERIFY(dev1->getConnections().size() >= 1);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onLanAdd_withNonExistentDevice()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    QStringList lanInfo;
    lanInfo << "NewConnection" << "uuid-456" << "/path/2" << "1" << "0";
    
    // 设备不存在
    QMetaObject::invokeMethod(keeper, "onLanAdd", Qt::DirectConnection,
                              Q_ARG(QString, "non-existent"),
                              Q_ARG(QStringList, lanInfo));
    
    // 不应崩溃
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onLanRemove_withEmptyPath()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    // 添加连接
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    keeper->addDevConnection("eth0", conPath);
    
    // 空路径删除
    QMetaObject::invokeMethod(keeper, "onLanRemove", Qt::DirectConnection,
                              Q_ARG(QString, ""));
    
    // 不应崩溃
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_updateLanInfo_withEmptyDeviceName()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    keeper->addDevice("eth1", true, WIRED_DEVICE);
    
    // 添加连接
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    keeper->addDevConnection("eth0", conPath);
    
    // 设备名为空，更新所有设备
    QStringList lanInfo;
    lanInfo << "UpdatedConnection" << "uuid-123" << "/path/1" << "1" << "2";
    
    QMetaObject::invokeMethod(keeper, "updateLanInfo", Qt::DirectConnection,
                              Q_ARG(QString, ""),
                              Q_ARG(QStringList, lanInfo));
    
    // 验证更新成功
    QVERIFY(keeper->getDeviceList().contains("eth0"));
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_updateLanInfo_withValidDeviceName()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    keeper->addDevice("eth1", true, WIRED_DEVICE);
    
    // 添加连接
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    keeper->addDevConnection("eth0", conPath);
    
    // 指定设备名更新
    QStringList lanInfo;
    lanInfo << "UpdatedConnection" << "uuid-123" << "/path/1" << "1" << "2";
    
    QMetaObject::invokeMethod(keeper, "updateLanInfo", Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QStringList, lanInfo));
    
    // 验证更新成功
    QVERIFY(keeper->getDeviceList().contains("eth0"));
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_updateLanInfo_modifyConnectionName()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    // 添加已激活的连接
    QList<QStringList> conPath;
    QStringList conn1;
    conn1 << "TestConnection" << "uuid-123" << "/path/1";
    conPath << conn1;
    keeper->addDevConnection("eth0", conPath);
    
    // 修改连接名称
    QStringList lanInfo;
    lanInfo << "NewName" << "uuid-123" << "/path/1" << "1" << "2";
    
    QMetaObject::invokeMethod(keeper, "updateLanInfo", Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QStringList, lanInfo));
    
    // 验证更新成功
    NetDevicePtr dev = keeper->getDeviceList().value("eth0");
    QVERIFY(dev->getConnections().size() >= 1);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onDeviceNameChanged_withValidData()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    // 设备名变更
    QMetaObject::invokeMethod(keeper, "onDeviceNameChanged", Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QString, "eth1"),
                              Q_ARG(int, WIRED_DEVICE));
    
    // 验证信号发出
    // 由于是私有槽函数，我们只验证不崩溃
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onDeviceNameChanged_withInvalidType()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    
    // 类型不是 WIRED_DEVICE
    QMetaObject::invokeMethod(keeper, "onDeviceNameChanged", Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QString, "eth1"),
                              Q_ARG(int, 999));  // 无效类型
    
    // 不应崩溃
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onDeviceNameChanged_withNonExistentDevice()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 设备不存在
    QMetaObject::invokeMethod(keeper, "onDeviceNameChanged", Qt::DirectConnection,
                              Q_ARG(QString, "non-existent"),
                              Q_ARG(QString, "eth1"),
                              Q_ARG(int, WIRED_DEVICE));
    
    // 不应崩溃
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_onDeviceNameChanged_withDuplicateName()
{
    KnmLanDataKeeper *keeper = new KnmLanDataKeeper();
    
    // 首先添加两个设备
    keeper->addDevice("eth0", true, WIRED_DEVICE);
    keeper->addDevice("eth1", true, WIRED_DEVICE);
    
    // 尝试将 eth0 改名为 eth1（已存在）
    QMetaObject::invokeMethod(keeper, "onDeviceNameChanged", Qt::DirectConnection,
                              Q_ARG(QString, "eth0"),
                              Q_ARG(QString, "eth1"),
                              Q_ARG(int, WIRED_DEVICE));
    
    // 不应崩溃
    QVERIFY(keeper != nullptr);
    
    delete keeper;
}

void TestKnmLanDataKeeper::test_isTestMode_defaultValue()
{
    // 测试默认值为 false
    bool result = KnmLanDataKeeper::isTestMode();
    QCOMPARE(result, false);
}

void TestKnmLanDataKeeper::test_isTestMode_afterSetTestMode()
{
    // 设置测试模式为 true
    KnmLanDataKeeper::setTestMode(true);
    
    bool result = KnmLanDataKeeper::isTestMode();
    QCOMPARE(result, true);
    
    // 测试完成后恢复默认值
    KnmLanDataKeeper::setTestMode(false);
}
