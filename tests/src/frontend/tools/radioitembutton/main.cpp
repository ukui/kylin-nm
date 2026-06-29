#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>
#include "test_radioitembutton.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication app(argc, argv);

    app.setAttribute(Qt::AA_EnableHighDpiScaling);

    QTest::qExec(new TestRadioItemButton, argc, argv);

    return 0;
}
