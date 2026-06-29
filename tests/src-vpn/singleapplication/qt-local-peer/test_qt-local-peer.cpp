#include "test_qt-local-peer.h"
#include <QTest>
#include <QSignalSpy>
#include <QCoreApplication>
#include <QLocalSocket>
#include <QDataStream>

TestQtLocalPeer::TestQtLocalPeer()
    : m_peer(nullptr), m_appId("test_app_local_peer")
{
}

TestQtLocalPeer::~TestQtLocalPeer()
{
}

void TestQtLocalPeer::initTestCase()
{
}

void TestQtLocalPeer::cleanupTestCase()
{
    if (m_peer) {
        delete m_peer;
        m_peer = nullptr;
    }
}

void TestQtLocalPeer::init()
{
}

void TestQtLocalPeer::cleanup()
{
}

void TestQtLocalPeer::test_constructor_with_empty_appId()
{
    QtLocalPeer peer(nullptr, QString());
    QVERIFY(!peer.applicationId().isEmpty());
}

void TestQtLocalPeer::test_constructor_with_custom_appId()
{
    QtLocalPeer peer(nullptr, m_appId);
    QVERIFY(peer.applicationId() == m_appId);
}

void TestQtLocalPeer::test_applicationId()
{
    QtLocalPeer peer(nullptr, m_appId);
    QString id = peer.applicationId();
    QVERIFY(id == m_appId);
}

void TestQtLocalPeer::test_isClient_first_instance()
{
    QtLocalPeer peer(nullptr, m_appId + "_first_");
    bool isClient = peer.isClient();
    QVERIFY(isClient == false);
}

void TestQtLocalPeer::test_isClient_second_instance()
{
    {
        QtLocalPeer peer1(nullptr, m_appId + "_second_");
        peer1.isClient();
        
        QString socketName = peer1.applicationId().section(QLatin1Char('-'), 0, 2);
        QLocalSocket socket;
        socket.connectToServer(socketName);
        socket.waitForConnected(500);
    }
    
    QtLocalPeer peer2(nullptr, m_appId + "_second_");
    bool isClient = peer2.isClient();
    QVERIFY(isClient == true);
}

void TestQtLocalPeer::test_sendMessage_when_not_client()
{
    QtLocalPeer peer(nullptr, m_appId + "_notclient_");
    peer.isClient();
    bool result = peer.sendMessage("test message", 100);
    QVERIFY(result == false);
}

void TestQtLocalPeer::test_sendMessage_to_server()
{
    {
        QtLocalPeer serverPeer(nullptr, m_appId + "_server_");
        serverPeer.isClient();
        
        QTest::qWait(100);
        
        QString socketName = serverPeer.applicationId().section(QLatin1Char('-'), 0, 2);
        QLocalSocket socket;
        socket.connectToServer(socketName);
        if (socket.waitForConnected(500)) {
            QByteArray msg = "test message to server";
            QDataStream ds(&socket);
            ds.writeBytes(msg.constData(), msg.size());
            socket.waitForBytesWritten(500);
            
            socket.waitForReadyRead(500);
            socket.read(socket.bytesAvailable());
        }
    }
    
    QTest::qWait(100);
    
    QtLocalPeer clientPeer(nullptr, m_appId + "_server_");
    bool isClient = clientPeer.isClient();
    if (isClient) {
        clientPeer.sendMessage("test message to server", 1000);
    }
}

void TestQtLocalPeer::test_messageReceived_signal()
{
    QtLocalPeer *peer = new QtLocalPeer(nullptr, m_appId + "_signal_");
    peer->isClient();
    
    QSignalSpy spy(peer, &QtLocalPeer::messageReceived);
    QVERIFY(spy.isValid());
    
    delete peer;
    peer = nullptr;
}
