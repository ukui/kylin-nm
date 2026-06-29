#include <QCoreApplication>
#include <QDebug>
#include "test_tabpage.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    TestTabPage testTabPage;
    return QTest::qExec(&testTabPage, argc, argv);
}