#ifndef TEST_WIFICONFIGDIALOG_H
#define TEST_WIFICONFIGDIALOG_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class TestWiFiConfigDialog : public QObject
{
    Q_OBJECT
public:
    explicit TestWiFiConfigDialog(QObject *parent = nullptr);
    ~TestWiFiConfigDialog();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_initTransparentState();
    void test_getTransparentData();
    void test_setEnableOfBtn_data();
    void test_setEnableOfBtn();
    void test_on_btnCancel_clicked();
    void test_on_btnOk_clicked();
    void test_on_checkBoxPwd_clicked_data();
    void test_on_checkBoxPwd_clicked();
    void test_on_leWifiId_textEdited_data();
    void test_on_leWifiId_textEdited();
    void test_on_leWiFiName_textEdited_data();
    void test_on_leWiFiName_textEdited();
    void test_on_leWifiPassword_textEdited_data();
    void test_on_leWifiPassword_textEdited();
    void test_paintEvent();

private:
    class WiFiConfigDialog *m_dialog;
};

#endif // TEST_WIFICONFIGDIALOG_H
