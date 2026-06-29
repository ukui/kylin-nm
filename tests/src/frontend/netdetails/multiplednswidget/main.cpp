#include <QApplication>
#include <QDebug>
#include <QTest>

#include "test_multiplednswidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestMultipleDnsWidget test;
    return QTest::qExec(&test, argc, argv);
}
