#include <QCoreApplication>
#include "test_kylable.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestKyLable tc;
    return QTest::qExec(&tc, argc, argv);
}
