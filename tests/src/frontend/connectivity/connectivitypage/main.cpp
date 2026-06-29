#include <QApplication>
#include <QDebug>

#include "test_connectivitypage.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestConnectivityPage tc;
    return QTest::qExec(&tc, argc, argv);
}
