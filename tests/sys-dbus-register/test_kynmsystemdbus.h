#ifndef TEST_KYNMSYSTEMDBUS_H
#define TEST_KYNMSYSTEMDBUS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>

class KynmSystemDbus;

class TestKynmSystemDbus : public QObject
{
    Q_OBJECT
public:
    explicit TestKynmSystemDbus(QObject *parent = nullptr);
    ~TestKynmSystemDbus();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_checkIpv4IsConflict();
    void test_checkIpv6IsConflict();
    void test_setWiredMainSwitch();
    void test_getWiredMainSwitch();
    void test_setWiredDeviceSwitch();
    void test_getWiredDeviceSwitch();
    void test_setOptionsEnhance();
    void test_getExtraDnsEnhance();
    void test_writeNmConfig();
    void test_getNmConfig();
    void test_setDeviceSwitch();

private:
    KynmSystemDbus *m_dbus;
    QTemporaryDir m_tempDir;
    QString m_testConfDir;
};

#endif // TEST_KYNMSYSTEMDBUS_H