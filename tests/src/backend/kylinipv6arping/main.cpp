#include <QCoreApplication>
#include <QDebug>
#include <QTest>
#include "test_kylinipv6arping.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKyIpv6Arping tc;
    QTest::qExec(&tc, argc, argv);

    return 0;
}
