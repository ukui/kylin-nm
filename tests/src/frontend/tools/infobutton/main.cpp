#include <QCoreApplication>
#include <QDebug>
#include "test_infobutton.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QTest::qExec(new TestInfoButton, argc, argv);

    return 0;
}
