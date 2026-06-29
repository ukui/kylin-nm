#include <QTest>
#include <QCoreApplication>
#include <QDebug>
#include <QtTest>
#include "test_networkqml_plugin.h"
#include "../../../../qml-network/plugin/networkqml_plugin.h"
#include "../../../../qml-network/plugin/knminterface.h"

TestNetworkqmlPlugin::TestNetworkqmlPlugin(QObject *parent)
    : QObject(parent)
    , m_plugin(nullptr)
{
}

TestNetworkqmlPlugin::~TestNetworkqmlPlugin()
{
}

void TestNetworkqmlPlugin::initTestCase()
{
}

void TestNetworkqmlPlugin::cleanupTestCase()
{
}

void TestNetworkqmlPlugin::init()
{
    m_plugin = new NetworkhqmlPlugin();
}

void TestNetworkqmlPlugin::cleanup()
{
    if (m_plugin) {
        delete m_plugin;
        m_plugin = nullptr;
    }
}

void TestNetworkqmlPlugin::test_registerTypes_correctUri()
{
    const char *correctUri = "org.ukui.shortcut.network";
    m_plugin->registerTypes(correctUri);
    QVERIFY(true);
}

void TestNetworkqmlPlugin::test_registerTypes_incorrectUri()
{
    QEXPECT_FAIL("incorrectUri", "Q_ASSERT will fail for incorrect URI", Continue);
    const char *incorrectUri = "wrong.uri";
    m_plugin->registerTypes(incorrectUri);
}
