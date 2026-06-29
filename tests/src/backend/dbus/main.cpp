#include <QCoreApplication>
#include <QDebug>
#include "test_dbus.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestDbus tc;
    return QTest::qExec(&tc, argc, argv);
}
