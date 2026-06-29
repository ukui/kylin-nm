#include "test_detailpage.h"

TestDetailPage::TestDetailPage()
{
}

TestDetailPage::~TestDetailPage()
{
}

void TestDetailPage::initTestCase()
{
}

void TestDetailPage::cleanupTestCase()
{
}

void TestDetailPage::init()
{
}

void TestDetailPage::cleanup()
{
    if (m_detailPage) {
        delete m_detailPage;
        m_detailPage = nullptr;
    }
}

void TestDetailPage::test_setSSID()
{
    DetailPage page(false, false);
    QString ssid = "TestSSID";
    page.setSSID(ssid);
    QVERIFY(page.m_SSIDLabel->text() == ssid);
}

void TestDetailPage::test_setSSID_longName()
{
    DetailPage page(false, false);
    QString longSsid = "ThisIsAVeryLongSSIDNameThatExceedsTheMaximumWidthLimitForDisplay";
    page.setSSID(longSsid);
    QVERIFY(!page.m_SSIDLabel->text().isEmpty());
    QVERIFY(!page.toolTip().isEmpty());
}

void TestDetailPage::test_setProtocol()
{
    DetailPage page(false, false);
    QString protocol = "802.11ac";
    page.setProtocol(protocol);
    QVERIFY(page.m_Protocol->text() == protocol);
}

void TestDetailPage::test_setSecType()
{
    DetailPage page(true, false);
    QString secType = "WPA2-Personal";
    page.setSecType(secType);
    QVERIFY(page.m_SecType->text() == secType);
}

void TestDetailPage::test_setHz()
{
    DetailPage page(true, false);
    QString hz = "5GHz";
    page.setHz(hz);
    QVERIFY(page.m_Hz->text() == hz);
}

void TestDetailPage::test_setChan()
{
    DetailPage page(true, false);
    QString chan = "36";
    page.setChan(chan);
    QVERIFY(page.m_Chan->text() == chan);
}

void TestDetailPage::test_setBandWidth()
{
    DetailPage page(false, false);
    QString bandWidth = "80MHz";
    page.setBandWidth(bandWidth);
    QVERIFY(page.m_BandWidth->text() == bandWidth);
}

void TestDetailPage::test_setIpv4()
{
    DetailPage page(false, false);
    QString ipv4 = "192.168.1.100";
    page.setIpv4(ipv4);
    QVERIFY(page.m_IPV4->text() == ipv4);
}

void TestDetailPage::test_setIpv4Dns()
{
    DetailPage page(false, false);
    QString ipv4Dns = "8.8.8.8";
    page.setIpv4Dns(ipv4Dns);
    QVERIFY(page.m_IPV4Dns->getText() == ipv4Dns);
}

void TestDetailPage::test_setIpv6()
{
    DetailPage page(false, false);
    QString ipv6 = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
    page.setIpv6(ipv6);
    QVERIFY(page.m_IPV6->getText() == ipv6);
}

void TestDetailPage::test_setMac()
{
    DetailPage page(false, false);
    QString mac = "00:11:22:33:44:55";
    page.setMac(mac);
    QVERIFY(page.m_Mac->text() == mac);
}

void TestDetailPage::test_checkIsChanged_not_wlan()
{
    DetailPage page(false, false);
    ConInfo info;
    bool changed = page.checkIsChanged(info);
    QVERIFY(changed == false);
}

void TestDetailPage::test_getSsid_create()
{
    DetailPage page(false, true);
    page.m_SSIDEdit->setText("TestSSID");
    QString ssid;
    page.getSsid(ssid);
    QVERIFY(ssid == "TestSSID");
}

void TestDetailPage::test_getSsid_not_create()
{
    DetailPage page(false, false);
    page.m_SSIDLabel->setText("TestSSID");
    QString ssid;
    page.getSsid(ssid);
    QVERIFY(ssid == "TestSSID");
}
