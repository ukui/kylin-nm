#include <QApplication>
#include <QDebug>
#include <QTest>

#include "test_ipv4page.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestIpv4Page test;
    return QTest::qExec(&test, argc, argv);
}
