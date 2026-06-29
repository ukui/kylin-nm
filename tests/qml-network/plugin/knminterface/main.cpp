#include <QCoreApplication>
#include <QDebug>
#include <QtTest>
#include "test_knminterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    TestKnmInterface test;
    return QTest::qExec(&test, argc, argv);
}
