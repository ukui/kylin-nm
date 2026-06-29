#include <QCoreApplication>
#include <QDebug>
#include <QtTest/QtTest>
#include "test_knmdatakeeper.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKnmDataKeeper tc;
    return QTest::qExec(&tc, argc, argv);
}
