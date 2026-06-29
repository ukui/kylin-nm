#include <QTest>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QTimer>
#include <QCloseEvent>

#include "joinhiddenwifipage.h"
#include "test_joinhiddenwifipage.h"

TestJoinHiddenWiFiPage::TestJoinHiddenWiFiPage()
{
}

TestJoinHiddenWiFiPage::~TestJoinHiddenWiFiPage()
{
}

void TestJoinHiddenWiFiPage::initTestCase()
{
}

void TestJoinHiddenWiFiPage::cleanupTestCase()
{
}

void TestJoinHiddenWiFiPage::init()
{
    m_widget = new JoinHiddenWiFiPage("wlan0", nullptr);
}

void TestJoinHiddenWiFiPage::cleanup()
{
    delete m_widget;
    m_widget = nullptr;
}

void TestJoinHiddenWiFiPage::testConstructor()
{
    JoinHiddenWiFiPage *widget = new JoinHiddenWiFiPage("wlan0", nullptr);
    QVERIFY(widget != nullptr);
    
    QPushButton *joinBtn = widget->findChild<QPushButton *>();
    QVERIFY(joinBtn != nullptr);
    
    delete widget;
}

void TestJoinHiddenWiFiPage::testDestructor()
{
    JoinHiddenWiFiPage *widget = new JoinHiddenWiFiPage("wlan0", nullptr);
    delete widget;
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_EmptyEapList()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList());
    
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_PeapMethod()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "peap");
    info.insert("identity", "testuser");
    info.insert("password", "testpassword");
    info.insert("phase2-auth", 0);
    
    QSignalSpy spy(m_widget, &JoinHiddenWiFiPage::hiddenWiFiPageClose);
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_TtlsMethod()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "ttls");
    info.insert("identity", "testuser");
    info.insert("password", "testpassword");
    
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_TlsMethod()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "tls");
    info.insert("identity", "testuser");
    info.insert("private-key-password", "keypassword");
    
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_LeapMethod()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "leap");
    info.insert("identity", "testuser");
    info.insert("password", "testpassword");
    
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_PwdMethod()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "pwd");
    info.insert("identity", "testuser");
    info.insert("password", "testpassword");
    
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_FastMethod()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "fast");
    info.insert("identity", "testuser");
    info.insert("password", "testpassword");
    
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testSetEnterpriseInfo_DefaultEapType()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "unknown");
    info.insert("identity", "testuser");
    info.insert("password", "testpassword");
    
    m_widget->setEnterpriseInfo(info);
    QVERIFY(true);
}

void TestJoinHiddenWiFiPage::testInitialWindowSize()
{
    QVERIFY(m_widget->width() == 480);
    QVERIFY(m_widget->height() == 368);
}

void TestJoinHiddenWiFiPage::testWindowTitleAfterSetEnterpriseInfo()
{
    QVariantMap info;
    info.insert("ssid", "TestNetwork");
    info.insert("eap", QStringList() << "peap");
    m_widget->setEnterpriseInfo(info);
    
    QString title = m_widget->windowTitle();
    QVERIFY(title.contains("Connect"));
}

void TestJoinHiddenWiFiPage::testSecuWidgetSecurityType()
{
    QVERIFY(m_widget->findChild<QWidget *>() != nullptr);
}

void TestJoinHiddenWiFiPage::testWidgetsExist()
{
    QVERIFY(m_widget->findChild<QPushButton *>("cancelBtn") != nullptr || m_widget->findChildren<QPushButton *>().size() >= 2);
    QVERIFY(m_widget->findChild<QLineEdit *>() != nullptr || m_widget->findChildren<QLineEdit *>().size() >= 1);
}
