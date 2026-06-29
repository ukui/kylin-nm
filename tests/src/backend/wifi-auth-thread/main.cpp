#include <QCoreApplication>
#include <QDebug>
#include <QTest>
#include "test_wifi-auth-thread.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    TestWifiAuthThread tc;
    QTest::qExec(&tc, argc, argv);
    
    return 0;
}
