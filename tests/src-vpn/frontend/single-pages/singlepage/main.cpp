#include <QCoreApplication>
#include <QDebug>
#include "test_singlepage.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    TestSinglePage testSinglePage;
    return QTest::qExec(&testSinglePage, argc, argv);
}
