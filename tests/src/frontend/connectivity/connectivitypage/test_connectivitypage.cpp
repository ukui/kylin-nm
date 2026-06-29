#include "test_connectivitypage.h"
#include "connectivitypage.h"

TestConnectivityPage::TestConnectivityPage(QObject *parent)
    : QObject(parent)
{
}

void TestConnectivityPage::initTestCase()
{
}

void TestConnectivityPage::cleanupTestCase()
{
}

void TestConnectivityPage::init()
{
}

void TestConnectivityPage::cleanup()
{
}

void TestConnectivityPage::test_constructor_emptyUri()
{
    QString uri = "";
    ConnectivityPage *page = new ConnectivityPage(uri);
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage with empty uri");
    QVERIFY2(page->windowTitle() == tr("Network connectivity detection"), "Window title should match");
    delete page;
}

void TestConnectivityPage::test_constructor_withUri()
{
    QString uri = "http://example.com";
    ConnectivityPage *page = new ConnectivityPage(uri);
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage with uri");
    QVERIFY2(page->windowTitle() == tr("Network connectivity detection"), "Window title should match");
    delete page;
}

void TestConnectivityPage::test_setWarning_Full()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QMetaObject::invokeMethod(page, "setWarning", Qt::DirectConnection,
        Q_ARG(NetworkManager::Connectivity, NetworkManager::Connectivity::Full));

    QTest::qWait(100);
    delete page;
}

void TestConnectivityPage::test_setWarning_Limited()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QMetaObject::invokeMethod(page, "setWarning", Qt::DirectConnection,
        Q_ARG(NetworkManager::Connectivity, NetworkManager::Connectivity::Limited));

    QTest::qWait(100);
    delete page;
}

void TestConnectivityPage::test_setWarning_Portal()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QMetaObject::invokeMethod(page, "setWarning", Qt::DirectConnection,
        Q_ARG(NetworkManager::Connectivity, NetworkManager::Connectivity::Portal));

    QTest::qWait(100);
    delete page;
}

void TestConnectivityPage::test_setWarning_UnknownConnectivity()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QMetaObject::invokeMethod(page, "setWarning", Qt::DirectConnection,
        Q_ARG(NetworkManager::Connectivity, NetworkManager::Connectivity::UnknownConnectivity));

    QTest::qWait(100);
    delete page;
}

void TestConnectivityPage::test_setWarning_NoConnectivity()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QMetaObject::invokeMethod(page, "setWarning", Qt::DirectConnection,
        Q_ARG(NetworkManager::Connectivity, NetworkManager::Connectivity::NoConnectivity));

    QTest::qWait(100);
    delete page;
}

void TestConnectivityPage::test_checkUri_empty()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QLineEdit *uriEdit = page->findChild<QLineEdit *>();
    QVERIFY2(uriEdit != nullptr, "Could not find QLineEdit");

    if (uriEdit) {
        QSignalSpy spy(uriEdit, &QLineEdit::textChanged);
        QMetaObject::invokeMethod(uriEdit, "setText", Qt::DirectConnection, Q_ARG(QString, ""));
        QTest::qWait(50);
    }

    delete page;
}

void TestConnectivityPage::test_checkUri_validUrl()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QLineEdit *uriEdit = page->findChild<QLineEdit *>();
    QVERIFY2(uriEdit != nullptr, "Could not find QLineEdit");

    if (uriEdit) {
        QMetaObject::invokeMethod(uriEdit, "setText", Qt::DirectConnection, Q_ARG(QString, "http://example.com"));
        QTest::qWait(50);
    }

    delete page;
}

void TestConnectivityPage::test_checkUri_invalidUrl()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QLineEdit *uriEdit = page->findChild<QLineEdit *>();
    QVERIFY2(uriEdit != nullptr, "Could not find QLineEdit");

    if (uriEdit) {
        QMetaObject::invokeMethod(uriEdit, "setText", Qt::DirectConnection, Q_ARG(QString, "invalid-url"));
        QTest::qWait(50);
    }

    delete page;
}

void TestConnectivityPage::test_closeEvent()
{
    ConnectivityPage *page = new ConnectivityPage("");
    QVERIFY2(page != nullptr, "Failed to create ConnectivityPage");

    QSignalSpy spy(page, &ConnectivityPage::pageClose);

    page->close();
    QTest::qWait(100);

    QVERIFY(spy.count() == 1);

    delete page;
}
