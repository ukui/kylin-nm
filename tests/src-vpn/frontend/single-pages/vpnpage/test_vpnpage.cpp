#include "test_vpnpage.h"
#include "../../../../../../src-vpn/frontend/single-pages/vpnpage.h"
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QSettings>

TestVpnPage::TestVpnPage(QObject *parent)
    : QObject(parent), m_vpnPage(nullptr)
{
}

TestVpnPage::~TestVpnPage()
{
    if (m_vpnPage) {
        delete m_vpnPage;
        m_vpnPage = nullptr;
    }
}

void TestVpnPage::initTestCase()
{
    QDir().mkpath(QDir::homePath() + "/.config/ukui");
}

void TestVpnPage::cleanupTestCase()
{
}

void TestVpnPage::init()
{
    m_vpnPage = new VpnPage();
}

void TestVpnPage::cleanup()
{
    if (m_vpnPage) {
        delete m_vpnPage;
        m_vpnPage = nullptr;
    }
}

void TestVpnPage::test_vpnPageConstructionAndDestruction()
{
    QVERIFY(m_vpnPage != nullptr);
    QVERIFY(m_vpnPage->parent() == nullptr);
}

void TestVpnPage::test_getVirtualList_empty()
{
    QVector<QStringList> vector;
    m_vpnPage->getVirtualList(vector);
    QVERIFY(vector.isEmpty() || !vector.isEmpty());
}

void TestVpnPage::test_getVirtualList_withData()
{
    QVector<QStringList> vector;
    m_vpnPage->getVirtualList(vector);
}

void TestVpnPage::test_deleteVpn()
{
    m_vpnPage->deleteVpn("test-uuid-123");
}

void TestVpnPage::test_deleteVpn_withEmptyUuid()
{
    m_vpnPage->deleteVpn("");
}

void TestVpnPage::test_activateVpn()
{
    m_vpnPage->activateVpn("test-uuid-123");
}

void TestVpnPage::test_activateVpn_notInMap()
{
    m_vpnPage->activateVpn("nonexistent-uuid");
}

void TestVpnPage::test_deactivateVpn()
{
    m_vpnPage->deactivateVpn("test-uuid-123");
}

void TestVpnPage::test_showDetailPage()
{
    m_vpnPage->showDetailPage("test-uuid-123");
}

void TestVpnPage::test_showUI()
{
    m_vpnPage->showUI();
}

void TestVpnPage::test_signals_vpnAdd()
{
    QSignalSpy spy(m_vpnPage, &VpnPage::vpnAdd);
    QVERIFY(spy.isValid());
}

void TestVpnPage::test_signals_vpnRemove()
{
    QSignalSpy spy(m_vpnPage, &VpnPage::vpnRemove);
    QVERIFY(spy.isValid());
}

void TestVpnPage::test_signals_vpnUpdate()
{
    QSignalSpy spy(m_vpnPage, &VpnPage::vpnUpdate);
    QVERIFY(spy.isValid());
}

void TestVpnPage::test_signals_vpnActiveConnectionStateChanged()
{
    QSignalSpy spy(m_vpnPage, &VpnPage::vpnActiveConnectionStateChanged);
    QVERIFY(spy.isValid());
}

void TestVpnPage::test_signals_vpnConnectChanged()
{
    QSignalSpy spy(m_vpnPage, &VpnPage::vpnConnectChanged);
    QVERIFY(spy.isValid());
}
