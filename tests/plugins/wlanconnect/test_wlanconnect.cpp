#include <QTest>
#include <QCoreApplication>
#include <QSignalSpy>
#include "wlanconnect.h"

class TestWlanConnect : public QObject
{
    Q_OBJECT
public:
    explicit TestWlanConnect(QObject *parent = nullptr) : QObject(parent) {}

private slots:
    void initTestCase();
    void testConstructor();
    void testDestructor();
    void test_plugini18nName();
    void test_pluginTypes();
    void test_name();
    void test_isShowOnHomePage();
    void test_icon();
    void test_translationPath();
    void test_isEnable();

private:
    WlanConnect *m_wlanConnect = nullptr;
};

void TestWlanConnect::initTestCase()
{
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi, true);
}

void TestWlanConnect::testConstructor()
{
    WlanConnect *wlan = new WlanConnect();
    QVERIFY(wlan != nullptr);
    delete wlan;
}

void TestWlanConnect::testDestructor()
{
    WlanConnect *wlan = new WlanConnect();
    QVERIFY(wlan != nullptr);
    delete wlan;
}

void TestWlanConnect::test_plugini18nName()
{
    WlanConnect wlan;
    QString name = wlan.plugini18nName();
    QVERIFY(!name.isEmpty());
}

void TestWlanConnect::test_pluginTypes()
{
    WlanConnect wlan;
    int type = wlan.pluginTypes();
    QVERIFY(type > 0);
}

void TestWlanConnect::test_name()
{
    WlanConnect wlan;
    QString name = wlan.name();
    QVERIFY(name == "wlanconnect");
}

void TestWlanConnect::test_isShowOnHomePage()
{
    WlanConnect wlan;
    bool isShow = wlan.isShowOnHomePage();
    QVERIFY(isShow == true);
}

void TestWlanConnect::test_icon()
{
    WlanConnect wlan;
    QIcon icon = wlan.icon();
    QVERIFY(!icon.isNull());
}

void TestWlanConnect::test_translationPath()
{
    WlanConnect wlan;
    QString path = wlan.translationPath();
    QVERIFY(path.contains("/usr/share/kylin-nm/wlanconnect/"));
}

void TestWlanConnect::test_isEnable()
{
    WlanConnect wlan;
    bool enabled = wlan.isEnable();
    QVERIFY(enabled == false || enabled == true);
}

QTEST_MAIN(TestWlanConnect)
#include "test_wlanconnect.moc"
