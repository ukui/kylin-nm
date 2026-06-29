#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QThread>
#include <QTest>
#include "test_qt-local-peer.h"

// Helper function to generate unique app ID
QString TestQtLocalPeer::generateUniqueAppId() {
    static int counter = 0;
    return QString("test_app_%1_%2").arg(QCoreApplication::applicationPid()).arg(counter++);
}

// Cleanup socket files
void TestQtLocalPeer::cleanupSocketFiles() {
    // Clean up any leftover socket files from previous tests
    QStringList socketFiles = QDir("/tmp").entryList(QStringList() << "qtsingleapp-*", QDir::Files);
    for (const QString &file : socketFiles) {
        QFile::remove("/tmp/" + file);
        QFile::remove("/tmp/" + file + "-lockfile");
    }
}

TestQtLocalPeer::TestQtLocalPeer() : peer(nullptr) {
}

TestQtLocalPeer::~TestQtLocalPeer() {
    if (peer) {
        delete peer;
        peer = nullptr;
    }
}

// Test fixture
class QtLocalPeerTest : public ::testing::Test {
protected:
    void SetUp() override {
        tester = new TestQtLocalPeer();
        tester->cleanupSocketFiles();
    }
    
    void TearDown() override {
        delete tester;
        tester = nullptr;
        cleanupSocketFiles();
    }
    
    void cleanupSocketFiles() {
        QStringList socketFiles = QDir("/tmp").entryList(QStringList() << "qtsingleapp-*", QDir::Files);
        for (const QString &file : socketFiles) {
            QFile::remove("/tmp/" + file);
            QFile::remove("/tmp/" + file + "-lockfile");
        }
    }
    
    TestQtLocalPeer* tester;
};

// Test constructor with empty appId
TEST_F(QtLocalPeerTest, ConstructorWithEmptyAppId) {
    tester->test_constructor_withEmptyAppId();
}

// Test constructor with valid appId
TEST_F(QtLocalPeerTest, ConstructorWithValidAppId) {
    tester->test_constructor_withValidAppId();
}

// Test constructor with different appId
TEST_F(QtLocalPeerTest, ConstructorWithDifferentAppId) {
    tester->test_constructor_withDifferentAppId();
}

// Test applicationId getter
TEST_F(QtLocalPeerTest, ApplicationId) {
    tester->test_applicationId();
}

// Test isClient for first instance
TEST_F(QtLocalPeerTest, IsClientFirstInstance) {
    tester->test_isClient_firstInstance();
}

// Test isClient for second instance
TEST_F(QtLocalPeerTest, IsClientSecondInstance) {
    tester->test_isClient_secondInstance();
}

// Test sendMessage when no server is running
TEST_F(QtLocalPeerTest, SendMessageNoServer) {
    tester->test_sendMessage_noServer();
}

// Test receiveConnection with valid data
TEST_F(QtLocalPeerTest, ReceiveConnectionValidData) {
    tester->test_receiveConnection_validData();
}

// Test receiveConnection with invalid socket
TEST_F(QtLocalPeerTest, ReceiveConnectionInvalidSocket) {
    tester->test_receiveConnection_invalidSocket();
}

// Test receiveConnection with partial data
TEST_F(QtLocalPeerTest, ReceiveConnectionPartialData) {
    tester->test_receiveConnection_partialData();
}
