#ifndef TEST_QT_LOCAL_PEER_H
#define TEST_QT_LOCAL_PEER_H

#include <QObject>
#include <QString>
#include "qt-local-peer.h"

class TestQtLocalPeer : public QObject {
    Q_OBJECT

public:
    TestQtLocalPeer();
    ~TestQtLocalPeer();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor_with_empty_appId();
    void test_constructor_with_custom_appId();
    void test_applicationId();
    void test_isClient_first_instance();
    void test_isClient_second_instance();
    void test_sendMessage_when_not_client();
    void test_sendMessage_to_server();
    void test_messageReceived_signal();

private:
    QtLocalPeer *m_peer;
    QString m_appId;
};

#endif // TEST_QT_LOCAL_PEER_H
