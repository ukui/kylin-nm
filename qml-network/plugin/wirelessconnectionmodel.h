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
#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <QHash>

class WirelessConnectionModel : public QAbstractListModel
{
    Q_OBJECT


public:
    enum Roles {
        SSIDRole = Qt::UserRole + 1,
        StrengthRole,
        SecurityTypeRole,
        UUIDRole,
        IsAPRole,
        CategoryRole,
        ConnectStatusRole,
        IsLoadingRole,
        ConfiguredRole,
        FrequencyRole,
        IsMixRole,
    };


    struct ST_ConnectionInfo {
        QString ssid;       // 网络名称
        QString signal;     // 信号强度
        QString security;   // 安全类型
        QString uuid;       // 网络名称
        QString isApConn;
        QString category;
        uint frequency;  //

        int status;     //
        bool Loading;
        int Configured;

        bool isMix;

    };
    explicit WirelessConnectionModel(QObject *parent = nullptr);
    ST_ConnectionInfo mapToConnectionInfo(QMap<QString, QVariant> value);

    // QAbstractListModel 接口
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    // 添加连接项
    void addConnection(struct ST_ConnectionInfo *pConnection);
    
    // 移除连接项
    void removeConnection(const QString &connectionId);
    
    // 更新连接属性
    void updateConnectionStrength(const QString &connectionId, int newStrength);
    void updateConnectionStatus(const QString &connectionId, int status);
    
    // 批量添加/更新
    void refreshConnections(const QVariantList newList);

    int getConButtonFromSsid(const QString itemId);
private:
    /*makeConnectionMap*/

    int findIndexById(const QString &id) const;
    QString generateUniqueId() const;
    
    QVector<ST_ConnectionInfo> m_connections;
};
