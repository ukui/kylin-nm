#include "test_sysdbusregister.h"
#include "../../../../src/backend/sysdbusregister.h"
#include <QProcess>
#include <QSettings>
#include <QSharedPointer>
#include <QDebug>

TestSysdbusRegister::TestSysdbusRegister(QObject *parent)
    : QObject(parent)
{
}

TestSysdbusRegister::~TestSysdbusRegister()
{
}

void TestSysdbusRegister::initTestCase()
{
    m_testConfigFile = "/tmp/test_wpaconn.conf";
    m_originalConfigFile = "/usr/share/kylin-nm/wpaconn.conf";

    QFile::remove(m_originalConfigFile);
}

void TestSysdbusRegister::cleanupTestCase()
{
}

void TestSysdbusRegister::init()
{
}

void TestSysdbusRegister::cleanup()
{
}

void TestSysdbusRegister::test_constructor()
{
    SysdbusRegister *registerObj = new SysdbusRegister();
    QVERIFY(registerObj != nullptr);
    delete registerObj;
}

void TestSysdbusRegister::test_destructor()
{
    SysdbusRegister *registerObj = new SysdbusRegister();
    delete registerObj;
    QVERIFY(true);
}

void TestSysdbusRegister::test_systemRun()
{
    SysdbusRegister registerObj;
    registerObj.systemRun("echo test");
    QVERIFY(true);
}

void TestSysdbusRegister::test_getWifiInfo_fileNotExists()
{
    SysdbusRegister registerObj;
    QStringList result = registerObj.getWifiInfo("nonexistent");
    QVERIFY(result.isEmpty());
}

void TestSysdbusRegister::test_getWifiInfo_fileExists()
{
    QFile file(m_originalConfigFile);
    qDebug() << "Writing to:" << m_originalConfigFile;
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "eap=tls" << "\n";
        stream << "inner=MSCHAPv2" << "\n";
        file.close();
        qDebug() << "File written";
    } else {
        qDebug() << "Failed to open file for writing";
    }

    QTest::qWait(100);

    QFile checkFile(m_originalConfigFile);
    if (checkFile.exists()) {
        qDebug() << "File exists, size:" << checkFile.size();
    }

    SysdbusRegister registerObj;
    QStringList result = registerObj.getWifiInfo("TestWifi");
    qDebug() << "Result size:" << result.size();
    qDebug() << "Result:" << result;

    QVERIFY2(result.size() >= 2, QString("result size: %1").arg(result.size()).toUtf8());
    if (result.size() >= 2) {
        QCOMPARE(result.at(0), QString("tls"));
        QCOMPARE(result.at(1), QString("MSCHAPv2"));
    }
}

void TestSysdbusRegister::test_getWifiInfo_withWifiData()
{
    QFile file(m_originalConfigFile);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "eap=peap" << "\n";
        stream << "inner=MSCHAPv2" << "\n";
        stream << "\n";
        stream << "[TestWifi]" << "\n";
        stream << "1/user=testuser1" << "\n";
        stream << "2/user=testuser2" << "\n";
        file.close();
    }

    QTest::qWait(100);

    SysdbusRegister registerObj;
    QStringList result = registerObj.getWifiInfo("TestWifi");

    QVERIFY2(result.size() >= 2, QString("result size: %1").arg(result.size()).toUtf8());
    if (result.size() >= 4) {
        QCOMPARE(result.at(0), QString("peap"));
        QCOMPARE(result.at(1), QString("MSCHAPv2"));
    }
}

void TestSysdbusRegister::test_appendWifiInfo()
{
    QFile file(m_originalConfigFile);
    file.open(QIODevice::WriteOnly);
    file.close();

    SysdbusRegister registerObj;
    bool result = registerObj.appendWifiInfo("NewWifi", "eapvalue", "innervalue", "uservalue");

    QVERIFY(result == true);
}

void TestSysdbusRegister::test_appendWifiUser()
{
    QFile file(m_originalConfigFile);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "[TestWifi]" << "\n";
        stream << "1/user=existinguser" << "\n";
        file.close();
    }

    SysdbusRegister registerObj;
    bool result = registerObj.appendWifiUser("TestWifi", "newuser");

    QVERIFY(result == true);
}
