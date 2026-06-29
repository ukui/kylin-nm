#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
