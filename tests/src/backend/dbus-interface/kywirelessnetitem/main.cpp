#include <QCoreApplication>
#include <QtTest>
#include "test_kywirelessnetitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKyWirelessNetItem tc;
    QTest::qExec(&tc, argc, argv);

    return 0;
}
