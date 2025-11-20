#ifndef KNMWLANDATAKEEPER_H
#define KNMWLANDATAKEEPER_H

#include <QDBusInterface>

#include "knmdatakeeper.h"

class KnmWlanDataKeeper;
typedef QSharedPointer<KnmWlanDataKeeper> KnmWlanDataKeeperPtr;

class KnmWlanDataKeeper : public KnmDataKeeper
{
    Q_OBJECT

public:
    explicit KnmWlanDataKeeper(QObject *parent = nullptr);
    ~KnmWlanDataKeeper();

    //添加网卡设备的连接
    void addDevConnection(QString devName, QList<QStringList> conPath);

    QString getWiFiIcon(QString signalStrength, QString security, QString isApConnection, int category);

signals:
    void onDeviceStatusChanged();

private:
    QMap<QString, QVariant> makeConnectionMap(int status, QStringList conPath);

    int setSignal(QString sg);

    QString getWiFiIconString(bool isLock, int strength, int category);

private slots:
    //已激活连接状态更新
    void onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, int status);

    //新增连接
    void onNetworkAdd(QString deviceName, QStringList wlanInfo);

    //删除连接
    void onNetworkRemove(QString deviceName, QString wlanName);

    //更新网卡设备名称
    void onDeviceNameChanged(QString oldName, QString newName, int type);

    //更新开关状态
    void onSwitchBtnChanged(bool status);

    //更新连接列表顺序
    void updateList();

    void onRequestInputPasswdAgent(QString agentName, QVariantMap parm);

    void onNetworkPropChanged(QVariantMap parm);

private:
    QDBusInterface      *m_pInterface = nullptr;
};

#endif // KNMWLANDATAKEEPER_H
