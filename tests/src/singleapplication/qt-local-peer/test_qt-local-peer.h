#ifndef TEST_QT_LOCAL_PEER_H
#define TEST_QT_LOCAL_PEER_H

#include <QObject>
#include <QString>
#include <QSignalSpy>
#include "qt-local-peer.h"

class TestQtLocalPeer {
public:
    TestQtLocalPeer();
    ~TestQtLocalPeer();

    void test_constructor_withEmptyAppId();
    void test_constructor_withValidAppId();
    void test_constructor_withDifferentAppId();
    void test_applicationId();
    void test_isClient_firstInstance();
    void test_isClient_secondInstance();
    void test_sendMessage_noServer();
    void test_receiveConnection_validData();
    void test_receiveConnection_invalidSocket();
    void test_receiveConnection_partialData();

    QString generateUniqueAppId();
    void cleanupSocketFiles();

private:
    QtLocalPeer* peer;
};

#endif // TEST_QT_LOCAL_PEER_H
