#include <QApplication>
#include "test_securitypage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestSecurityPage tc;
    return QTest::qExec(&tc, argc, argv);
}
