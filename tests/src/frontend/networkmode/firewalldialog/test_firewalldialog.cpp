#include "test_firewalldialog.h"
#include "../../../../../../src/frontend/networkmode/firewalldialog.h"
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QEvent>

TestFirewallDialog::TestFirewallDialog(QObject *parent)
    : QObject(parent), m_firewallDialog(nullptr)
{
}

TestFirewallDialog::~TestFirewallDialog()
{
    if (m_firewallDialog) {
        delete m_firewallDialog;
        m_firewallDialog = nullptr;
    }
}

void TestFirewallDialog::initTestCase()
{
    QDir().mkpath(QDir::homePath() + "/.config/ukui");
}

void TestFirewallDialog::cleanupTestCase()
{
}

void TestFirewallDialog::init()
{
    m_firewallDialog = new FirewallDialog();
}

void TestFirewallDialog::cleanup()
{
    if (m_firewallDialog) {
        delete m_firewallDialog;
        m_firewallDialog = nullptr;
    }
}

void TestFirewallDialog::test_firewallDialogConstructionAndDestruction()
{
    QVERIFY(m_firewallDialog != nullptr);
    QVERIFY(m_firewallDialog->parent() == nullptr);
}

void TestFirewallDialog::test_setUuid()
{
    QString testUuid = "test-uuid-12345";
    m_firewallDialog->setUuid(testUuid);
}

void TestFirewallDialog::test_centerToScreen()
{
    m_firewallDialog->centerToScreen();
    QVERIFY(m_firewallDialog->isVisible() == false);
}

void TestFirewallDialog::test_onPaletteChanged()
{
    Q_UNUSED(m_firewallDialog);
    QTest::qWait(100);
}

void TestFirewallDialog::test_closeMyself_matchingUuidAndStatus()
{
    QString testUuid = "test-uuid-12345";
    m_firewallDialog->setUuid(testUuid);
    m_firewallDialog->closeMyself(testUuid, 4);
}

void TestFirewallDialog::test_closeMyself_nonMatchingUuid()
{
    QString testUuid = "test-uuid-12345";
    m_firewallDialog->setUuid(testUuid);
    m_firewallDialog->closeMyself("different-uuid", 4);
}

void TestFirewallDialog::test_closeMyself_nonMatchingStatus()
{
    QString testUuid = "test-uuid-12345";
    m_firewallDialog->setUuid(testUuid);
    m_firewallDialog->closeMyself(testUuid, 0);
}

void TestFirewallDialog::test_signals_setPublicNetMode()
{
    QSignalSpy spy(m_firewallDialog, &FirewallDialog::setPublicNetMode);
    QVERIFY(spy.isValid());
    emit m_firewallDialog->setPublicNetMode();
    QVERIFY(spy.count() == 1);
}

void TestFirewallDialog::test_signals_setPrivateNetMode()
{
    QSignalSpy spy(m_firewallDialog, &FirewallDialog::setPrivateNetMode);
    QVERIFY(spy.isValid());
    emit m_firewallDialog->setPrivateNetMode();
    QVERIFY(spy.count() == 1);
}
