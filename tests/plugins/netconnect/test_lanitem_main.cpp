#include <QApplication>
#include "test_lanitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestLanItem tc;
    return QTest::qExec(&tc, argc, argv);
}
