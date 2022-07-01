#include <QApplication>
#include <QDebug>
#include <QWidget>
#include "kylinagent.h"
#include <QTranslator>

bool isAppRuning();

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);

    KylinAgent* agent = new KylinAgent();
    agent->startKylinAgent();

    QTranslator trans_global;
    qDebug() << "QLocale " << QLocale();
    if (trans_global.load(QLocale(), "kylin-keyring", "_", "/usr/share/kylin-keyring/"))
    {
        a.installTranslator(&trans_global);
        qDebug()<<"Translations load success";
    } else {
        qWarning() << "Translations load fail";
    }

    return a.exec();
}
