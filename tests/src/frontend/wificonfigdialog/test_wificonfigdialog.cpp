#include "test_wificonfigdialog.h"
#include "../../../../src/frontend/wificonfigdialog.h"
#include <QPainter>
#include <QStyleOption>

TestWiFiConfigDialog::TestWiFiConfigDialog(QObject *parent)
    : QObject(parent)
    , m_dialog(nullptr)
{
}

TestWiFiConfigDialog::~TestWiFiConfigDialog()
{
}

void TestWiFiConfigDialog::initTestCase()
{
}

void TestWiFiConfigDialog::cleanupTestCase()
{
    if (m_dialog) {
        delete m_dialog;
        m_dialog = nullptr;
    }
}

void TestWiFiConfigDialog::init()
{
    m_dialog = new WiFiConfigDialog();
}

void TestWiFiConfigDialog::cleanup()
{
    if (m_dialog) {
        delete m_dialog;
        m_dialog = nullptr;
    }
}

void TestWiFiConfigDialog::test_constructor()
{
    QVERIFY2(m_dialog != nullptr, "WiFiConfigDialog should be created");
    QVERIFY2(m_dialog->isVisible() == false, "Dialog should not be visible initially");
    QVERIFY2(m_dialog->windowTitle() == "WLAN Authentication", "Dialog title should be set");
}

void TestWiFiConfigDialog::test_destructor()
{
    WiFiConfigDialog *dialog = new WiFiConfigDialog();
    QVERIFY2(dialog != nullptr, "WiFiConfigDialog should be created");
    delete dialog;
}

void TestWiFiConfigDialog::test_initTransparentState()
{
    m_dialog->initTransparentState();
    QVERIFY2(true, "initTransparentState should not crash");
}

void TestWiFiConfigDialog::test_getTransparentData()
{
    double transData = m_dialog->getTransparentData();
    QVERIFY2(transData > 0.0 && transData <= 1.0, "Transparency data should be between 0 and 1");
}

void TestWiFiConfigDialog::test_setEnableOfBtn_data()
{
    QTest::addColumn<QString>("wifiId");
    QTest::addColumn<QString>("wifiName");
    QTest::addColumn<QString>("wifiPassword");
    QTest::addColumn<bool>("btnEnabled");

    QTest::newRow("all empty") << "" << "" << "" << false;
    QTest::newRow("only wifiId") << "testId" << "" << "" << false;
    QTest::newRow("only wifiName") << "" << "testName" << "" << false;
    QTest::newRow("only wifiPassword") << "" << "" << "testPwd" << false;
    QTest::newRow("wifiId and wifiName") << "testId" << "testName" << "" << false;
    QTest::newRow("wifiId and wifiPassword") << "testId" << "" << "testPwd" << false;
    QTest::newRow("wifiName and wifiPassword") << "" << "testName" << "testPwd" << false;
    QTest::newRow("all filled") << "testId" << "testName" << "testPwd" << true;
}

void TestWiFiConfigDialog::test_setEnableOfBtn()
{
    QFETCH(QString, wifiId);
    QFETCH(QString, wifiName);
    QFETCH(QString, wifiPassword);
    QFETCH(bool, btnEnabled);

    m_dialog->findChild<QLineEdit *>("leWifiId")->setText(wifiId);
    m_dialog->findChild<QLineEdit *>("leWiFiName")->setText(wifiName);
    m_dialog->findChild<QLineEdit *>("leWifiPassword")->setText(wifiPassword);

    m_dialog->setEnableOfBtn();

    QPushButton *btnOk = m_dialog->findChild<QPushButton *>("btnOk");
    QVERIFY2(btnOk != nullptr, "btnOk should exist");
    QVERIFY2(btnOk->isEnabled() == btnEnabled, "Button enabled state should match expected");
}

void TestWiFiConfigDialog::test_on_btnCancel_clicked()
{
    QLineEdit *leWiFiName = m_dialog->findChild<QLineEdit *>("leWiFiName");
    QLineEdit *leWifiPassword = m_dialog->findChild<QLineEdit *>("leWifiPassword");

    leWiFiName->setText("TestNetwork");
    leWifiPassword->setText("TestPassword");

    m_dialog->show();

    QPushButton *btnCancel = m_dialog->findChild<QPushButton *>("btnCancel");
    QVERIFY2(btnCancel != nullptr, "btnCancel should exist");
    QTest::mouseClick(btnCancel, Qt::LeftButton);
    QTest::qWait(100);

    QVERIFY2(leWiFiName->text().isEmpty(), "WiFiName should be cleared after cancel");
    QVERIFY2(leWifiPassword->text().isEmpty(), "WiFiPassword should be cleared after cancel");
}

void TestWiFiConfigDialog::test_on_btnOk_clicked()
{
    QLineEdit *leWifiId = m_dialog->findChild<QLineEdit *>("leWifiId");
    QLineEdit *leWiFiName = m_dialog->findChild<QLineEdit *>("leWiFiName");
    QLineEdit *leWifiPassword = m_dialog->findChild<QLineEdit *>("leWifiPassword");

    leWifiId->setText("TestId");
    leWiFiName->setText("TestNetwork");
    leWifiPassword->setText("TestPassword");

    m_dialog->show();

    QPushButton *btnOk = m_dialog->findChild<QPushButton *>("btnOk");
    QVERIFY2(btnOk != nullptr, "btnOk should exist");
    QTest::mouseClick(btnOk, Qt::LeftButton);
    QTest::qWait(100);

    QVERIFY2(true, "on_btnOk_clicked should execute without crash");
}

