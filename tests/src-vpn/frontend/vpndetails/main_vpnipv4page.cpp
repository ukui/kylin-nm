#include <QCoreApplication>
#include <QDebug>
#include "test_vpnipv4page.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestVpnIpv4Page test;
    return QTest::qExec(&test, argc, argv);
}
