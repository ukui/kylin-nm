    #include "knmdbuscaller.h"
#include "knminterface.h"

KnmDBusCaller::KnmDBusCaller(QObject *parent)
    : QObject{parent}
{
    KnmLanDataKeeperPtr ldk(new KnmLanDataKeeper(this));
    lanDataKeeper = ldk;
    KnmWlanDataKeeperPtr wdk(new KnmWlanDataKeeper(this));
    wlanDataKeeper = wdk;
    m_pInterface = new QDBusInterface("com.kylin.network",
                                     "/com/kylin/network",
                                     "com.kylin.network",
                                     QDBusConnection::sessionBus());
    if(!m_pInterface->isValid()) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

    qDBusRegisterMetaType<QVector<QStringList>>();

    this->connect(lanDataKeeper.data(), &KnmLanDataKeeper::onDeviceStatusChanged, this, &KnmDBusCaller::updateWiredDeviceMap);
    this->connect(wlanDataKeeper.data(), &KnmWlanDataKeeper::onDeviceStatusChanged, this, &KnmDBusCaller::updateWirelessDevice);

    getWiredDeviceMap();
    getWiredMainSwitchState();
    getWirelessSwitchState();
    getWirelessDeviceMap();

}

KnmDBusCaller::~KnmDBusCaller()
{

}

QMap<QString, NetDevicePtr> KnmDBusCaller::wiredDeviceList()
{
    return lanDataKeeper->getDeviceList();
}

QMap<QString, NetDevicePtr> KnmDBusCaller::wirelessDeviceList()
{
    return wlanDataKeeper->getDeviceList();
}

QVariantList KnmDBusCaller::wiredDeviceConnList(QString devName)
{
    return lanDataKeeper->getDevConnections(devName);
}

QVariantList KnmDBusCaller::wirelessDeviceConnList(QString devName)
{
    return wlanDataKeeper->getDevConnections(devName);
}

bool KnmDBusCaller::wirelessSwitchState()
{
    return wlanDataKeeper->getSwitchState();
}

bool KnmDBusCaller::wiredMainSwitchState()
{
    qWarning()<< Q_FUNC_INFO << __LINE__;

    return lanDataKeeper->getWiredMainSwitchState();
}

QString KnmDBusCaller::upwardRateDate()
{
    return lanDataKeeper->getUpwardRate();
}

QString KnmDBusCaller::downwardRateDate()
{
    return lanDataKeeper->getDownwardRate();
}

QString KnmDBusCaller::getWiFiIcon(QString signalStrength, QString security, QString isApConnection, int category)
{
    return wlanDataKeeper->getWiFiIcon(signalStrength, security, isApConnection, category);
}

QDBusPendingCallWatcher *KnmDBusCaller::asyncCall(const QString & methed, const QList<QVariant> & params)
{
    if (!m_pInterface->isValid()) {
        qWarning() << "not connect dbus server: " << m_pInterface->lastError().message();
        return nullptr;
    }

    QDBusPendingCall pendingCall = m_pInterface->asyncCallWithArgumentList(methed, params);

    return new QDBusPendingCallWatcher(pendingCall);
}

void KnmDBusCaller::activateConnect(QString devName, QString conUid, int type)
{
    QList<QVariant> list;
    list.append(type);
    list.append(devName);
    list.append(conUid);
    this->asyncCall("activateConnect", list);
}

void KnmDBusCaller::deActivateConnect(QString devName, QString conUid, int type)
{
    QList<QVariant> list;
    list.append(type);
    list.append(devName);
    list.append(conUid);
    this->asyncCall("deActivateConnect", list);
}

void KnmDBusCaller::passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect)
{
    QList<QVariant> list;
    list.append(devName);
    list.append(ssid);
    list.append(type);
    list.append(psk);
    list.append(autoConnect);
    this->asyncCall("passwordConnect", list);
}

void KnmDBusCaller::showPropertyWidget(QString devName, QString ssid)
{
    QList<QVariant> list;
    list.append(devName);
    list.append(ssid);
    this->asyncCall("showPropertyWidget", list);
}

void KnmDBusCaller::deleteConnect(int type, QString ssid)
{
    QList<QVariant> list;
    list.append(type);
    list.append(ssid);
    this->asyncCall("deleteConnect", list);
}

