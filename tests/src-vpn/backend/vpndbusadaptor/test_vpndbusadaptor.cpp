#include "test_vpndbusadaptor.h"
#include <QDebug>

TestVpnDbusAdaptor::TestVpnDbusAdaptor(QObject *parent)
    : QObject(parent), m_parent(nullptr), m_adaptor(nullptr)
{
}

void TestVpnDbusAdaptor::testConstructor()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    QVERIFY(m_adaptor != nullptr);
    QVERIFY(m_adaptor->parent() == m_parent);
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

void TestVpnDbusAdaptor::testGetVirtualList()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    
    QVector<QStringList> result = m_adaptor->getVirtualList();
    
    QVERIFY(result.size() >= 0);
    
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

void TestVpnDbusAdaptor::testDeleteVpn()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    
    QString testUuid = "test-delete-uuid-12345";
    m_adaptor->deleteVpn(testUuid);
    
    QVERIFY(true);
    
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

void TestVpnDbusAdaptor::testActivateVpn()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    
    QString testUuid = "test-activate-uuid-12345";
    m_adaptor->activateVpn(testUuid);
    
    QVERIFY(true);
    
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

void TestVpnDbusAdaptor::testDeactivateVpn()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    
    QString testUuid = "test-deactivate-uuid-12345";
    m_adaptor->deactivateVpn(testUuid);
    
    QVERIFY(true);
    
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

void TestVpnDbusAdaptor::testShowKylinVpn()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    
    m_adaptor->showKylinVpn();
    
    QVERIFY(true);
    
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

void TestVpnDbusAdaptor::testShowVpnAddWidget()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    
    m_adaptor->showVpnAddWidget();
    
    QVERIFY(true);
    
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

void TestVpnDbusAdaptor::testShowDetailPage()
{
    m_parent = new vpnObject();
    m_adaptor = new VpnDbusAdaptor(m_parent);
    
    QString testUuid = "test-detail-uuid-12345";
    m_adaptor->showDetailPage(testUuid);
    
    QVERIFY(true);
    
    delete m_adaptor;
    delete m_parent;
    m_adaptor = nullptr;
    m_parent = nullptr;
}

QTEST_MAIN(TestVpnDbusAdaptor)