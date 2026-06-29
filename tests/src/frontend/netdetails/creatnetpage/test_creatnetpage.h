#ifndef TEST_CREATNETPAGE_H
#define TEST_CREATNETPAGE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QApplication>
#include <QDebug>

#include "creatnetpage.h"

class TestCreatNetPage : public QObject
{
    Q_OBJECT
public:
    TestCreatNetPage();
    ~TestCreatNetPage();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_publicInterfaces();
    void test_setCreatePageStateSignal();
    void test_ipv4EditFinishedSignal();
    void test_loadingFunctions();
    void test_showIpv4AddressConflict();
    void test_getErrorMessage();

private:
    CreatNetPage *m_creatNetPage = nullptr;
};

#endif // TEST_CREATNETPAGE_H