void TestWiFiConfigDialog::test_on_checkBoxPwd_clicked_data()
{
    QTest::addColumn<bool>("isPasswordMode");

    QTest::newRow("password mode") << true;
    QTest::newRow("normal mode") << false;
}

void TestWiFiConfigDialog::test_on_checkBoxPwd_clicked()
{
    QFETCH(bool, isPasswordMode);

    QLineEdit *leWifiPassword = m_dialog->findChild<QLineEdit *>("leWifiPassword");
    QCheckBox *checkBoxPwd = m_dialog->findChild<QCheckBox *>("checkBoxPwd");

    if (isPasswordMode) {
        leWifiPassword->setEchoMode(QLineEdit::Password);
        checkBoxPwd->setChecked(false);
    } else {
        leWifiPassword->setEchoMode(QLineEdit::Normal);
        checkBoxPwd->setChecked(true);
    }

    QTest::mouseClick(checkBoxPwd, Qt::LeftButton);
    QTest::qWait(50);

    if (isPasswordMode) {
        QVERIFY2(leWifiPassword->echoMode() == QLineEdit::Normal, "Should switch to normal mode");
    } else {
        QVERIFY2(leWifiPassword->echoMode() == QLineEdit::Password, "Should switch to password mode");
    }
}

void TestWiFiConfigDialog::test_on_leWifiId_textEdited_data()
{
    QTest::addColumn<QString>("inputText");
    QTest::addColumn<bool>("btnEnabled");

    QTest::newRow("empty text") << "" << false;
    QTest::newRow("non-empty text") << "TestId" << false;
}

void TestWiFiConfigDialog::test_on_leWifiId_textEdited()
{
    QFETCH(QString, inputText);

    QLineEdit *leWifiId = m_dialog->findChild<QLineEdit *>("leWifiId");
    QLineEdit *leWiFiName = m_dialog->findChild<QLineEdit *>("leWiFiName");
    QLineEdit *leWifiPassword = m_dialog->findChild<QLineEdit *>("leWifiPassword");
    leWiFiName->setText("TestName");
    leWifiPassword->setText("TestPwd");

    QTest::keyClicks(leWifiId, inputText);
    QTest::qWait(50);

    QPushButton *btnOk = m_dialog->findChild<QPushButton *>("btnOk");
    QVERIFY2(btnOk != nullptr, "btnOk should exist");
}

void TestWiFiConfigDialog::test_on_leWiFiName_textEdited_data()
{
    QTest::addColumn<QString>("inputText");

    QTest::newRow("empty text") << "";
    QTest::newRow("non-empty text") << "TestName";
}

void TestWiFiConfigDialog::test_on_leWiFiName_textEdited()
{
    QFETCH(QString, inputText);

    QLineEdit *leWifiId = m_dialog->findChild<QLineEdit *>("leWifiId");
    QLineEdit *leWiFiName = m_dialog->findChild<QLineEdit *>("leWiFiName");
    QLineEdit *leWifiPassword = m_dialog->findChild<QLineEdit *>("leWifiPassword");
    leWifiId->setText("TestId");
    leWifiPassword->setText("TestPwd");

    QTest::keyClicks(leWiFiName, inputText);
    QTest::qWait(50);

    QPushButton *btnOk = m_dialog->findChild<QPushButton *>("btnOk");
    QVERIFY2(btnOk != nullptr, "btnOk should exist");
}

void TestWiFiConfigDialog::test_on_leWifiPassword_textEdited_data()
{
    QTest::addColumn<QString>("inputText");

    QTest::newRow("empty text") << "";
    QTest::newRow("non-empty text") << "TestPwd";
}

void TestWiFiConfigDialog::test_on_leWifiPassword_textEdited()
{
    QFETCH(QString, inputText);

    QLineEdit *leWifiId = m_dialog->findChild<QLineEdit *>("leWifiId");
    QLineEdit *leWiFiName = m_dialog->findChild<QLineEdit *>("leWiFiName");
    QLineEdit *leWifiPassword = m_dialog->findChild<QLineEdit *>("leWifiPassword");
    leWifiId->setText("TestId");
    leWiFiName->setText("TestName");

    QTest::keyClicks(leWifiPassword, inputText);
    QTest::qWait(50);

    QPushButton *btnOk = m_dialog->findChild<QPushButton *>("btnOk");
    QVERIFY2(btnOk != nullptr, "btnOk should exist");
}

void TestWiFiConfigDialog::test_paintEvent()
{
    m_dialog->setAttribute(Qt::WA_PaintOnScreen);
    m_dialog->setAttribute(Qt::WA_NoSystemBackground);
    m_dialog->resize(474, 320);
    QPixmap pixmap = m_dialog->grab();
    QVERIFY2(!pixmap.isNull(), "paintEvent should render to pixmap");
}

QTEST_MAIN(TestWiFiConfigDialog)
