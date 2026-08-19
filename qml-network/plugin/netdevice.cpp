/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
#include "netdevice.h"

NetDevice::NetDevice(QString devName, DeviceType type)
    : m_Name(devName)
    , m_Available(false)
    , m_DevType(type)
{
}

NetDevice::~NetDevice()
{
}

bool NetDevice::isAvailable()
{
    return this->m_Available;
}

QString NetDevice::devName()
{
    return this->m_Name;
}

void NetDevice::setAvailable(bool available)
{
    this->m_Available = available;
}

DeviceType NetDevice::getDevType()
{
    return this->m_DevType;
}

void NetDevice::addConnection(QMap<QString, QVariant> connections)
{
    if (m_connectionsList.contains(connections))
        return;

    int status = connections.value("State").toInt();
    // 已激活的连接放在最前面
    if (status == ACTIVATED || status == ACTIVATING) {
        m_connectionsList.push_front(connections);
        return;
    }

    // 对于未激活连接，按照排序规则插入
    int insertIndex = 0;
    bool isConfigured = connections.value("Configured", 0).toInt() == 1;
    int signalStrength = connections.value("Signal", "0").toString().toInt();

    // 找到第一个未激活连接的位置
    while (insertIndex < m_connectionsList.count()) {
        QMap<QString, QVariant> existingConn = m_connectionsList.at(insertIndex).toMap();
        int existingStatus = existingConn.value("State").toInt();

        // 跳过已激活的连接
        if (existingStatus == ACTIVATED || existingStatus == ACTIVATING) {
            insertIndex++;
            continue;
        }

        bool existingConfigured = existingConn.value("Configured", 0).toInt() == 1;
        int existingSignal = existingConn.value("Signal", "0").toString().toInt();

        if (isConfigured && !existingConfigured) {
            // 当前连接已配置，而对比的连接未配置，插入此处
            break;
        } else if (isConfigured == existingConfigured) {
            // 配置状态相同，按信号强度排序；信号相同时新增的放前面
            if (signalStrength >= existingSignal) {
                break;
            }
        }
        // 如果当前连接未配置，而已有连接已配置，则继续向后查找
        // 如果信号强度更弱，也继续向后查找
        insertIndex++;
    }

    // 插入到正确位置
    m_connectionsList.insert(insertIndex, connections);
}

void NetDevice::removeConnection(QMap<QString, QVariant> connections)
{
    if (!m_connectionsList.contains(connections))
        return;
    m_connectionsList.removeOne(connections);
}

void NetDevice::removeConnection(QString path)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Path").toString() == path) {
            m_connectionsList.removeAt(i);
        }
    }
}

void NetDevice::removeConnectionByName(QString devName)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Name").toString() == devName) {
            m_connectionsList.removeAt(i);
        }
    }
}

QVariantList NetDevice::getConnections()
{
    QVariantList list;
    if(m_connectionsList.isEmpty()) {
        return list;
    }
    return m_connectionsList;
}

QMap<QString, QVariant> NetDevice::updateConnection(QString uuid, int status)
{
    int i = 0;
    QMap<QString, QVariant> conn;
    while (i < m_connectionsList.count()) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (!valueMap.contains("Uuid"))
            continue;
        if (valueMap.value("Uuid").toString() == uuid) {
            conn = m_connectionsList.takeAt(i).toMap();
            conn.remove("State");
            conn.insert("State", status);
            if (status == ACTIVATED) {
                m_connectionsList.push_front(conn);
            } else
                m_connectionsList.insert(i, conn);
            break;
        }
        i++;
    }
    return conn;
}

bool NetDevice::containsConnection(QString uuidOrPath)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Uuid").toString() == uuidOrPath) {
            return true;
        }
        if (m_DevType == WIRELESS_DEVICE)
            continue;
        if (valueMap.value("Path").toString() == uuidOrPath) {
            return true;
        }
    }
    return false;
}

bool NetDevice::containsConnectionName(QString connName)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Name").toString() == connName) {
            return true;
        }
    }
    return false;
}

void NetDevice::clearConnections()
{
    m_connectionsList.clear();
}

QMap<QString, QVariant> NetDevice::getConnectionByName(QString connName)
{
    QMap<QString, QVariant> ret;
    for (int i = 0; i < m_connectionsList.count(); i++) {
         QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Name").toString() == connName) {
            return m_connectionsList.at(i).toMap();
        }
    }
    return ret;
}

QMap<QString, QVariant> NetDevice::updateConnectivity(QString uuid, int status, int connectivity)
{
    int i = 0;
    QMap<QString, QVariant> conn;
    while (i < m_connectionsList.count()) {
        QMap<QString, QVariant> valueMap = m_connectionsList.at(i).toMap();
        if (valueMap.value("Uuid").toString() == uuid) {
            conn = m_connectionsList.takeAt(i).toMap();
            conn.remove("State");
            conn.insert("State", status);
            conn.remove("Connectivity");
            conn.insert("Connectivity", connectivity);
            
            if (status == ACTIVATED) {
                m_connectionsList.push_front(conn);
            } else {
                m_connectionsList.insert(i, conn);
            }
            break;
        }
        i++;
    }
    return conn;
}

void NetDevice::updateConnectionProp(QVariantMap parm)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << parm;
    int i = 0;

    QMap<QString, QVariant> conn;
    while (i < m_connectionsList.count()) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
         if (valueMap.value("Name").toString() == parm.value("Name").toString()) {
            conn = m_connectionsList.takeAt(i).toMap();
            if(parm.contains("Signal")){
                conn.remove("Signal");
                conn.insert("Signal", parm.value("Signal"));
            }
            if(parm.contains("Security")){
                conn.remove("Security");
                conn.insert("Security", parm.value("Security"));
            }
            if(parm.contains("Uuid")){
                conn.remove("Uuid");
                conn.insert("Uuid", parm.value("Uuid"));
            }
            if(parm.contains("frequency")){
                conn.remove("frequency");
                conn.insert("frequency", parm.value("frequency"));
            }
            if(parm.contains("Configured")){
                conn.remove("Configured");
                conn.insert("Configured", parm.value("Configured"));
            }
            if(parm.contains("isMix")){
                conn.remove("isMix");
                conn.insert("isMix", parm.value("isMix"));
            }
            if(parm.contains("State")){
                conn.remove("State");
                conn.insert("State", parm.value("State"));
            }
            if(parm.contains("autoConnect")){
                conn.remove("autoConnect");
                conn.insert("autoConnect", parm.value("autoConnect"));
            }
            if(parm.contains("isApConn")){
                conn.remove("isApConn");
                conn.insert("isApConn", parm.value("isApConn"));
            }
            addConnection(conn);
            break;
        }
        i++;
    }
    return ;
}
