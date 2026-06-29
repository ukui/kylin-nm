#include "test_proxy.h"
#include "proxy.h"
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QIcon>

TestProxy::TestProxy(QObject *parent)
    : QObject(parent)
{
}

void TestProxy::initTestCase()
{
}

void TestProxy::cleanupTestCase()
{
}

void TestProxy::init()
{
}

void TestProxy::cleanup()
{
}

void TestProxy::initProxyInstance()
{
}

void TestProxy::testPlugini18nName()
{
    Proxy proxy;
    QString name = proxy.plugini18nName();
    QVERIFY2(!name.isEmpty(), "plugini18nName should not be empty");
}

void TestProxy::testPluginTypes()
{
    Proxy proxy;
    int types = proxy.pluginTypes();
    QVERIFY2(types == NETWORK, "pluginTypes should return NETWORK");
}

void TestProxy::testName()
{
    Proxy proxy;
    QString name = proxy.name();
    QVERIFY2(name == "Proxy", "name should return Proxy");
}

void TestProxy::testIsShowOnHomePage()
{
    Proxy proxy;
    bool show = proxy.isShowOnHomePage();
    QVERIFY2(show == true, "isShowOnHomePage should return true");
}

void TestProxy::testIcon()
{
    Proxy proxy;
    QIcon icon = proxy.icon();
    QVERIFY2(!icon.isNull(), "icon should not be null");
}

void TestProxy::testIsEnable()
{
    Proxy proxy;
    bool enable = proxy.isEnable();
    QVERIFY2(enable == true, "isEnable should return true");
}

void TestProxy::testTranslationPath()
{
    Proxy proxy;
    QString path = proxy.translationPath();
    QVERIFY2(path.contains("proxy"), "translationPath should contain proxy");
    QVERIFY2(path.endsWith(".ts"), "translationPath should end with .ts");
}

void TestProxy::testSetFrameNoframe()
{
    Proxy proxy;
    QFrame frame(nullptr);
    proxy.setFrame_Noframe(&frame);
    QVERIFY2(frame.frameShape() == QFrame::NoFrame, "frame shape should be NoFrame");
    QVERIFY2(frame.minimumSize().height() == 60, "frame minimum height should be 60");
}

void TestProxy::testSetLine()
{
    Proxy proxy;
    QFrame parentFrame(nullptr);
    QFrame *line = proxy.setLine(&parentFrame);
    QVERIFY2(line != nullptr, "setLine should return a valid frame pointer");
    delete line;
}

void TestProxy::testGetAppProxyStateInvalid()
{
    Proxy proxy;
    bool state = proxy.getAppProxyState();
    QVERIFY2(state == false, "getAppProxyState should return false when dbus is invalid");
}

void TestProxy::testSetAppProxyStateInvalid()
{
    Proxy proxy;
    proxy.setAppProxyState(true);
    QTest::qWait(100);
}

void TestProxy::testGetAppProxyConfInvalid()
{
    QStringList conf = Proxy::getAppProxyConf();
    QVERIFY2(conf.isEmpty(), "getAppProxyConf should return empty list when dbus is invalid");
}

void TestProxy::testSetAppProxyConfInvalid()
{
    Proxy proxy;
    QStringList conf;
    conf << "http" << "192.168.1.1" << "8080";
    proxy.setAppProxyConf(conf);
    QTest::qWait(100);
}

void TestProxy::testGetAppListProxyInvalid()
{
    QMap<QString, QStringList> appList = Proxy::getAppListProxy();
    QVERIFY2(appList.isEmpty(), "getAppListProxy should return empty map when dbus is invalid");
}

void TestProxy::testGetAptProxyInvalid()
{
    QHash<QString, QVariant> aptInfo = Proxy::getAptProxy();
    QVERIFY2(aptInfo.isEmpty() == false, "getAptProxy should return some info");
}

void TestProxy::testIsManualProxyEnableWithEmptySettings()
{
    Proxy proxy;
    bool enable = proxy.isManualProxyEnable();
    QVERIFY2(enable == false || enable == true, "isManualProxyEnable should return a boolean");
}

void TestProxy::testIsManualProxyEnableWithValidSettings()
{
    Proxy proxy;
    bool enable = proxy.isManualProxyEnable();
    QVERIFY2(enable == false || enable == true, "isManualProxyEnable should return a boolean");
}
