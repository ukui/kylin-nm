#ifndef TEST_NETDETAIL_H
#define TEST_NETDETAIL_H

#include <QObject>
#include <QString>
#include <QTest>
#include <QSignalSpy>
#include <QDebug>

class WarningDialog;
class NetTabBar;
class ThreadObject;

class TestNetDetail : public QObject
{
    Q_OBJECT
public:
    explicit TestNetDetail(QObject *parent = nullptr);
    ~TestNetDetail();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testWarningDialogConstructor();
    void testWarningDialogSetMessage();
    void testNetTabBarConstructor();
    void testThreadObjectConstructor();
    void testThreadObjectStop();

private:
    ThreadObject *m_threadObject;
};

#endif // TEST_NETDETAIL_H
