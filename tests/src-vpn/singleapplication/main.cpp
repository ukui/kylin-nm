#include <QCoreApplication>
#include <QTest>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    return QTest::qExec(argc, argv);
}
