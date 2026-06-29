#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTimer>
#include "test_qt-local-peer.h"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    // Initialize Google Test
    testing::InitGoogleTest(&argc, argv);
    
    // Run all tests and then quit
    QTimer::singleShot(0, [&]() {
        QCoreApplication::exit(RUN_ALL_TESTS());
    });
    
    return app.exec();
}
