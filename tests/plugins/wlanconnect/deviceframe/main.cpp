#include <QApplication>
#include <QDebug>
#include "test_deviceframe.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    
    QTest::qExec(new TestDeviceFrame, argc, argv);
    
    return 0;
}
