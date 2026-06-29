#ifndef TEST_JOINHIDDENWIFIPAGE_H
#define TEST_JOINHIDDENWIFIPAGE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class JoinHiddenWiFiPage;
class TestJoinHiddenWiFiPage : public QObject
{
    Q_OBJECT

public:
    TestJoinHiddenWiFiPage();
    ~TestJoinHiddenWiFiPage();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testDestructor();
    void testSetEnterpriseInfo_EmptyEapList();
    void testSetEnterpriseInfo_PeapMethod();
    void testSetEnterpriseInfo_TtlsMethod();
    void testSetEnterpriseInfo_TlsMethod();
    void testSetEnterpriseInfo_LeapMethod();
    void testSetEnterpriseInfo_PwdMethod();
    void testSetEnterpriseInfo_FastMethod();
    void testSetEnterpriseInfo_DefaultEapType();
    void testInitialWindowSize();
    void testWindowTitleAfterSetEnterpriseInfo();
    void testSecuWidgetSecurityType();
    void testWidgetsExist();

private:
    JoinHiddenWiFiPage *m_widget;
};

#endif // TEST_JOINHIDDENWIFIPAGE_H
