#ifndef TEST_BLACKLISTPAGE_H
#define TEST_BLACKLISTPAGE_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QDBusInterface>

class BlacklistPage;

class TestBlacklistPage : public QObject
{
    Q_OBJECT
public:
    explicit TestBlacklistPage(QObject *parent = nullptr);
    ~TestBlacklistPage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test_constructor();
    void test_myLine();
    void test_getBlacklistDevice();
    void test_initBlacklistDev();
    void test_onsetStaIntoBlacklist();
    void test_addBlacklistDevFrame();
    void test_clearBlacklistLayout();
    void test_removeStaFromBlacklist();
    void test_resetLayoutHight();
    void test_refreshBlacklist();
    void test_onRemoveFromBlacklistBtnClicked();

private:
    BlacklistPage *m_blacklistPage = nullptr;
    QDBusInterface *m_mockInterface = nullptr;
};

#endif // TEST_BLACKLISTPAGE_H
