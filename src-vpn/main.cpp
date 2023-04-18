/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

//#include "mainwindow.h"
#include "kylinvpndbus.h"
#include <QTranslator>
#include <QLocale>
#include "qt-single-application.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include <ukui-log4qt.h>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
#include "xatom-helper.h"
#endif

#include "vpnobject.h"

#define LOG_IDENT "kylin_vpn"

const QString QT_TRANSLATE_FILE = "/usr/share/qt5/translations/qt_zh_CN.qm";

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray currentDateTime = QDateTime::currentDateTime().toString().toLocal8Bit();

    bool showDebug = true;
    QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/ukui/kylin-nm.log";
    FILE *log_file = nullptr;

    if (showDebug) {
        log_file = fopen(logFilePath.toLocal8Bit().constData(), "a+");
    }

    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        if (!log_file) {
            break;
        }
        fprintf(log_file, "Debug: %s: %s (%s:%u, %s)\n", currentDateTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(log_file? log_file: stdout, "Info: %s: %s (%s:%u, %s)\n", currentDateTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(log_file? log_file: stderr, "Warning: %s: %s (%s:%u, %s)\n", currentDateTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(log_file? log_file: stderr, "Critical: %s: %s (%s:%u, %s)\n", currentDateTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(log_file? log_file: stderr, "Fatal: %s: %s (%s:%u, %s)\n", currentDateTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    }

    if (log_file)
        fclose(log_file);
}

int main(int argc, char *argv[])
{
    initUkuiLog4qt("kylin-vpn");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QString id = QString("kylin-vpn"+ QLatin1String(getenv("DISPLAY")));
    QtSingleApplication a(id, argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);

//    QThread thread;
//    KyNetworkResourceManager *p_networkResource = KyNetworkResourceManager::getInstance();
//    p_networkResource->moveToThread(&thread);
//    QObject::connect(&thread, SIGNAL(started()), p_networkResource, SLOT(onInitNetwork()));
//    thread.start();

    // Internationalization
    QString locale = QLocale::system().name();
    QTranslator trans_global;
    qDebug() << "QLocale " << QLocale();
    if (trans_global.load(QLocale(), "kylin-vpn", "_", ":/translations/"))
    {
        a.installTranslator(&trans_global);
        qDebug()<<"Translations load success";
    } else {
        qWarning() << "Translations load fail";
    }

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load(QLocale(), "qt", "_", "/usr/share/qt5/translations/"))
    {
        a.installTranslator(&qtBaseTranslator);
        qDebug()<<"QtBase Translations load success";
    } else {
        qWarning() << "QtBase Translations load fail";
    }

//    while (!p_networkResource->NetworkManagerIsInited()) {
//        ::usleep(1000);
//    }

    vpnObject vpnobject;
    a.setActivationWindow(&vpnobject);
    vpnobject.setProperty("useStyleWindowManager", false); //禁用拖动
    a.setWindowIcon(QIcon::fromTheme("ukui-vpn-symbolic"));

    auto connection = QDBusConnection::sessionBus();
    KylinVpnDbus dbusObject(&vpnobject);
    if (!connection.registerService("com.kylin.kylinvpntest")
            || !connection.registerObject("/com/kylin/kylinvpntest", &dbusObject,
                                          QDBusConnection::ExportAllSlots | QDBusConnection :: ExportAllSignals)) {
        qCritical() << "QDbus register service failed reason:" << connection.lastError();
    }

    return a.exec();
}
