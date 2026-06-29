#ifndef TEST_VPNADVANCEDPAGE_H
#define TEST_VPNADVANCEDPAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class VpnAdvancedPage;

class TestVpnAdvancedPage : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnAdvancedPage(QObject *parent = nullptr);
    ~TestVpnAdvancedPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();

private:
    VpnAdvancedPage *m_vpnAdvancedPage;
};

#endif // TEST_VPNADVANCEDPAGE_H
