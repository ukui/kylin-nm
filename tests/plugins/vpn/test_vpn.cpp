#include <QCoreApplication>
#include <QDebug>
#include <QtTest>
#include <QSignalSpy>
#include <QWidget>

#include "vpn.h"

class TestVpn : public QObject
{
    Q_OBJECT

public:
    TestVpn();
    ~TestVpn();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_plugini18nName();
    void test_pluginTypes();
    void test_name();
    void test_isShowOnHomePage();
    void test_icon();
    void test_isEnable();
    void test_translationPath();
    void test_pluginUi();
    void test_pluginUi_twice();
    void test_destructor();

private:
    Vpn *m_vpn;
    QWidget *m_pluginWidget;
};

TestVpn::TestVpn()
    : m_vpn(nullptr)
    , m_pluginWidget(nullptr)
{}

TestVpn::~TestVpn()
{}

void TestVpn::initTestCase()
{
    m_vpn = new Vpn();
    QVERIFY(m_vpn != nullptr);
}

void TestVpn::cleanupTestCase()
{
    if (m_vpn) {
        delete m_vpn;
        m_vpn = nullptr;
    }
}

void TestVpn::init()
{}

void TestVpn::cleanup()
{}

void TestVpn::test_constructor()
{
    Vpn *vpn = new Vpn();
    QVERIFY(vpn != nullptr);
    delete vpn;
}

void TestVpn::test_plugini18nName()
{
    QString name = m_vpn->plugini18nName();
    QVERIFY(!name.isEmpty());
    qDebug() << "plugini18nName:" << name;
}

void TestVpn::test_pluginTypes()
{
    int type = m_vpn->pluginTypes();
    QVERIFY(type >= 0);
    qDebug() << "pluginTypes:" << type;
}

void TestVpn::test_name()
{
    QString name = m_vpn->name();
    QVERIFY(!name.isEmpty());
    QVERIFY(name == QStringLiteral("Vpn"));
    qDebug() << "name:" << name;
}

void TestVpn::test_isShowOnHomePage()
{
    bool show = m_vpn->isShowOnHomePage();
    QVERIFY(show == true);
    qDebug() << "isShowOnHomePage:" << show;
}

void TestVpn::test_icon()
{
    QIcon icon = m_vpn->icon();
    QVERIFY(!icon.isNull());
    qDebug() << "icon valid:" << !icon.isNull();
}

void TestVpn::test_isEnable()
{
    bool enable = m_vpn->isEnable();
    QVERIFY(enable == true);
    qDebug() << "isEnable:" << enable;
}

void TestVpn::test_translationPath()
{
    QString path = m_vpn->translationPath();
    QVERIFY(!path.isEmpty());
    QVERIFY(path.contains("%1"));
    qDebug() << "translationPath:" << path;
}

void TestVpn::test_pluginUi()
{
    QWidget *widget = m_vpn->pluginUi();
    QVERIFY(widget != nullptr);
    QVERIFY(m_pluginWidget == nullptr);
    m_pluginWidget = widget;
    qDebug() << "pluginUi widget created";
}

void TestVpn::test_pluginUi_twice()
{
    QWidget *widget = m_vpn->pluginUi();
    QVERIFY(widget != nullptr);
    QVERIFY(m_pluginWidget != nullptr);
    QVERIFY(widget == m_pluginWidget);
    qDebug() << "pluginUi returns same widget";
}

void TestVpn::test_destructor()
{
    Vpn *vpn = new Vpn();
    QWidget *widget = vpn->pluginUi();
    QVERIFY(widget != nullptr);
    delete vpn;
    qDebug() << "destructor test passed";
}

QTEST_MAIN(TestVpn)

#include "test_vpn.moc"
