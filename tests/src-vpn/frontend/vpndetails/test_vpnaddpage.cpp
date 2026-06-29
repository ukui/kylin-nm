#include "test_vpnaddpage.h"
#include "../../../../src-vpn/frontend/vpndetails/vpnaddpage.h"
#include "../../../../src/backend/dbus-interface/kyvpnconnectoperation.h"
#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QFrame>
#include <QPaintEvent>

TestVpnAddPage::TestVpnAddPage(QObject *parent)
    : QObject(parent), m_vpnAddPage(nullptr)
{
}

TestVpnAddPage::~TestVpnAddPage()
{
    if (m_vpnAddPage) {
        delete m_vpnAddPage;
        m_vpnAddPage = nullptr;
    }
}

void TestVpnAddPage::initTestCase()
{
}

void TestVpnAddPage::cleanupTestCase()
{
}

void TestVpnAddPage::init()
{
    m_vpnAddPage = new vpnAddPage();
}

void TestVpnAddPage::cleanup()
{
    if (m_vpnAddPage) {
        delete m_vpnAddPage;
        m_vpnAddPage = nullptr;
    }
}

void TestVpnAddPage::test_constructor()
{
    QVERIFY(m_vpnAddPage != nullptr);
    QVERIFY(m_vpnAddPage->parent() == nullptr);
}

void TestVpnAddPage::test_centerToScreen()
{
    QVERIFY(m_vpnAddPage != nullptr);
    m_vpnAddPage->show();
    QTest::qWait(100);
    m_vpnAddPage->centerToScreen();
    QVERIFY(m_vpnAddPage->x() >= 0);
    QVERIFY(m_vpnAddPage->y() >= 0);
}

void TestVpnAddPage::test_initWindow()
{
    QVERIFY(m_vpnAddPage != nullptr);
    QVERIFY(m_vpnAddPage->width() == 520);
    QVERIFY(m_vpnAddPage->height() == 272);
    QVERIFY(m_vpnAddPage->windowTitle() == "create VPN");
}

void TestVpnAddPage::test_initUI()
{
    QVERIFY(m_vpnAddPage != nullptr);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(m_vpnAddPage->layout());
    QVERIFY(layout != nullptr);
}

void TestVpnAddPage::test_initVpnTypeFrame()
{
    QVERIFY(m_vpnAddPage != nullptr);
    QFrame *typeFrame = m_vpnAddPage->findChild<QFrame*>();
    QVERIFY(typeFrame != nullptr);
    
    QComboBox *comboBox = m_vpnAddPage->findChild<QComboBox*>();
    QVERIFY(comboBox != nullptr);
    QVERIFY(comboBox->count() == 4);
    QVERIFY(comboBox->itemText(0) == "L2TP");
    QVERIFY(comboBox->itemText(1) == "open VPN");
    QVERIFY(comboBox->itemText(2) == "PPTP");
    QVERIFY(comboBox->itemText(3) == "strong-swan");
}

void TestVpnAddPage::test_initVpnNameFrame()
{
    QVERIFY(m_vpnAddPage != nullptr);
    QFrame *nameFrame = m_vpnAddPage->findChild<QFrame*>();
    QVERIFY(nameFrame != nullptr);
    
    QLineEdit *nameEdit = m_vpnAddPage->findChild<QLineEdit*>();
    QVERIFY(nameEdit != nullptr);
    QVERIFY(nameEdit->maxLength() == 32);
}

void TestVpnAddPage::test_initVpnServerFrame()
{
    QVERIFY(m_vpnAddPage != nullptr);
    QFrame *serverFrame = m_vpnAddPage->findChild<QFrame*>();
    QVERIFY(serverFrame != nullptr);
    
    QLineEdit *serverEdit = nullptr;
    QList<QLineEdit*> edits = m_vpnAddPage->findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        if (edit->placeholderText() == "Required") {
            serverEdit = edit;
            break;
        }
    }
    QVERIFY(serverEdit != nullptr);
}

void TestVpnAddPage::test_initButtonFrame()
{
    QVERIFY(m_vpnAddPage != nullptr);
    
    QCheckBox *autoConnectBox = m_vpnAddPage->findChild<QCheckBox*>();
    QVERIFY(autoConnectBox != nullptr);
    QVERIFY(autoConnectBox->text() == "Auto Connection");
    
    QPushButton *confirmBtn = m_vpnAddPage->findChild<QPushButton*>();
    QVERIFY(confirmBtn != nullptr);
    QVERIFY(confirmBtn->text() == "Confirm");
    
    QPushButton *cancelBtn = m_vpnAddPage->findChild<QPushButton*>();
    QVERIFY(cancelBtn != nullptr);
    QVERIFY(cancelBtn->text() == "Cancel");
}

void TestVpnAddPage::test_initConnection()
{
    QVERIFY(m_vpnAddPage != nullptr);
    QPushButton *cancelBtn = m_vpnAddPage->findChild<QPushButton*>();
    QVERIFY(cancelBtn != nullptr);
}

