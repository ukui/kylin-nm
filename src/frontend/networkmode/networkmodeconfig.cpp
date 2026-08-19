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
//#include "utils.h"
#include <QDebug>

#define LOG_FLAG  "[NetworkMode]"

bool checkLiveMode()
{
    QFile file("/proc/cmdline");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file /proc/cmdline!";
        file.close();
        file.remove();
        return false;
    }

    QString result_info = file.readAll();
    file.close();
    file.remove();
    int index = result_info.indexOf("security");
    if (index >= 0) {
        QString str = result_info.mid(index);
        if (str.contains("live")) {
            return true;
        }
    }
    return false;
}

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
    m_dbusInterface->setTimeout(2000);
}

// 异步版本：核心改造点
void NetworkModeConfig::getNetworkModeConfigAsync(const QString &uuid)
{
    if (uuid.isEmpty()) {
        qWarning() << LOG_FLAG << "uuid is empty, can not get network mode config";
        Q_EMIT networkModeConfigReady(uuid, NO_CONFIG, false);
        return;
    }

    if(m_dbusInterface == nullptr || !m_dbusInterface->isValid()) {
        qWarning() << LOG_FLAG << "com.ksc.defender dbus is invalid";
        Q_EMIT networkModeConfigReady(uuid, DBUS_INVALID, false);
        return;
    }

    // 异步调用DBus，无阻塞
    QDBusPendingCall pendingCall = m_dbusInterface->asyncCall("get_networkModeConfig", uuid);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *w) {
        QDBusPendingReply<int> reply = *w;
        if (reply.isValid()) {
            Q_EMIT networkModeConfigReady(uuid, reply.value(), true);
        } else {
            qWarning() << LOG_FLAG << "call get_networkModeConfig failed:" << reply.error().message();
            Q_EMIT networkModeConfigReady(uuid, NO_CONFIG, false);
        }
        w->deleteLater(); // 释放资源
    });
}

// 保留原同步接口（标记为废弃，建议新逻辑用异步）
int NetworkModeConfig::getNetworkModeConfig(QString uuid)
{
    if (uuid.isEmpty()) {
           qWarning()<< /*LOG_FLAG <<*/ "uuid is empty, so can not get network mode config";
           return NO_CONFIG;
       }

    if(m_dbusInterface == nullptr || !m_dbusInterface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return DBUS_INVALID;
    }

    QDBusReply<int> reply = m_dbusInterface->call("get_networkModeConfig", uuid);
    if (reply.isValid()) {
        return reply.value();
    } else {
        qWarning() << "call get_networkModeConfig failed" << reply.error().message();
    }
    return NO_CONFIG;
}

void NetworkModeConfig::setNetworkModeConfig(QString uuid, QString cardName, QString ssid, int mode)
{
    qWarning() << "setNetworkModeConfig" <<  uuid<<cardName<<ssid<<mode;
    if(m_dbusInterface == nullptr || !m_dbusInterface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return;
    }
    // 异步调用
    QDBusPendingCall pendingCall = m_dbusInterface->asyncCall("set_networkModeConfig", uuid, cardName, ssid, mode);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall,this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *w) {
       // 解析调用结果
       QDBusPendingReply<int> reply = *w;
       if (reply.isValid()) {
           qDebug() << "set_networkModeConfig" << ",result" << reply.value();
       } else {
           QDBusError error = reply.error();
           qWarning() << "异步调用失败：" << error.name() << " - " << error.message()<<error.type();
       }
       w->deleteLater();
    });
    /*
    QDBusReply<int> reply = m_dbusInterface->call("set_networkModeConfig", uuid, cardName, ssid, mode);
    if (reply.isValid()) {
        qDebug() << "set_networkModeConfig" << ssid << uuid << cardName << mode << ",result" << reply.value();
    } else {
        qWarning() << "call set_networkModeConfig" << reply.error().message();
    }
    */
}

/*原返回int 代码中未使用该返回 改成异步并取消返回*/
void NetworkModeConfig::breakNetworkConnect(QString uuid, QString cardName, QString ssid)
{
    if(m_dbusInterface == nullptr || !m_dbusInterface->isValid()) {
        qWarning () << "com.ksc.defender dbus is invalid";
        return ;
    }


    QDBusPendingCall pendingCall = m_dbusInterface->asyncCall("break_networkConnect", uuid, cardName, ssid);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall,this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *w) {
       // 解析调用结果
       QDBusPendingReply<int> reply = *w;
       if (reply.isValid()) {
           qDebug() << "break_networkConnect"  << ",result" << reply.value();
       } else {
           QDBusError error = reply.error();
           qWarning() << "call break_networkConnect failed" << error.name() << " - " << error.message()<<error.type();
       }
       w->deleteLater();
    });

    /*
    QDBusReply<int> reply = m_dbusInterface->call("break_networkConnect", uuid, cardName, ssid);
    if (reply.isValid()) {
        qDebug() << "break_networkConnect" << ssid << uuid << cardName << ",result" << reply.value();
        return reply.value();
    } else {
        qWarning() << "call break_networkConnect failed" << reply.error().message();
        return -1;
    }
    */
}

