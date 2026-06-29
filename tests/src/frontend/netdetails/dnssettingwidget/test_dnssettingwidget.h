#ifndef TEST_DNSSETTINGWIDGET_H
#define TEST_DNSSETTINGWIDGET_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QString>

class TestDnsSettingWidget : public QObject
{
    Q_OBJECT
public:
    explicit TestDnsSettingWidget(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test constructor with different parameters
    void test_constructor_default();
    void test_constructor_with_parameters();
    void test_constructor_empty_strings();

    // Test getDnsSettings method
    void test_getDnsSettings_default();
    void test_getDnsSettings_with_values();

    // Test onPaletteChanged slot
    void test_onPaletteChanged();

    // Test UI elements creation
    void test_ui_elements_created();

    // Test button signals
    void test_cancel_button_clicked();
    void test_confirm_button_clicked();

    // Test combo box value changes
    void test_combo_box_value_changes();
};

#endif // TEST_DNSSETTINGWIDGET_H
