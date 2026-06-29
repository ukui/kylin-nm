#ifndef TEST_PROXY_H
#define TEST_PROXY_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QHash>
#include <QVariant>

class TestProxy : public QObject
{
    Q_OBJECT
public:
    explicit TestProxy(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPlugini18nName();
    void testPluginTypes();
    void testName();
    void testIsShowOnHomePage();
    void testIcon();
    void testIsEnable();
    void testTranslationPath();
    void testSetFrameNoframe();
    void testSetLine();
    void testGetAppProxyStateInvalid();
    void testSetAppProxyStateInvalid();
    void testGetAppProxyConfInvalid();
    void testSetAppProxyConfInvalid();
    void testGetAppListProxyInvalid();
    void testGetAptProxyInvalid();
    void testIsManualProxyEnableWithEmptySettings();
    void testIsManualProxyEnableWithValidSettings();

private:
    void initProxyInstance();
};

#endif // TEST_PROXY_H