void KnmDBusCaller::getWirelessSwitchState()
{
    QList<QVariant> list;
    QDBusPendingCallWatcher * watcher = this->asyncCall("getWirelessSwitchBtnState", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWirelessSwitchStateFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWiredMainSwitchState()
{

    qDebug() << Q_FUNC_INFO << __LINE__;
    QList<QVariant> list;
    QDBusPendingCallWatcher * watcher = this->asyncCall("getWiredMainSwitchBtnState", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWiredMainSwitchStateFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWiredDeviceMap()
{
    QList<QVariant> list;
    list.append(0);
    QDBusPendingCallWatcher * watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWiredDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWirelessDeviceMap()
{
    QList<QVariant> list;
    list.append(1);
    QDBusPendingCallWatcher * watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWirelessDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::setWirelessSwitchEnable(bool enable)
{
    QList<QVariant> list;
    list.append(enable);
    QDBusPendingCallWatcher * watcher = this->asyncCall("setWirelessSwitchEnable", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::setWirelessSwitchEnableFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::setUpwareRateData(QString str)
{

}

void KnmDBusCaller::setDownwareRateData(QString str)
{

}

void KnmDBusCaller::updateWiredDeviceMap()
{
    QList<QVariant> list;
    list.append(0);
    QDBusPendingCallWatcher * watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::updateWiredDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::updateWirelessDeviceMap()
{
    QList<QVariant> list;
    list.append(1);
    QDBusPendingCallWatcher * watcher = this->asyncCall("getDeviceListAndEnabled", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::updateWirelessDeviceMapFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::updateWirelessDevice()
{
    getWirelessSwitchState();
    updateWirelessDeviceMap();

   // qWarning() << "mqtets updateWirelessDevice";
}

void KnmDBusCaller::rescanWirelessConn()
{
    QList<QVariant> list;
    this->asyncCall("reScan", list);
}

void KnmDBusCaller::getWiredConList(QString dev)
{
    QList<QVariant> list;
    list.append(dev);
    QDBusPendingCallWatcher * watcher = this->asyncCall("getWiredList", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *watcher) {
            getWiredConListFinished(watcher, dev);
        });
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWirelessConList(QString dev)
{
    QList<QVariant> list;
    list.append(dev);
    QDBusPendingCallWatcher * watcher = this->asyncCall("getWirelessList", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *watcher) {
            getWirelessConListFinished(watcher, dev);
        });
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::getWiredDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            while (item != retMap.value().cend()) {
                lanDataKeeper->addDevice(item.key(), item.value().toBool(),WIRED_DEVICE);
                getWiredConList(item.key());
                item++;
            }
            KInterface::getInstance()->updateWiredDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::updateWiredDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            lanDataKeeper->clearDevice();
            while (item != retMap.value().cend()) {
                lanDataKeeper->addDevice(item.key(), item.value().toBool(),WIRED_DEVICE);
                lanDataKeeper->clearConnectionList(item.key());
                getWiredConList(item.key());
                item++;
            }
            KInterface::getInstance()->updateWiredDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::updateWirelessDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            wlanDataKeeper->clearDevice();
            while (item != retMap.value().cend()) {
                wlanDataKeeper->addDevice(item.key(), item.value().toBool(),WIRELESS_DEVICE);
                wlanDataKeeper->clearConnectionList(item.key());
                updateWirelessConListSync(item.key());
                item++;
            }
            KInterface::getInstance()->rebuildCurrentWirelessList();//设备状态变更应该全量更新合理点
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::getWirelessDeviceMapFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {

        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantMap> retMap = reply;
            QVariantMap::const_iterator item = retMap.value().cbegin();
            while (item != retMap.value().cend()) {
                wlanDataKeeper->addDevice(item.key(), item.value().toBool(), WIRELESS_DEVICE);
                updateWirelessConListSync(item.key());
                item++;
            }
            KInterface::getInstance()->rebuildCurrentWirelessList();//设备状态变更应该全量更新合理点
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::getWiredConListFinished(QDBusPendingCallWatcher *watcher, QString dev)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantList> retList = reply;
            QList<QStringList> list;
            for (int j = 0; j < retList.value().size(); ++j) {
                list << retList.value().at(j).toStringList();
            }
            lanDataKeeper->addDevConnection(dev, list);
            KInterface::getInstance()->updateWiredDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::getWirelessConListFinished(QDBusPendingCallWatcher *watcher, QString dev)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<QVariantList> retList = reply;
            QList<QStringList> list;
            for (int j = 0; j < retList.value().size(); ++j) {
                list << retList.value().at(j).toStringList();
            }
            wlanDataKeeper->clearConnectionList(dev);
            wlanDataKeeper->addDevConnection(dev, list);
            KInterface::getInstance()->updateWirelessDeviceList();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::getWirelessSwitchStateFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<bool> retList = reply;
            wlanDataKeeper->setSwitchState(retList.value());
            KInterface::getInstance()->updateWirelessSwitch();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::getWiredMainSwitchStateFinished(QDBusPendingCallWatcher *watcher)
{
    qDebug() << Q_FUNC_INFO << __LINE__;
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    qDebug() << Q_FUNC_INFO << __LINE__ << reply;

    if(reply.type() == QDBusMessage::ReplyMessage) {
        if(reply.arguments().size() > 0) {
            QDBusReply<bool> retList = reply;
            lanDataKeeper->setWiredMainSwitchState(retList.value());
            KInterface::getInstance()->updateWiredMainSwitch();
        }
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::setWirelessSwitchEnableFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        updateWirelessDeviceMap();
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::setWiredMainSwitchEnable(bool enable)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << enable;
    QList<QVariant> list;
    list.append(enable);

    QDBusPendingCallWatcher * watcher = this->asyncCall("setWiredSwitchEnable", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::setWiredMainSwitchEnableFinished);
    }
    else {
        qWarning() << "null pending";
    }
}

void KnmDBusCaller::setWiredMainSwitchEnableFinished(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusMessage reply = watcher->reply();
    if(reply.type() == QDBusMessage::ReplyMessage) {
        getWiredMainSwitchState();
        updateWiredDeviceMap();
    } else {
        qWarning() << reply.errorMessage();
    }
}

void KnmDBusCaller::showAddOtherWlanPage(QString devName)
{
    QList<QVariant> list;
    list.append(devName);
    this->asyncCall("showAddOtherWlanWidget",list);
}


void KnmDBusCaller::updateWirelessConListSync(QString dev)
{
    QDBusReply<QVariantList> reply = m_pInterface->call(QStringLiteral("getWirelessList"), dev);
    //qDebug() << "[NetConnect]call getWirelessList respond"  << __LINE__;
    if(!reply.isValid()) {
        qWarning() << "getWirelessList error:" << reply.error().message();
        return;
    }

    QList<QStringList> list;
    for (int j = 0; j < reply.value().size(); ++j) {
        list << reply.value().at(j).toStringList();
    }
    wlanDataKeeper->clearConnectionList(dev);
    wlanDataKeeper->addDevConnection(dev, list);
    KInterface::getInstance()->updateWirelessDeviceList();
}


// void KnmDBusCaller::getWiredMainSwitchState()
// {
//     QList<QVariant> list;
//     QDBusPendingCallWatcher * watcher = this->asyncCall("getWirelessSwitchBtnState", list);
//     if(watcher) {
//         m_pendingCount++;
//         this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::getWirelessSwitchStateFinished);
//     }
//     else {
//         qWarning() << "null pending";
//     }
// }

/*
*   调用getCableStateByDevice dbus接口，返回值为 bool：是否插入网线
*/
void KnmDBusCaller::getCableStateByDevice(const QString &dev)
{
    QList<QVariant> list;
    list.append(dev);
    QDBusPendingCallWatcher * watcher = this->asyncCall("getCableStateByDevice", list);
    if(watcher) {
        m_pendingCount++;
        this->connect(watcher, &QDBusPendingCallWatcher::finished, this, &KnmDBusCaller::updateCableState);
    }
    else {
        qWarning() << "null pending";
    }
}

/* 
*  getCableStateByDevice dbus返回槽函数
*/
void KnmDBusCaller::updateCableState(QDBusPendingCallWatcher *watcher)
{
    m_pendingCount--;
    QDBusPendingReply<bool> reply = *watcher;
    if (!reply.isError()) {
        bool ret = reply.value();
        KInterface::getInstance()->updateCable(ret);
    } else {
        qWarning() << reply.isError();
    }
}