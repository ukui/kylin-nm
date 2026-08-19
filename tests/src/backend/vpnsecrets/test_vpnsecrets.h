/*
 * Kylin-NM Unit Test for vpnsecrets
 * Test functions: kylinvpnrequest functions, kylinvpnagent functions
 */

#ifndef TEST_VPNSECRETS_H
#define TEST_VPNSECRETS_H

#include <gtest/gtest.h>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QTemporaryDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Forward declarations from the vpnsecrets source
extern "C" {
#include "kylinvpnrequest.h"
#include "kylinvpnagent.h"
}

// Test fixture for kylinVpnSecretsResultGet function
class TestVpnSecretsResultGet : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    NMConnection *testConnection;
};

// Test fixture for kylinAgentVpnSave function
class TestKylinAgentVpnSave : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    NMConnection *testConnection;
    QTemporaryDir tempDir;
};


#endif // TEST_VPNSECRETS_H