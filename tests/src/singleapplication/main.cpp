#include <QCoreApplication>
#include <QTest>
#include "test_qt-single-application.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TestQtSingleApplication tc;
    return QTest::qExec(&tc, argc, argv);
}
