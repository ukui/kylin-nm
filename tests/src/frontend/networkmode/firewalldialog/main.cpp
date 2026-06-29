#include <QCoreApplication>
#include <QDebug>
#include "test_firewalldialog.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    TestFirewallDialog test;
    return QTest::qExec(&test, argc, argv);
}
