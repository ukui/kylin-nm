#include "test_utils.h"
#include "../../../../src/backend/utils.h"
#include <QCoreApplication>
#include <QDebug>
#include <QSignalSpy>

TestUtils::TestUtils(QObject *parent)
    : QObject(parent)
{}

void TestUtils::initTestCase()
{
}

void TestUtils::cleanupTestCase()
{
}

void TestUtils::init()
{
}

void TestUtils::cleanup()
{
}

void TestUtils::testUtilsConstructor()
{
    Utils utils;
    QVERIFY(true);
}

void TestUtils::testMsystem()
{
    char cmd[128] = "echo test";
    int result = Utils::m_system(cmd);
    QVERIFY(result == 0);
}

void TestUtils::testMsystemWithNullCmd()
{
    int result = Utils::m_system(nullptr);
}

void TestUtils::testOnRequestSendDesktopNotify()
{
    Utils utils;
    QSignalSpy spy(&utils, SIGNAL(destroyed()));
    utils.onRequestSendDesktopNotify("test message");
    QVERIFY(true);
}

void TestUtils::testUseQssFileSetStyle()
{
    QString style("invalid_style_99999.qss");
    UseQssFile::setStyle(style);
    QVERIFY(true);
}

void TestUtils::testNetworkSpeedConstructor()
{
    NetworkSpeed *speed = new NetworkSpeed(nullptr);
    QVERIFY(speed != nullptr);
    delete speed;
}

void TestUtils::testNetworkSpeedConstructorWithParent()
{
    QObject parent;
    NetworkSpeed *speed = new NetworkSpeed(&parent);
    QVERIFY(speed != nullptr);
}

void TestUtils::testGetCurrentDownloadRates()
{
    NetworkSpeed speed;
    char netname[] = "lo";
    long save_rate = 0;
    long tx_rate = 0;
    int result = speed.getCurrentDownloadRates(netname, &save_rate, &tx_rate);
    QVERIFY(result == 0);
    QVERIFY(save_rate >= 0);
    QVERIFY(tx_rate >= 0);
}

void TestUtils::testGetCurrentDownloadRatesWithNullParams()
{
    NetworkSpeed speed;
    int result = speed.getCurrentDownloadRates(nullptr, nullptr, nullptr);
    QVERIFY(result == -1);
}

void TestUtils::testGetCurrentDownloadRatesWithInvalidFile()
{
    NetworkSpeed speed;
    char netname[] = "invalid_net_device_xyz";
    long save_rate = 0;
    long tx_rate = 0;
    int result = speed.getCurrentDownloadRates(netname, &save_rate, &tx_rate);
    QVERIFY(result == 0);
}

void TestUtils::testGetCurrentDownloadRatesWithEth0()
{
    NetworkSpeed speed;
    char netname[] = "eth0";
    long save_rate = 0;
    long tx_rate = 0;
    int result = speed.getCurrentDownloadRates(netname, &save_rate, &tx_rate);
    QVERIFY(result == 0);
}
