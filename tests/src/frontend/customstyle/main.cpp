#include <QCoreApplication>
#include <QDebug>
#include "test_customstyle.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestCustomStyle test;
    return QTest::qExec(&test, argc, argv);
}
