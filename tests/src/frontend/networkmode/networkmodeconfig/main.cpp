#include <QCoreApplication>
#include <QDebug>
#include "test_networkmodeconfig.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestNetworkModeConfig test;
    return QTest::qExec(&test, argc, argv);
}

