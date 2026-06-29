#include <QCoreApplication>
#include <QDebug>
#include <QtTest>
#include "test_networkqml_plugin.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestNetworkqmlPlugin test;
    return QTest::qExec(&test, argc, argv);
}
