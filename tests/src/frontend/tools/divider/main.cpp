#include <QApplication>
#include <QDebug>
#include <QtTest>
#include "test_divider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestDivider tc;
    return QTest::qExec(&tc, argc, argv);
}
