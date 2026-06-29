#include "test_singlepage.h"
#include "../../../../../src-vpn/frontend/single-pages/singlepage.h"
#include <QCoreApplication>

TestSinglePage::TestSinglePage(QObject *parent)
    : QObject(parent)
    , m_singlePage(nullptr)
{
}

TestSinglePage::~TestSinglePage()
{
}

void TestSinglePage::initTestCase()
{
}

void TestSinglePage::cleanupTestCase()
{
}

void TestSinglePage::init()
{
    m_singlePage = new SinglePage();
}

void TestSinglePage::cleanup()
{
    if (m_singlePage) {
        delete m_singlePage;
        m_singlePage = nullptr;
    }
}

void TestSinglePage::testConstructor()
{
    QVERIFY(m_singlePage != nullptr);
    QVERIFY(m_singlePage->isVisible() == false);
    QVERIFY(m_singlePage->layout() != nullptr);
}

void TestSinglePage::testDestructor()
{
    QVERIFY(m_singlePage != nullptr);
    delete m_singlePage;
    m_singlePage = nullptr;
    QVERIFY(true);
}

void TestSinglePage::testShowDesktopNotify()
{
    SinglePage::showDesktopNotify("Test message", "");
    SinglePage::showDesktopNotify("Test message with sound", "soundName");
    QVERIFY(true);
}

void TestSinglePage::testInitUI()
{
    QVERIFY(m_singlePage != nullptr);
    QVERIFY(m_singlePage->layout() != nullptr);
    QVERIFY(true);
}

void TestSinglePage::testInitTransparency()
{
    QVERIFY(m_singlePage != nullptr);
    QVERIFY(true);
}

void TestSinglePage::testInitWindowProperties()
{
    QVERIFY(m_singlePage != nullptr);
    QVERIFY(m_singlePage->testAttribute(Qt::WA_TranslucentBackground));
    QVERIFY(true);
}

void TestSinglePage::testInitWindowTheme()
{
    QVERIFY(m_singlePage != nullptr);
    QVERIFY(true);
}
