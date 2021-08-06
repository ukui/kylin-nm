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

#include "kylinwiredconnectoperation.h"

#include <NetworkManagerQt/AdslDevice>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/Ipv4Setting>
#include <NetworkManagerQt/Ipv6Setting>
#include <NetworkManagerQt/WiredSetting>

KyWiredConnectOperation::KyWiredConnectOperation()
{
}

KyWiredConnectOperation::~KyWiredConnectOperation()
{
}

void KyWiredConnectOperation::createWiredConnect(KyConnectSetting &connectSettingsInfo)
{
    qDebug()<<"create wired connect";
    connectSettingsInfo.dumpInfo();

    createConnect(connectSettingsInfo);

    return;
}

void KyWiredConnectOperation::updateWiredConnect(const QString &connectUuid, const KyConnectSetting &connectSettingsInfo)
{
    qDebug()<<"update connect"<<connectUuid;

    updateConnect(connectUuid, connectSettingsInfo);

    return ;
}

void KyWiredConnectOperation::deleteWiredConnect(const QString &connectUuid)
{
    qDebug()<<"delete wired connect uuid " << connectUuid;

    deleteConnect(connectUuid);

    return ;
}

void KyWiredConnectOperation::activateWiredConnection(const QString connectUuid)
{
    activateConnection(connectUuid);
    return ;
}

void KyWiredConnectOperation::deactivateWiredConnection(const QString activeConnectName, const QString &activeConnectUuid)
{
    qDebug()<<"deactivetate connect name"<<activeConnectName<<"uuid"<<activeConnectUuid;

    deactivateConnection(activeConnectName, activeConnectUuid);

    return;
}

void KyWiredConnectOperation::activateVpnConnection(const QString connectUuid)
{
    QString connectPath = "";
    QString deviceIdentifier = "";
    QString connectName = "";
    //QString deviceName = "";
    QString specificObject = "";
    NetworkManager::Connection::Ptr connectPtr = nullptr;

    qDebug()<<"it will activate vpn connect"<<connectUuid;
    connectPtr = NetworkManager::findConnectionByUuid(connectUuid);
    if (nullptr == connectPtr) {
        QString errorMessage = "the connect uuid " + connectUuid + "is not exsit";
        qWarning()<<errorMessage;
        Q_EMIT activateConnectionError(errorMessage);
        return;
    }

    if (NetworkManager::ConnectionSettings::Vpn != connectPtr->settings()->connectionType()) {
        QString errorMessage = tr("the connect type is")
                                + connectPtr->settings()->connectionType()
                                + tr(", but it is not vpn");
        qWarning()<<errorMessage;
        Q_EMIT activateConnectionError(errorMessage);
        return;
    }

    connectPath = connectPtr->path();
    connectName = connectPtr->name();
    //deviceName = connectPtr->settings()->interfaceName();
    specificObject = deviceIdentifier = QStringLiteral("/");

    qDebug() <<"active wired connect: path "<< connectPath
             << "device identify " << deviceIdentifier
             << "connect name " << connectName
            // << "device name" << deviceName
             << "specific parameter"<< specificObject;

    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::activateConnection(connectPath, deviceIdentifier, specificObject), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [this, connectName] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            QString errorMessage = tr("activate vpn connection failed: ") + watcher->error().message();
            qWarning()<<errorMessage;
            emit this->activateConnectionError(errorMessage);
         } else {
            qWarning()<<"active vpn connect complete.";
         }

         watcher->deleteLater();
    });

    return;
}
