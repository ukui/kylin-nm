#ifndef TEST_NETWORKMODECONFIG_H
#define TEST_NETWORKMODECONFIG_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QDBusInterface>
#include <QString>

class NetworkModeConfig;

class TestNetworkModeConfig : public QObject
{
    Q_OBJECT
public:
    explicit TestNetworkModeConfig(QObject *parent = nullptr);
    ~TestNetworkModeConfig();

private:
    NetworkModeConfig *m_netModeConfig = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // NetworkModeConfig tests
    void test_getInstance();
    void test_networkModeConfigConstruction();
    void test_getNetworkModeConfigAsync_withValidUuid();
    void test_getNetworkModeConfigAsync_withEmptyUuid();
    void test_getNetworkModeConfig_withValidUuid();
    void test_getNetworkModeConfig_withEmptyUuid();
    void test_setNetworkModeConfig();
    void test_breakNetworkConnect();
    void test_signal_networkModeConfigReady();
};

#endif // TEST_NETWORKMODECONFIG_H

