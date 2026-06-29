#include <QApplication>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>
#include "vpnitem.h"

class TestVpnItem : public QObject
{
    Q_OBJECT
public:
    TestVpnItem() {}

private slots:
    void testConstructor();
    void testStartLoading();
    void testStopLoading();
    void testSetConnectActionText();
    void testSetCornerType();
    void testConnectActionTriggeredConnect();
    void testConnectActionTriggeredDisconnect();
    void testDeleteActionTriggered();
};

void TestVpnItem::testConstructor()
{
    VpnItem *item = new VpnItem(true, nullptr);
    QVERIFY2(item != nullptr, "Failed to create VpnItem instance");
    QVERIFY(item->m_isAcitve == true);
    QVERIFY(item->m_iconLabel != nullptr);
    QVERIFY(item->m_titileLabel != nullptr);
    QVERIFY(item->m_statusLabel != nullptr);
    QVERIFY(item->m_infoLabel != nullptr);
    QVERIFY(item->m_moreButton != nullptr);
    QVERIFY(item->m_moreMenu != nullptr);
    QVERIFY(item->m_connectAction != nullptr);
    QVERIFY(item->m_deleteAction != nullptr);
    delete item;

    VpnItem *item2 = new VpnItem(false, nullptr);
    QVERIFY(item2->m_isAcitve == false);
    delete item2;
}

void TestVpnItem::testStartLoading()
{
    VpnItem *item = new VpnItem(false, nullptr);

    item->startLoading();
    QVERIFY(item->m_loading == true);

    delete item;
}

void TestVpnItem::testStopLoading()
{
    VpnItem *item = new VpnItem(false, nullptr);
    item->startLoading();

    item->stopLoading();
    QVERIFY(item->m_loading == false);

    delete item;
}

void TestVpnItem::testSetConnectActionText()
{
    VpnItem *item = new VpnItem(false, nullptr);
    item->setConnectActionText(true);
    QVERIFY(item->m_connectAction->text() == QString::fromUtf8("Disconnect"));

    item->setConnectActionText(false);
    QVERIFY(item->m_connectAction->text() == QString::fromUtf8("Connect"));

    delete item;
}

void TestVpnItem::testSetCornerType()
{
    VpnItem *item = new VpnItem(false, nullptr);

    item->setCornerType(VpnItem::Top);
    item->repaint();

    item->setCornerType(VpnItem::None);
    item->repaint();

    delete item;
}

void TestVpnItem::testConnectActionTriggeredConnect()
{
    VpnItem *item = new VpnItem(false, nullptr);
    item->setConnectActionText(false);
    QSignalSpy spy(item, SIGNAL(connectActionTriggered()));

    QMetaObject::invokeMethod(item, "onConnectTriggered", Qt::DirectConnection);
    QVERIFY(spy.count() == 1);

    delete item;
}

void TestVpnItem::testConnectActionTriggeredDisconnect()
{
    VpnItem *item = new VpnItem(false, nullptr);
    item->setConnectActionText(true);
    QSignalSpy spy(item, SIGNAL(disconnectActionTriggered()));

    QMetaObject::invokeMethod(item, "onConnectTriggered", Qt::DirectConnection);
    QVERIFY(spy.count() == 1);

    delete item;
}

void TestVpnItem::testDeleteActionTriggered()
{
    VpnItem *item = new VpnItem(false, nullptr);
    QSignalSpy spy(item, SIGNAL(deleteActionTriggered()));

    QMetaObject::invokeMethod(item, "onDeletetTriggered", Qt::DirectConnection);
    QVERIFY(spy.count() == 1);

    delete item;
}

QTEST_MAIN(TestVpnItem)
#include "test_vpnitem.moc"
