#include <QCoreApplication>
#include "test_vpnadvancedpage.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    TestVpnAdvancedPage test;
    return QTest::qExec(&test, argc, argv);
}
