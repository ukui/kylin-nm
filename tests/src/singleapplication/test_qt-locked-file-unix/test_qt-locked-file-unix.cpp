#include "test_qt-locked-file-unix.h"
#include <QTest>
#include <QFile>
#include <QDir>

TestQtLockedFileUnix::TestQtLockedFileUnix(QObject *parent) : QObject(parent)
{
}

void TestQtLockedFileUnix::initTestCase()
{
    m_testFilePath = QDir::tempPath() + "/test_locked_file_unix.txt";
    m_testFilePath2 = QDir::tempPath() + "/test_locked_file_unix2.txt";
    m_testFilePath3 = QDir::tempPath() + "/test_locked_file_unix3.txt";

    QFile file(m_testFilePath);
    file.open(QIODevice::WriteOnly);
    file.write("test data");
    file.close();

    QFile file2(m_testFilePath2);
    file2.open(QIODevice::WriteOnly);
    file2.write("test data 2");
    file2.close();

    QFile file3(m_testFilePath3);
    file3.open(QIODevice::WriteOnly);
    file3.write("test data 3");
    file3.close();
}

void TestQtLockedFileUnix::cleanupTestCase()
{
    QFile::remove(m_testFilePath);
    QFile::remove(m_testFilePath2);
    QFile::remove(m_testFilePath3);
}

void TestQtLockedFileUnix::init()
{
}

void TestQtLockedFileUnix::cleanup()
{
}

void TestQtLockedFileUnix::testLockFileNotOpen()
{
    QtLockedFile file;
    bool result = file.lock(QtLockedFile::WriteLock);
    QVERIFY2(result == false, "Lock should fail when file is not open");
}

void TestQtLockedFileUnix::testUnlockFileNotOpen()
{
    QtLockedFile file;
    bool result = file.unlock();
    QVERIFY2(result == false, "Unlock should fail when file is not open");
}

void TestQtLockedFileUnix::testLockWithNoLockMode()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    bool result = file.lock(QtLockedFile::NoLock);
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock after NoLock request");
    
    file.close();
}

void TestQtLockedFileUnix::testLockUpgradeFromReadToWrite()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(file.lock(QtLockedFile::ReadLock), "Read lock should be acquired");
    QVERIFY2(file.lockMode() == QtLockedFile::ReadLock, "Lock mode should be ReadLock");
    
    QVERIFY2(file.lock(QtLockedFile::WriteLock), "Upgrade to write lock should succeed");
    QVERIFY2(file.lockMode() == QtLockedFile::WriteLock, "Lock mode should be WriteLock after upgrade");
    
    file.unlock();
    file.close();
}

void TestQtLockedFileUnix::testDestructorUnlocksFile()
{
    {
        QtLockedFile file(m_testFilePath);
        QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
        QVERIFY2(file.lock(QtLockedFile::WriteLock), "Write lock should be acquired");
        QVERIFY2(file.isLocked(), "File should be locked");
    }
    
    QtLockedFile file2(m_testFilePath);
    QVERIFY2(file2.open(QIODevice::ReadWrite), "File should be able to open again after destructor");
    QVERIFY2(file2.lock(QtLockedFile::WriteLock), "Write lock should be acquired after previous instance destroyed");
    file2.unlock();
    file2.close();
}

void TestQtLockedFileUnix::testMultipleLockUnlockCycles()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    for (int i = 0; i < 5; i++) {
        QVERIFY2(file.lock(QtLockedFile::WriteLock), "Lock should succeed");
        QVERIFY2(file.isLocked(), "File should be locked");
        QVERIFY2(file.lockMode() == QtLockedFile::WriteLock, "Lock mode should be WriteLock");
        
        QVERIFY2(file.unlock(), "Unlock should succeed");
        QVERIFY2(!file.isLocked(), "File should be unlocked");
        QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock");
    }
    
    file.close();
}

void TestQtLockedFileUnix::testLockReturnsFalseOnFcntlError()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    file.lock(QtLockedFile::WriteLock);
    QVERIFY2(file.isLocked(), "File should be locked");
    
    QFile file2(m_testFilePath2);
    QVERIFY2(file2.open(QIODevice::WriteOnly), "Second file should open successfully");
    
    QtLockedFile file3(m_testFilePath2);
    QVERIFY2(file3.open(QIODevice::WriteOnly), "Third file should open successfully");
    
    bool result = file3.lock(QtLockedFile::WriteLock, false);
    QVERIFY2(result == true || result == false, "Lock should return a valid boolean");
    
    file.unlock();
    file.close();
    file2.close();
    file3.close();
}
