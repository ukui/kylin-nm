#ifndef TEST_VPNDBUSADAPTOR_H
#define TEST_VPNDBUSADAPTOR_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class MockVpnObject : public QObject
{
    Q_OBJECT
public:
    MockVpnObject(QObject *parent = nullptr) : QObject(parent), m_virtualListCalled(false), m_deleteVpnUuid(""),
        m_activateVpnUuid(""), m_deactivateVpnUuid(""), m_showMainWindowCalled(false) {}

    void getVirtualList(QVector<QStringList> &vector) {
        m_virtualListCalled = true;
        QStringList item;
        item << "test-vpn" << "uuid-123" << "connected";
        vector.append(item);
    }

    void deleteVpn(const QString &uuid) {
        m_deleteVpnUuid = uuid;
    }

    void activateVpn(const QString &connUuid) {
        m_activateVpnUuid = connUuid;
    }

    void deactivateVpn(const QString &connUuid) {
        m_deactivateVpnUuid = connUuid;
    }

    void onShowMainWindow() {
        m_showMainWindowCalled = true;
    }

    bool m_virtualListCalled;
    QString m_deleteVpnUuid;
    QString m_activateVpnUuid;
    QString m_deactivateVpnUuid;
    bool m_showMainWindowCalled;
};

class TestVpnDbusAdaptor : public QObject
{
    Q_OBJECT

public:
    TestVpnDbusAdaptor(QObject *parent = nullptr);
    ~TestVpnDbusAdaptor();

private:
    MockVpnObject *m_mockVpnObject;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testGetVirtualList();
    void testDeleteVpn();
    void testActivateVpn();
    void testDeactivateVpn();
    void testShowKylinVpn();
    void testSignals();
};

#endif // TEST_VPNDBUSADAPTOR_H
