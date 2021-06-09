/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp com.kylin.weather.xml -a dbusadaptor -c DbusAdaptor -l MainWindow
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "dbusadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class DbusAdaptor
 */

DbusAdaptor::DbusAdaptor(MainWindow *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    qDBusRegisterMetaType<QVector<QStringList>>();
    setAutoRelaySignals(true);
    //setAutoRelaySignals(true)后会自动转发同名信号，因此不必再额外像如下代码一样写一个转发
//    connect(parent, &MainWindow::getWifiListFinished, this, [ = ]() {
//        QDBusMessage msg = QDBusMessage::createSignal("/com/kylin/network",  "com.kylin.network", "getWifiListFinished");
//        QDBusConnection::sessionBus().send(msg);
//    });
}

DbusAdaptor::~DbusAdaptor()
{
    // destructor
}

void DbusAdaptor::showMainWindow()
{
    // handle method call com.kylin.weather.showMainWindow
    //edited the code manually by lixiang
    //parent()->handleIconClicked();
    parent()->on_showWindowAction();
}

void DbusAdaptor::showPb(QString type, QString name)
{
    parent()->showPb(type, name);
}

void DbusAdaptor::requestRefreshWifiList()
{
    parent()->requestRefreshWifiList();
}

QVector<QStringList> DbusAdaptor::getWifiList()
{
    return parent()->dbus_wifiList;
}

