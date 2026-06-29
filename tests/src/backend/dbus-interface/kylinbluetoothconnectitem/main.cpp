#include <QCoreApplication>
#include <QTest>
#include "test_kylinbluetoothconnectitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestKylinBluetoothConnectItem tc;
    return QTest::qExec(&tc, argc, argv);
}