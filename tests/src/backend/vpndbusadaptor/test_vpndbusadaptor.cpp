#include "test_vpndbusadaptor.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDBusMetaType>

TestVpnDbusAdaptor::TestVpnDbusAdaptor(QObject *parent)
    : QObject(parent), m_mockVpnObject(nullptr)
{
}

TestVpnDbusAdaptor::~TestVpnDbusAdaptor()
{
    delete m_mockVpnObject;
}

void TestVpnDbusAdaptor::initTestCase()
{
    QVERIFY2(QCoreApplication::instance(), "QCoreApplication instance required");
    m_mockVpnObject = new MockVpnObject();
    QVERIFY(m_mockVpnObject != nullptr);
}

void TestVpnDbusAdaptor::cleanupTestCase()
{
}

void TestVpnDbusAdaptor::init()
{
}

void TestVpnDbusAdaptor::cleanup()
{
}

void TestVpnDbusAdaptor::testConstructor()
{
    QVERIFY(m_mockVpnObject != nullptr);
    
    VpnDbusAdaptor *adaptor = new VpnDbusAdaptor(m_mockVpnObject);
    QVERIFY(adaptor != nullptr);
    QVERIFY(adaptor->parent() == m_mockVpnObject);
    delete adaptor;
}

void TestVpnDbusAdaptor::testDestructor()
{
    VpnDbusAdaptor *adaptor = new VpnDbusAdaptor(m_mockVpnObject);
    QVERIFY(adaptor != nullptr);
    delete adaptor;
}

void TestVpnDbusAdaptor::testGetVirtualList()
{
    VpnDbusAdaptor adaptor(m_mockVpnObject);
    m_mockVpnObject->m_virtualListCalled = false;
    
    QVector<QStringList> result = adaptor.getVirtualList();
    
    QVERIFY(m_mockVpnObject->m_virtualListCalled == true);
    QVERIFY(result.size() > 0);
    QVERIFY(result[0].size() == 3);
    QVERIFY(result[0][0] == "test-vpn");
}

void TestVpnDbusAdaptor::testDeleteVpn()
{
    VpnDbusAdaptor adaptor(m_mockVpnObject);
    m_mockVpnObject->m_deleteVpnUuid = "";
    
    QString testUuid = "test-uuid-12345";
    adaptor.deleteVpn(testUuid);
    
    QVERIFY(m_mockVpnObject->m_deleteVpnUuid == testUuid);
}

void TestVpnDbusAdaptor::testActivateVpn()
{
    VpnDbusAdaptor adaptor(m_mockVpnObject);
    m_mockVpnObject->m_activateVpnUuid = "";
    
    QString testUuid = "test-activate-uuid";
    adaptor.activateVpn(testUuid);
    
    QVERIFY(m_mockVpnObject->m_activateVpnUuid == testUuid);
}

void TestVpnDbusAdaptor::testDeactivateVpn()
{
    VpnDbusAdaptor adaptor(m_mockVpnObject);
    m_mockVpnObject->m_deactivateVpnUuid = "";
    
    QString testUuid = "test-deactivate-uuid";
    adaptor.deactivateVpn(testUuid);
    
    QVERIFY(m_mockVpnObject->m_deactivateVpnUuid == testUuid);
}

void TestVpnDbusAdaptor::testShowKylinVpn()
{
    VpnDbusAdaptor adaptor(m_mockVpnObject);
    m_mockVpnObject->m_showMainWindowCalled = false;
    
    adaptor.showKylinVpn();
    
    QVERIFY(m_mockVpnObject->m_showMainWindowCalled == true);
}

void TestVpnDbusAdaptor::testSignals()
{
    VpnDbusAdaptor adaptor(m_mockVpnObject);
    
    QSignalSpy spyAdd(&adaptor, SIGNAL(vpnAdd(QStringList)));
    QSignalSpy spyRemove(&adaptor, SIGNAL(vpnRemove(QString)));
    QSignalSpy spyUpdate(&adaptor, SIGNAL(vpnUpdate(QStringList)));
    QSignalSpy spyStateChanged(&adaptor, SIGNAL(vpnActiveConnectionStateChanged(QString,int)));
    QSignalSpy spyActivateFailed(&adaptor, SIGNAL(activateFailed(QString)));
    QSignalSpy spyDeactivateFailed(&adaptor, SIGNAL(deactivateFailed(QString)));
    
    QVERIFY(spyAdd.isValid());
    QVERIFY(spyRemove.isValid());
    QVERIFY(spyUpdate.isValid());
    QVERIFY(spyStateChanged.isValid());
    QVERIFY(spyActivateFailed.isValid());
    QVERIFY(spyDeactivateFailed.isValid());
}
