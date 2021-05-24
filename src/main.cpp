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

#include "mainwindow.h"
#include "ksimplenm.h"
#include "kylin-network-interface.h"
#include "wireless-security/dlghidewifi.h"
#include "dbusadaptor.h"
#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <X11/Xlib.h>
#include <syslog.h>
#include <QFile>

#define LOG_IDENT "ukui_kylin_nm"

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray currentTime = QTime::currentTime().toString().toLocal8Bit();

    bool showDebug = true;
    QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/ukui/kylin-nm.log";
    //若不需要自动创建日志文件，请放开此注释
//    if (!QFile::exists(logFilePath)) {
//        showDebug = false;
//    }
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
        fprintf(log_file, "Debug: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(log_file? log_file: stdout, "Info: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(log_file? log_file: stderr, "Warning: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(log_file? log_file: stderr, "Critical: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(log_file? log_file: stderr, "Fatal: %s: %s (%s:%u, %s)\n", currentTime.constData(), localMsg.constData(), file, context.line, function);
        break;
    }

    if (log_file)
        fclose(log_file);
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);
    qInstallMessageHandler(messageOutput);

    openlog(LOG_IDENT, LOG_NDELAY | LOG_NOWAIT | LOG_PID, LOG_USER);
    qDebug()<<"Kylin Network Manager Is Already Launched";
    //syslog(LOG_DEBUG, "Kylin Network Manager Is Already Launched");

    int loopNum = 0;
    while (!QSystemTrayIcon::isSystemTrayAvailable()) {
        if (loopNum == 15) return 1;
        qDebug()<<"I couldn't detect any system tray on this system now";
        //syslog(LOG_DEBUG, "I couldn't detect any system tray on this system now");
        loopNum += 1;
        sleep(1);
    }
    QApplication::setQuitOnLastWindowClosed(false);

    // Internationalization
    QString locale = QLocale::system().name();
    QTranslator trans_global;
    if (locale == "zh_CN") {
        trans_global.load(":/translations/kylin-nm_zh_CN.qm");
        //trans_global.load(":/translations/kylin-nm_bo.qm");
        a.installTranslator(&trans_global);
    }
    if (locale == "tr_TR") {
        trans_global.load(":/translations/kylin-nm_tr.qm");
        a.installTranslator(&trans_global);
    }

    MainWindow w;

    DbusAdaptor adaptor(&w);
    Q_UNUSED(adaptor);
    auto connection = QDBusConnection::sessionBus();
    if (!connection.registerService("com.kylin.network") || !connection.registerObject("/com/kylin/network", &w)) {
        qCritical() << "QDbus register service failed reason:" << connection.lastError();
        QDBusInterface iface("com.kylin.network",
                                       "/com/kylin/network",
                                       "com.kylin.network",
                                       connection);
        iface.call("showMainWindow");

        return 0;
    }

    w.justShowTrayIcon();

    return a.exec();
}
