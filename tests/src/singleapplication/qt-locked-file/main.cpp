#include <QCoreApplication>
#include "test_qt-locked-file.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(QCoreApplication::AA_Use96Dpi, true);
    TestQtLockedFile tc;
    return QTest::qExec(&tc, argc, argv);
}
