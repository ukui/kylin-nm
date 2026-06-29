#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QStandardPaths>

int main(int argc, char *argv[]) {
    // Initialize environment before creating QCoreApplication
    qputenv("HOME", "/tmp");
    
    QCoreApplication app(argc, argv);
    app.setApplicationName("kylin-nm-tests");
    
    // Use a temporary directory for tests
    QString originalHome = QString::fromLocal8Bit(qgetenv("HOME"));
    qputenv("HOME", "/tmp");
    
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    
    // Restore original HOME
    qputenv("HOME", originalHome.toLocal8Bit());
    
    return result;
}