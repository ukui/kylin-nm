#include <gtest/gtest.h>
#include <QCoreApplication>

// Rename main function to avoid conflict
#define main test_main_main

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
