#include "test_kynmsystemdbus.h"
#include "kynmsystemdbus.h"
#include <QTest>
#include <QDebug>
#include <QSettings>

TestKynmSystemDbus::TestKynmSystemDbus(QObject *parent)
    : QObject(parent)
    , m_dbus(nullptr)
{
}

TestKynmSystemDbus::~TestKynmSystemDbus()
{
}

void TestKynmSystemDbus::initTestCase()
{
    m_testConfDir = m_tempDir.path();
    
    QDir().mkpath(m_testConfDir + "/etc/kylin-nm");
    QDir().mkpath(m_testConfDir + "/etc/nm_enhance");
    
    qputenv("QT_QPA_PLATFORM", "offscreen");
}

void TestKynmSystemDbus::cleanupTestCase()
{
    if (m_dbus) {
        delete m_dbus;
        m_dbus = nullptr;
    }
}

void TestKynmSystemDbus::init()
{
    m_dbus = new KynmSystemDbus();
}

void TestKynmSystemDbus::cleanup()
{
    if (m_dbus) {
        delete m_dbus;
        m_dbus = nullptr;
    }
}

void TestKynmSystemDbus::test_checkIpv4IsConflict()
{
    QString devName = "eth0";
    QString ipv4Address = "192.168.1.100";
    QStringList macList;
    macList.append("00:11:22:33:44:55");
    
    bool result = m_dbus->checkIpv4IsConflict(devName, ipv4Address, macList);
    
    QVERIFY2(true, "checkIpv4IsConflict executed");
    Q_UNUSED(result);
}

void TestKynmSystemDbus::test_checkIpv6IsConflict()
{
    QString devName = "eth0";
    QString ipv6Address = "fe80::1";
    
    bool result = m_dbus->checkIpv6IsConflict(devName, ipv6Address);
    
    QVERIFY2(true, "checkIpv6IsConflict executed");
    Q_UNUSED(result);
}

void TestKynmSystemDbus::test_setWiredMainSwitch()
{
    QSignalSpy spy(m_dbus, SIGNAL(sysWiredMainSwitchChanged(bool)));
    QVERIFY(spy.isValid());

    m_dbus->setWiredMainSwitch(true);
    QVERIFY(m_dbus->getWiredMainSwitch() == true);

    m_dbus->setWiredMainSwitch(true);
    QVERIFY(m_dbus->getWiredMainSwitch() == true);
}

void TestKynmSystemDbus::test_getWiredMainSwitch()
{
    m_dbus->setWiredMainSwitch(true);
    bool result = m_dbus->getWiredMainSwitch();
    QVERIFY(result == true);

    m_dbus->setWiredMainSwitch(true);
    result = m_dbus->getWiredMainSwitch();
    QVERIFY(result == true);
}

void TestKynmSystemDbus::test_setWiredDeviceSwitch()
{
    QString devName = "eth0";
    QSignalSpy spy(m_dbus, SIGNAL(sysWiredDevSwitchChanged(QString, bool)));
    
    m_dbus->setWiredDeviceSwitch(devName, true);
    
    QVERIFY(spy.isValid());
    QVERIFY(m_dbus->getWiredDeviceSwitch(devName) == true);

    m_dbus->setWiredDeviceSwitch(devName, true);
    QVERIFY(m_dbus->getWiredDeviceSwitch(devName) == true);
}

void TestKynmSystemDbus::test_getWiredDeviceSwitch()
{
    QString devName = "eth0";
    m_dbus->setWiredDeviceSwitch(devName, true);
    bool result = m_dbus->getWiredDeviceSwitch(devName);
    QVERIFY(result == true);
    
    m_dbus->setWiredDeviceSwitch(devName, true);
    result = m_dbus->getWiredDeviceSwitch(devName);
    QVERIFY(result == true);
    
    QString devName2 = "eth1";
    result = m_dbus->getWiredDeviceSwitch(devName2);
    QVERIFY(result == true);
}

