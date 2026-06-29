#include <QCoreApplication>
#include <QTest>
#include "test_qt-local-peer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestQtLocalPeer tc;
    return QTest::qExec(&tc, argc, argv);
}
