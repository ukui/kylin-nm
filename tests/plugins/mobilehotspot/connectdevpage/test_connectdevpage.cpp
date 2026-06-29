#include <QTest>
#include <QSignalSpy>
#include <QDBusInterface>
#include "connectdevpage.h"

class TestConnectdevPage : public QObject
{
    Q_OBJECT

public:
    TestConnectdevPage();
    ~TestConnectdevPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_onStaDevAdded();
    void test_onStaDevRemoved();
    void test_refreshStalistWithValidInterface();
    void test_refreshStalistWithNullInterface();

private:
    ConnectdevPage *m_connectdevPage;
};

TestConnectdevPage::TestConnectdevPage()
    : m_connectdevPage(nullptr)
{
}

TestConnectdevPage::~TestConnectdevPage()
{
}

void TestConnectdevPage::initTestCase()
{
}

void TestConnectdevPage::cleanupTestCase()
{
}

void TestConnectdevPage::init()
{
    m_connectdevPage = new ConnectdevPage();
}

void TestConnectdevPage::cleanup()
{
    if (m_connectdevPage) {
        delete m_connectdevPage;
        m_connectdevPage = nullptr;
    }
}

void TestConnectdevPage::test_constructor()
{
    QVERIFY(m_connectdevPage != nullptr);
    QVERIFY(m_connectdevPage->isVisible() == false);

    QList<QWidget *> children = m_connectdevPage->findChildren<QWidget *>();
    QVERIFY(children.size() > 0);
}

void TestConnectdevPage::test_onStaDevAdded()
{
    QSignalSpy spy(m_connectdevPage, &ConnectdevPage::setStaIntoBlacklist);

    m_connectdevPage->onStaDevAdded(true, "AA:BB:CC:DD:EE:FF", "TestDevice1");
    QVERIFY(m_connectdevPage->isVisible() == true);

    m_connectdevPage->onStaDevAdded(true, "11:22:33:44:55:66", "TestDevice2");
    QVERIFY(spy.count() == 0);
}

void TestConnectdevPage::test_onStaDevRemoved()
{
    m_connectdevPage->onStaDevAdded(true, "AA:BB:CC:DD:EE:FF", "TestDevice1");
    QVERIFY(m_connectdevPage->isVisible() == true);

    m_connectdevPage->onStaDevRemoved(true, "AA:BB:CC:DD:EE:FF", "TestDevice1");
    QVERIFY(m_connectdevPage->isVisible() == false);

    m_connectdevPage->onStaDevAdded(true, "AA:BB:CC:DD:EE:FF", "TestDevice1");
    m_connectdevPage->onStaDevAdded(true, "11:22:33:44:55:66", "TestDevice2");
    QVERIFY(m_connectdevPage->isVisible() == true);

    m_connectdevPage->onStaDevRemoved(true, "AA:BB:CC:DD:EE:FF", "TestDevice1");
    QVERIFY(m_connectdevPage->isVisible() == true);

    m_connectdevPage->onStaDevRemoved(true, "11:22:33:44:55:66", "TestDevice2");
    QVERIFY(m_connectdevPage->isVisible() == false);
}

void TestConnectdevPage::test_refreshStalistWithValidInterface()
{
    m_connectdevPage->refreshStalist();
    QVERIFY(m_connectdevPage->isVisible() == false);
}

void TestConnectdevPage::test_refreshStalistWithNullInterface()
{
    m_connectdevPage->setInterface(nullptr);
    m_connectdevPage->refreshStalist();
    QVERIFY(m_connectdevPage->isVisible() == false);
}

QTEST_MAIN(TestConnectdevPage)

#include "test_connectdevpage.moc"
