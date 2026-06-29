#ifndef TEST_NETWORKQML_PLUGIN_H
#define TEST_NETWORKQML_PLUGIN_H

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QJSEngine>

class NetworkhqmlPlugin;

class TestNetworkqmlPlugin : public QObject
{
    Q_OBJECT
public:
    explicit TestNetworkqmlPlugin(QObject *parent = nullptr);
    ~TestNetworkqmlPlugin();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_registerTypes_correctUri();
    void test_registerTypes_incorrectUri();

private:
    NetworkhqmlPlugin *m_plugin;
};

#endif // TEST_NETWORKQML_PLUGIN_H
