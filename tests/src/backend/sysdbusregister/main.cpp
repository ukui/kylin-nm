#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include "test_sysdbusregister.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QDir::setCurrent("/home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/tests/src/backend/sysdbusregister");

    TestSysdbusRegister test;
    return QTest::qExec(&test, argc, argv);
}
