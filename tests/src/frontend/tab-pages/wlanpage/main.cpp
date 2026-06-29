#include <QCoreApplication>
#include <QDebug>
#include "test_wlanpage.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestWlanPage test;
    return QTest::qExec(&test, argc, argv);
}
