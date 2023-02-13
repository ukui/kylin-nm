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
#include "networkmodeconfig.h"
#include <QDebug>

NetworkModeConfig *NetworkModeConfig::m_netModeInstance = nullptr;

NetworkModeConfig *NetworkModeConfig::getInstance()
{
    if (m_netModeInstance == NULL) {
        m_netModeInstance = new NetworkModeConfig();
    }
    return m_netModeInstance;
}


NetworkModeConfig::NetworkModeConfig(QObject *parent) : QObject(parent)
{
    m_dbusInterface = new QDBusInterface("com.ksc.defender",
                       "/firewall",
                       "com.ksc.defender.firewall",
                       QDBusConnection::systemBus());
    if(!m_dbusInterface->isValid()) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }
}

int NetworkModeConfig::getNetworkModeConfig(QString uuid)
{
    if (uuid.isEmpty()) {
           qWarning()<< /*LOG_FLAG <<*/ "uuid is empty, so can not get network mode config";
           return -1;
       }

    if(m_dbusInterface == nullptr || !m_dbusInterface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return -1;
    }

    QDBusReply<int> reply = m_dbusInterface->call("get_networkModeConfig", uuid);
    if (reply.isValid()) {
        return reply.value();
    } else {
        qWarning() << "call get_networkModeConfig failed" << reply.error().message();
    }
    return -1;
}

void NetworkModeConfig::setNetworkModeConfig(QString uuid, QString cardName, QString ssid, int mode)
{
    if(m_dbusInterface == nullptr || !m_dbusInterface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return;
    }

    QDBusReply<int> reply = m_dbusInterface->call("set_networkModeConfig", uuid, cardName, ssid, mode);
    if (reply.isValid()) {
        qDebug() << "set_networkModeConfig" << ssid << uuid << cardName << mode << ",result" << reply.value();
    } else {
        qWarning() << "call set_networkModeConfig" << reply.error().message();
    }
}

int NetworkModeConfig::breakNetworkConnect(QString uuid, QString cardName, QString ssid)
{
    if(m_dbusInterface == nullptr || !m_dbusInterface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return -1;
    }

    QDBusReply<int> reply = m_dbusInterface->call("break_networkConnect", uuid, cardName, ssid);
    if (reply.isValid()) {
        qDebug() << "break_networkConnect" << ssid << uuid << cardName << ",result" << reply.value();
        return reply.value();
    } else {
        qWarning() << "call break_networkConnect failed" << reply.error().message();
        return -1;
    }
}
