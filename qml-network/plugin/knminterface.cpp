#include "knminterface.h"
#include "knmdbuscaller.h"

KnmInterface::KnmInterface()
{
    KNMDC::instance(this);
    loadTimer = new QTimer(this);
    loadTimer->setInterval(100);
    connect(loadTimer, &QTimer::timeout, this, [=]() {
        QString icon = getIconData("ukui-loading-" + QString::number(loadCount) + "-symbolic");
        loadCount += 1;
        if (loadCount == 8)
            loadCount = 0;
        emit updateLoadingIcon(QVariant(icon));
    });
    m_pRefreshTimer = new QTimer(this);
    m_pRefreshTimer->start(20000);
    connect(m_pRefreshTimer, &QTimer::timeout, this,&KnmInterface::slotRefreshTimeout);
}

KnmInterface::~KnmInterface()
{
    if (nullptr != loadTimer) {
        delete loadTimer ;
        loadTimer = nullptr;
    }
}

QString KnmInterface::getIconData(QString name, int size /*= 24*/)
{
    QIcon icon =QIcon::fromTheme(name);
    QPixmap pixmap = icon.pixmap(size, size);
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    return "data:image/png;base64," + data.toBase64();
}

QVariantList KnmInterface::wiredDeviceList()
{
    QVariantList list;

    auto dev = KNMDC::getInstance()->wiredDeviceList();

    // fixbug=377873 已经连接的网卡需要在第一个显示
    int connect_id = -1;
    for (auto iter : dev) {
        if (!iter.isNull()) {
            if (connect_id == -1) {
                for (auto devices : iter->getConnections()) {
                    int status = devices.toMap().value("State").toInt();
                    if (2 == status) {    //2==已连接
                        connect_id = list.size();
                        break;
                    }
                }
            }
            list.append(iter->devName());
        }
    }

    if (connect_id != -1) {
        list.swap(0, connect_id);
    }

    return list;
}

QVariantList KnmInterface::wirelessDeviceList()
{
    QVariantList list;

    auto dev = KNMDC::getInstance()->wirelessDeviceList();
    for(auto iter : dev){
        if(!iter.isNull()) list.append(iter->devName());
    }
    return list;
}

QVariantList KnmInterface::wiredDevConnList()
{
    return m_wiredDevConnList;
}

QVariantList KnmInterface::wirelessDevConnList()
{
    return m_wirelessDevConnList;
}

bool KnmInterface::wirelessSwitchState()
{
    return KNMDC::getInstance()->wirelessSwitchState();
}

bool KnmInterface::wiredMainSwitchState()
{
    qWarning()<< Q_FUNC_INFO << __LINE__;
    return KNMDC::getInstance()->wiredMainSwitchState();
}

QString KnmInterface::upwareRateDate()
{
    return KNMDC::getInstance()->upwardRateDate();
}

QString KnmInterface::downwareRateDate()
{
    return KNMDC::getInstance()->downwardRateDate();
}

bool KnmInterface::getNetMacConnectStatus(QString devmac)
{
    qWarning() << devmac;
    int status = 0;
    auto conList = KNMDC::getInstance()->wiredDeviceConnList(devmac);
    qWarning() << conList;
    if(conList.count() != 0) {
        for (auto iter : conList) {
            status = iter.toMap().value("State").toInt();
            if(2 == status)
                return true;
        }
    }

    conList = KNMDC::getInstance()->wirelessDeviceConnList(devmac);
    qWarning() << conList;
    if(conList.count() != 0) {
        for (auto iter : conList) {
            status = iter.toMap().value("State").toInt();
            if(2 == status)
                return true;
        }
    }

    return false;
}

void KnmInterface::openNetworkSetting()
{
    if(m_pProcess) {
         m_pProcess->deleteLater();
    }

    ConnectStatus connect_status = getConnectionStatus();

    QProcess process;
    if (connect_status == ConnectStatus::Wireless){
        process.startDetached("ukui-control-center -m wlanconnect");
    } else {
        process.startDetached("ukui-control-center -m netconnect");
    }
}


