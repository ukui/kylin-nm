#include <QCoreApplication>
#include "test_main.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestMain tc;
    return QTest::qExec(&tc, argc, argv);
}
