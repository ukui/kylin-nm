#ifndef TEST_MOBILEHOTSPOT_H
#define TEST_MOBILEHOTSPOT_H

#include <QObject>
#include <QTest>
#include <QWidget>
#include <QIcon>

class TestMobileHotspot : public QObject
{
    Q_OBJECT
public:
    explicit TestMobileHotspot(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_plugini18nName();
    void test_pluginTypes();
    void test_pluginUi();
    void test_name();
    void test_isShowOnHomePage();
    void test_icon();
    void test_isEnable();
    void test_translationPath();
    void test_destructor();

private:
    class MobileHotspot *m_mobileHotspot;
};

#endif // TEST_MOBILEHOTSPOT_H
