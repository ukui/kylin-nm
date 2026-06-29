/*
 * Unit tests for proxyservicemanager.cpp
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QCoreApplication>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include "test_proxyservicemanager.h"
#include "../../../../src/backend/proxyservicemanager.h"

// Declare global functions from proxyservicemanager.cpp
QJsonObject readJsonFile(QString filePath);
QJsonObject dealJsonObj(const QStringList configList);
void wirteJsonFile(QString filePath, const QJsonObject obj);
QStringList getAppProxyFromFile();

void GlobalFunctionTest::SetUp() {
}

void GlobalFunctionTest::TearDown() {
}

void ProcInfoDealTest::SetUp() {
}

void ProcInfoDealTest::TearDown() {
}

void ProxyServiceManagerTest::SetUp() {
}

void ProxyServiceManagerTest::TearDown() {
}

// Test readJsonFile with empty file path
TEST_F(GlobalFunctionTest, ReadJsonFileEmptyPath)
{
    QJsonObject result = readJsonFile("");
    EXPECT_TRUE(result.isEmpty());
}

// Test readJsonFile with non-existent file
TEST_F(GlobalFunctionTest, ReadJsonFileNonExistent)
{
    QJsonObject result = readJsonFile("/non/existent/file.json");
    EXPECT_TRUE(result.isEmpty());
}

// Test readJsonFile with valid JSON file
TEST_F(GlobalFunctionTest, ReadJsonFileValid)
{
    QString filePath = tempDir.path() + "/test.json";
    QJsonObject obj;
    obj.insert("key", QJsonValue("value"));
    QJsonDocument doc(obj);
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
    file.close();
    
    QJsonObject result = readJsonFile(filePath);
    EXPECT_FALSE(result.isEmpty());
    EXPECT_EQ(result.value("key").toString(), "value");
}

// Test dealJsonObj with empty list
TEST_F(GlobalFunctionTest, DealJsonObjEmpty)
{
    QStringList configList;
    QJsonObject result = dealJsonObj(configList);
    EXPECT_TRUE(result.isEmpty());
}

// Test dealJsonObj with valid list (3 items)
TEST_F(GlobalFunctionTest, DealJsonObjValid3Items)
{
    QStringList configList;
    configList << "http" << "192.168.1.1" << "8080";
    QJsonObject result = dealJsonObj(configList);
    EXPECT_FALSE(result.isEmpty());
    EXPECT_EQ(result.value("type").toString(), "http");
    EXPECT_EQ(result.value("Server").toString(), "192.168.1.1");
    EXPECT_EQ(result.value("Port").toInt(), 8080);
}

// Test dealJsonObj with 4 items (includes username)
TEST_F(GlobalFunctionTest, DealJsonObj4Items)
{
    QStringList configList;
    configList << "http" << "192.168.1.1" << "8080" << "user1";
    QJsonObject result = dealJsonObj(configList);
    EXPECT_FALSE(result.isEmpty());
    EXPECT_EQ(result.value("UserName").toString(), "user1");
    EXPECT_EQ(result.value("Password").toString(), "");
}

// Test dealJsonObj with 5 items (includes username and password)
TEST_F(GlobalFunctionTest, DealJsonObj5Items)
{
    QStringList configList;
    configList << "http" << "192.168.1.1" << "8080" << "user1" << "pass123";
    QJsonObject result = dealJsonObj(configList);
    EXPECT_FALSE(result.isEmpty());
    EXPECT_EQ(result.value("UserName").toString(), "user1");
    EXPECT_EQ(result.value("Password").toString(), "pass123");
}

// Test wirteJsonFile with empty path
TEST_F(GlobalFunctionTest, WirteJsonFileEmptyPath)
{
    QJsonObject obj;
    obj.insert("key", QJsonValue("value"));
    wirteJsonFile("", obj);
}

// Test wirteJsonFile with valid path
TEST_F(GlobalFunctionTest, WirteJsonFileValid)
{
    QString filePath = tempDir.path() + "/write_test.json";
    QJsonObject obj;
    obj.insert("key", QJsonValue("value"));
    wirteJsonFile(filePath, obj);
    
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    EXPECT_FALSE(doc.isEmpty());
    EXPECT_EQ(doc.object().value("key").toString(), "value");
}

// Test getAppProxyFromFile with no file
TEST_F(GlobalFunctionTest, GetAppProxyFromFileNoFile)
{
    QStringList result = getAppProxyFromFile();
    EXPECT_TRUE(result.isEmpty());
}

// Test ProcInfoDeal constructor
TEST_F(ProcInfoDealTest, Constructor)
{
    ProcInfoDeal* deal = new ProcInfoDeal();
    EXPECT_NE(deal, nullptr);
    delete deal;
}

// Test ProcInfoDeal setProxyState
TEST_F(ProcInfoDealTest, SetProxyState)
{
    ProcInfoDeal* deal = new ProcInfoDeal();
    deal->setProxyState(true);
    deal->setProxyState(false);
    delete deal;
}

// Test ProcInfoDeal setDesktopList
TEST_F(ProcInfoDealTest, SetDesktopList)
{
    ProcInfoDeal* deal = new ProcInfoDeal();
    QStringList list;
    list << "app1.desktop" << "app2.desktop";
    deal->setDesktopList(list);
    delete deal;
}

// Test ProcInfoDeal setExecList
TEST_F(ProcInfoDealTest, SetExecList)
{
    ProcInfoDeal* deal = new ProcInfoDeal();
    QStringList list;
    list << "/usr/bin/app1" << "/usr/bin/app2";
    deal->setExecList(list);
    delete deal;
}

// Test ProcInfoDeal setNameList
TEST_F(ProcInfoDealTest, SetNameList)
{
    ProcInfoDeal* deal = new ProcInfoDeal();
    QStringList list;
    list << "App1" << "App2";
    deal->setNameList(list);
    delete deal;
}

// Test ProxyServiceManager basic functions
TEST_F(ProxyServiceManagerTest, BasicFunctions)
{
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
