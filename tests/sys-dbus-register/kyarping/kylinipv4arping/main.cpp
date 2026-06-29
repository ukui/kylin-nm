#include <QCoreApplication>
#include "test_kylinipv4arping.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    TestKyIpv4Arping test;
    return QTest::qExec(&test, argc, argv);
}
