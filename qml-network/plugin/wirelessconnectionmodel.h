// wirelessconnectionmodel.h
#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <QHash>

class WirelessConnectionModel : public QAbstractListModel
{
    Q_OBJECT
   //Q_PROPERTY(WirelessConnectionModel wirelessConList READ wirelessConList NOTIFY updateWirelessDevConnList)

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
    };


    struct ST_ConnectionInfo {
        QString ssid;        // 网络名称
        QString signal;       // 信号强度
        QString security;   // 安全类型
        QString uuid;        // 网络名称
        QString isApConn;
        QString category;
        int status;     //
        bool Loading;
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


private:
    /*makeConnectionMap*/

    int findIndexById(const QString &id) const;
    QString generateUniqueId() const;
    
    QVector<ST_ConnectionInfo> m_connections;
};
