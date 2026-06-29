#include <QCoreApplication>
#include <QDebug>
#include "test_vpndbusadaptor.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestVpnDbusAdaptor tc;
    return QTest::qExec(&tc, argc, argv);
}
