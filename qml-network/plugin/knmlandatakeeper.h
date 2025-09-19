#ifndef KNMLANDATAKEEPER_H
#define KNMLANDATAKEEPER_H

#include <QDBusInterface>

#include "knmdatakeeper.h"

class KnmLanDataKeeper;
typedef QSharedPointer<KnmLanDataKeeper> KnmLanDataKeeperPtr;

class KnmLanDataKeeper : public KnmDataKeeper
{
    Q_OBJECT

public:
    explicit KnmLanDataKeeper(QObject *parent = nullptr);
    ~KnmLanDataKeeper();

    //添加网卡设备的连接
    void addDevConnection(QString devName, QList<QStringList> conPath);

signals:
    void onDeviceStatusChanged();


private:
    QMap<QString, QVariant> makeConnectionMap(int status, QStringList conPath);
    bool isDslConnection(const QString &uuid);

private slots:
    //已激活连接状态更新
    void onActiveConnectionChanged(QString deviceName, QString uuid, int status);

    //新增连接
    void onLanAdd(QString deviceName, QStringList lanInfo);

    //删除连接
    void onLanRemove(QString path);

    //连接更新属性(名称)或更改从属网卡设备
    void updateLanInfo(QString deviceName, QStringList lanInfo);

    //更新网卡设备名称
    void onDeviceNameChanged(QString oldName, QString newName, int type);

    //更新 开关状态
    void onSwitchBtnChanged(bool status);

private:
    QDBusInterface      *m_pInterface = nullptr;
};

#endif // KNMLANDATAKEEPER_H
