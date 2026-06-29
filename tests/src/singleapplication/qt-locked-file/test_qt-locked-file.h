#ifndef TEST_QT_LOCKED_FILE_H
#define TEST_QT_LOCKED_FILE_H

#include <QObject>
#include <QString>
#include <QtLockedFile>

class TestQtLockedFile : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testDefaultConstructor();
    void testParameterizedConstructor();
    void testOpen();
    void testOpenTruncateNotAllowed();
    void testLockReadMode();
    void testLockWriteMode();
    void testLockNonBlocking();
    void testUnlock();
    void testIsLocked();
    void testLockMode();
    void testLockTwiceSameMode();
    void testLockDifferentMode();
    void testUnlockWithoutLock();
    void testOpenAfterLock();

private:
    QString m_testFilePath;
    QString m_testFilePath2;
};

#endif // TEST_QT_LOCKED_FILE_H
