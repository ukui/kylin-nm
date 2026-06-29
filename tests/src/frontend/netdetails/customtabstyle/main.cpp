#include <QCoreApplication>
#include <QDebug>
#include <QTest>

#include "test_customtabstyle.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestCustomTabStyle test;
    return QTest::qExec(&test, argc, argv);
}
