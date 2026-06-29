#ifndef TEST_QT_LOCKED_FILE_UNIX_H
#define TEST_QT_LOCKED_FILE_UNIX_H

#include <QObject>
#include <QString>
#include <QtLockedFile>

class TestQtLockedFileUnix : public QObject
{
    Q_OBJECT
public:
    explicit TestQtLockedFileUnix(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testLockFileNotOpen();
    void testUnlockFileNotOpen();
    void testLockWithNoLockMode();
    void testLockUpgradeFromReadToWrite();
    void testDestructorUnlocksFile();
    void testMultipleLockUnlockCycles();
    void testLockReturnsFalseOnFcntlError();

private:
    QString m_testFilePath;
    QString m_testFilePath2;
    QString m_testFilePath3;
};

#endif // TEST_QT_LOCKED_FILE_UNIX_H