//安全中心-网络防火墙模式配置
NetworkMode::NetworkMode(QObject *parent)
    :QObject(parent)
{
    qRegisterMetaType<NetworkManager::Device::State>("NetworkManager::Device::State");
    qRegisterMetaType<NetworkManager::Device::StateChangeReason>("NetworkManager::Device::StateChangeReason");
    m_activatedConnectResource = new KyActiveConnectResource(this);
    connect(m_activatedConnectResource, &KyActiveConnectResource::stateChangeReason,
            this, &NetworkMode::onConnectionStateChanged);

    // 连接NetworkModeConfig的异步结果信号
    connect(NetworkModeConfig::getInstance(), &NetworkModeConfig::networkModeConfigReady,
            this, &NetworkMode::onGetNetworkModeConfigReady);

}

// 改造为异步初始化有线网络配置
void NetworkMode::initWiredNetworkMode()
{
    qDebug()<< LOG_FLAG << "initWiredNetworkMode (async)";
    QStringList wiredDevList;
    KyNetworkDeviceResourse deviceResource;
    deviceResource.getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, wiredDevList);
    if (wiredDevList.isEmpty()) {
        return;
    }

    // 遍历设备，发起异步查询
    for (auto devName : wiredDevList) {
        QList<KyConnectItem *> activedList;
        m_activatedConnectResource->getActiveConnectionList(devName,
                                                            NetworkManager::ConnectionSettings::Wired,
                                                            activedList);
        if (activedList.isEmpty()) {
            continue;
        }

        QString uuid = activedList.at(0)->m_connectUuid;
        QString devNameCopy = devName;
        QString connName = activedList.at(0)->m_connectName;

        // 保存待处理的配置信息（用于回调）
        PendingConfigData data;
        data.uuid = uuid;
        data.deviceName = devNameCopy;
        data.ssid = connName;
        data.isWired = true;
        m_pendingConfigMap[uuid] = data;

        // 发起异步查询，无阻塞
        NetworkModeConfig::getInstance()->getNetworkModeConfigAsync(uuid);
    }

    /*
    qDebug()<< LOG_FLAG << "initWiredNetworkMode";
    QStringList wiredDevList;
    KyNetworkDeviceResourse deviceResource;
    deviceResource.getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, wiredDevList);
    if (wiredDevList.isEmpty()) {
        return;
    }

    for (auto devName : wiredDevList) {
        QList<KyConnectItem *> activedList;
        m_activatedConnectResource->getActiveConnectionList(devName,
                                                            NetworkManager::ConnectionSettings::Wired,
                                                            activedList);
        if (activedList.isEmpty()) {
            continue;
        }

        int configType = NetworkModeConfig::getInstance()->getNetworkModeConfig(activedList.at(0)->m_connectUuid);
        if (configType == DBUS_INVALID) {
            return;
        } else if (configType == NO_CONFIG) {
            //已连接网络无配置 默认公有配置
            NetworkModeConfig::getInstance()->setNetworkModeConfig(activedList.at(0)->m_connectUuid, devName,
                                                                   activedList.at(0)->m_connectName, KSC_FIREWALL_PUBLIC);
        } else {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(activedList.at(0)->m_connectUuid, devName,
                                                                   activedList.at(0)->m_connectName, configType);
        }
    }
    */
}

