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
#include "wireless-security/dlgconnhidwifi.h"

#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include <QDebug>

#define LOG_IDENT "ukui_kylin_nm"


int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    openlog(LOG_IDENT, LOG_NDELAY | LOG_NOWAIT | LOG_PID, LOG_USER);

    syslog(LOG_DEBUG, "Kylin Network Manager Is Already Launched");

    // Internationalization
    QString locale = QLocale::system().name();
    QTranslator trans_global;
    if (locale == "zh_CN") {
        trans_global.load(":/translations/kylin-nm_zh_CN.qm");
        //trans_global.load(":/translations/kylin-nm_bo.qm");
        a.installTranslator(&trans_global);
    }

    MainWindow w;
    return a.exec();
}
