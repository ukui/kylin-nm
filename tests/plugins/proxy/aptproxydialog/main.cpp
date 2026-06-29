#include <QCoreApplication>
#include <QDebug>
#include "test_aptproxydialog.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    TestAptProxyDialog test;
    return QTest::qExec(&test, argc, argv);
}
