#ifndef TEST_PROXYSERVICEMANAGER_H
#define TEST_PROXYSERVICEMANAGER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QTemporaryDir>
#include <QDBusInterface>
#include <QDBusReply>
#include <QGSettings>

using namespace testing;

class GlobalFunctionTest : public Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QTemporaryDir tempDir;
};

class ProcInfoDealTest : public Test
{
protected:
    void SetUp() override;
    void TearDown() override;
};

class ProxyServiceManagerTest : public Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QTemporaryDir tempDir;
};

#endif
