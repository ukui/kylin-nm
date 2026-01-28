/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef VPNDBUSADAPTOR_H
#define VPNDBUSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusMetaType>
#include <QStringList>
#include <QVector>
#include "singlepage.h"
#include "kylinnetworkdeviceresource.h"
#include "kyvpnconnectoperation.h"

QT_BEGIN_NAMESPACE
class QByteArray;
template<class Key, class Value> class QMap;
class QString;
class QVariant;
QT_END_NAMESPACE

#include "vpnobject.h"

class VpnDbusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.kylinvpn")
public:
    VpnDbusAdaptor(vpnObject *parent);

    inline vpnObject *parent() const
    { return static_cast<vpnObject *>(QObject::parent()); }

public Q_SLOTS:
    QVector<QStringList> getVirtualList();
    Q_NOREPLY void deleteVpn(QString uuid);
    Q_NOREPLY void activateVpn(const QString& connUuid);
    Q_NOREPLY void deactivateVpn(const QString& connUuid);
    Q_NOREPLY void showVpnAddWidget();
    Q_NOREPLY void showDetailPage(const QString& connUuid);

    void showKylinVpn();

Q_SIGNALS:
    void vpnAdd(QStringList info);
    void vpnRemove(QString dbusPath);
    void vpnUpdate(QStringList info);
    void vpnActiveConnectionStateChanged(QString uuid, int status);

    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
};

#endif
