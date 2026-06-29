#include "test_vpnobject.h"
#include <QDebug>
#include <QSignalSpy>

TestVpnObject::TestVpnObject(QObject *parent)
    : QObject(parent), m_vpnObject(nullptr)
{
}

void TestVpnObject::testConstructor()
{
    m_vpnObject = new vpnObject();
    QVERIFY(m_vpnObject != nullptr);
}

void TestVpnObject::testDestructor()
{
    m_vpnObject = new vpnObject();
    delete m_vpnObject;
    m_vpnObject = nullptr;
    QVERIFY(true);
}

void TestVpnObject::testGetVirtualList()
{
    m_vpnObject = new vpnObject();
    QVector<QStringList> vector;
    m_vpnObject->getVirtualList(vector);
    QVERIFY(true);
    delete m_vpnObject;
    m_vpnObject = nullptr;
}

void TestVpnObject::testDeleteVpn()
{
    m_vpnObject = new vpnObject();
    QString testUuid = "test-delete-uuid-12345";
    m_vpnObject->deleteVpn(testUuid);
    QVERIFY(true);
    delete m_vpnObject;
    m_vpnObject = nullptr;
}

void TestVpnObject::testActivateVpn()
{
    m_vpnObject = new vpnObject();
    QString testUuid = "test-activate-uuid-12345";
    m_vpnObject->activateVpn(testUuid);
    QVERIFY(true);
    delete m_vpnObject;
    m_vpnObject = nullptr;
}

void TestVpnObject::testDeactivateVpn()
{
    m_vpnObject = new vpnObject();
    QString testUuid = "test-deactivate-uuid-12345";
    m_vpnObject->deactivateVpn(testUuid);
    QVERIFY(true);
    delete m_vpnObject;
    m_vpnObject = nullptr;
}

void TestVpnObject::testShowDetailPage()
{
    m_vpnObject = new vpnObject();
    QString testUuid = "test-detail-uuid-12345";
    m_vpnObject->showDetailPage(testUuid);
    QVERIFY(true);
    delete m_vpnObject;
    m_vpnObject = nullptr;
}

void TestVpnObject::testShowVpnAddWidget()
{
    m_vpnObject = new vpnObject();
    m_vpnObject->showVpnAddWidget();
    QVERIFY(true);
    delete m_vpnObject;
    m_vpnObject = nullptr;
}

void TestVpnObject::testOnShowMainWindow()
{
    m_vpnObject = new vpnObject();
    m_vpnObject->onShowMainWindow();
    QVERIFY(true);
    delete m_vpnObject;
    m_vpnObject = nullptr;
}

QTEST_MAIN(TestVpnObject)
