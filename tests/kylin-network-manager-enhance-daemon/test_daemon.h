/*
 * Kylin-NM Unit Test for kylin-network-manager-enhance-daemon
 * Test functions: file_monitor functions, dbus-server functions
 */

#ifndef TEST_DAEMON_H
#define TEST_DAEMON_H

#include <gtest/gtest.h>
#include <QString>
#include <QByteArray>
#include <QTemporaryDir>
#include <QFile>
#include <QTextStream>

// Forward declarations from the daemon source
extern "C" {
#include "public.h"
}


// Test fixture for get_file_path function
class TestGetFilePath : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

// Test fixture for extara_dns_conf_is_exist function
class TestExtraDnsConfExist : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    
    QTemporaryDir tempDir;
};

// Test fixture for read_extra_dns functions
class TestReadExtraDns : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    
    QTemporaryDir tempDir;
    QString testConfigFile;
};

// Test fixture for set_extra_dns functions
class TestSetExtraDns : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    
    QTemporaryDir tempDir;
    QString testConfigFile;
};

// Test fixture for D-Bus server constants and gdbus command line tests
class DbusServerConstants : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    
    QTemporaryDir tempDir;
};


#endif // TEST_DAEMON_H