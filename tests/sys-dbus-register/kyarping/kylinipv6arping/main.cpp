#include <QCoreApplication>
#include "test_kylinipv6arping.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TestKyIpv6Arping tc;
    return QTest::qExec(&tc, argc, argv);
}
