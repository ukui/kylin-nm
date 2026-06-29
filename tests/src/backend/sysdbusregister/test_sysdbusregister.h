#ifndef TEST_SYSDBUSREGISTER_H
#define TEST_SYSDBUSREGISTER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTest>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

class TestSysdbusRegister : public QObject
{
    Q_OBJECT

public:
    explicit TestSysdbusRegister(QObject *parent = nullptr);
    ~TestSysdbusRegister();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_systemRun();
    void test_getWifiInfo_fileNotExists();
    void test_getWifiInfo_fileExists();
    void test_getWifiInfo_withWifiData();
    void test_appendWifiInfo();
    void test_appendWifiUser();

private:
    QString m_testConfigFile;
    QString m_originalConfigFile;
};

#endif // TEST_SYSDBUSREGISTER_H
