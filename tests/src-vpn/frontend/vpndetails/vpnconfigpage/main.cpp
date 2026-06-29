#include <QApplication>
#include <QDebug>
#include "test_vpnconfigpage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    TestVpnConfigPage testVpnConfigPage;
    return QTest::qExec(&testVpnConfigPage, argc, argv);
}
