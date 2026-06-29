#include <gtest/gtest.h>
#include <QFile>
#include <QDir>
#include <QTemporaryDir>
#include "qt-locked-file.h"

using QtLP_Private::QtLockedFile;

class QtLockedFileTest : public ::testing::Test {
protected:
    QTemporaryDir tempDir;
    QString testFilePath;

    void SetUp() override {
        ASSERT_TRUE(tempDir.isValid());
        testFilePath = tempDir.filePath("test.lock");
    }

    void TearDown() override {
        QFile file(testFilePath);
        if (file.exists()) {
            file.remove();
        }
    }
};

TEST_F(QtLockedFileTest, DefaultConstructor) {
    QtLockedFile file;
    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);
    EXPECT_FALSE(file.isOpen());
}

TEST_F(QtLockedFileTest, ConstructorWithName) {
    QtLockedFile file(testFilePath);
    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);
    EXPECT_FALSE(file.isOpen());
}

TEST_F(QtLockedFileTest, OpenFileSuccess) {
    QtLockedFile file(testFilePath);
    EXPECT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    EXPECT_TRUE(file.isOpen());
    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);
    file.close();
}

TEST_F(QtLockedFileTest, OpenFileWithTruncateMode) {
    QtLockedFile file(testFilePath);
    EXPECT_FALSE(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    EXPECT_FALSE(file.isOpen());
}

TEST_F(QtLockedFileTest, OpenFileReadWrite) {
    QtLockedFile file(testFilePath);
    EXPECT_TRUE(file.open(QIODevice::ReadWrite | QIODevice::Text));
    EXPECT_TRUE(file.isOpen());
    file.close();
}

TEST_F(QtLockedFileTest, OpenFileAppendMode) {
    QtLockedFile file(testFilePath);
    EXPECT_TRUE(file.open(QIODevice::Append | QIODevice::Text));
    EXPECT_TRUE(file.isOpen());
    file.close();
}

TEST_F(QtLockedFileTest, LockWriteModeNonBlocking) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file.lock(QtLockedFile::WriteLock, false));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::WriteLock);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);

    file.close();
}

TEST_F(QtLockedFileTest, LockReadModeNonBlocking) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file.lock(QtLockedFile::ReadLock, false));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::ReadLock);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);

    file.close();
}

TEST_F(QtLockedFileTest, LockWriteModeBlocking) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file.lock(QtLockedFile::WriteLock, true));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::WriteLock);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());

    file.close();
}

TEST_F(QtLockedFileTest, LockReadModeBlocking) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file.lock(QtLockedFile::ReadLock, true));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::ReadLock);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());

    file.close();
}

TEST_F(QtLockedFileTest, LockNoLockMode) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file.lock(QtLockedFile::NoLock, false));
    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);

    file.close();
}

TEST_F(QtLockedFileTest, LockSameModeTwice) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file.lock(QtLockedFile::WriteLock, false));
    EXPECT_TRUE(file.isLocked());

    EXPECT_TRUE(file.lock(QtLockedFile::WriteLock, false));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::WriteLock);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());

    file.close();
}

TEST_F(QtLockedFileTest, LockDifferentMode) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file.lock(QtLockedFile::ReadLock, false));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::ReadLock);

    EXPECT_TRUE(file.lock(QtLockedFile::WriteLock, false));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::WriteLock);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());

    file.close();
}

TEST_F(QtLockedFileTest, UnlockWhenNotLocked) {
    QtLockedFile file(testFilePath);
    ASSERT_TRUE(file.open(QIODevice::ReadWrite));

    EXPECT_FALSE(file.isLocked());
    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());

    file.close();
}

TEST_F(QtLockedFileTest, UnlockNotOpenedFile) {
    QtLockedFile file(testFilePath);
    EXPECT_FALSE(file.isOpen());
    EXPECT_FALSE(file.unlock());
}

TEST_F(QtLockedFileTest, LockNotOpenedFile) {
    QtLockedFile file(testFilePath);
    EXPECT_FALSE(file.isOpen());
    EXPECT_FALSE(file.lock(QtLockedFile::WriteLock, false));
}

TEST_F(QtLockedFileTest, FullWorkflow) {
    QtLockedFile file(testFilePath);

    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);

    ASSERT_TRUE(file.open(QIODevice::ReadWrite | QIODevice::Text));
    EXPECT_TRUE(file.isOpen());

    QByteArray data = "Test data for locking";
    EXPECT_EQ(file.write(data), data.size());
    file.flush();

    EXPECT_TRUE(file.lock(QtLockedFile::WriteLock, true));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::WriteLock);

    file.seek(0);
    QByteArray readData = file.readAll();
    EXPECT_EQ(readData, data);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::NoLock);

    EXPECT_TRUE(file.lock(QtLockedFile::ReadLock, true));
    EXPECT_TRUE(file.isLocked());
    EXPECT_EQ(file.lockMode(), QtLockedFile::ReadLock);

    EXPECT_TRUE(file.unlock());
    EXPECT_FALSE(file.isLocked());

    file.close();
    EXPECT_FALSE(file.isOpen());
}

TEST_F(QtLockedFileTest, MultipleLocks) {
    QtLockedFile file1(testFilePath);
    QtLockedFile file2(testFilePath);

    ASSERT_TRUE(file1.open(QIODevice::ReadWrite));
    ASSERT_TRUE(file2.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file1.lock(QtLockedFile::WriteLock, false));
    EXPECT_TRUE(file1.isLocked());

    EXPECT_FALSE(file2.lock(QtLockedFile::WriteLock, false));
    EXPECT_FALSE(file2.isLocked());

    EXPECT_TRUE(file1.unlock());

    EXPECT_TRUE(file2.lock(QtLockedFile::WriteLock, false));
    EXPECT_TRUE(file2.isLocked());

    EXPECT_TRUE(file2.unlock());

    file1.close();
    file2.close();
}

TEST_F(QtLockedFileTest, ReadLockAllowsMultiple) {
    QtLockedFile file1(testFilePath);
    QtLockedFile file2(testFilePath);

    ASSERT_TRUE(file1.open(QIODevice::ReadWrite));
    ASSERT_TRUE(file2.open(QIODevice::ReadWrite));

    EXPECT_TRUE(file1.lock(QtLockedFile::ReadLock, false));
    EXPECT_TRUE(file1.isLocked());

    EXPECT_TRUE(file2.lock(QtLockedFile::ReadLock, false));
    EXPECT_TRUE(file2.isLocked());

    EXPECT_TRUE(file1.unlock());
    EXPECT_TRUE(file2.unlock());

    file1.close();
    file2.close();
}
