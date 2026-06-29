#ifndef TEST_VPNADDPAGE_H
#define TEST_VPNADDPAGE_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

class vpnAddPage;

class TestVpnAddPage : public QObject
{
    Q_OBJECT
public:
    explicit TestVpnAddPage(QObject *parent = nullptr);
    ~TestVpnAddPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_centerToScreen();
    void test_initWindow();
    void test_initUI();
    void test_initVpnTypeFrame();
    void test_initVpnNameFrame();
    void test_initVpnServerFrame();
    void test_initButtonFrame();
    void test_initConnection();
    void test_checkConfimBtnIsEnabled_nameEmpty();
    void test_checkConfimBtnIsEnabled_serverEmpty();
    void test_checkConfimBtnIsEnabled_bothFilled();
    void test_onSetConfimBtnEnable();
    void test_onConfimBtnClicked();
    void test_createVpnConnect();
    void test_paintEvent();
    void test_signals_closed();

private:
    vpnAddPage *m_vpnAddPage;
};

#endif // TEST_VPNADDPAGE_H
