#include "kylinutil.h"

#define  LOG_FLAG  "kylin-util"

QString getConnectTypeByDbus(QString &connectPath)
{
    QString connectType = "";

    if (connectPath.isEmpty()) {
        qWarning()<< LOG_FLAG << "connect path is empty, so can not get connect type";
        return connectType;
    }

    QDBusInterface dbusInterface("org.freedesktop.NetworkManager",
                              connectPath,
                              "org.freedesktop.NetworkManager.Settings.Connection",
                              QDBusConnection::systemBus());

    QDBusMessage result = dbusInterface.call("GetSettings");
    const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
    QMap<QString, QMap<QString, QVariant>> map;
    dbusArg1st >> map;

    if (map.isEmpty()) {
        qWarning() << LOG_FLAG <<"get connection settings failed.";
        return connectType;
    }

    QMap<QString,QVariant> connectMap = map.value(KEY_CONNECTION);
    if (connectMap.isEmpty()) {
        qWarning() << LOG_FLAG <<"threre is not connection settings";
        return connectType;
    }

    connectType = connectMap.value(KEY_CONNECT_TYPE).toString();

    qDebug() << LOG_FLAG << "connection type" << connectType;

    return connectType;

}
