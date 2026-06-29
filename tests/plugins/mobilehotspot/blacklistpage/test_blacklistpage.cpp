#include "test_blacklistpage.h"
#include "blacklistpage.h"
#include <QDebug>
#include <QDBusReply>

TestBlacklistPage::TestBlacklistPage(QObject *parent)
    : QObject(parent)
{
}

TestBlacklistPage::~TestBlacklistPage()
{
}

void TestBlacklistPage::initTestCase()
{
}

void TestBlacklistPage::cleanupTestCase()
{
}

void TestBlacklistPage::init()
{
    m_mockInterface = new QDBusInterface("com.kylin.nm",
                                          "/com/kylin/nm",
                                          "com.kylin.nm.interface",
                                          QDBusConnection::sessionBus(),
                                          this);
    m_blacklistPage = new BlacklistPage();
    m_blacklistPage->setInterface(m_mockInterface);
}

void TestBlacklistPage::cleanup()
{
    if (m_blacklistPage) {
        delete m_blacklistPage;
        m_blacklistPage = nullptr;
    }
    if (m_mockInterface) {
        delete m_mockInterface;
        m_mockInterface = nullptr;
    }
}

void TestBlacklistPage::test_constructor()
{
    BlacklistPage *page = new BlacklistPage();
    QVERIFY(page != nullptr);
    QVERIFY(page->isVisible());
    delete page;
}

void TestBlacklistPage::test_refreshBlacklist()
{
    m_blacklistPage->refreshBlacklist();
}

void TestBlacklistPage::test_clearBlacklistLayout()
{
}

void TestBlacklistPage::test_resetLayoutHight()
{
}

void TestBlacklistPage::test_myLine()
{
}

void TestBlacklistPage::test_getBlacklistDevice()
{
}

void TestBlacklistPage::test_initBlacklistDev()
{
}

void TestBlacklistPage::test_onsetStaIntoBlacklist()
{
}

void TestBlacklistPage::test_addBlacklistDevFrame()
{
}

void TestBlacklistPage::test_removeStaFromBlacklist()
{
}

void TestBlacklistPage::test_onRemoveFromBlacklistBtnClicked()
{
}
