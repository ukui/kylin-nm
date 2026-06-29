#include <QCoreApplication>
#include <QDebug>
#include "test_mobilehotspot.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QTest::qExec(new TestMobileHotspot, argc, argv);

    return 0;
}
