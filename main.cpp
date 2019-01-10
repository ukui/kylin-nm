#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "ksimplenm.h"
#include "kylin-network-interface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