// 改造为异步初始化无线网络配置
void NetworkMode::initWirelessNetworkMode()
{
    qDebug()<< LOG_FLAG << "initWirelessNetworkMode (async)";
    QStringList wirelessDevList;
    KyNetworkDeviceResourse deviceResource;
    deviceResource.getNetworkDeviceList(NetworkManager::Device::Type::Wifi, wirelessDevList);
    if (wirelessDevList.isEmpty()) {
        return;
    }

    // 遍历设备，发起异步查询
    for (auto devName : wirelessDevList) {
        KyWirelessNetItem wirelessNetItem;
        KyWirelessNetResource wirelessNetResource;
        bool ret = wirelessNetResource.getActiveWirelessNetItem(devName, wirelessNetItem);

        if (ret == true) {
            QString uuid = wirelessNetItem.m_connectUuid;
            QString devNameCopy = devName;
            QString ssid = wirelessNetItem.m_connName;

            // 保存待处理的配置信息
            PendingConfigData data;
            data.uuid = uuid;
            data.deviceName = devNameCopy;
            data.ssid = ssid;
            data.isWired = false;
            m_pendingConfigMap[uuid] = data;

            // 发起异步查询，无阻塞
            NetworkModeConfig::getInstance()->getNetworkModeConfigAsync(uuid);
        }
    }

    /*
    qDebug()<< LOG_FLAG << "initWirelessNetworkMode";
    QStringList wirelessDevList;
    KyNetworkDeviceResourse deviceResource;
    deviceResource.getNetworkDeviceList(NetworkManager::Device::Type::Wifi, wirelessDevList);
    if (wirelessDevList.isEmpty()) {
        return;
    }

    for (auto devName : wirelessDevList) {
        KyWirelessNetItem wirelessNetItem;
        KyWirelessNetResource wirelessNetResource;
        bool ret = wirelessNetResource.getActiveWirelessNetItem(devName, wirelessNetItem);

        if (ret == true) {
            int configType = NetworkModeConfig::getInstance()->getNetworkModeConfig(wirelessNetItem.m_connectUuid);
            if (configType == DBUS_INVALID) {
                return;
            } else if (configType == NO_CONFIG) {
                //已连接网络无配置 默认公有配置
                NetworkModeConfig::getInstance()->setNetworkModeConfig(wirelessNetItem.m_connectUuid, devName,
                                                                       wirelessNetItem.m_connName, KSC_FIREWALL_PUBLIC);
            } else {
                NetworkModeConfig::getInstance()->setNetworkModeConfig(wirelessNetItem.m_connectUuid, devName,
                                                                       wirelessNetItem.m_connName, configType);
            }
        }
    }
    */
}

void NetworkMode::setFirstConnectNetworkMode(QString uuid, QString deviceName, QString ssid)
{
    NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC); //默认公有配置
    if (checkLiveMode()) {
        return;
    }
    QSettings setting(CONFIG_FILE_PATH, QSettings::IniFormat);
    if (!setting.value(FIRE_WALL_PERMISSION_SHOW).toBool()) {
        if (setting.value(AUTO_FIRE_WALL_PERMIITTED).toBool()) {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
        } else {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
        }
        return;
    }
    FirewallDialog *fireWallDialog = new FirewallDialog();
    fireWallDialog->setUuid(uuid);
    fireWallDialog->setWindowTitle(ssid);

    connect(fireWallDialog, &FirewallDialog::setPrivateNetMode, this, [=](){
        fireWallDialog->hide();
        NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
    });

    connect(fireWallDialog, &FirewallDialog::setPublicNetMode, this, [=](){
        fireWallDialog->hide();
        NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
    });

    connect(m_activatedConnectResource, &KyActiveConnectResource::stateChangeReason, fireWallDialog, &FirewallDialog::closeMyself);

    fireWallDialog->show();
    fireWallDialog->centerToScreen();
}

