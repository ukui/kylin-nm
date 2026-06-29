#include <QCoreApplication>
#include <QDebug>
#include "test_kylinipv4arping.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKyIpv4Arping test;
    return QTest::qExec(&test, argc, argv);
}