void KnmInterface::openwLanNetworkSetting()
{
    if(m_pProcess) {
        m_pProcess->deleteLater();
    }

    m_pProcess = new QProcess();
    QString cmd = "ukui-control-center";
    QStringList arg;
    arg.clear();
    arg << "-m";
    arg << "wlanconnect";
    m_pProcess->startDetached(cmd,arg);
}

void KnmInterface::getWiredDevConnList(QString devName)
{
    m_currentWiredDevice = devName;
    m_wiredDevConnList = KNMDC::getInstance()->wiredDeviceConnList(devName);
    emit updateWiredDevConnList();
}

void KnmInterface::getWiredDevConnList()
{
    getWiredDevConnList(m_currentWiredDevice);
}

void KnmInterface::getWirelessDevConnList()
{
    getWirelessDevConnList(m_currentWirelessDevice);
}

/*全量更新*/
void KnmInterface::rebuildCurrentWirelessList()
{
    if(m_currentWirelessDevice.isEmpty()) {
        QMap<QString, NetDevicePtr>devMap=KNMDC::getInstance()->wirelessDeviceList();
        if(!devMap.isEmpty()) m_currentWirelessDevice=devMap.first()->devName();
        qWarning() << Q_FUNC_INFO <<__LINE__ << "set currentdevice"<<m_currentWirelessDevice;
     }
    m_wirelessDevConnList.clear();
    m_wirelessDevConnList=KNMDC::getInstance()->wirelessDeviceConnList(m_currentWirelessDevice);
    mWirelessConnecModel.refreshConnections(m_wirelessDevConnList);
    emit wirelessConListChanged();
}

/*增量更新*/
void KnmInterface::getWirelessDevConnList(QString devName)
{
    QVariantList conList;
    /*后端add与remove信号可能先于qml的设备名传递，造成崩溃，先做保护,后边要重新优化下，规避方案不影响实际效果，设备变化会重建列表不影响*/
    if(devName.isEmpty()) {
        qWarning() << Q_FUNC_INFO <<__LINE__ << devName<<"devname is empty";
        return;
     }
    m_currentWirelessDevice = devName;
    if(m_wirelessDevConnList.isEmpty()) {
        m_wirelessDevConnList=KNMDC::getInstance()->wirelessDeviceConnList(devName);
        mWirelessConnecModel.refreshConnections(m_wirelessDevConnList);
        emit updateWirelessDevConnList();
        emit wirelessConListChanged();
        return;
    }

    conList = KNMDC::getInstance()->wirelessDeviceConnList(devName);

    for(int i=0;i<m_wirelessDevConnList.count();i++)
    {
        if(conList.contains(m_wirelessDevConnList.at(i)))
        {
            continue;
        }

        mWirelessConnecModel.removeConnection(m_wirelessDevConnList.at(i).toMap().value("Name").toString());
        m_wirelessDevConnList.removeAt(i);
    }

    for(int i=0;i<conList.count();i++)
    {
        if(m_wirelessDevConnList.contains(conList.at(i)))
        {
            continue;
        }

        if (conList.at(i).toMap().value("State").toInt() == ACTIVATED
            || conList.at(i).toMap().value("State").toInt() == ACTIVATING)
        {
            m_wirelessDevConnList.push_front(conList.at(i));
        }
        else
        {
            m_wirelessDevConnList.append(conList.at(i));
        }
        WirelessConnectionModel::ST_ConnectionInfo con;
        con=mWirelessConnecModel.mapToConnectionInfo(conList.at(i).toMap());
        mWirelessConnecModel.addConnection(&con);
    }
    emit updateWirelessDevConnList();
}

QVariant KnmInterface::getLoadingIcon()
{
    QString icon = getIconData("ukui-loading-" + QString::number(loadCount) + "-symbolic");
    loadCount += 1;
    if (loadCount == 8)
        loadCount = 0;

    return icon;
}

QVariant KnmInterface::getWiFiIcon(QString signalStrength, QString security, QString isApConnection, int category)
{
    return KNMDC::getInstance()->getWiFiIcon(signalStrength, security, isApConnection, category);
}

