#include <QCoreApplication>
#include <QDebug>
#include <QtTest/QtTest>
#include "test_knmdbuscaller.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKnmDBusCaller tc;
    return QTest::qExec(&tc, argc, argv);
}
