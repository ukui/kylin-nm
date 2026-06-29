#include "test_kylinactiveconnectresource.h"

TestKyActiveConnectResource::TestKyActiveConnectResource(QObject *parent)
    : QObject(parent)
    , m_activeConnectResource(nullptr)
{
}

TestKyActiveConnectResource::~TestKyActiveConnectResource()
{
}

void TestKyActiveConnectResource::initTestCase()
{
    m_activeConnectResource = new KyActiveConnectResource();
    QVERIFY(m_activeConnectResource != nullptr);
}

void TestKyActiveConnectResource::cleanupTestCase()
{
    if (m_activeConnectResource) {
        delete m_activeConnectResource;
        m_activeConnectResource = nullptr;
    }
}

void TestKyActiveConnectResource::init()
{
}

void TestKyActiveConnectResource::cleanup()
{
}

void TestKyActiveConnectResource::testConstructorAndDestructor()
{
    KyActiveConnectResource *resource = new KyActiveConnectResource();
    QVERIFY(resource != nullptr);
    delete resource;
    resource = nullptr;

    KyActiveConnectResource *resourceWithParent = new KyActiveConnectResource(this);
    QVERIFY(resourceWithParent != nullptr);
}

void TestKyActiveConnectResource::testGetActiveConnectionByUuid()
{
    QString emptyUuid = "";
    KyConnectItem *item = m_activeConnectResource->getActiveConnectionByUuid(emptyUuid);
    QVERIFY(item == nullptr);

    QString invalidUuid = "invalid-uuid-12345";
    KyConnectItem *item2 = m_activeConnectResource->getActiveConnectionByUuid(invalidUuid);
    QVERIFY(item2 == nullptr);
}

void TestKyActiveConnectResource::testGetActiveConnectionByUuidWithDeviceName()
{
    QString emptyUuid = "";
    QString emptyDevice = "";
    KyConnectItem *item = m_activeConnectResource->getActiveConnectionByUuid(emptyUuid, emptyDevice);
    QVERIFY(item == nullptr);

    QString invalidUuid = "invalid-uuid-12345";
    QString deviceName = "eth0";
    KyConnectItem *item2 = m_activeConnectResource->getActiveConnectionByUuid(invalidUuid, deviceName);
    QVERIFY(item2 == nullptr);
}

void TestKyActiveConnectResource::testGetActiveConnectionList()
{
    QString deviceName = "";
    NetworkManager::ConnectionSettings::ConnectionType type = NetworkManager::ConnectionSettings::Wired;
    QList<KyConnectItem *> itemList;

    m_activeConnectResource->getActiveConnectionList(deviceName, type, itemList);
    QVERIFY(itemList.isEmpty());

    deviceName = "eth0";
    QList<KyConnectItem *> itemList2;
    m_activeConnectResource->getActiveConnectionList(deviceName, type, itemList2);
}

void TestKyActiveConnectResource::testGetActiveConnectIpInfo()
{
    QString uuid = "invalid-uuid-for-test";
    QString ipv4 = "";
    QString ipv6 = "";

    m_activeConnectResource->getActiveConnectIpInfo(uuid, ipv4, ipv6);
}

void TestKyActiveConnectResource::testGetActiveConnectDnsInfo()
{
    QString uuid = "invalid-uuid-for-test";
    QList<QHostAddress> ipv4Dns;
    QList<QHostAddress> ipv6Dns;

    m_activeConnectResource->getActiveConnectDnsInfo(uuid, ipv4Dns, ipv6Dns);
}

void TestKyActiveConnectResource::testGetVpnActivateConnect()
{
    QList<KyVpnConnectItem *> vpnList;
    m_activeConnectResource->getVpnActivateConnect(vpnList);
    QVERIFY(vpnList.isEmpty());
}

// TODO: Re-enable after Bluetooth dependencies are fixed
//void TestKyActiveConnectResource::testGetBtActivateConnect()
//{
//    QList<KyBluetoothConnectItem *> btList;
//    m_activeConnectResource->getBtActivateConnect(btList);
//    QVERIFY(btList.isEmpty());
//}

void TestKyActiveConnectResource::testGetApActivateConnect()
{
    QList<KyApConnectItem *> apList;
    m_activeConnectResource->getApActivateConnect(apList);
    QVERIFY(apList.isEmpty());
}

void TestKyActiveConnectResource::testGetDeviceOfActivateConnect()
{
    QString emptyUuid = "";
    QString deviceName = m_activeConnectResource->getDeviceOfActivateConnect(emptyUuid);
    QVERIFY(deviceName.isEmpty());

    QString invalidUuid = "invalid-uuid-12345";
    QString deviceName2 = m_activeConnectResource->getDeviceOfActivateConnect(invalidUuid);
    QVERIFY(deviceName2.isEmpty());
}

void TestKyActiveConnectResource::testConnectionIsVirtual()
{
    QString emptyUuid = "";
    bool isVirtual = m_activeConnectResource->connectionIsVirtual(emptyUuid);
    QVERIFY(isVirtual == false);

    QString invalidUuid = "invalid-uuid-12345";
    bool isVirtual2 = m_activeConnectResource->connectionIsVirtual(invalidUuid);
    QVERIFY(isVirtual2 == false);
}

void TestKyActiveConnectResource::testWiredConnectIsActived()
{
    bool isActived = m_activeConnectResource->wiredConnectIsActived();
}

void TestKyActiveConnectResource::testCheckWirelessStatus()
{
    NetworkManager::ActiveConnection::State state = NetworkManager::ActiveConnection::State::Unknown;
    bool result = m_activeConnectResource->checkWirelessStatus(state);

    state = NetworkManager::ActiveConnection::State::Activated;
    bool result2 = m_activeConnectResource->checkWirelessStatus(state);

    state = NetworkManager::ActiveConnection::State::Deactivated;
    bool result3 = m_activeConnectResource->checkWirelessStatus(state);
}

void TestKyActiveConnectResource::testGetAcitveConnectionPathByUuid()
{
    QString emptyUuid = "";
    QString path = m_activeConnectResource->getAcitveConnectionPathByUuid(emptyUuid);
    QVERIFY(path.isEmpty());

    QString invalidUuid = "invalid-uuid-12345";
    QString path2 = m_activeConnectResource->getAcitveConnectionPathByUuid(invalidUuid);
    QVERIFY(path2.isEmpty());
}

void TestKyActiveConnectResource::testGetActivateWifiSignal()
{
    QString emptyDevName = "";
    int signal = m_activeConnectResource->getActivateWifiSignal(emptyDevName);

    QString devName = "wlan0";
    int signal2 = m_activeConnectResource->getActivateWifiSignal(devName);
}

void TestKyActiveConnectResource::testCheckInternetLoading()
{
    bool loading = m_activeConnectResource->checkInternetLoading();
}

void TestKyActiveConnectResource::testGetActivateWifiUni()
{
    QString emptyDevName = "";
    QString uni = m_activeConnectResource->getActivateWifiUni(emptyDevName);
    QVERIFY(uni.isEmpty());

    QString devName = "wlan0";
    QString uni2 = m_activeConnectResource->getActivateWifiUni(devName);
}
