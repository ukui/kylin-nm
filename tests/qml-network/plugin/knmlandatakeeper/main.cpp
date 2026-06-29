#include <QCoreApplication>
#include <QDebug>
#include "test_knmlandatakeeper.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKnmLanDataKeeper test;
    return QTest::qExec(&test, argc, argv);
}