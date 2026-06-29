#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <QObject>
#include <QString>
#include <QtTest/QtTest>

class TestUtils : public QObject
{
    Q_OBJECT
public:
    explicit TestUtils(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testUtilsConstructor();
    void testMsystem();
    void testMsystemWithNullCmd();
    void testOnRequestSendDesktopNotify();
    void testUseQssFileSetStyle();
    void testNetworkSpeedConstructor();
    void testNetworkSpeedConstructorWithParent();
    void testGetCurrentDownloadRates();
    void testGetCurrentDownloadRatesWithNullParams();
    void testGetCurrentDownloadRatesWithInvalidFile();
    void testGetCurrentDownloadRatesWithEth0();
};

#endif // TEST_UTILS_H
