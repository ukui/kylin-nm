#include "test_networkmodeconfig.h"
#include "../../../../../../src/frontend/networkmode/networkmodeconfig.h"
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QEvent>
#include <QDebug>

TestNetworkModeConfig::TestNetworkModeConfig(QObject *parent)
    : QObject(parent), m_netModeConfig(nullptr)
{
}

TestNetworkModeConfig::~TestNetworkModeConfig()
{
    if (m_netModeConfig) {
        delete m_netModeConfig;
        m_netModeConfig = nullptr;
    }
}

void TestNetworkModeConfig::initTestCase()
{
    QDir().mkpath(QDir::homePath() + "/.config/ukui");
}

void TestNetworkModeConfig::cleanupTestCase()
{
}

void TestNetworkModeConfig::init()
{
    m_netModeConfig = NetworkModeConfig::getInstance();
}

void TestNetworkModeConfig::cleanup()
{
    if (m_netModeConfig) {
        m_netModeConfig = nullptr;
    }
}

void TestNetworkModeConfig::test_getInstance()
{
    NetworkModeConfig *instance = NetworkModeConfig::getInstance();
    QVERIFY(instance != nullptr);
    NetworkModeConfig *instance2 = NetworkModeConfig::getInstance();
    QVERIFY(instance == instance2);
}

void TestNetworkModeConfig::test_networkModeConfigConstruction()
{
    QVERIFY(m_netModeConfig != nullptr);
}

void TestNetworkModeConfig::test_getNetworkModeConfigAsync_withValidUuid()
{
    QString testUuid = "test-uuid-12345";
    QSignalSpy spy(m_netModeConfig, &NetworkModeConfig::networkModeConfigReady);
    QVERIFY(spy.isValid());
    m_netModeConfig->getNetworkModeConfigAsync(testUuid);
    QTest::qWait(3000);
}

void TestNetworkModeConfig::test_getNetworkModeConfigAsync_withEmptyUuid()
{
    QString emptyUuid = "";
    QSignalSpy spy(m_netModeConfig, &NetworkModeConfig::networkModeConfigReady);
    QVERIFY(spy.isValid());
    m_netModeConfig->getNetworkModeConfigAsync(emptyUuid);
    QTest::qWait(100);
}

void TestNetworkModeConfig::test_getNetworkModeConfig_withValidUuid()
{
    QString testUuid = "test-uuid-12345";
    int result = m_netModeConfig->getNetworkModeConfig(testUuid);
    QVERIFY(result >= -2 && result <= 1);
}

void TestNetworkModeConfig::test_getNetworkModeConfig_withEmptyUuid()
{
    QString emptyUuid = "";
    int result = m_netModeConfig->getNetworkModeConfig(emptyUuid);
    QVERIFY(result == NO_CONFIG);
}

void TestNetworkModeConfig::test_setNetworkModeConfig()
{
    QString uuid = "test-uuid-12345";
    QString cardName = "eth0";
    QString ssid = "TestNetwork";
    int mode = KSC_FIREWALL_PUBLIC;
    m_netModeConfig->setNetworkModeConfig(uuid, cardName, ssid, mode);
    QTest::qWait(100);
}

void TestNetworkModeConfig::test_breakNetworkConnect()
{
    QString uuid = "test-uuid-12345";
    QString cardName = "eth0";
    QString ssid = "TestNetwork";
    m_netModeConfig->breakNetworkConnect(uuid, cardName, ssid);
    QTest::qWait(100);
}

void TestNetworkModeConfig::test_signal_networkModeConfigReady()
{
    QSignalSpy spy(m_netModeConfig, &NetworkModeConfig::networkModeConfigReady);
    QVERIFY(spy.isValid());
    emit m_netModeConfig->networkModeConfigReady("test-uuid", 0, true);
    QVERIFY(spy.count() == 1);
    QList<QVariant> args = spy.takeFirst();
    QVERIFY(args.at(0).toString() == "test-uuid");
    QVERIFY(args.at(1).toInt() == 0);
    QVERIFY(args.at(2).toBool() == true);
}

