#include <QApplication>
#include <QDebug>
#include "test_listitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestListItem tc;
    return QTest::qExec(&tc, argc, argv);
}

