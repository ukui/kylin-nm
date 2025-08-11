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
#include "wirelessconnectionmodel.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QDebug>

WirelessConnectionModel::WirelessConnectionModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int WirelessConnectionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) 
        return 0;
    return m_connections.size();
}

QVariant WirelessConnectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_connections.size())
        return QVariant();

    const ST_ConnectionInfo &connection = m_connections.at(index.row());
    
    switch (role) {
    case SSIDRole: return connection.ssid;
    case StrengthRole: return connection.signal;
    case ConnectStatusRole: return connection.status;
    case SecurityTypeRole: return connection.security;
    case CategoryRole: return connection.category;
    case IsAPRole: return connection.isApConn;
    case UUIDRole: return connection.uuid;
    case IsLoadingRole: return connection.Loading;
    case ConfiguredRole: return connection.Configured;
    case FrequencyRole: return connection.frequency;
    case IsMixRole: return connection.isMix;

    default: return QVariant();
    }
}

bool WirelessConnectionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_connections.size())
        return false;

    ST_ConnectionInfo &connection = m_connections[index.row()];
    
    switch (role) {
    case SSIDRole:
        connection.ssid = value.toString();
        break;
    case StrengthRole:
        connection.signal = value.toString();
        break;
    case IsAPRole:
        connection.isApConn = value.toString();
        break;
    case SecurityTypeRole:
        connection.security = value.toString();
        break;
    case UUIDRole:
        connection.uuid = value.toString();
        break;
    case CategoryRole:
        connection.category = value.toUInt();
        break;
    case ConnectStatusRole:
        connection.status = value.toInt();
        break;
    case IsLoadingRole:
        connection.Loading = value.toBool();
        break;
    case ConfiguredRole:
        connection.Configured = value.toBool();
        break;
    case FrequencyRole:
        connection.frequency = value.toUInt();
        break;
    case IsMixRole:
        connection.isMix = value.toBool();
        break;
    default:
        return false;
    }
    
    emit dataChanged(index, index, {role});
    return true;
}

QHash<int, QByteArray> WirelessConnectionModel::roleNames() const
{
    return {
        {SSIDRole, "ssid"},
        {StrengthRole, "signal"},
        {SecurityTypeRole, "security"},
        {UUIDRole, "uuid"},
        {IsAPRole, "isApConn"},
        {CategoryRole, "category"},
        {ConnectStatusRole,"status"},
        {IsLoadingRole,"Loading"},
        {ConfiguredRole,"Configured"},
        {FrequencyRole, "frequency"},
        {IsMixRole, "isMix"}

    };
}

void WirelessConnectionModel::addConnection(struct ST_ConnectionInfo *pConnection)
{
    // 确保ID唯一性
    QString id = generateUniqueId();
    //

    if(pConnection->status==1 || pConnection->status == 2)
    {
        beginInsertRows(QModelIndex(), 0, 0);
        m_connections.push_front(*pConnection);
        endInsertRows();
    }
    else
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_connections.append(*pConnection);
        endInsertRows();
    }
    
    qDebug() << "Added connection:" << pConnection->ssid << "(" << pConnection->uuid << ")";
}

void WirelessConnectionModel::removeConnection(const QString &connectionId)
{
    int index = findIndexById(connectionId);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_connections.remove(index);
        endRemoveRows();
        qDebug() << "Removed connection:" << connectionId;
    }
}

void WirelessConnectionModel::updateConnectionStrength(const QString &connectionId, int newStrength)
{
    int index = findIndexById(connectionId);
    if (index != -1) {
        m_connections[index].signal = newStrength;
        QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex, {StrengthRole});
    }
}

void WirelessConnectionModel::updateConnectionStatus(const QString &connectionId, int status)
{
    int index = findIndexById(connectionId);
    if (index != -1) {
        m_connections[index].status = status;
        QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex, {ConnectStatusRole});
    }
}

WirelessConnectionModel::ST_ConnectionInfo WirelessConnectionModel::mapToConnectionInfo(QMap<QString, QVariant> value)
{
    ST_ConnectionInfo connect;

    connect.ssid=value.value("Name").toString();
    connect.signal=value.value("Signal").toString();
    connect.security=value.value("Security").toString();
    connect.uuid=value.value("Uuid").toString();
    connect.isApConn=value.value("isApConn").toString();
    connect.category=value.value("category").toString();
    connect.frequency=value.value("frequency").toUInt();

    connect.status=value.value("State").toInt();
    connect.Loading=value.value("Loading").toBool();
    connect.Configured=value.value("Configured").toInt();
    connect.isMix=value.value("isMix").toBool();

    return connect;
}


void WirelessConnectionModel::refreshConnections(const QVariantList newList)
{
    m_connections.clear();
    beginResetModel();
    for(int i=0;i<newList.count();i++)
    {
        ST_ConnectionInfo connect=mapToConnectionInfo(newList.at(i).toMap());

        //qWarning()<<"mqtest refreshConnections append "<<newList.at(i).toMap();
        m_connections.append(connect);
    }
    endResetModel();
}

int WirelessConnectionModel::findIndexById(const QString &id) const
{
    for (int i = 0; i < m_connections.size(); ++i) {
        if (m_connections[i].ssid == id) {
            return i;
        }
    }
    return -1;
}

QString WirelessConnectionModel::generateUniqueId() const
{
    return QString("conn_%1_%2").arg(QDateTime::currentMSecsSinceEpoch())
                                .arg(QRandomGenerator::global()->generate() % 10000);
}
