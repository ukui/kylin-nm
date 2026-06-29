#ifndef TEST_DBUS_H
#define TEST_DBUS_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class MockMainWindow : public QObject
{
    Q_OBJECT
public:
    MockMainWindow(QObject *parent = nullptr) : QObject(parent) {}

    void getWirelessList(QString devName, QList<QStringList> &list) {
        Q_UNUSED(devName);
        Q_UNUSED(list);
    }

    void getWiredList(QString devName, QList<QStringList> &list) {
        Q_UNUSED(devName);
        Q_UNUSED(list);
    }

    bool getWirelessSwitchBtnState() { return false; }
    int getDeviceConnectivity(const QString deviceName) { Q_UNUSED(deviceName); return 0; }
    bool getCableStateByDevice(const QString deviceName) { Q_UNUSED(deviceName); return false; }
    void getWirelessDeviceCap(QMap<QString, int> &map) { Q_UNUSED(map); }

    void activateWired(const QString& devName, const QString& connUuid) {
        Q_UNUSED(devName);
        Q_UNUSED(connUuid);
    }

    void deactivateWired(const QString& devName, const QString& connUuid, bool concise = false) {
        Q_UNUSED(devName);
        Q_UNUSED(connUuid);
        Q_UNUSED(concise);
    }

    void activateWireless(const QString& devName, const QString& ssid) {
        Q_UNUSED(devName);
        Q_UNUSED(ssid);
    }

    void deactivateWireless(const QString& devName, const QString& ssid) {
        Q_UNUSED(devName);
        Q_UNUSED(ssid);
    }

    void deleteWiredConnect(int type, const QString& connUuid) {
        Q_UNUSED(type);
        Q_UNUSED(connUuid);
    }

    void deleteWireleeConnect(int type, const QString& connUuid) {
        Q_UNUSED(type);
        Q_UNUSED(connUuid);
    }

    void setWiredDeviceAutoconnect(const QString& devName, bool state) {
        Q_UNUSED(devName);
        Q_UNUSED(state);
    }

    void setWiredConnectAutoconnect(const QString& uuid, bool state) {
        Q_UNUSED(uuid);
        Q_UNUSED(state);
    }

    void setWirelessConnectAutoconnect(const QString& uuid, bool state) {
        Q_UNUSED(uuid);
        Q_UNUSED(state);
    }

    void setWirelessSwitchEnable(bool enable) { Q_UNUSED(enable); }
    void setWiredDeviceEnable(const QString& devName, bool enable) {
        Q_UNUSED(devName);
        Q_UNUSED(enable);
    }

    void showPropertyWidget(QString devName, QString ssid) {
        Q_UNUSED(devName);
        Q_UNUSED(ssid);
    }

    void showCreateWiredConnectWidget(const QString devName) {
        Q_UNUSED(devName);
    }

    void showAddOtherWlanWidget(QString devName) {
        Q_UNUSED(devName);
    }

    void activeWirelessAp(const QString apName, const QString apPassword, const QString wirelessBand, const QString apDevice) {
        Q_UNUSED(apName);
        Q_UNUSED(apPassword);
        Q_UNUSED(wirelessBand);
        Q_UNUSED(apDevice);
    }

    void deactiveWirelessAp(const QString apName, const QString uuid) {
        Q_UNUSED(apName);
        Q_UNUSED(uuid);
    }

    void getStoredApInfo(QStringList &list) {
        Q_UNUSED(list);
    }

    void getApInfoBySsid(QString devName, QString ssid, QStringList &list) {
        Q_UNUSED(devName);
        Q_UNUSED(ssid);
        Q_UNUSED(list);
    }

    void getApConnectionPath(QString &path, QString uuid) {
        Q_UNUSED(path);
        Q_UNUSED(uuid);
    }

    void getActiveConnectionPath(QString &path, QString uuid) {
        Q_UNUSED(path);
        Q_UNUSED(uuid);
    }

    void passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect) {
        Q_UNUSED(devName);
        Q_UNUSED(ssid);
        Q_UNUSED(type);
        Q_UNUSED(psk);
        Q_UNUSED(autoConnect);
    }

    void rescan() {}
    void keyRingInit() {}
    void keyRingClear() {}

    void onShowMainWindow(int type) { Q_UNUSED(type); }

    QString getDefaultDeviceName(int type) {
        Q_UNUSED(type);
        return QString();
    }

    void handleEnterpriseWifiReconnection(const QString &requestId, const QString &deviceName, const QString &ssid, const QString &password, const QVariantMap &enterpriseInfo) {
        Q_UNUSED(requestId);
        Q_UNUSED(deviceName);
        Q_UNUSED(ssid);
        Q_UNUSED(password);
        Q_UNUSED(enterpriseInfo);
    }

    class KylinSecretAgent : public QObject
    {
    public:
        KylinSecretAgent(QObject *parent = nullptr) : QObject(parent) {}
        void onSubmitPassword(const QString &requestId, const QString &ssid, const QVariantMap &secretMap) {
            Q_UNUSED(requestId);
            Q_UNUSED(ssid);
            Q_UNUSED(secretMap);
        }
        void onCancelPassword(const QString &requestId, const QString &ssid) {
            Q_UNUSED(requestId);
            Q_UNUSED(ssid);
        }
    };

    KylinSecretAgent* secretAgent() { return &m_secretAgent; }

