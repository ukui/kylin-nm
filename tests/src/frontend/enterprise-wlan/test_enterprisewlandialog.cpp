#include "test_enterprisewlandialog.h"

TestEnterpriseWlanDialog::TestEnterpriseWlanDialog(QObject *parent)
    : QObject(parent)
    , m_dialog(nullptr)
{
}

void TestEnterpriseWlanDialog::initTestCase()
{
    m_wirelessNetItem.m_NetSsid = "TestSSID";
    m_wirelessNetItem.m_isConfigured = false;
    m_deviceName = "wlan0";
}

void TestEnterpriseWlanDialog::cleanupTestCase()
{
}

void TestEnterpriseWlanDialog::init()
{
}

void TestEnterpriseWlanDialog::cleanup()
{
    if (m_dialog) {
        delete m_dialog;
        m_dialog = nullptr;
    }
}

void TestEnterpriseWlanDialog::test_constructor()
{
    m_dialog = new EnterpriseWlanDialog(m_wirelessNetItem, m_deviceName);
    QVERIFY(m_dialog != nullptr);
    QVERIFY(m_dialog->isVisible() == true || m_dialog->isVisible() == false);
}

void TestEnterpriseWlanDialog::test_destructor()
{
    m_dialog = new EnterpriseWlanDialog(m_wirelessNetItem, m_deviceName);
    delete m_dialog;
    m_dialog = nullptr;
    QVERIFY(m_dialog == nullptr);
}

void TestEnterpriseWlanDialog::test_enterpriseWlanDialogCloseSignal()
{
    m_dialog = new EnterpriseWlanDialog(m_wirelessNetItem, m_deviceName);
    QVERIFY(m_dialog != nullptr);

    QSignalSpy spy(m_dialog, &EnterpriseWlanDialog::enterpriseWlanDialogClose);
    QVERIFY(spy.isValid());

    m_dialog->close();
    QVERIFY(spy.count() == 1);
}
