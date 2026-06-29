#ifndef TEST_NETCONNECT_H
#define TEST_NETCONNECT_H

#include <QObject>
#include <QTest>
#include <QWidget>
#include <QIcon>

class NetConnect;

class TestNetConnect : public QObject
{
    Q_OBJECT
public:
    explicit TestNetConnect(QObject *parent = nullptr);
    ~TestNetConnect();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_plugini18nName();
    void test_pluginTypes();
    void test_pluginUi();
    void test_name();
    void test_isEnable();
    void test_isShowOnHomePage();
    void test_icon();
    void test_translationPath();

private:
    NetConnect *m_netConnect;
    QWidget *m_pluginWidget;
};

#endif // TEST_NETCONNECT_H
