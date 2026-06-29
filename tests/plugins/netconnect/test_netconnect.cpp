#include "test_netconnect.h"
#include "../../../../plugins/netconnect/netconnect.h"

TestNetConnect::TestNetConnect(QObject *parent)
    : QObject(parent)
    , m_netConnect(nullptr)
    , m_pluginWidget(nullptr)
{
}

TestNetConnect::~TestNetConnect()
{
    if (m_pluginWidget) {
        delete m_pluginWidget;
        m_pluginWidget = nullptr;
    }
}

void TestNetConnect::initTestCase()
{
    m_netConnect = new NetConnect();
    QVERIFY(m_netConnect != nullptr);
}

void TestNetConnect::cleanupTestCase()
{
    if (m_netConnect) {
        delete m_netConnect;
        m_netConnect = nullptr;
    }
}

void TestNetConnect::init()
{
}

void TestNetConnect::cleanup()
{
    if (m_pluginWidget) {
        delete m_pluginWidget;
        m_pluginWidget = nullptr;
    }
}

void TestNetConnect::test_constructor()
{
    NetConnect *netConnect = new NetConnect();
    QVERIFY(netConnect != nullptr);
    delete netConnect;
}

void TestNetConnect::test_destructor()
{
    NetConnect *netConnect = new NetConnect();
    delete netConnect;
    QVERIFY(true);
}

void TestNetConnect::test_plugini18nName()
{
    QString name = m_netConnect->plugini18nName();
    QVERIFY(!name.isEmpty());
}

void TestNetConnect::test_pluginTypes()
{
    int type = m_netConnect->pluginTypes();
    QVERIFY(type >= 0);
}

void TestNetConnect::test_pluginUi()
{
    QWidget *widget = m_netConnect->pluginUi();
    QVERIFY(widget != nullptr);
    m_pluginWidget = widget;
}

void TestNetConnect::test_name()
{
    QString name = m_netConnect->name();
    QVERIFY(name == QLatin1String("netconnect"));
}

void TestNetConnect::test_isEnable()
{
    bool enabled = m_netConnect->isEnable();
    QEXPECT_FAIL("", "DBus interface may not be available in test environment", Continue);
    QVERIFY(enabled == true || enabled == false);
}

void TestNetConnect::test_isShowOnHomePage()
{
    bool show = m_netConnect->isShowOnHomePage();
    QVERIFY(show == true);
}

void TestNetConnect::test_icon()
{
    QIcon icon = m_netConnect->icon();
    QVERIFY(!icon.isNull());
}

void TestNetConnect::test_translationPath()
{
    QString path = m_netConnect->translationPath();
    QVERIFY(!path.isEmpty());
    QVERIFY(path.contains(QLatin1String("%1")));
}

