#include <QCoreApplication>
#include <QDebug>
#include "test_qt-locked-file-unix.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(QCoreApplication::AA_Use96Dpi, true);
    
    TestQtLockedFileUnix tc;
    QTest::qExec(&tc, argc, argv);
    
    return 0;
}
