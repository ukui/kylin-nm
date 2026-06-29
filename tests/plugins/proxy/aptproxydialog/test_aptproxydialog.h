#ifndef TEST_APTPROXYDIALOG_H
#define TEST_APTPROXYDIALOG_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QHash>
#include <QVariant>

class TestAptProxyDialog : public QObject
{
    Q_OBJECT
public:
    explicit TestAptProxyDialog(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructor();
    void testInitUi();
    void testSetupComponentWithData();
    void testSetupComponentWithoutData();
    void testInitConnectSignals();
    void testConfirmButtonEnabledState();
    void testCancelButtonClosesDialog();

private:
    void prepareMockProxyData(bool hasData);
};

#endif // TEST_APTPROXYDIALOG_H
