#include "knminterface.h"
#include "knmdbuscaller.h"
#include "uisecurityconfig.h"
#include <QGSettings>

enum E_KylinDeviceType{
    KEYLIN_NC_NONE= 0, //无
    KEYLIN_NC_WIRED=1ul<<0,//有线
    KEYLIN_NC_WIRELESS=1ul<<1,//无线

    KEYLIN_NC_ALL=KEYLIN_NC_WIRED|KEYLIN_NC_WIRELESS,
};

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
    initNetCtrl();
    componentSettings();
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

    //设置默认有线网卡
    KNMDC::getInstance()->setDefaultWiredDevice(devName);

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
        if(!devMap.isEmpty() && !devMap.first().isNull()) {
            m_currentWirelessDevice=devMap.first()->devName();
        }
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

    QVariantList conList = KNMDC::getInstance()->wirelessDeviceConnList(devName);
    qDebug() << Q_FUNC_INFO <<__LINE__ << "conList:"<<conList;

    for(int i=0;i<m_wirelessDevConnList.count();i++)
    {
        if(conList.contains(m_wirelessDevConnList.at(i)))
        {
            continue;
        }

        mWirelessConnecModel.removeConnection(m_wirelessDevConnList.at(i).toMap().value("Name").toString());
        m_wirelessDevConnList.removeAt(i);
    }

    // 处理新增和更新的连接（保持排序）
    for(int i = 0; i < conList.count(); i++) {
        QVariantMap newConn = conList.at(i).toMap();
        QString newSsid = newConn.value("Name").toString();
        int newStatus = newConn.value("State").toInt();
        int newConfigured = newConn.value("Configured", 0).toInt();
        int newSignal = newConn.value("Signal", "0").toString().toInt();

        // 检查是否已存在
        bool exists = false;
        int existingIndex = -1;
        for(int j = 0; j < m_wirelessDevConnList.count(); j++) {
            QVariantMap existingConn = m_wirelessDevConnList.at(j).toMap();
            QString existingSsid = existingConn.value("Name").toString();
            if(existingSsid == newSsid) {
                exists = true;
                existingIndex = j;
                break;
            }
        }

        WirelessConnectionModel::ST_ConnectionInfo con = mWirelessConnecModel.mapToConnectionInfo(newConn);
        if(exists) {
            // 更新现有连接
            m_wirelessDevConnList.replace(existingIndex, newConn);
            mWirelessConnecModel.replaceConnection(&con);
        } else {
            // 插入新连接（按排序规则）
            bool inserted = false;

            // 如果是激活状态，插入到最前面
            if(newStatus == ACTIVATED || newStatus == ACTIVATING) {
                m_wirelessDevConnList.push_front(newConn);
                mWirelessConnecModel.addConnection(0, &con);
                inserted = true;
            } else {
                // 按排序规则找到插入位置；规则：已激活的连接在最前面，然后是已配置的连接，最后是未配置的连接；在同一类别中按信号强度降序排序，信号相同时新增的放前面。
                for(int j = 0; j < m_wirelessDevConnList.count(); j++) {
                    QVariantMap existingConn = m_wirelessDevConnList.at(j).toMap();
                    int existingStatus = existingConn.value("State").toInt();

                    // 跳过已激活的连接（它们已经在最前面）
                    if(existingStatus == ACTIVATED || existingStatus == ACTIVATING) {
                        continue;
                    }

                    int existingConfigured = existingConn.value("Configured", 0).toInt();
                    int existingSignal = existingConn.value("Signal", "0").toString().toInt();

                    // 按排序规则比较
                    if(newConfigured > existingConfigured) {
                        // 新连接已配置，现有连接未配置，插入此处
                        m_wirelessDevConnList.insert(j, newConn);
                        mWirelessConnecModel.addConnection(j, &con);
                        inserted = true;
                        break;
                    } else if(newConfigured == existingConfigured) {
                        // 配置状态相同，按信号强度排序；信号相同时新增的放前面
                        if(newSignal >= existingSignal) {
                            m_wirelessDevConnList.insert(j, newConn);
                            mWirelessConnecModel.addConnection(j, &con);
                            inserted = true;
                            break;
                        }
                    }
                    // 如果新连接未配置，而已有连接已配置，则继续向后查找，直到找到未配置的连接，然后比较信号强度
                }
            }

            // 如果没找到合适位置，则添加到末尾
            if(!inserted) {
                int index = m_wirelessDevConnList.count();
                m_wirelessDevConnList.append(newConn);
                mWirelessConnecModel.addConnection(index, &con);
            }
        }
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
    qDebug() << Q_FUNC_INFO << __LINE__ << state;
    if (state) {
        rescanWirelessConn();
        if(m_pRefreshTimer && !m_pRefreshTimer->isActive()) {
            m_pRefreshTimer->start();
        }
    } else {
        if(m_pRefreshTimer && m_pRefreshTimer->isActive()) {
            qDebug() << Q_FUNC_INFO << __LINE__ << "m_pRefreshTimer set stop";
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
    bool wiredDeviceExists = false;  // 添加标志判断是否有真实设备

    for (auto iter : wiredDev) {
        if (!iter.isNull()) {
            wiredDeviceExists = true;  // 有真实设备存在
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
    bool wirelessDeviceExists = false;  // 添加标志判断是否有真实设备

    for (auto iter : wirelessDev) {
        if (!iter.isNull()) {
            wirelessDeviceExists = true;  // 有真实设备存在
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

    qWarning() << Q_FUNC_INFO << __LINE__ << " wiredConnect " << wiredConnect
               << "wirelessConnect:" << wirelessConnect
               << "wiredDeviceExists:" << wiredDeviceExists
               << "wirelessDeviceExists:" << wirelessDeviceExists;

    if (wiredConnect && wirelessConnect) {
        return ConnectStatus::All;
    } else if (wiredConnect) {
        return ConnectStatus::Wire;
    } else if (wirelessConnect) {
        return ConnectStatus::Wireless;
    } else {
        // 修改判断逻辑，基于是否有真实设备而不是Map元素数量
        if (wiredDeviceExists) {
            return ConnectStatus::Wire;  // 有有线设备但未连接
        } else if (wirelessDeviceExists) {
            return ConnectStatus::Wireless;  // 有无线设备但未连接
        }

        return ConnectStatus::NoConnect;  // 没有任何网络设备
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
    qDebug() << Q_FUNC_INFO << __LINE__ << " devName :" << devName << " ssid :" << ssid;
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

void KnmInterface::setNetworkConnectAutoConnectState(int type, QString uuid, bool state)
{
    qWarning() << Q_FUNC_INFO << __LINE__ << type << uuid << state;

    // 调用后端接口
    KNMDC::getInstance()->setNetworkConnectionAutoConnectState(type, uuid, state);

    // 更新 m_wirelessDevConnList
    for(int j = 0; j < m_wirelessDevConnList.count(); j++) {
        QVariantMap connMap = m_wirelessDevConnList.at(j).toMap();
        if (connMap.value("Uuid").toString() == uuid) {
            // 创建新的map并更新autoConnect值
            QVariantMap newConnMap = connMap;
            newConnMap["autoConnect"] = state;
            qWarning() << Q_FUNC_INFO << __LINE__ << "DtTest ======:" << newConnMap;

            // 替换列表中的项
            m_wirelessDevConnList.replace(j, newConnMap);

            // 更新模型
            WirelessConnectionModel::ST_ConnectionInfo con;
            con = mWirelessConnecModel.mapToConnectionInfo(newConnMap);
            mWirelessConnecModel.replaceConnection(&con);

            qDebug() << "Updated autoConnect for UUID:" << uuid << "to:" << state;
            break;
        }
    }

    // 发射信号通知UI更新
    emit updateWirelessDevConnList();
    emit wirelessConListChanged();
}


QVariantMap KnmInterface::getUiCtlData()
{
    qInfo()<<"[updateNetCtrl]"<<m_uiCtlData<<Q_FUNC_INFO<<__LINE__;
    return m_uiCtlData;
}

void KnmInterface::updateNetCtrl(QString modName,QVariantMap value)
{
    bool enable=false;

    if(modName!="Connect") return;

    qInfo()<<"[WlanPage]"<<modName<<value;
    for (auto it = value.cbegin(); it != value.cend(); ++it) {
        QString key = it.key();
        QVariant value = it.value();
        if(key==QString("addConnectCtrol")) {

            m_uiCtlData["wlanAddButton"]=((value.toUInt()&KEYLIN_NC_WIRELESS)==KEYLIN_NC_WIRELESS)? false:true;//为了m_uiCtlData参数意义一致 所以此处管控时wlanAddButton为false
            emit updateUiCtlData(m_uiCtlData);
            qInfo()<<"[updateNetCtrl]"<<m_uiCtlData;
        }
    }
    return;
}

void KnmInterface::initNetCtrl()
{
    QVariantMap map;
    int errCode=0;
    m_uiCtlData["wlanAddButton"]=true;//初始化管控为true
    QString netCtrlConnectName="Connect";
    QDBusInterface dbusInterface("com.kylin.networkCtrol",
                                 "/com/kylin/networkCtrol",
                                 "com.kylin.networkCtrol",
                                 QDBusConnection::systemBus());
    if (!dbusInterface.isValid()) {
        qWarning()<<Q_FUNC_INFO<<__LINE__<<"dbusInterface error!";
    } else {
        dbusInterface.setTimeout(2000);
        QDBusMessage result = dbusInterface.call("getNetContrlRule",netCtrlConnectName);
        if(result.type() == QDBusMessage::ErrorMessage) {
            qWarning() << "[WlanPage]getNetContrlRule error:" << result.errorMessage();
        } else {
            if( result.arguments().size()>=2) {
                const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
                dbusArg1st >> map;
                errCode = result.arguments().at( 1 ).toInt();
                qInfo()<<"[WlanPage]"<<map<<errCode;
                if(errCode==0) updateNetCtrl(netCtrlConnectName,map);
                map.clear();
            }
        }
    }

    QDBusConnection::systemBus().connect("com.kylin.networkCtrol",
                                         "/com/kylin/networkCtrol",
                                         "com.kylin.networkCtrol",
                                         "sigNetContrlRuleChanged",
                                         this,
                                         SLOT(updateNetCtrl(QString ,QVariantMap)));

    qInfo()<<"[WlanPage] initNetCtrl success";
    return;
}

void KnmInterface::componentSettings()
{
    m_uiCtlData["netMainSwitch"]=true;//初始化管控为true
    QVariant configData=UiSecurityConfig::getInstance()->getConnectSettingsData("netconnect","netconnectSettings");
    SDK_TYPE_PROJECT projectId=UiSecurityConfig::getInstance()->getProjectIdentity();
    QString settings=configData.toString();

    if (settings.contains("netMainSwitch:false") || projectId==SDK_TYPE_YDSYY){
       qInfo() << Q_FUNC_INFO << __LINE__ << "netMainSwitch:false";
       m_uiCtlData["netMainSwitch"]=false;
       emit updateUiCtlData(m_uiCtlData);
    }
}
