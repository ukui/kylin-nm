#include <QCoreApplication>
#include <QDebug>

#include "test_kylin_netctrol_server_interface.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKylinNetCtrlServerInterface tc;
    return QTest::qExec(&tc, argc, argv);
}

