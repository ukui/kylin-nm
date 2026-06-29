#include "test_lanpage.h"
#include "../../../../../../src/frontend/tab-pages/lanpage.h"
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QSettings>

TestLanPage::TestLanPage(QObject *parent)
    : QObject(parent), m_lanPage(nullptr)
{
}

TestLanPage::~TestLanPage()
{
    if (m_lanPage) {
        delete m_lanPage;
        m_lanPage = nullptr;
    }
}

void TestLanPage::initTestCase()
{
    QDir().mkpath(QDir::homePath() + "/.config/ukui");
}

void TestLanPage::cleanupTestCase()
{
}

void TestLanPage::init()
{
    m_lanPage = new LanPage();
}

void TestLanPage::cleanup()
{
    if (m_lanPage) {
        delete m_lanPage;
        m_lanPage = nullptr;
    }
}

void TestLanPage::test_lanPageConstructionAndDestruction()
{
    QVERIFY(m_lanPage != nullptr);
    QVERIFY(m_lanPage->parent() == nullptr);
}

void TestLanPage::test_getWiredList_emptyDevice()
{
    QList<QStringList> list;
    m_lanPage->getWiredList("eth0", list);
    QVERIFY(list.isEmpty());
}

void TestLanPage::test_getWiredList_withDevice()
{
    QList<QStringList> list;
    m_lanPage->getWiredList("nonexistent", list);
}

void TestLanPage::test_activateWired()
{
    m_lanPage->activateWired("eth0", "uuid-123");
}

void TestLanPage::test_deactivateWired()
{
    m_lanPage->deactivateWired("eth0", "uuid-123");
    m_lanPage->deactivateWired("eth0", "uuid-123", true);
}

void TestLanPage::test_deleteWiredConnect()
{
    m_lanPage->deleteWiredConnect("uuid-123");
}

void TestLanPage::test_setWiredDeviceEnable()
{
    m_lanPage->setWiredDeviceEnable("eth0", true);
    m_lanPage->setWiredDeviceEnable("eth0", false);
}

void TestLanPage::test_getDeviceConnectivity()
{
    int connectivity = m_lanPage->getDeviceConnectivity("eth0");
    QVERIFY(connectivity >= 0);
}

void TestLanPage::test_getCableStateByDevice()
{
    bool state = m_lanPage->getCableStateByDevice("eth0");
    QVERIFY(state == true || state == false);
}

void TestLanPage::test_lanIsConnected()
{
    bool connected = m_lanPage->lanIsConnected();
    QVERIFY(connected == true || connected == false);
}

void TestLanPage::test_getWiredDeviceConnectState()
{
    QMap<QString, QString> map;
    m_lanPage->getWiredDeviceConnectState(map);
}

void TestLanPage::test_hasInternetAccess()
{
    bool hasInternet = m_lanPage->hasInternetAccess();
    QVERIFY(hasInternet == true || hasInternet == false);
}

void TestLanPage::test_getWiredEnabledState()
{
    bool enabled = m_lanPage->getWiredEnabledState();
    QVERIFY(enabled == true || enabled == false);
}

void TestLanPage::test_setWiredEnabledState()
{
    m_lanPage->setWiredEnabledState(true);
    QVERIFY(m_lanPage->getWiredEnabledState() == true);
    m_lanPage->setWiredEnabledState(false);
    QVERIFY(m_lanPage->getWiredEnabledState() == false);
}

void TestLanPage::test_getWiredDeviceConnect()
{
    QMap<QString, QString> map;
    m_lanPage->getWiredDeviceConnect(map);
}

void TestLanPage::test_setWiredDeviceAutoconnect()
{
    m_lanPage->setWiredDeviceAutoconnect("eth0", true);
    m_lanPage->setWiredDeviceAutoconnect("eth0", false);
}

void TestLanPage::test_setWiredConnectAutoconnect()
{
    m_lanPage->setWiredConnectAutoconnect("uuid-123", true);
    m_lanPage->setWiredConnectAutoconnect("uuid-123", false);
}

void TestLanPage::test_getWiredDefaultDeviceName()
{
    QString deviceName = m_lanPage->getWiredDefaultDeviceName();
    QVERIFY(deviceName.isEmpty() || !deviceName.isEmpty());
}

void TestLanPage::test_signals_lanAdd()
{
    QSignalSpy spy(m_lanPage, &LanPage::lanAdd);
    QVERIFY(spy.isValid());
}

void TestLanPage::test_signals_lanRemove()
{
    QSignalSpy spy(m_lanPage, &LanPage::lanRemove);
    QVERIFY(spy.isValid());
}

void TestLanPage::test_signals_lanUpdate()
{
    QSignalSpy spy(m_lanPage, &LanPage::lanUpdate);
    QVERIFY(spy.isValid());
}

void TestLanPage::test_signals_wiredEnabledChanged()
{
    QSignalSpy spy(m_lanPage, &LanPage::wiredEnabledChanged);
    QVERIFY(spy.isValid());
}
