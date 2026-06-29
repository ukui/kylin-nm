#include <QApplication>
#include "test_wlanitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestWlanItem tc;
    return QTest::qExec(&tc, argc, argv);
}
