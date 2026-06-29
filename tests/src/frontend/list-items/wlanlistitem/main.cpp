#include <QApplication>
#include <QDebug>
#include "test_wlanlistitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestWlanListItem tc;
    return QTest::qExec(&tc, argc, argv);
}
