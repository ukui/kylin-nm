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
#include "firewalldialog.h"
#include <QDebug>

#define LOG_FLAG  "[NetworkModeConfig]"

NetworkModeConfig::NetworkModeConfig(QObject *parent)
    :QObject(parent)
{
    qRegisterMetaType<KyConnectState>("KyConnectState");

    m_thread = new QThread;
    m_manager = new KyNetworkManager();
    m_manager->moveToThread(m_thread);
    connect(m_thread, &QThread::started, m_manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(m_thread, &QThread::finished, m_manager, &KyNetworkManager::deleteLater);
    m_thread->start();
    while (!m_manager->isInitFinished()) {
        ::usleep(1000);
    }

    //init dbus
    m_firewallDbusIface = new QDBusInterface("com.ksc.defender",
                                             "/firewall",
                                             "com.ksc.defender.firewall",
                                             QDBusConnection::systemBus());
    if(!m_firewallDbusIface->isValid()) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

    //connect
    connect(m_manager, &KyNetworkManager::wiredStateChange, this, [=](QString deviceName, QString uuid, KyConnectState status) {
        KyConnectSetting connectSetting;
        m_manager->getConnectIpInfo(uuid, connectSetting);
        onActiveConnectionChanged(deviceName, connectSetting.m_connectName, uuid, status);
    });
    connect(m_manager, &KyNetworkManager::wirelessStateChange, this, &NetworkModeConfig::onActiveConnectionChanged);
}

void NetworkModeConfig::initWiredNetworkMode()
{
    qDebug()<< LOG_FLAG << "initWiredNetworkMode";
    QStringList wiredDevList;
    m_manager->getNetworkDeviceList(DEVICE_TYPE_ETHERNET, wiredDevList);
    if (wiredDevList.isEmpty()) {
        return;
    }

    for (auto devName : wiredDevList) {
        QList<KyActivateItem> activedList;
        m_manager->getActiveConnectionList(devName, CONNECT_TYPE_WIRED, activedList);
        if (activedList.isEmpty()) {
            continue;
        }

        int configType = getNetworkModeConfig(activedList.at(0).m_uuid);
        if (configType == DBUS_INVAILD) {
            return;
        } else if (configType == NO_CONFIG) {
            //已连接网络无配置 默认公有配置
            setNetworkModeConfig(activedList.at(0).m_uuid, devName,
                                 activedList.at(0).m_connName, KSC_FIREWALL_PUBLIC);
        } else {
            setNetworkModeConfig(activedList.at(0).m_uuid, devName,
                                 activedList.at(0).m_connName, configType);
        }
    }
}

void NetworkModeConfig::initWirelessNetworkMode()
{
    qDebug()<< LOG_FLAG << "initWirelessNetworkMode";
    QStringList wirelessDevList;
    m_manager->getNetworkDeviceList(DEVICE_TYPE_WIFI, wirelessDevList);
    if (wirelessDevList.isEmpty()) {
        return;
    }

    for (auto devName : wirelessDevList) {
        QList<KyActivateItem> activedList;
        m_manager->getActiveConnectionList(devName, CONNECT_TYPE_WIRELESS, activedList);
        if (activedList.isEmpty()) {
            return;
        }

        int configType = getNetworkModeConfig(activedList.at(0).m_uuid);
        if (configType == DBUS_INVAILD) {
            return;
        } else if (configType == NO_CONFIG) {
            //已连接网络无配置 默认公有配置
            setNetworkModeConfig(activedList.at(0).m_uuid, devName,
                                 activedList.at(0).m_ssid, KSC_FIREWALL_PUBLIC);
        } else {
            setNetworkModeConfig(activedList.at(0).m_uuid, devName,
                                 activedList.at(0).m_ssid, configType);
        }
    }
}

int NetworkModeConfig::getNetworkModeConfig(QString uuid)
{
    if (uuid.isEmpty()) {
        qWarning()<< /*LOG_FLAG <<*/ "uuid is empty, so can not get network mode config";
        return NO_CONFIG;
    }

    if(m_firewallDbusIface == nullptr || !m_firewallDbusIface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return DBUS_INVAILD;
    }

    QDBusReply<int> reply = m_firewallDbusIface->call("get_networkModeConfig", uuid);
    if (reply.isValid()) {
        return reply.value();
    } else {
        qWarning() << "call get_networkModeConfig failed" << reply.error().message();
    }
    return NO_CONFIG;
}

void NetworkModeConfig::setNetworkModeConfig(QString uuid, QString cardName, QString ssid, int mode)
{
    if(m_firewallDbusIface == nullptr || !m_firewallDbusIface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return;
    }

    QDBusReply<int> reply = m_firewallDbusIface->call("set_networkModeConfig", uuid, cardName, ssid, mode);
    if (reply.isValid()) {
        qDebug() << "set_networkModeConfig" << ssid << uuid << cardName << mode << ",result" << reply.value();
    } else {
        qWarning() << "call set_networkModeConfig" << reply.error().message();
    }
}

int NetworkModeConfig::breakNetworkConnect(QString uuid, QString cardName, QString ssid)
{
    if(m_firewallDbusIface == nullptr || !m_firewallDbusIface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return -1;
    }

    QDBusReply<int> reply = m_firewallDbusIface->call("break_networkConnect", uuid, cardName, ssid);
    if (reply.isValid()) {
        qDebug() << "break_networkConnect" << ssid << uuid << cardName << ",result" << reply.value();
        return reply.value();
    } else {
        qWarning() << "call break_networkConnect failed" << reply.error().message();
        return -1;
    }
}

void NetworkModeConfig::setFirstConnectNetworkMode(QString uuid, QString deviceName, QString ssid)
{
    setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC); //默认公有配置
    FirewallDialog *fireWallDialog = new FirewallDialog();
    fireWallDialog->setUuid(uuid);
    fireWallDialog->setWindowTitle(ssid);

    connect(fireWallDialog, &FirewallDialog::setPrivateNetMode, this, [=](){
        fireWallDialog->hide();
        setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
    });

    connect(fireWallDialog, &FirewallDialog::setPublicNetMode, this, [=](){
        fireWallDialog->hide();
        setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
    });

    connect(m_manager, &KyNetworkManager::wiredStateChange, this, [=](QString devName, QString connUuid, KyConnectState state) {
        Q_UNUSED(devName)
        fireWallDialog->closeMyself(connUuid, state);
    });
    connect(m_manager, &KyNetworkManager::wirelessStateChange, this, [=](QString devName,
            QString connsSsid, QString connUuid, KyConnectState state) {
        Q_UNUSED(devName)
        Q_UNUSED(connsSsid)
        fireWallDialog->closeMyself(connUuid, state);
    });

    fireWallDialog->show();
    fireWallDialog->centerToScreen();
}

void NetworkModeConfig::onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, KyConnectState status)
{
    qDebug()<< LOG_FLAG << "onActiveConnectionChanged" << deviceName << ssid << uuid << status;

    if (ssid.isEmpty()) {
        breakNetworkConnect(uuid, deviceName, ssid);
        return;
    }

    if (status == CONNECT_STATE_ACTIVATED) {
        int configType = getNetworkModeConfig(uuid);

        if (configType == NO_CONFIG) {
            setFirstConnectNetworkMode(uuid, deviceName, ssid);
        } else if (configType == KSC_FIREWALL_PUBLIC) {
            setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
        } else if (configType == KSC_FIREWALL_PRIVATE) {
            setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
        }
    } else if (status == CONNECT_STATE_DEACTIVATED) {
        breakNetworkConnect(uuid, deviceName, ssid);
    }
}
