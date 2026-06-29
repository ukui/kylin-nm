#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QApplication>
#include "test_enterprisewlandialog.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication app(argc, argv);

    app.setAttribute(Qt::AA_EnableHighDpiScaling);

    QTest::qExec(new TestEnterpriseWlanDialog, argc, argv);

    return 0;
}