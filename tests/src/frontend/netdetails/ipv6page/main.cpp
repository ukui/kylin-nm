#include <QApplication>
#include <QDebug>
#include <QTest>

#include "test_ipv6page.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestIpv6Page test;
    return QTest::qExec(&test, argc, argv);
}
