#include "test_qt-locked-file.h"
#include <QTest>
#include <QFile>
#include <QDir>

TestQtLockedFile::TestQtLockedFile()
{
}

void TestQtLockedFile::initTestCase()
{
    m_testFilePath = QDir::tempPath() + "/test_locked_file.txt";
    m_testFilePath2 = QDir::tempPath() + "/test_locked_file2.txt";
    
    QFile file(m_testFilePath);
    file.open(QIODevice::WriteOnly);
    file.write("test data");
    file.close();
    
    QFile file2(m_testFilePath2);
    file2.open(QIODevice::WriteOnly);
    file2.write("test data 2");
    file2.close();
}

void TestQtLockedFile::cleanupTestCase()
{
    QFile::remove(m_testFilePath);
    QFile::remove(m_testFilePath2);
}

void TestQtLockedFile::init()
{
}

void TestQtLockedFile::cleanup()
{
}

void TestQtLockedFile::testDefaultConstructor()
{
    QtLockedFile file;
    QVERIFY2(!file.isOpen(), "File should not be open initially");
    QVERIFY2(!file.isLocked(), "File should not be locked initially");
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock initially");
}

void TestQtLockedFile::testParameterizedConstructor()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(!file.isOpen(), "File should not be open after constructor");
    QVERIFY2(!file.isLocked(), "File should not be locked after constructor");
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock");
}

void TestQtLockedFile::testOpen()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadOnly), "File should open successfully");
    QVERIFY2(file.isOpen(), "File should be open");
    file.close();
}

void TestQtLockedFile::testOpenTruncateNotAllowed()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(!file.open(QIODevice::WriteOnly | QIODevice::Truncate), 
             "Truncate mode should not be allowed");
    QVERIFY2(!file.isOpen(), "File should not be open after failed open");
}

void TestQtLockedFile::testLockReadMode()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(file.lock(QtLockedFile::ReadLock), "Read lock should be acquired");
    QVERIFY2(file.isLocked(), "File should be locked");
    QVERIFY2(file.lockMode() == QtLockedFile::ReadLock, "Lock mode should be ReadLock");
    
    file.unlock();
    QVERIFY2(!file.isLocked(), "File should be unlocked");
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock");
    file.close();
}

void TestQtLockedFile::testLockWriteMode()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(file.lock(QtLockedFile::WriteLock), "Write lock should be acquired");
    QVERIFY2(file.isLocked(), "File should be locked");
    QVERIFY2(file.lockMode() == QtLockedFile::WriteLock, "Lock mode should be WriteLock");
    
    file.unlock();
    QVERIFY2(!file.isLocked(), "File should be unlocked");
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock");
    file.close();
}

void TestQtLockedFile::testLockNonBlocking()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(file.lock(QtLockedFile::WriteLock, false), "Non-blocking write lock should succeed");
    QVERIFY2(file.isLocked(), "File should be locked");
    
    file.unlock();
    file.close();
}

void TestQtLockedFile::testUnlock()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(file.lock(QtLockedFile::WriteLock), "Lock should be acquired");
    QVERIFY2(file.isLocked(), "File should be locked");
    
    QVERIFY2(file.unlock(), "Unlock should return true");
    QVERIFY2(!file.isLocked(), "File should be unlocked");
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock");
    
    file.close();
}

void TestQtLockedFile::testIsLocked()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(!file.isLocked(), "File should not be locked initially");
    
    file.lock(QtLockedFile::ReadLock);
    QVERIFY2(file.isLocked(), "File should be locked after lock()");
    
    file.unlock();
    QVERIFY2(!file.isLocked(), "File should not be locked after unlock()");
    
    file.close();
}

void TestQtLockedFile::testLockMode()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock initially");
    
    file.lock(QtLockedFile::ReadLock);
    QVERIFY2(file.lockMode() == QtLockedFile::ReadLock, "Lock mode should be ReadLock");
    
    file.unlock();
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock after unlock");
    
    file.lock(QtLockedFile::WriteLock);
    QVERIFY2(file.lockMode() == QtLockedFile::WriteLock, "Lock mode should be WriteLock");
    
    file.unlock();
    file.close();
}

void TestQtLockedFile::testLockTwiceSameMode()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(file.lock(QtLockedFile::WriteLock), "First lock should succeed");
    QVERIFY2(file.isLocked(), "File should be locked");
    
    QVERIFY2(file.lock(QtLockedFile::WriteLock), "Second lock with same mode should succeed");
    QVERIFY2(file.isLocked(), "File should still be locked");
    
    file.unlock();
    QVERIFY2(!file.isLocked(), "File should be unlocked");
    file.close();
}

void TestQtLockedFile::testLockDifferentMode()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    file.lock(QtLockedFile::ReadLock);
    QVERIFY2(file.lockMode() == QtLockedFile::ReadLock, "Lock mode should be ReadLock");
    
    file.lock(QtLockedFile::WriteLock);
    QVERIFY2(file.lockMode() == QtLockedFile::WriteLock, "Lock mode should change to WriteLock");
    
    file.unlock();
    file.close();
}

void TestQtLockedFile::testUnlockWithoutLock()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    QVERIFY2(!file.isLocked(), "File should not be locked initially");
    QVERIFY2(file.unlock(), "Unlock should return true even without lock");
    QVERIFY2(!file.isLocked(), "File should still not be locked");
    
    file.close();
}

void TestQtLockedFile::testOpenAfterLock()
{
    QtLockedFile file(m_testFilePath);
    QVERIFY2(file.open(QIODevice::ReadWrite), "File should open successfully");
    
    file.lock(QtLockedFile::WriteLock);
    QVERIFY2(file.isLocked(), "File should be locked");
    
    file.close();
    QVERIFY2(!file.isLocked(), "File should not be locked after close");
    QVERIFY2(file.lockMode() == QtLockedFile::NoLock, "Lock mode should be NoLock after close");
}