// 改造onConnectionStateChanged中的同步调用为异步
void NetworkMode::onConnectionStateChanged(QString uuid,
                                           NetworkManager::ActiveConnection::State state,
                                           NetworkManager::ActiveConnection::Reason reason)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << " uuid : "<< uuid << " state : "<< state << " reason : "<< reason ;
    if (state == NetworkManager::ActiveConnection::State::Activated) {
        QString deviceName = "";
        QString ssid = "";
        bool isWired = false;
        bool isWireless = false;

        // 1. 先获取网络类型和基本信息
        KyConnectResourse connectResource;
        if (connectResource.isWiredConnection(uuid)) {
            KyConnectItem *p_newItem = m_activatedConnectResource->getActiveConnectionByUuid(uuid);
            if (nullptr == p_newItem) {
                qDebug()<< LOG_FLAG << "delete wired connect:" << uuid << ", call break_networkConnect";
                NetworkModeConfig::getInstance()->breakNetworkConnect(uuid, "", "");
                return;
            }
            deviceName = p_newItem->m_ifaceName;
            ssid = p_newItem->m_connectName;
            isWired = true;
        }

        if (connectResource.isWirelessConnection(uuid)) {
            KyWirelessNetResource wirelessNetResource;
            wirelessNetResource.getSsidByUuid(uuid, ssid);
            wirelessNetResource.getDeviceByUuid(uuid, deviceName);
            if (ssid.isEmpty()) {
                qDebug()<< LOG_FLAG << "forgrt wireless connect:" << uuid <<", call break_networkConnect";
                NetworkModeConfig::getInstance()->breakNetworkConnect(uuid, "", "");
                return;
            }
            isWireless = true;
        }

        // 2. 保存上下文，发起异步查询
        PendingStateData stateData;
        stateData.uuid = uuid;
        stateData.deviceName = deviceName;
        stateData.ssid = ssid;
        stateData.isWired = isWired;
        stateData.isWireless = isWireless;
        m_pendingStateMap[uuid] = stateData;

        // 3. 异步查询配置，无阻塞
        NetworkModeConfig::getInstance()->getNetworkModeConfigAsync(uuid);

    } else if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        NetworkModeConfig::getInstance()->breakNetworkConnect(uuid, "", "");
    }

    /*
    if (state == NetworkManager::ActiveConnection::State::Activated) {
        QString deviceName = "";
        QString ssid = "";

        int configType = NetworkModeConfig::getInstance()->getNetworkModeConfig(uuid);
        KyConnectResourse connectResource;

        //有线网络连接
        if (connectResource.isWiredConnection(uuid)) {
            KyConnectItem *p_newItem = nullptr;
            p_newItem = m_activatedConnectResource->getActiveConnectionByUuid(uuid);
            if (nullptr == p_newItem) {
                //删除此网络
                qDebug()<< LOG_FLAG << "delete wired connect:" << uuid << ", call break_networkConnect";
                NetworkModeConfig::getInstance()->breakNetworkConnect(uuid, "", "");
                return;
            }

            deviceName = p_newItem->m_ifaceName;
            ssid = p_newItem->m_connectName;
            if (configType == NO_CONFIG) {
                //首次连接的网络
                setFirstConnectNetworkMode(uuid, deviceName, ssid);
            }  else if (configType == KSC_FIREWALL_PUBLIC) {
                NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
            } else if (configType == KSC_FIREWALL_PRIVATE) {
                NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
            }
        }
        //无线网络连接
        if (connectResource.isWirelessConnection(uuid)) {
            KyWirelessNetResource wirelessNetResource;
            wirelessNetResource.getSsidByUuid(uuid, ssid);
            wirelessNetResource.getDeviceByUuid(uuid, deviceName);
            if (ssid.isEmpty()) {
                //忘记此网络
                qDebug()<< LOG_FLAG << "forgrt wireless connect:" << uuid <<", call break_networkConnect";
                NetworkModeConfig::getInstance()->breakNetworkConnect(uuid, "", "");
            }

            if (configType == NO_CONFIG) {
                setFirstConnectNetworkMode(uuid, deviceName, ssid);
            }  else if (configType == KSC_FIREWALL_PUBLIC) {
                NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PUBLIC);
            } else if (configType == KSC_FIREWALL_PRIVATE) {
                NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, deviceName, ssid, KSC_FIREWALL_PRIVATE);
            }
        }

    } else if (state == NetworkManager::ActiveConnection::State::Deactivated) {
        NetworkModeConfig::getInstance()->breakNetworkConnect(uuid, "", "");
    }
    */
}

// 唯一的异步查询结果回调函数（整合所有场景）
void NetworkMode::onGetNetworkModeConfigReady(const QString &uuid, int configType, bool success)
{
    // 场景1：处理初始化阶段的配置请求（initWired/initWireless）
    if (m_pendingConfigMap.contains(uuid)) {
        PendingConfigData data = m_pendingConfigMap.take(uuid); // 取出并移除上下文
        qDebug() << LOG_FLAG << "处理初始化配置 - UUID:" << uuid;

        if (!success || configType == DBUS_INVALID) {
            qWarning() << LOG_FLAG << "初始化配置查询失败 - UUID:" << uuid;
            return;
        }

        // 执行初始化阶段的配置逻辑
        if (configType == NO_CONFIG) {
            // 无配置，默认公有模式
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, data.deviceName, data.ssid, KSC_FIREWALL_PUBLIC);
        } else {
            // 有配置，使用现有配置
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, data.deviceName, data.ssid, configType);
        }
        return; // 处理完初始化场景，直接返回
    }

    // 场景2：处理连接状态变化的配置请求（onConnectionStateChanged）
    if (m_pendingStateMap.contains(uuid)) {
        PendingStateData data = m_pendingStateMap.take(uuid); // 取出并移除上下文
        qDebug() << LOG_FLAG << "处理连接状态配置 - UUID:" << uuid;

        if (!success || configType == DBUS_INVALID) {
            qWarning() << LOG_FLAG << "连接状态配置查询失败 - UUID:" << uuid;
            return;
        }

        // 执行连接状态变化的配置逻辑
        if (configType == NO_CONFIG) {
            // 首次连接，弹出对话框/自动配置
            setFirstConnectNetworkMode(uuid, data.deviceName, data.ssid);
        }  else if (configType == KSC_FIREWALL_PUBLIC) {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, data.deviceName, data.ssid, KSC_FIREWALL_PUBLIC);
        } else if (configType == KSC_FIREWALL_PRIVATE) {
            NetworkModeConfig::getInstance()->setNetworkModeConfig(uuid, data.deviceName, data.ssid, KSC_FIREWALL_PRIVATE);
        }
        return; // 处理完连接状态场景，直接返回
    }

    // 场景3：未知的UUID请求（日志记录，避免漏处理）
    qWarning() << LOG_FLAG << "收到未知UUID的配置查询结果 - UUID:" << uuid;
}
