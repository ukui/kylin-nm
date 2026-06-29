#ifndef TEST_KYLINACTIVECONNECTRESOURCE_H
#define TEST_KYLINACTIVECONNECTRESOURCE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QString>
#include <QList>
#include <QHostAddress>

#include "../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h"

class TestKyActiveConnectResource : public QObject
{
    Q_OBJECT
public:
    explicit TestKyActiveConnectResource(QObject *parent = nullptr);
    ~TestKyActiveConnectResource();

private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructorAndDestructor();
    void testGetActiveConnectionByUuid();
    void testGetActiveConnectionByUuidWithDeviceName();
    void testGetActiveConnectionList();
    void testGetActiveConnectIpInfo();
    void testGetActiveConnectDnsInfo();
    void testGetVpnActivateConnect();
    // TODO: Re-enable after Bluetooth dependencies are fixed
    //void testGetBtActivateConnect();
    void testGetApActivateConnect();
    void testGetDeviceOfActivateConnect();
    void testConnectionIsVirtual();
    void testWiredConnectIsActived();
    void testCheckWirelessStatus();
    void testGetAcitveConnectionPathByUuid();
    void testGetActivateWifiSignal();
    void testCheckInternetLoading();
    void testGetActivateWifiUni();

private:
    KyActiveConnectResource *m_activeConnectResource;
};

#endif // TEST_KYLINACTIVECONNECTRESOURCE_H
