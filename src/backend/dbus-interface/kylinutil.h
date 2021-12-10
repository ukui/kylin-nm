#ifndef KYLINUTIL_H
#define KYLINUTIL_H

#include <QObject>
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QDBusReply>
#include <QDBusObjectPath>

#define  KEY_CONNECTION   "connection"
#define  KEY_CONNECT_TYPE "type"
#define  ETHERNET_TYPE    "802-3-ethernet"

QString getConnectTypeByDbus(QString &connectPath);

#endif // KYLINUTIL_H
