/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 *  This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>

#include <syslog.h>
#include "kylin_netctrol_server_interface.h"

int main(int argc, char *argv[]){

    QCoreApplication app(argc, argv);
    openlog("sys-dbus-netctrl", LOG_PID | LOG_NDELAY, LOG_USER);
	
	QThread *threadNetCt = new QThread;
    NetCtrlAdaptor *netCtAdaptor=NetCtrlAdaptor::getNetContrlInstance();
    netCtAdaptor->moveToThread(threadNetCt);
    QObject::connect(threadNetCt, &QThread::started, netCtAdaptor, &NetCtrlAdaptor::initServer);
    threadNetCt->start();

    return app.exec();
}
