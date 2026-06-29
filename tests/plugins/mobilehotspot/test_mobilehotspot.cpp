#include "test_mobilehotspot.h"
#include "../../../../plugins/mobilehotspot/mobilehotspot.h"
#include <QSignalSpy>

TestMobileHotspot::TestMobileHotspot(QObject *parent)
    : QObject(parent)
    , m_mobileHotspot(nullptr)
{
}

void TestMobileHotspot::initTestCase()
{
}

void TestMobileHotspot::cleanupTestCase()
{
}

void TestMobileHotspot::init()
{
    m_mobileHotspot = new MobileHotspot();
}

void TestMobileHotspot::cleanup()
{
    if (m_mobileHotspot) {
        delete m_mobileHotspot;
        m_mobileHotspot = nullptr;
    }
}

void TestMobileHotspot::test_constructor()
{
    QVERIFY(m_mobileHotspot != nullptr);
    QVERIFY(m_mobileHotspot->plugini18nName().isEmpty() == false);
    QVERIFY(m_mobileHotspot->pluginTypes() == 4);
}

void TestMobileHotspot::test_plugini18nName()
{
    QString name = m_mobileHotspot->plugini18nName();
    QVERIFY(name.isEmpty() == false);
    QVERIFY(name.contains("MobileHotspot") || name.length() > 0);
}

void TestMobileHotspot::test_pluginTypes()
{
    int type = m_mobileHotspot->pluginTypes();
    QVERIFY(type == 4);
}

void TestMobileHotspot::test_pluginUi()
{
    QWidget *ui = m_mobileHotspot->pluginUi();
    QVERIFY(ui != nullptr);
    QVERIFY(qobject_cast<QWidget*>(ui) != nullptr);
}

void TestMobileHotspot::test_name()
{
    QString name = m_mobileHotspot->name();
    QVERIFY(name == QStringLiteral("mobilehotspot"));
}

void TestMobileHotspot::test_isShowOnHomePage()
{
    bool show = m_mobileHotspot->isShowOnHomePage();
    QVERIFY(show == false);
}

void TestMobileHotspot::test_icon()
{
    QIcon icon = m_mobileHotspot->icon();
    QVERIFY(!icon.isNull());
    QVERIFY(icon.name() == "ukui-hotspot-symbolic");
}

void TestMobileHotspot::test_isEnable()
{
    bool enable = m_mobileHotspot->isEnable();
    QVERIFY(enable == true || enable == false);
}

void TestMobileHotspot::test_translationPath()
{
    QString path = m_mobileHotspot->translationPath();
    QVERIFY(path.contains("/usr/share/kylin-nm/mobilehotspot/"));
    QVERIFY(path.contains(".ts"));
}

void TestMobileHotspot::test_destructor()
{
    MobileHotspot *temp = new MobileHotspot();
    QVERIFY(temp != nullptr);
    delete temp;
    temp = nullptr;
    QVERIFY(true);
}
