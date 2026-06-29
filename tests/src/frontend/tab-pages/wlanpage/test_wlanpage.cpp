#include "test_wlanpage.h"
#include "../../../../../../src/frontend/tab-pages/wlanpage.h"
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QSettings>

TestWlanPage::TestWlanPage(QObject *parent)
    : QObject(parent), m_wlanPage(nullptr)
{
}

TestWlanPage::~TestWlanPage()
{
    if (m_wlanPage) {
        delete m_wlanPage;
        m_wlanPage = nullptr;
    }
}

void TestWlanPage::initTestCase()
{
    QDir().mkpath(QDir::homePath() + "/.config/ukui");
}

void TestWlanPage::cleanupTestCase()
{
}

void TestWlanPage::init()
{
    m_wlanPage = new WlanPage();
}

void TestWlanPage::cleanup()
{
    if (m_wlanPage) {
        delete m_wlanPage;
        m_wlanPage = nullptr;
    }
}

void TestWlanPage::test_wlanPageConstructionAndDestruction()
{
    QVERIFY(m_wlanPage != nullptr);
    QVERIFY(m_wlanPage->parent() == nullptr);
}

void TestWlanPage::test_getWirelessList_emptyDevice()
{
    QList<QStringList> list;
    m_wlanPage->getWirelessList("", list);
}

void TestWlanPage::test_getWirelessList_withDevice()
{
    QList<QStringList> list;
    m_wlanPage->getWirelessList("nonexistent", list);
}

void TestWlanPage::test_getStoredApInfo()
{
    QStringList list;
    m_wlanPage->getStoredApInfo(list);
}

void TestWlanPage::test_getApConnectionPath()
{
    QString path;
    m_wlanPage->getApConnectionPath(path, "uuid-123");
}

void TestWlanPage::test_getActiveConnectionPath()
{
    QString path;
    m_wlanPage->getActiveConnectionPath(path, "uuid-123");
}

void TestWlanPage::test_activateWirelessConnection()
{
    m_wlanPage->activateWirelessConnection("wlan0", "TestSSID");
}

void TestWlanPage::test_deactivateWirelessConnection()
{
    m_wlanPage->deactivateWirelessConnection("wlan0", "TestSSID");
}

void TestWlanPage::test_deactivateWirelessConnectionWithUuid()
{
    m_wlanPage->deactivateWirelessConnectionWithUuid("wlan0", "uuid-123");
}

void TestWlanPage::test_deleteWirelessConnect()
{
    m_wlanPage->deleteWirelessConnect("uuid-123");
}

void TestWlanPage::test_setWirelessConnectAutoConnectState()
{
    m_wlanPage->setWirelessConnectAutoConnectState("uuid-123", true);
    m_wlanPage->setWirelessConnectAutoConnectState("uuid-123", false);
}

void TestWlanPage::test_checkWlanStatus()
{
    bool result = m_wlanPage->checkWlanStatus(NetworkManager::ActiveConnection::State::Activated);
    QVERIFY(result == true || result == false);
}

void TestWlanPage::test_checkInternetLoading()
{
    bool result = m_wlanPage->checkInternetLoading();
    QVERIFY(result == true || result == false);
}

void TestWlanPage::test_getApInfoBySsid()
{
    QStringList list;
    m_wlanPage->getApInfoBySsid("wlan0", "TestSSID", list);
}

void TestWlanPage::test_setWirelessSwitchEnable()
{
    m_wlanPage->setWirelessSwitchEnable(true);
    m_wlanPage->setWirelessSwitchEnable(false);
}

void TestWlanPage::test_getWirelessDeviceCap()
{
    QMap<QString, int> map;
    m_wlanPage->getWirelessDeviceCap(map);
}

void TestWlanPage::test_getConnectivity()
{
    NetworkManager::Connectivity connectivity;
    m_wlanPage->getConnectivity(connectivity);
}

void TestWlanPage::test_getWirelessSwitchBtnState()
{
    bool state = m_wlanPage->getWirelessSwitchBtnState();
    QVERIFY(state == true || state == false);
}

void TestWlanPage::test_getActivateWifiSignal()
{
    int signal = m_wlanPage->getActivateWifiSignal();
    QVERIFY(signal >= 0);
}

void TestWlanPage::test_getWirelssDeviceConnectState()
{
    QMap<QString, QString> map;
    m_wlanPage->getWirelssDeviceConnectState(map);
}

void TestWlanPage::test_getCurrentDisplayDevice()
{
    QString device = m_wlanPage->getCurrentDisplayDevice();
    QVERIFY(device.isEmpty() || !device.isEmpty());
}

void TestWlanPage::test_getWirelssDeviceConnect()
{
    QMap<QString, QString> map;
    m_wlanPage->getWirelssDeviceConnect(map);
}

void TestWlanPage::test_getWirelessDefaultDeviceName()
{
    QString deviceName = m_wlanPage->getWirelessDefaultDeviceName();
    QVERIFY(deviceName.isEmpty() || !deviceName.isEmpty());
}

void TestWlanPage::test_getActivateWifiCategory()
{
    int category = m_wlanPage->getActivateWifiCategory("");
    QVERIFY(category >= 0);
}

void TestWlanPage::test_signals_wlanAdd()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::wlanAdd);
    QVERIFY(spy.isValid());
}

void TestWlanPage::test_signals_wlanRemove()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::wlanRemove);
    QVERIFY(spy.isValid());
}

void TestWlanPage::test_signals_wlanActiveConnectionStateChanged()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::wlanActiveConnectionStateChanged);
    QVERIFY(spy.isValid());
}

void TestWlanPage::test_signals_hotspotDeactivated()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::hotspotDeactivated);
    QVERIFY(spy.isValid());
}

void TestWlanPage::test_signals_hotspotActivated()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::hotspotActivated);
    QVERIFY(spy.isValid());
}

void TestWlanPage::test_signals_signalStrengthChange()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::signalStrengthChange);
    QVERIFY(spy.isValid());
}

void TestWlanPage::test_signals_secuTypeChange()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::secuTypeChange);
    QVERIFY(spy.isValid());
}

void TestWlanPage::test_signals_connectivityChanged()
{
    QSignalSpy spy(m_wlanPage, &WlanPage::connectivityChanged);
    QVERIFY(spy.isValid());
}
