#ifndef TEST_LANPAGE_H
#define TEST_LANPAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class LanPage;

class TestLanPage : public QObject
{
    Q_OBJECT
public:
    explicit TestLanPage(QObject *parent = nullptr);
    ~TestLanPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_lanPageConstructionAndDestruction();

    void test_getWiredList_emptyDevice();
    void test_getWiredList_withDevice();

    void test_activateWired();
    void test_deactivateWired();

    void test_deleteWiredConnect();

    void test_setWiredDeviceEnable();
    void test_getDeviceConnectivity();
    void test_getCableStateByDevice();

    void test_lanIsConnected();
    void test_getWiredDeviceConnectState();
    void test_hasInternetAccess();

    void test_getWiredEnabledState();
    void test_setWiredEnabledState();

    void test_getWiredDeviceConnect();
    void test_setWiredDeviceAutoconnect();
    void test_setWiredConnectAutoconnect();

    void test_getWiredDefaultDeviceName();

    void test_signals_lanAdd();
    void test_signals_lanRemove();
    void test_signals_lanUpdate();

    void test_signals_wiredEnabledChanged();

private:
    LanPage *m_lanPage;
};

#endif // TEST_LANPAGE_H
