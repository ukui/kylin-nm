#include <QApplication>
#include <QDebug>
#include <QTest>

#include "test_creatnetpage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestCreatNetPage test;
    return QTest::qExec(&test, argc, argv);
}