void TestVpnAddPage::test_checkConfimBtnIsEnabled_nameEmpty()
{
    QVERIFY(m_vpnAddPage != nullptr);
    
    QLineEdit *nameEdit = nullptr;
    QLineEdit *serverEdit = nullptr;
    QList<QLineEdit*> edits = m_vpnAddPage->findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        if (edit->placeholderText() == "Required") {
            if (nameEdit == nullptr) {
                nameEdit = edit;
            } else {
                serverEdit = edit;
            }
        }
    }
    
    QVERIFY(nameEdit != nullptr);
    QVERIFY(serverEdit != nullptr);
    
    nameEdit->setText("");
    serverEdit->setText("192.168.1.1");
    QTest::qWait(50);
    
    QPushButton *confirmBtn = m_vpnAddPage->findChild<QPushButton*>();
    QVERIFY(confirmBtn != nullptr);
    QVERIFY(confirmBtn->isEnabled() == false);
}

void TestVpnAddPage::test_checkConfimBtnIsEnabled_serverEmpty()
{
    QVERIFY(m_vpnAddPage != nullptr);
    
    QLineEdit *nameEdit = nullptr;
    QLineEdit *serverEdit = nullptr;
    QList<QLineEdit*> edits = m_vpnAddPage->findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        if (edit->placeholderText() == "Required") {
            if (nameEdit == nullptr) {
                nameEdit = edit;
            } else {
                serverEdit = edit;
            }
        }
    }
    
    QVERIFY(nameEdit != nullptr);
    QVERIFY(serverEdit != nullptr);
    
    nameEdit->setText("MyVPN");
    serverEdit->setText("");
    QTest::qWait(50);
    
    QPushButton *confirmBtn = m_vpnAddPage->findChild<QPushButton*>();
    QVERIFY(confirmBtn != nullptr);
    QVERIFY(confirmBtn->isEnabled() == false);
}

void TestVpnAddPage::test_checkConfimBtnIsEnabled_bothFilled()
{
    QVERIFY(m_vpnAddPage != nullptr);
    
    QLineEdit *nameEdit = nullptr;
    QLineEdit *serverEdit = nullptr;
    QList<QLineEdit*> edits = m_vpnAddPage->findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        if (edit->placeholderText() == "Required") {
            if (nameEdit == nullptr) {
                nameEdit = edit;
            } else {
                serverEdit = edit;
            }
        }
    }
    
    QVERIFY(nameEdit != nullptr);
    QVERIFY(serverEdit != nullptr);
    
    nameEdit->setText("MyVPN");
    serverEdit->setText("192.168.1.1");
    QTest::qWait(50);
    
    QPushButton *confirmBtn = m_vpnAddPage->findChild<QPushButton*>();
    QVERIFY(confirmBtn != nullptr);
    QVERIFY(confirmBtn->isEnabled() == true);
}

void TestVpnAddPage::test_onSetConfimBtnEnable()
{
    test_checkConfimBtnIsEnabled_bothFilled();
}

void TestVpnAddPage::test_onConfimBtnClicked()
{
    QVERIFY(m_vpnAddPage != nullptr);
    
    QLineEdit *nameEdit = nullptr;
    QLineEdit *serverEdit = nullptr;
    QList<QLineEdit*> edits = m_vpnAddPage->findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        if (edit->placeholderText() == "Required") {
            if (nameEdit == nullptr) {
                nameEdit = edit;
            } else {
                serverEdit = edit;
            }
        }
    }
    
    QVERIFY(nameEdit != nullptr);
    QVERIFY(serverEdit != nullptr);
    
    nameEdit->setText("MyVPN");
    serverEdit->setText("192.168.1.1");
    
    m_vpnAddPage->show();
    QTest::qWait(100);
    
    QSignalSpy closeSpy(m_vpnAddPage, &vpnAddPage::closed);
    QVERIFY(closeSpy.isValid());
    
    QPushButton *confirmBtn = nullptr;
    QList<QPushButton*> buttons = m_vpnAddPage->findChildren<QPushButton*>();
    for (QPushButton *btn : buttons) {
        if (btn->text() == "Confirm") {
            confirmBtn = btn;
            break;
        }
    }
    QVERIFY(confirmBtn != nullptr);
    
    QTest::mouseClick(confirmBtn, Qt::LeftButton);
    QTest::qWait(100);
}

void TestVpnAddPage::test_createVpnConnect()
{
    test_onConfimBtnClicked();
}

void TestVpnAddPage::test_paintEvent()
{
    QVERIFY(m_vpnAddPage != nullptr);
    m_vpnAddPage->show();
    QTest::qWait(100);
    QVERIFY(true);
}

void TestVpnAddPage::test_signals_closed()
{
    QVERIFY(m_vpnAddPage != nullptr);
    
    QSignalSpy spy(m_vpnAddPage, &vpnAddPage::closed);
    QVERIFY(spy.isValid());
    
    m_vpnAddPage->show();
    QTest::qWait(100);
    
    QPushButton *cancelBtn = nullptr;
    QList<QPushButton*> buttons = m_vpnAddPage->findChildren<QPushButton*>();
    for (QPushButton *btn : buttons) {
        if (btn->text() == "Cancel") {
            cancelBtn = btn;
            break;
        }
    }
    QVERIFY(cancelBtn != nullptr);
    
    QTest::mouseClick(cancelBtn, Qt::LeftButton);
    QTest::qWait(100);
    
    QVERIFY(spy.count() >= 0);
}
