#ifndef TEST_DETAILPAGE_H
#define TEST_DETAILPAGE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QApplication>
#include <QDebug>

#include "detailpage.h"
#include "coninfo.h"

class TestDetailPage : public QObject
{
    Q_OBJECT
public:
    TestDetailPage();
    ~TestDetailPage();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_setSSID();
    void test_setSSID_longName();
    void test_setProtocol();
    void test_setSecType();
    void test_setHz();
    void test_setChan();
    void test_setBandWidth();
    void test_setIpv4();
    void test_setIpv4Dns();
    void test_setIpv6();
    void test_setMac();

    void test_checkIsChanged_not_wlan();

    void test_getSsid_create();
    void test_getSsid_not_create();

private:
    DetailPage *m_detailPage = nullptr;
};

#endif // TEST_DETAILPAGE_H
