#ifndef TEST_CONNECTIVITYPAGE_H
#define TEST_CONNECTIVITYPAGE_H

#include <QTest>
#include <QObject>
#include <QSignalSpy>
#include <QDialog>

class TestConnectivityPage : public QObject
{
    Q_OBJECT
public:
    explicit TestConnectivityPage(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor_emptyUri();
    void test_constructor_withUri();
    void test_setWarning_Full();
    void test_setWarning_Limited();
    void test_setWarning_Portal();
    void test_setWarning_UnknownConnectivity();
    void test_setWarning_NoConnectivity();
    void test_checkUri_empty();
    void test_checkUri_validUrl();
    void test_checkUri_invalidUrl();
    void test_closeEvent();
};

#endif // TEST_CONNECTIVITYPAGE_H