void KnmInterface::activateConnect(QString devName, QString conUid, int type)
{
    loadTimer->start();
    KNMDC::getInstance()->activateConnect(devName, conUid, type);
}

void KnmInterface::deActivateConnect(QString devName, QString conUid, int type)
{
    loadTimer->start();
    KNMDC::getInstance()->deActivateConnect(devName, conUid, type);
}

void KnmInterface::setWiredMainSwitch(bool switched)
{
    qWarning() << Q_FUNC_INFO << __LINE__ << switched;
    KNMDC::getInstance()->setWiredMainSwitchEnable(switched);
}

void KnmInterface::setWirelessSwitch(bool switched)
{
    KNMDC::getInstance()->setWirelessSwitchEnable(switched);
}

void KnmInterface::setUpwareRateData(QString str)
{
    qDebug() << Q_FUNC_INFO <<__LINE__ << str;
    KNMDC::getInstance()->setUpwareRateData(str);
}

void KnmInterface::setDownwareRateData(QString str)
{
    qDebug() << Q_FUNC_INFO <<__LINE__ << str;
    //KNMDC::getInstance()->setDownwareRateData(str);
}

void KnmInterface::rescanWirelessConn()
{
    KNMDC::getInstance()->rescanWirelessConn();
}

void KnmInterface::passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect)
{
    KNMDC::getInstance()->passwordConnect(devName, ssid, type, psk, autoConnect);
}

void KnmInterface::showPropertyWidget(QString devName, QString ssid)
{
    qDebug() << Q_FUNC_INFO <<__LINE__ << devName << ssid;
    KNMDC::getInstance()->showPropertyWidget(devName, ssid);
}

void KnmInterface::deleteConnect(int type, QString ssid)
{
    qDebug() << Q_FUNC_INFO <<__LINE__ << type << ssid;
    KNMDC::getInstance()->deleteConnect(type, ssid);
}

void KnmInterface::showAddOtherWlanPage(QString devName)
{
    qDebug() << Q_FUNC_INFO <<__LINE__ << devName;
    KNMDC::getInstance()->showAddOtherWlanPage(devName);
}

/*
*   获取有线和无线的连接状态
*   1、无线有线都有连接
*   2、仅有有线连接
*   3、仅有无线连接
*   3、都未连接
*/
ConnectStatus KnmInterface::getConnectionStatus()
{
    auto wiredDev = KNMDC::getInstance()->wiredDeviceList();

    bool wiredConnect = false;
    for (auto iter : wiredDev) {
        if (!iter.isNull()) {
            if (!wiredConnect) {
                for (auto devices : iter->getConnections()) {
                    int status = devices.toMap().value("State").toInt();
                    if (2 == status) {    //2==已连接
                        wiredConnect = true;
                        break;
                    }
                }
            }
        }
        if (wiredConnect) {
            break;
        }
    }


    auto wirelessDev = KNMDC::getInstance()->wirelessDeviceList();
    bool wirelessConnect = false;

    for (auto iter : wirelessDev) {
        if (!iter.isNull()) {
            if (!wirelessConnect) {
                for (auto devices : iter->getConnections()) {
                    int status = devices.toMap().value("State").toInt();
                    if (2 == status) {    //2==已连接
                        wirelessConnect = true;
                        break;
                    }
                }
            }
        }
        if (wirelessConnect) {
            break;
        }
    }

    if (wiredConnect && wirelessConnect){
        return ConnectStatus::All;
    } else if (wiredConnect) {
        return ConnectStatus::Wire;
    } else if (wirelessConnect) {
        return ConnectStatus::Wireless;
    } else {
        return ConnectStatus::NoConnect;
    }
}

void KnmInterface::slotRefreshTimeout()
{
    rescanWirelessConn();
}

bool KnmInterface::getCableStatus()
{
    KNMDC::getInstance()->getCableStateByDevice("");
    return false;
}

bool KnmInterface::getCableStatusByDev(const QString &devName)
{
    KNMDC::getInstance()->getCableStateByDevice(devName);
    return false;
}
