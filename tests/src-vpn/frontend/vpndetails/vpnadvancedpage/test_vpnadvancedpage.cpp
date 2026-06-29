#include "test_vpnadvancedpage.h"
#include "../../../../../../src-vpn/frontend/vpndetails/vpnadvancedpage.h"
#include "../../../../../../src-vpn/frontend/vpndetails/vpnconfigpage.h"
#include "../../../../../../src/backend/dbus-interface/kyvpnconnectoperation.h"
#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

TestVpnAdvancedPage::TestVpnAdvancedPage(QObject *parent)
    : QObject(parent), m_vpnAdvancedPage(nullptr)
{
}

TestVpnAdvancedPage::~TestVpnAdvancedPage()
{
    if (m_vpnAdvancedPage) {
        delete m_vpnAdvancedPage;
        m_vpnAdvancedPage = nullptr;
    }
}

void TestVpnAdvancedPage::initTestCase()
{
}

void TestVpnAdvancedPage::cleanupTestCase()
{
}

void TestVpnAdvancedPage::init()
{
    m_vpnAdvancedPage = new VpnAdvancedPage();
}

void TestVpnAdvancedPage::cleanup()
{
    if (m_vpnAdvancedPage) {
        delete m_vpnAdvancedPage;
        m_vpnAdvancedPage = nullptr;
    }
}

void TestVpnAdvancedPage::test_constructor()
{
    QVERIFY(m_vpnAdvancedPage != nullptr);
    QVERIFY(m_vpnAdvancedPage->parent() == nullptr);
    
    QList<QCheckBox*> checkboxes = m_vpnAdvancedPage->findChildren<QCheckBox*>();
    QVERIFY(checkboxes.count() > 0);
}
