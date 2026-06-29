#include <QCoreApplication>
#include "test_utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestUtils test;
    return QTest::qExec(&test, argc, argv);
}
