/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
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
#include <QSettings>
#include <QTextCodec>
#include <QDir>
#include <dbus-1.0/dbus/dbus.h>
#include <glib-2.0/glib.h>
#include <dbus-1.0/dbus/dbus-glib.h>
#include <gio/gio.h>
#include "kylinnetworkmanagerutil.h"
#include "kylinnetworkdeviceresource.h"
#define  LOG_FLAG  "[kylin-util]"
const QString CONFIG_FILE_PATH   =  QDir::homePath() + "/.config/ukui/kylin-nm.conf";

QString getConnectTypeByDbus(QString connectPath)
{
    QString connectType = "";
    if (connectPath.isEmpty()) {
        qWarning() << LOG_FLAG << "connect path is empty, so can not get connect type";
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
    return connectType;
}

QString getSsidFromByteArray(QByteArray &rawSsid)
{
    QString wifiSsid = "";

    if (rawSsid.isEmpty()) {
        qWarning() << LOG_FLAG << "wifi raw ssid is empty";
        return wifiSsid;
    }

    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    codec->toUnicode( rawSsid.constData(), rawSsid.size(), &state);
    if (state.invalidChars > 0) {
        wifiSsid = QTextCodec::codecForName("GBK")->toUnicode(rawSsid);
    } else {
        wifiSsid = rawSsid;
    }

    return wifiSsid;
}

QString enumToQstring(NetworkManager::AccessPoint::Capabilities apCapability,
                      NetworkManager::AccessPoint::WpaFlags wpaFlags,
                      NetworkManager::AccessPoint::WpaFlags rsnFlags)
{
    QString out;

    if ((apCapability & AP_CAPABILITY_PRIVACY)
           && (wpaFlags == AP_SECURITY_NONE)
           && (rsnFlags == AP_SECURITY_NONE)) {
        out += "WEP ";
    }

    if (wpaFlags != AP_SECURITY_NONE) {
        out += "WPA1 ";
    }

    if ((rsnFlags & AP_SECURITY_KEY_MGMT_PSK)
            || (rsnFlags & AP_SECURITY_KEY_MGMT_802_1X)) {
        out += "WPA2 ";
    }

    if (rsnFlags & AP_SECURITY_KEY_MGMT_SAE) {
        out += "WPA3 ";
    }

    if ((wpaFlags & AP_SECURITY_KEY_MGMT_802_1X)
           || (rsnFlags & AP_SECURITY_KEY_MGMT_802_1X)) {
        out += "802.1X ";
    }

    return out;
}

void setWiredEnabledByGDbus(bool enabled)
{
    GDBusProxy *props_proxy;
    GVariant *ret = NULL;
    GError *error = NULL;

    /* Create a D-Bus object proxy for the active connection object's properties */
    props_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                 G_DBUS_PROXY_FLAGS_NONE,
                                                 NULL,
                                                 "org.freedesktop.NetworkManager",
                                                 "/org/freedesktop/NetworkManager",
                                                 "org.freedesktop.DBus.Properties",
                                                 NULL, NULL);
    g_assert (props_proxy);

    /* Get the object path of the Connection details */
    ret = g_dbus_proxy_call_sync (props_proxy,
                                  "Set",
                                  g_variant_new ("(ssv)",
                                                 "org.freedesktop.NetworkManager",
                                                 "WiredEnabled",
                                                 g_variant_new_boolean(enabled)),
                                  G_DBUS_CALL_FLAGS_NONE, -1,
                                  NULL, &error);
    if (!ret) {
        g_dbus_error_strip_remote_error (error);
        qDebug() << "failed to setWiredEnabledByGDbus";
        g_error_free (error);
    }

out:
    if (ret)
        g_variant_unref (ret);
    g_object_unref (props_proxy);
}

bool getWiredEnabledByGDbus()
{
    GDBusProxy *props_proxy;
    GVariant *ret = NULL, *path_value = NULL;
    GError *error = NULL;
    gboolean bRet = false;

    /* Create a D-Bus object proxy for the active connection object's properties */
    props_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                 G_DBUS_PROXY_FLAGS_NONE,
                                                 NULL,
                                                 "org.freedesktop.NetworkManager",
                                                 "/org/freedesktop/NetworkManager",
                                                 "org.freedesktop.DBus.Properties",
                                                 NULL, NULL);
    g_assert (props_proxy);

    /* Get the object path of the Connection details */
    ret = g_dbus_proxy_call_sync (props_proxy,
                                  "Get",
                                  g_variant_new ("(ss)",
                                                 "org.freedesktop.NetworkManager",
                                                 "WiredEnabled"),
                                  G_DBUS_CALL_FLAGS_NONE, -1,
                                  NULL, &error);
    if (!ret) {
        g_dbus_error_strip_remote_error (error);
        qDebug() << "failed to setWiredEnabledByGDbus";
        g_error_free (error);
    }

    g_variant_get (ret, "(v)", &path_value);
    if (!g_variant_is_of_type (path_value, G_VARIANT_TYPE_BOOLEAN)) {
        g_warning ("Unexpected type returned getting Connection property: %s",
                   g_variant_get_type_string (path_value));
        goto out;
    }

    bRet = g_variant_get_boolean (path_value);

out:
    if (path_value)
        g_variant_unref (path_value);
    if (ret)
        g_variant_unref (ret);
    g_object_unref (props_proxy);

    return bRet;
}


void setDeviceManagedByGDbus(QString dbusPath, bool managed)
{
    GDBusProxy *props_proxy;
    GVariant *ret = NULL;
    GError *error = NULL;

    /* Create a D-Bus object proxy for the active connection object's properties */
    props_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                 G_DBUS_PROXY_FLAGS_NONE,
                                                 NULL,
                                                 "org.freedesktop.NetworkManager",
                                                 dbusPath.toStdString().c_str(),
                                                 "org.freedesktop.DBus.Properties",
                                                 NULL, NULL);
    g_assert (props_proxy);

    /* Get the object path of the Connection details */
    ret = g_dbus_proxy_call_sync (props_proxy,
                                  "Set",
                                  g_variant_new ("(ssv)",
                                                 "org.freedesktop.NetworkManager.Device",
                                                 "Managed",
                                                 g_variant_new_boolean(managed)),
                                  G_DBUS_CALL_FLAGS_NONE, -1,
                                  NULL, &error);
    if (!ret) {
        g_dbus_error_strip_remote_error (error);
        qDebug() << "failed to setWiredEnabledByGDbus";
        g_error_free (error);
    }

out:
    if (ret)
        g_variant_unref (ret);
    g_object_unref (props_proxy);
}
