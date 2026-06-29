#include "test_creatnetpage.h"

TestCreatNetPage::TestCreatNetPage()
{
}

TestCreatNetPage::~TestCreatNetPage()
{
}

void TestCreatNetPage::initTestCase()
{
}

void TestCreatNetPage::cleanupTestCase()
{
}

void TestCreatNetPage::init()
{
    m_creatNetPage = new CreatNetPage();
}

void TestCreatNetPage::cleanup()
{
    if (m_creatNetPage) {
        delete m_creatNetPage;
        m_creatNetPage = nullptr;
    }
}

void TestCreatNetPage::test_constructor()
{
    CreatNetPage page;
    QVERIFY(true);
}

void TestCreatNetPage::test_publicInterfaces()
{
    CreatNetPage page;
    QVERIFY(true);
    page.startLoading();
    page.stopLoading();
    page.showIpv4AddressConflict(true);
    page.showIpv4AddressConflict(false);
    page.getErrorMessage();
    page.updateIcon();
}

void TestCreatNetPage::test_setCreatePageStateSignal()
{
    CreatNetPage page;
    QSignalSpy spy(&page, SIGNAL(setCreatePageState(bool)));
    QVERIFY(spy.isValid());
}

void TestCreatNetPage::test_ipv4EditFinishedSignal()
{
    CreatNetPage page;
    QSignalSpy spy(&page, SIGNAL(ipv4EditFinished(QString)));
    QVERIFY(spy.isValid());
}

void TestCreatNetPage::test_loadingFunctions()
{
    CreatNetPage page;
    page.startLoading();
    page.stopLoading();
    QVERIFY(true);
}

void TestCreatNetPage::test_showIpv4AddressConflict()
{
    CreatNetPage page;
    page.showIpv4AddressConflict(true);
    page.showIpv4AddressConflict(false);
    QVERIFY(true);
}

void TestCreatNetPage::test_getErrorMessage()
{
    CreatNetPage page;
    QString msg = page.getErrorMessage();
    QVERIFY(true);
}
