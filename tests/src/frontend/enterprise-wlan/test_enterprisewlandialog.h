#ifndef TEST_ENTERPRISEWLANDIALOG_H
#define TEST_ENTERPRISEWLANDIALOG_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include "enterprisewlandialog.h"

class TestEnterpriseWlanDialog : public QObject
{
    Q_OBJECT
public:
    explicit TestEnterpriseWlanDialog(QObject *parent = nullptr);
    ~TestEnterpriseWlanDialog() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_destructor();
    void test_enterpriseWlanDialogCloseSignal();

private:
    EnterpriseWlanDialog *m_dialog;
    KyWirelessNetItem m_wirelessNetItem;
    QString m_deviceName;
};

#endif // TEST_ENTERPRISEWLANDIALOG_H
