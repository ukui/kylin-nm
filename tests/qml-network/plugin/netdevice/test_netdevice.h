#ifndef TEST_NEDEVIDE_H
#define TEST_NEDEVIDE_H

#include <QObject>
#include <QTest>
#include <QString>
#include <QMap>
#include <QVariant>

#include "netdevice.h"

class TestNetDevice : public QObject
{
    Q_OBJECT
public:
    explicit TestNetDevice(QObject *parent = nullptr);
    ~TestNetDevice();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_isAvailable();
    void test_devName();
    void test_getDevType();
    void test_setAvailable();
    void test_containsConnection();
    void test_containsConnectionName();
    void test_addConnection();
    void test_addConnection_activated();
    void test_addConnection_sorted();
    void test_removeConnection_byMap();
    void test_removeConnection_byPath();
    void test_removeConnectionByName();
    void test_updateConnection();
    void test_updateConnection_activated();
    void test_getConnectionByName();
    void test_getConnections();
    void test_getConnections_empty();
    void test_clearConnections();
    void test_updateConnectivity();
    void test_updateConnectivity_activated();
    void test_updateConnectionProp();
    void test_updateConnectionProp_frequency();
    void test_updateConnectionProp_isMix();
    void test_updateConnectionProp_State();
    void test_updateConnectionProp_autoConnect();
    void test_updateConnectionProp_notFound();
    void test_addConnection_multipleActivated();
    void test_containsConnection_wiredDevice();
    void test_containsConnection_wirelessDevice();
    void test_updateConnection_notFound();
    void test_updateConnectivity_notFound();

private:
    NetDevice *m_device;
    QString m_testDevName;
    DeviceType m_testDevType;

    QMap<QString, QVariant> createConnection(const QString &name, const QString &uuid,
                                               const QString &path, int state, bool configured = true, int signal = 0);
};

#endif