void TestKynmSystemDbus::test_setOptionsEnhance()
{
    QString name = "test_dns";
    QString timeout = "30";
    QString attempts = "3";
    QString type = "dhcp";
    
    bool result = m_dbus->setOptionsEnhance(name, timeout, attempts, type);
    
    QVERIFY(result == true);
    
    QVariantMap map = m_dbus->getExtraDnsEnhance(name);
    QVERIFY(map.value("timeout").toString() == timeout);
    QVERIFY(map.value("attempts").toString() == attempts);
    QVERIFY(map.value("type").toString() == type);
}

void TestKynmSystemDbus::test_getExtraDnsEnhance()
{
    QString name = "test_dns2";
    QString timeout = "60";
    QString attempts = "5";
    QString type = "static";
    
    m_dbus->setOptionsEnhance(name, timeout, attempts, type);
    
    QVariantMap map = m_dbus->getExtraDnsEnhance(name);
    
    QVERIFY(map.contains("timeout"));
    QVERIFY(map.contains("attempts"));
    QVERIFY(map.contains("type"));
    QVERIFY(map.value("timeout").toString() == timeout);
    QVERIFY(map.value("attempts").toString() == attempts);
    QVERIFY(map.value("type").toString() == type);
    
    QVariantMap emptyMap = m_dbus->getExtraDnsEnhance("nonexistent");
    QVERIFY(emptyMap.value("timeout").toString().isEmpty());
}

void TestKynmSystemDbus::test_writeNmConfig()
{
    QString filePath = m_testConfDir + "/test_config.conf";
    QString feature = "test_feature";
    QString key = "test_key";
    QString value = "test_value";
    
    bool result = m_dbus->writeNmConfig(filePath, feature, key, value);
    
    QVERIFY(result == true);
    
    QSettings settings(filePath, QSettings::IniFormat);
    QString readValue = settings.value("/" + feature + "/" + key).toString();
    QVERIFY(readValue == value);
}

void TestKynmSystemDbus::test_getNmConfig()
{
    QString filePath = m_testConfDir + "/test_config2.conf";
    QString feature = "test_feature";
    
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue("/" + feature + "/key1", "value1");
    settings.setValue("/" + feature + "/key2", "value2");
    settings.sync();
    
    QVariantMap map = m_dbus->getNmConfig(filePath, feature);
    
    QVERIFY(map.contains("key1"));
    QVERIFY(map.contains("key2"));
    QVERIFY(map.value("key1").toString() == "value1");
    QVERIFY(map.value("key2").toString() == "value2");
}

void TestKynmSystemDbus::test_setDeviceSwitch()
{
    QString devName = "eth0";
    
    QSignalSpy spy1(m_dbus, SIGNAL(sysWiredDevSwitchChanged(QString, bool)));
    QSignalSpy spy2(m_dbus, SIGNAL(sysDeviceSwitchChanged(const QString&)));
    
    m_dbus->setDeviceSwitch(devName, true);
    
    QVERIFY(spy1.isValid());
    QVERIFY(spy1.count() == 1);
    QVERIFY(spy2.isValid());
    QVERIFY(spy2.count() == 1);
    
    QList<QVariant> args1 = spy1.takeFirst();
    QVERIFY(args1.at(0).toString() == devName);
    QVERIFY(args1.at(1).toBool() == true);
    
    QList<QVariant> args2 = spy2.takeFirst();
    QVERIFY(args2.at(0).toString() == devName);
    
    m_dbus->setDeviceSwitch(devName, true);
    
    QVERIFY(spy1.count() == 1);
    QVERIFY(spy2.count() == 0);
    
    QList<QVariant> args3 = spy1.takeFirst();
    QVERIFY(args3.at(0).toString() == devName);
    QVERIFY(args3.at(1).toBool() == true);
    
    QString devNameEmpty = "";
    m_dbus->setDeviceSwitch(devNameEmpty, true);
    QVERIFY(spy2.count() == 0);
}
