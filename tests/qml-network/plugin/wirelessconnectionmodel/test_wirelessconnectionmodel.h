#ifndef TEST_WIRELESSCONNECTIONMODEL_H
#define TEST_WIRELESSCONNECTIONMODEL_H

#include <QObject>
#include <QTest>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QVariantList>
#include <QModelIndex>
#include <QSignalSpy>

#include "wirelessconnectionmodel.h"

class TestWirelessConnectionModel : public QObject
{
    Q_OBJECT
public:
    explicit TestWirelessConnectionModel(QObject *parent = nullptr);
    ~TestWirelessConnectionModel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_rowCount();
    void test_rowCount_invalidParent();
    void test_data_validIndex();
    void test_data_invalidIndex();
    void test_data_outOfRange();
    void test_data_allRoles();
    void test_setData_validIndex();
    void test_setData_invalidIndex();
    void test_setData_outOfRange();
    void test_setData_allRoles();
    void test_roleNames();
    void test_addConnection_status1or2();
    void test_addConnection_otherStatus();
    void test_removeConnection_existing();
    void test_removeConnection_nonExisting();
    void test_updateConnectionStrength_existing();
    void test_updateConnectionStrength_nonExisting();
    void test_updateConnectionStatus_existing();
    void test_updateConnectionStatus_nonExisting();
    void test_mapToConnectionInfo();
    void test_refreshConnections();
    void test_findIndexById_existing();
    void test_findIndexById_nonExisting();
    void test_generateUniqueId();
    void test_getConButtonFromSsid_existing();
    void test_getConButtonFromSsid_nonExisting();
    void test_replaceConnection_existing();
    void test_replaceConnection_nonExisting();

private:
    WirelessConnectionModel *m_model;
    WirelessConnectionModel::ST_ConnectionInfo createConnectionInfo(
        const QString &ssid, const QString &signal, const QString &security,
        const QString &uuid, const QString &isApConn, const QString &category,
        uint frequency, int status, bool Loading, int Configured,
        bool isMix, bool autoConnect);

    QMap<QString, QVariant> createConnectionMap(
        const QString &name, const QString &signal, const QString &security,
        const QString &uuid, const QString &isApConn, const QString &category,
        const QString &frequency, int state, bool Loading, int Configured,
        bool isMix, bool autoConnect);
};

#endif
