#include <QApplication>
#include "test_dlghotspotcreate.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestDlgHotspotCreate test;
    return QTest::qExec(&test, argc, argv);
}
