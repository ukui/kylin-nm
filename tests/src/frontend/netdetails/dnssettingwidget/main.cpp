#include <QApplication>
#include "test_dnssettingwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    TestDnsSettingWidget test;
    return QTest::qExec(&test, argc, argv);
}
