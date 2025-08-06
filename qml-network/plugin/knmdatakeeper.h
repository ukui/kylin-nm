#ifndef KNMDATAKEEPER_H
#define KNMDATAKEEPER_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QtGlobal>
#include <QDebug>
#include <QList>
#include <QStringList>
#include <QTimer>

#include "netdevice.h"

class NetDevice;

//共享指针初始化
typedef QSharedPointer<NetDevice> NetDevicePtr;

//dbus调用之后用于保存数据的类
class KnmDataKeeper : public QObject
{
    Q_OBJECT
public:
    explicit KnmDataKeeper(QObject *parent = nullptr);

    ~KnmDataKeeper();

    //添加一个网卡设备
    void addDevice(QString devName, bool isAvailable, DeviceType type);

    //清空列表(用于刷新)
    void clearDevice();

    //清空网卡设备连接(用于列表更新)
    void clearConnectionList(QString devName);

    //获取网卡设备列表
    QMap<QString, NetDevicePtr> getDeviceList();

    //获取单个网卡设备的连接列表
    QVariantList getDevConnections(QString devName);

    void setSwitchState(bool switched);

    bool getSwitchState();

    void setWiredMainSwitchState(bool switched);

    bool getWiredMainSwitchState();

    QString getUpwardRate();

    QString getDownwardRate();

    void setUpwardRate(QString);

    void setDownwardRate(QString);

protected:
    //网速计时器初始化
    void netSpeedInit();

    //网速获取和计算函数
    void netSpeedHandler(QString dev, QString &upLoad, QString &downLoad);

    //获取当前网卡负载率
    int getCurrentLoadRate(QString dev, long *save_rate, long *tx_rate);

protected:
    bool m_connected = false;

    bool m_wiredMainSwitchState = false;

    bool m_switchState = false;

    QTimer *m_pSpeedTimer = nullptr;

    QString m_currentDev;

    //保存开始时的流量计数
    long int start_rcv_rates = 0;
    //保存结束时的流量计数
    long int end_rcv_rates = 0;
    //保存开始时的流量计数
    long int start_tx_rates = 0;
    //保存结束时的流量计数
    long int end_tx_rates = 0;

    //
    QString m_upward_rate;

    QString m_downward_rate;


    //设备列表,QString为网卡名,NetDevicePtr为网卡数据类
    QMap<QString, NetDevicePtr> m_deviceList;
};

#endif // KNMDATAKEEPER_H
