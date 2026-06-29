/*
 * Kylin-NM Unit Test Main for kylin-network-manager-enhance-daemon
 */

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}