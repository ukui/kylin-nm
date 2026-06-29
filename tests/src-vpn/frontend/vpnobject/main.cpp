#include <QApplication>
#include "test_vpnobject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseOpenGLES);
    TestVpnObject tc;
    return QTest::qExec(&tc, argc, argv);
}
