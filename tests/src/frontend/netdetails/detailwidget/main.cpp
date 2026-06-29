#include <QCoreApplication>
#include <QDebug>
#include "test_detailwidget.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    TestDetailWidget testWidget;
    return QTest::qExec(&testWidget, argc, argv);
}
