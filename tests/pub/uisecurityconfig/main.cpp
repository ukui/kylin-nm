#include <QCoreApplication>
#include "test_uisecurityconfig.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TestUiSecurityConfig tc;
    return QTest::qExec(&tc, argc, argv);
}
