#include "test_netdetail.h"
#include "../../../../../../src/frontend/netdetails/netdetail.h"

TestNetDetail::TestNetDetail(QObject *parent)
    : QObject(parent)
    , m_threadObject(nullptr)
{
}

TestNetDetail::~TestNetDetail()
{
}

void TestNetDetail::initTestCase()
{
}

void TestNetDetail::cleanupTestCase()
{
}

void TestNetDetail::init()
{
    m_threadObject = new ThreadObject("eth0");
}

void TestNetDetail::cleanup()
{
    delete m_threadObject;
    m_threadObject = nullptr;
}

void TestNetDetail::testWarningDialogConstructor()
{
    WarningDialog *dialog = new WarningDialog();
    QVERIFY(dialog != nullptr);
    delete dialog;
}

void TestNetDetail::testWarningDialogSetMessage()
{
    WarningDialog *dialog = new WarningDialog();
    QString testMessage = "This is a test warning message";
    dialog->setWarningMessage(testMessage);
    QVERIFY(dialog != nullptr);
    delete dialog;
}

void TestNetDetail::testNetTabBarConstructor()
{
    NetTabBar *tabBar = new NetTabBar();
    QVERIFY(tabBar != nullptr);
    delete tabBar;
}

void TestNetDetail::testThreadObjectConstructor()
{
    ThreadObject *obj = new ThreadObject("wlan0");
    QVERIFY(obj != nullptr);
    delete obj;

    obj = new ThreadObject("");
    QVERIFY(obj != nullptr);
    delete obj;
}

void TestNetDetail::testThreadObjectStop()
{
    m_threadObject->stop();
    QVERIFY(true);
}

QTEST_MAIN(TestNetDetail)

#include "test_netdetail.moc"
