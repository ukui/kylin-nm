#ifndef TEST_FIREWALLDIALOG_H
#define TEST_FIREWALLDIALOG_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>

class FirewallDialog;

class TestFirewallDialog : public QObject
{
    Q_OBJECT
public:
    explicit TestFirewallDialog(QObject *parent = nullptr);
    ~TestFirewallDialog();

private:
    FirewallDialog *m_firewallDialog = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_firewallDialogConstructionAndDestruction();
    void test_setUuid();
    void test_centerToScreen();
    void test_onPaletteChanged();
    void test_closeMyself_matchingUuidAndStatus();
    void test_closeMyself_nonMatchingUuid();
    void test_closeMyself_nonMatchingStatus();
    void test_signals_setPublicNetMode();
    void test_signals_setPrivateNetMode();
};

#endif // TEST_FIREWALLDIALOG_H
