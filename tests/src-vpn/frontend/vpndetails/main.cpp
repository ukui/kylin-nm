#include <QCoreApplication>
#include <QDebug>
#include "test_vpnaddpage.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestVpnAddPage test;
    return QTest::qExec(&test, argc, argv);
}
