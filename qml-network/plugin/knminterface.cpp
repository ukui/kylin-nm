#include "knminterface.h"
#include "knmdbuscaller.h"
#include <QGSettings>

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

    // init font settings from org.ukui.style
    if (QGSettings::isSchemaInstalled("org.ukui.style")) {
        m_fontSettings = new QGSettings("org.ukui.style", QByteArray(), this);
        QVariant val = m_fontSettings->get("systemFontSize");
        if (val.isValid()) {
            m_fontSize = val.toString();
            emit fontSizeChanged(m_fontSize);
        }
        connect(m_fontSettings, &QGSettings::changed, this, [this](const QString &key) {
            if (key == "systemFontSize") {
                QString fontSize = m_fontSettings->get(key).toString();
                if (m_fontSize != fontSize) {
                    m_fontSize = fontSize;
                    emit fontSizeChanged(m_fontSize);
                }
            }
        });
    }
}

KnmInterface::~KnmInterface()
{
    qDebug() << Q_FUNC_INFO << __LINE__ ;
    if (nullptr != loadTimer) {
        delete loadTimer ;
        loadTimer = nullptr;
    }

    if (nullptr != m_pRefreshTimer) {
        delete m_pRefreshTimer ;
        m_pRefreshTimer = nullptr;
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

//这里是返回给qml连接信息
//fixbug 377873,已经连接的网卡需要在第一个显示
//遍历连接列表，将已经连接的网卡swap到第0个位置
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

//网络托盘-更多网络设置的跳转问题：
//1、若连接了无线网络且没有连接有线网络 跳转至无线网络
//2、否则跳转有线网络
void KnmInterface::openNetworkSetting()
{
    if(m_pProcess) {
         m_pProcess->deleteLater();
    }

    //获取当前的网络状态
    ConnectStatus connect_status = getConnectionStatus();

    QProcess process;
    if (connect_status == ConnectStatus::Wireless) {
        process.startDetached("ukui-control-center -m wlanconnect");
    } else if (connect_status == ConnectStatus::Wire || connect_status == ConnectStatus::All) {
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

void KnmInterface::setWirelessScanState(bool state)
{
    qWarning() << Q_FUNC_INFO << __LINE__ << state;
    if (state) {
        if(m_pRefreshTimer && !m_pRefreshTimer->isActive())
            m_pRefreshTimer->start();
    } else {
        if(m_pRefreshTimer && m_pRefreshTimer->isActive()) {
            qWarning() << Q_FUNC_INFO << __LINE__ << "========= m_pRefreshTimer set stop";

            m_pRefreshTimer->stop();
        }
    }
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

    if (wiredConnect && wirelessConnect) {
        return ConnectStatus::All;
    } else if (wiredConnect) {
        return ConnectStatus::Wire;
    } else if (wirelessConnect) {
        return ConnectStatus::Wireless;
    } else {
        if (wiredDev.count() > 0)
            return ConnectStatus::Wire;
        else if (wirelessDev.count() > 0)
            return ConnectStatus::Wireless;

        return ConnectStatus::NoConnect;
    }
}

void KnmInterface::slotRefreshTimeout()
{
    qWarning() << Q_FUNC_INFO << __LINE__ << "timeout..." ;
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

QString KnmInterface::fontSize()
{
    return m_fontSize;
}

int KnmInterface::changeSelectDevice(QString deviceName)
{
    int index=0;
    if(deviceName.isEmpty()) return -1;
    auto dev = KNMDC::getInstance()->wirelessDeviceList();

    for (auto it = dev.begin(); it != dev.end(); ++it) {
        if (it.key() == deviceName) {
            qDebug() << Q_FUNC_INFO <<__LINE__ << deviceName<<index;
            connect(this, &KnmInterface::wirelessConListChanged, this,&KnmInterface::passwdAgentChangeSelectSsid);
            emit changeSelectWirelessDevice(index);
            return index;
        }
        ++index;
    }

    return -1;
}

void KnmInterface::onRequestInputPasswdAgent(QString agentName,QVariantMap parm)
{
    QString inputSsid="";
    QString inputDevice="";

    if(parm.contains("ssid") && parm.contains("device")) {
        inputSsid=parm.value("ssid").toString();
        inputDevice=parm.value("device").toString();
        m_inputSsid=inputSsid;
        //多网卡需要切换的网卡为当前网卡则不需要等待网卡切换完毕直接跳到ssid 否则需要等切换完毕信号
        if(inputDevice!=m_currentWirelessDevice){
            if(changeSelectDevice(inputDevice)<0){
                qDebug() << Q_FUNC_INFO <<__LINE__ << "no inputDevice , invalid request";
                return;
            }
        } else {
            passwdAgentChangeSelectSsid();
        }

    } else {
        qDebug() << Q_FUNC_INFO <<__LINE__ << "no ssid , invalid request";
    }

    return ;
}

void KnmInterface::passwdAgentChangeSelectSsid()
{
    int index=0;
    index=mWirelessConnecModel.getConButtonFromSsid(m_inputSsid);
    if(index>=0) {
        emit triggerButtonRequested(index);
    } else {
        qDebug() << Q_FUNC_INFO <<__LINE__ << "no ssid , changeSelectSsid failed"<<m_inputSsid;
    }
    disconnect(this, &KnmInterface::wirelessConListChanged, this,&KnmInterface::passwdAgentChangeSelectSsid);
    m_inputSsid="";
    return ;
}

/*属性更新*/
void KnmInterface::wirelessDevConnListPropUpdate(QString devName,QString ssid)
{
    QVariantList conList;

    if(devName!=m_currentWirelessDevice && !m_currentWirelessDevice.isEmpty()) {
        qWarning() << Q_FUNC_INFO <<__LINE__ << devName << m_currentWirelessDevice;
        return;
     }

    if(m_wirelessDevConnList.isEmpty()) {
        m_wirelessDevConnList=KNMDC::getInstance()->wirelessDeviceConnList(devName);
        mWirelessConnecModel.refreshConnections(m_wirelessDevConnList);
        emit updateWirelessDevConnList();
        emit wirelessConListChanged();
        return;
    }

    conList = KNMDC::getInstance()->wirelessDeviceConnList(devName);

    for(int i=0;i<conList.count();i++){
        if (conList.at(i).toMap().value("Name").toString() == ssid){
            for(int j=0;j<m_wirelessDevConnList.count();j++){
                if (m_wirelessDevConnList.at(j).toMap().value("Name").toString() == ssid){
                    m_wirelessDevConnList.replace(j,conList.at(i));
                    WirelessConnectionModel::ST_ConnectionInfo con;
                    con=mWirelessConnecModel.mapToConnectionInfo(conList.at(i).toMap());

                    mWirelessConnecModel.replaceConnection(&con);
                    qDebug() << Q_FUNC_INFO <<__LINE__ << "replaceConnection"<<devName<<ssid;
                    break;
                }
            }
        }
    }
    emit updateWirelessDevConnList();
}
