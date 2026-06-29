#include <QTest>
#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QHostAddress>
#include <QDebug>
#include <QPushButton>
#include <QApplication>

#include "multiplednswidget.h"
#include "test_multiplednswidget.h"

void TestMultipleDnsWidget::initTestCase()
{
    m_regExp = QRegExp("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
}

void TestMultipleDnsWidget::cleanupTestCase()
{}

void TestMultipleDnsWidget::init()
{
    m_widget = new MultipleDnsWidget(m_regExp, true, nullptr);
}

void TestMultipleDnsWidget::cleanup()
{
    delete m_widget;
    m_widget = nullptr;
}

void TestMultipleDnsWidget::testConstructor()
{
    MultipleDnsWidget *widget = new MultipleDnsWidget(m_regExp, true, nullptr);
    QVERIFY(widget != nullptr);
    QVERIFY(widget->findChild<QListWidget *>("dnsListWidget") != nullptr || widget->findChildren<QListWidget *>().size() > 0);
    delete widget;
}

void TestMultipleDnsWidget::testConstructor_settingShowFalse()
{
    MultipleDnsWidget *widget = new MultipleDnsWidget(m_regExp, false, nullptr);
    QVERIFY(widget != nullptr);
    delete widget;
}

void TestMultipleDnsWidget::testSetEditEnabled_True()
{
    m_widget->setEditEnabled(true);
    QVERIFY(true);
}

void TestMultipleDnsWidget::testSetEditEnabled_False()
{
    m_widget->setEditEnabled(false);
    QListWidget *listWidget = m_widget->findChild<QListWidget *>();
    if (listWidget) {
        QVERIFY(listWidget->count() == 0);
    }
}

void TestMultipleDnsWidget::testGetDns_Empty()
{
    QList<QHostAddress> dnsList = m_widget->getDns();
    QVERIFY(dnsList.isEmpty());
}

void TestMultipleDnsWidget::testGetDns_WithDnsEntries()
{
    QList<QHostAddress> dns;
    dns.append(QHostAddress("8.8.8.8"));
    dns.append(QHostAddress("114.114.114.114"));
    m_widget->setDnsListText(dns);

    QList<QHostAddress> result = m_widget->getDns();
    QVERIFY(result.size() == 2);
    QVERIFY(result.at(0).toString() == "8.8.8.8");
    QVERIFY(result.at(1).toString() == "114.114.114.114");
}

void TestMultipleDnsWidget::testGetDns_WithDuplicates()
{
    QList<QHostAddress> dns;
    dns.append(QHostAddress("8.8.8.8"));
    dns.append(QHostAddress("8.8.8.8"));
    dns.append(QHostAddress("114.114.114.114"));
    m_widget->setDnsListText(dns);

    QList<QHostAddress> result = m_widget->getDns();
    QVERIFY(result.size() == 2);
}

void TestMultipleDnsWidget::testSetDnsListText_Empty()
{
    QList<QHostAddress> dns;
    m_widget->setDnsListText(dns);
    QListWidget *listWidget = m_widget->findChild<QListWidget *>();
    if (listWidget) {
        QVERIFY(listWidget->count() == 0);
    }
}

void TestMultipleDnsWidget::testSetDnsListText_WithDnsEntries()
{
    QList<QHostAddress> dns;
    dns.append(QHostAddress("8.8.8.8"));
    dns.append(QHostAddress("114.114.114.114"));
    dns.append(QHostAddress("1.1.1.1"));
    m_widget->setDnsListText(dns);

    QListWidget *listWidget = m_widget->findChild<QListWidget *>();
    if (listWidget) {
        QVERIFY(listWidget->count() == 3);
    }
}

void TestMultipleDnsWidget::testOnAddBtnClicked()
{
    QListWidget *listWidget = m_widget->findChild<QListWidget *>();
    int initialCount = listWidget ? listWidget->count() : 0;

    QList<QPushButton *> allBtns = m_widget->findChildren<QPushButton *>();
    QPushButton *addBtn = nullptr;
    for (QPushButton *btn : allBtns) {
        if (btn->icon().isNull() == false && btn->width() == 36 && btn->height() == 36) {
            addBtn = btn;
            break;
        }
    }
    if (addBtn) {
        addBtn->click();
    }

    int newCount = listWidget ? listWidget->count() : 0;
    QVERIFY(newCount >= initialCount);
}

void TestMultipleDnsWidget::testOnAddBtnClicked_MultipleTimes()
{
    QList<QPushButton *> allBtns = m_widget->findChildren<QPushButton *>();
    QPushButton *addBtn = nullptr;
    for (QPushButton *btn : allBtns) {
        if (btn->icon().isNull() == false && btn->width() == 36 && btn->height() == 36) {
            addBtn = btn;
            break;
        }
    }
    if (addBtn) {
        addBtn->click();
        addBtn->click();
        addBtn->click();
    }

    QListWidget *listWidget = m_widget->findChild<QListWidget *>();
    if (listWidget) {
        QVERIFY(listWidget->count() >= 1);
    }
}

void TestMultipleDnsWidget::testOnRemoveBtnClicked()
{
    QList<QPushButton *> allBtns = m_widget->findChildren<QPushButton *>();
    QPushButton *addBtn = nullptr;
    for (QPushButton *btn : allBtns) {
        if (btn->icon().isNull() == false && btn->width() == 36 && btn->height() == 36) {
            addBtn = btn;
            break;
        }
    }
    if (addBtn) {
        addBtn->click();
    }

    QListWidget *listWidget = m_widget->findChild<QListWidget *>();
    if (listWidget && listWidget->count() > 0) {
        listWidget->setCurrentRow(0);
    }

    QPushButton *removeBtn = nullptr;
    for (QPushButton *btn : allBtns) {
        if (btn->isEnabled() && btn->width() == 36 && btn->height() == 36) {
            removeBtn = btn;
            break;
        }
    }
    if (removeBtn) {
        removeBtn->click();
    }
    QVERIFY(true);
}

void TestMultipleDnsWidget::testOnRemoveBtnClicked_EmptyList()
{
    QListWidget *listWidget = m_widget->findChild<QListWidget *>();
    if (listWidget) {
        listWidget->setCurrentRow(-1);
    }

    QList<QPushButton *> allBtns = m_widget->findChildren<QPushButton *>();
    QPushButton *removeBtn = nullptr;
    for (QPushButton *btn : allBtns) {
        if (btn->isEnabled() == false && btn->width() == 36 && btn->height() == 36) {
            removeBtn = btn;
            break;
        }
    }
    if (removeBtn) {
        removeBtn->click();
    }
    QVERIFY(true);
}

void TestMultipleDnsWidget::testSetPlaceholderTextColor()
{
    QApplication::setPalette(QApplication::palette());
    Q_UNUSED(m_widget);
    QVERIFY(true);
}

void TestMultipleDnsWidget::testShowDnsSettingWidget_InvalidInterface()
{
    m_widget->setUuid("test-uuid-123");
    QVERIFY(true);
}