signals:
    void lanAdd(QString devName, QStringList info);
    void lanRemove(QString dbusPath);
    void lanUpdate(QString devName, QStringList info);
    void wlanAdd(QString devName, QStringList info);
    void wlanRemove(QString devName, QString ssid);
    void wlanactiveConnectionStateChanged(QString devName, QString ssid, QString uuid, int status);
    void lanActiveConnectionStateChanged(QString devName, QString uuid, int status);
    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
    void deviceStatusChanged();
    void wirelessDeviceStatusChanged();
    void deviceNameChanged(QString oldName, QString newName, int type);
    void wirelessSwitchBtnChanged(bool state);
    void wiredMainSwitchBtnChanged(bool state);
    void hotspotDeactivated(QString devName, QString ssid);
    void hotspotActivated(QString devName, QString ssid, QString uuid, QString activePath, QString settingPath);
    void signalStrengthChange(QString devName, QString ssid, int strength);
    void secuTypeChange(QString devName, QString ssid, QString secuType);
    void timeToUpdate();
    void sigNetworkPropChanged(QVariantMap parm);

private:
    KylinSecretAgent m_secretAgent;
};

class TestDbus : public QObject
{
    Q_OBJECT
public:
    explicit TestDbus(QObject *parent = nullptr);
    ~TestDbus();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testGetWirelessList();
    void testGetWiredList();
    void testGetWirelessSwitchBtnState();
    void testGetWiredMainSwitchBtnState();
    void testGetDeviceConnectivity();
    void testGetCableStateByDevice();
    void testSetWiredSwitchEnable();
    void testSetWirelessSwitchEnable();
    void testSetDeviceEnable();
    void testActivateConnect();
    void testDeActivateConnect();
    void testDeActivateConnectConcise();
    void testDeleteConnect();
    void testSetDeviceAutoConnectState();
    void testSetNetworkConnectionAutoConnectState();
    void testGetDeviceListAndEnabled();
    void testGetWirelessDeviceCap();
    void testShowPropertyWidget();
    void testShowCreateWiredConnectWidget();
    void testShowAddOtherWlanWidget();
    void testActiveWirelessAp();
    void testDeactiveWirelessAp();
    void testPasswordConnect();
    void testGetStoredApInfo();
    void testGetApInfoBySsid();
    void testGetApConnectionPath();
    void testGetActiveConnectionPath();
    void testReScan();
    void testKeyRingInit();
    void testKeyRingClear();
    void testShowKylinNM();
    void testGetNetworkDeviceData();
    void testRegisterInputPasswdAgent();
    void testRequestInputPasswdAgent();
    void testSetDefaultWiredDevice();
    void testGetDefaultWiredDevice();
    void testSetDefaultWirelessDevice();
    void testGetDefaultWirelessDevice();
    void testSendPasswordError();
    void testSubmitWirelessPassword();
    void testCancelWirelessPassword();

private:
    MockMainWindow *m_mockMainWindow;
};

#endif
