/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
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

#ifndef VPNDBUSADAPTOR_H
#define VPNDBUSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusMetaType>

#include "singlepage.h"
#include "../dbus-interface/kylinnetworkdeviceresource.h"
QT_BEGIN_NAMESPACE
class QByteArray;
//template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
template<class T> class QVector;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.kylin.weather
 */

#include "vpnobject.h"

class VpnDbusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.vpnTool")
public:
    VpnDbusAdaptor(vpnObject *parent);
    virtual ~VpnDbusAdaptor();

    inline vpnObject *parent() const
    { return static_cast<vpnObject *>(QObject::parent()); }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    //虚拟连接列表
    QVector<QStringList> getVirtualList();
    //刪除 根据网络名称 参数1 0:lan 1:wlan 参数2 为ssid/uuid
    Q_NOREPLY void deleteVpn(QString uuid);
    //连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
    Q_NOREPLY void activateVpn(const QString& connUuid);
    //断开连接 根据网卡类型 参数1 0:lan 1:wlan 参数3 为ssid/uuid
    Q_NOREPLY void deactivateVpn(const QString& connUuid);
    //just show
    void showKylinVpn();

Q_SIGNALS: // SIGNALS
    void vpnAdd(QStringList info);
    void vpnRemove(QString dbusPath);
    void vpnUpdate(QStringList info);
    void vpnActiveConnectionStateChanged(QString uuid, int status);

    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
};

#endif
