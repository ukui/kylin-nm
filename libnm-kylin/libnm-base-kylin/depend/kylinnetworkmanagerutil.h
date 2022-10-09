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

#include <NetworkManagerQt/AccessPoint>
#include "kylinnetworkresourcetype.h"
#include "kywirelessconnectoperation.h"

#define  KEY_CONNECTION   "connection"
#define  KEY_CONNECT_TYPE "type"
#define  ETHERNET_TYPE    "802-3-ethernet"

QString getConnectTypeByDbus(QString connectPath);
QString getSsidFromByteArray(QByteArray &rawSsid);
QString enumToQstring(NetworkManager::AccessPoint::Capabilities cap, NetworkManager::AccessPoint::WpaFlags wpa_flags,NetworkManager::AccessPoint::WpaFlags rsn_flags);

void setWiredEnabledByGDbus(bool enabled);
void setDeviceManagedByGDbus(QString dbusPath, bool managed);
bool getWiredEnabledByGDbus();


#endif // KYLINUTIL_H
