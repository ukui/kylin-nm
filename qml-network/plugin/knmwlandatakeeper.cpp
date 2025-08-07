#include "knmwlandatakeeper.h"
#include "knminterface.h"

#define EXCELLENT_SIGNAL 80
#define GOOD_SIGNAL 55
#define OK_SIGNAL 30
#define LOW_SIGNAL 5
#define NONE_SIGNAL 0

#define SIGNAL_EXCELLENT 1
#define SIGNAL_GOOD      2
#define SIGNAL_OK        3
#define SIGNAL_LOW       4
#define SIGNAL_NONE      5
#define ICON_SIZE 16,16

const QString WIRELESS_SWITCH = "wirelessswitch";
const QByteArray GSETTINGS_SCHEMA = "org.ukui.kylin-nm.switch";

const QString KWifiSymbolic     = "network-wireless-signal-excellent";
const QString KWifiLockSymbolic = "network-wireless-secure-signal-excellent";
const QString KWifiGood         = "network-wireless-signal-good";
const QString KWifiLockGood     = "network-wireless-secure-signal-good";
const QString KWifiOK           = "network-wireless-signal-ok";
const QString KWifiLockOK       = "network-wireless-secure-signal-ok";
const QString KWifiLow          = "network-wireless-signal-low";
const QString KWifiLockLow      = "network-wireless-secure-signal-low";
const QString KWifiNone         = "network-wireless-signal-none";
const QString KWifiLockNone     = "network-wireless-secure-signal-none";

const QString KWifi6Symbolic        = "ukui-wifi6-full-symbolic";
const QString KWifi6PlusSymbolic    = "ukui-wifi6+-full-symbolic";

const QString KWifi6LockSymbolic    = "ukui-wifi6-full-pwd-symbolic";
const QString KWifi6PlusLockSymbolic= "ukui-wifi6+-full-pwd-symbolic";

const QString KWifi6Good            = "ukui-wifi6-high-symbolic";
const QString KWifi6PlusGood        = "ukui-wifi6+-high-symbolic";

const QString KWifi6LockGood        = "ukui-wifi6-high-pwd-symbolic";
const QString KWifi6PlusLockGood    = "ukui-wifi6+-high-pwd-symbolic";

const QString KWifi6OK              = "ukui-wifi6-medium-symbolic";
const QString KWifi6PlusOK          = "ukui-wifi6+-high-medium-symbolic";

const QString KWifi6LockOK          = "ukui-wifi6-medium-pwd-symbolic";
const QString KWifi6PlusLockOK      = "ukui-wifi6+-medium-pwd-symbolic";

const QString KWifi6Low             = "ukui-wifi6-low-symbolic";
const QString KWifi6PlusLow         = "ukui-wifi6+-low-symbolic";

const QString KWifi6LockLow         = "ukui-wifi6-low-pwd-symbolic";
const QString KWifi6PlusLockLow     = "ukui-wifi6+-low-pwd-symbolic";

const QString KWifi6None            = "ukui-wifi6-none-symbolic";
const QString KWifi6PlusNone        = "ukui-wifi6+-none-symbolic";

const QString KWifi6LockNone        = "ukui-wifi6-none-pwd-symbolic";
const QString KWifi6PlusLockNone    = "ukui-wifi6+-none-pwd-symbolic";

const QString KLanSymbolic      = ":/img/plugins/netconnect/eth.svg";
const QString NoNetSymbolic     = ":/img/plugins/netconnect/nonet.svg";

const QString KApSymbolic       = "network-wireless-hotspot-symbolic";

const QString IsApConnection    = "1";

KnmWlanDataKeeper::KnmWlanDataKeeper(QObject *parent) : KnmDataKeeper(parent)
{
    m_pInterface = new QDBusInterface("com.kylin.network",
                                     "/com/kylin/network",
                                     "com.kylin.network",
                                     QDBusConnection::sessionBus());
    if(!m_pInterface->isValid()) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

    qDBusRegisterMetaType<QVector<QStringList>>();

    // 无线网络断开或连接时刷新可用网络列表
    connect(m_pInterface, SIGNAL(wlanactiveConnectionStateChanged(QString, QString, QString, int)), this, SLOT(onActiveConnectionChanged(QString, QString, QString, int)), Qt::QueuedConnection);

    //无线网络新增时添加网络
    connect(m_pInterface, SIGNAL(wlanAdd(QString, QStringList)), this, SLOT(onNetworkAdd(QString, QStringList)), Qt::QueuedConnection);

    //删除无线网络
    connect(m_pInterface, SIGNAL(wlanRemove(QString, QString)), this, SLOT(onNetworkRemove(QString, QString)), Qt::QueuedConnection);

    //网卡插拔处理
    connect(m_pInterface, SIGNAL(wirelessDeviceStatusChanged()), this, SIGNAL(onDeviceStatusChanged()), Qt::QueuedConnection);

    //网卡name处理
    connect(m_pInterface, SIGNAL(deviceNameChanged(QString, QString, int)), this, SLOT(onDeviceNameChanged(QString, QString, int)), Qt::QueuedConnection);

    //无线开关处理
    connect(m_pInterface, SIGNAL(wirelessSwitchBtnChanged(bool)), this, SLOT(onSwitchBtnChanged(bool)), Qt::QueuedConnection);

    //更新列表顺序
    //对部分QML功能有影响，待完善
//    connect(m_pInterface, SIGNAL(timeToUpdate()), this, SLOT(updateList()), Qt::QueuedConnection);

    netSpeedInit();
}

KnmWlanDataKeeper::~KnmWlanDataKeeper()
{

}

QMap<QString, QVariant> KnmWlanDataKeeper::makeConnectionMap(int status, QStringList conPath)
{
    //属性个数偏移量,已连接的WiFi比未连接的WiFi属性多一个uuid

    qWarning() << Q_FUNC_INFO << __LINE__ << "status:" << status << "conPath:" << conPath;
    int index = 0;

    QMap<QString, QVariant> connectionMap;
    connectionMap.insert("Name", conPath.at(0));
    connectionMap.insert("Signal", conPath.at(1));
    connectionMap.insert("Security", conPath.at(2));

    /*这个7根据后端链表每个项添加的参数来的，改了后端需要改这里，这里补充判断是否大>=7起保护作用*/
    if (status == ACTIVATED || conPath.count()>=9) {
        connectionMap.insert("Uuid", conPath.at(3));
        index = 1;
    }

    connectionMap.insert("isApConn", conPath.at(3 + index));
    connectionMap.insert("category", conPath.at(4 + index).toInt());
    connectionMap.insert("State", status);
    connectionMap.insert("Loading", false);
    connectionMap.insert("Configured", conPath.at(5 + index).toInt());
    connectionMap.insert("frequency", conPath.at(6 + index).toUInt());
    connectionMap.insert("isMix", conPath.at(7 + index).toInt());

    return connectionMap;
}

int KnmWlanDataKeeper::setSignal(QString sg)
{
    int signal = sg.toInt();
    int signalLv = 0;

    if (signal > EXCELLENT_SIGNAL) {
        signalLv = 1;
    } else if (signal > GOOD_SIGNAL) {
        signalLv = 2;
    } else if (signal > OK_SIGNAL) {
        signalLv = 3;
    } else if (signal > LOW_SIGNAL) {
        signalLv = 4;
    } else {
        signalLv = 5;
    }
    return signalLv;
}

QString KnmWlanDataKeeper::getWiFiIconString(bool isLock, int strength, int category)
{
    if (category == 0) {
        switch (strength) {
        case SIGNAL_EXCELLENT:
            return isLock ? KWifiLockSymbolic : KWifiSymbolic;
        case SIGNAL_GOOD:
            return isLock ? KWifiLockGood : KWifiGood;
        case SIGNAL_OK:
            return isLock ? KWifiLockOK : KWifiOK;
        case SIGNAL_LOW:
            return isLock ? KWifiLockLow : KWifiLow;
        case SIGNAL_NONE:
            return isLock ? KWifiLockNone : KWifiNone;
        default:
            return "";
        }
    } else if (category == 1) {
        switch (strength) {
        case SIGNAL_EXCELLENT:
            return isLock ? KWifi6LockSymbolic : KWifi6Symbolic;
        case SIGNAL_GOOD:
            return isLock ? KWifi6LockGood : KWifi6Good;
        case SIGNAL_OK:
            return isLock ? KWifi6LockOK : KWifi6OK;
        case SIGNAL_LOW:
            return isLock ? KWifi6LockLow : KWifi6Low;
        case SIGNAL_NONE:
            return isLock ? KWifi6LockNone : KWifi6None;
        default:
            return "";
        }
    } else {
            switch (strength) {
            case SIGNAL_EXCELLENT:
                return isLock ? KWifi6PlusLockSymbolic : KWifi6PlusSymbolic;
            case SIGNAL_GOOD:
                return isLock ? KWifi6PlusLockGood : KWifi6PlusGood;
            case SIGNAL_OK:
                return isLock ? KWifi6PlusLockOK : KWifi6PlusOK;
            case SIGNAL_LOW:
                return isLock ? KWifi6PlusLockLow : KWifi6PlusLow;
            case SIGNAL_NONE:
                return isLock ? KWifi6PlusLockNone : KWifi6PlusNone;
            default:
                return "";
            }
        }
}

QString KnmWlanDataKeeper::getWiFiIcon(QString signalStrength, QString security, QString isApConnection, int category)
{
    qDebug() << "sig: " << signalStrength << "  sec: " << security << " isAp: " << isApConnection << " cat: " << category;
    int sign = setSignal(signalStrength);
    bool isLock = true;
    if (security.isEmpty()) {
        isLock = false;
    } else {
        isLock = true;
    }

    QString iconamePath;
    if (isApConnection == IsApConnection) {
        iconamePath = KApSymbolic;
    } else {
        iconamePath = getWiFiIconString(isLock, sign, category);
    }
    return iconamePath;
}

void KnmWlanDataKeeper::addDevConnection(QString devName, QList<QStringList> conPath)
{
    qDebug() << Q_FUNC_INFO << __LINE__
             << "devName:" << devName
             << "conPath:" << conPath;

    if (conPath.isEmpty())
        return;

    NetDevicePtr dev = m_deviceList.take(devName);

    if (!conPath.at(0).contains("--")) {
        dev->addConnection(makeConnectionMap(ACTIVATED, conPath.at(0)));
        m_connected = true;
    }

    conPath.takeAt(0);

    for (int i = 0; i < conPath.count(); i++)
    {
        dev->addConnection(makeConnectionMap(DEACTIVATED, conPath.at(i)));
    }

    m_deviceList.insert(devName, dev);
}

/*连接状态变化全量更新*/
void KnmWlanDataKeeper::onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, int status)
{
    qDebug() << Q_FUNC_INFO << __LINE__
             << "deviceName:" << deviceName
             << "ssid:" << ssid
             << "uuid:" << uuid
             << "status:" << status;
    QMap<QString, QVariant>valueMap;
    if (!m_switchState)
        return;

    if (uuid.isEmpty())
        return;

    if (deviceName.isEmpty() || ssid.isEmpty()) {
        if (status == ACTIVATING || status == ACTIVATED) {
            return;
        }
        QStringList devList = m_deviceList.keys();
        for (int i = 0; i < devList.count(); i++) {
            QVariantList connList = m_deviceList.value(devList.at(i))->getConnections();
            valueMap=connList.at(0).toMap();
            if (!valueMap.contains("Uuid"))
                continue;
            if (valueMap.value("Uuid").toString() != uuid)
                continue;
            NetDevicePtr dev = m_deviceList.value(devList.at(i));
            QMap<QString, QVariant> conn = dev->updateConnection(uuid, status);
            if (status == DEACTIVATED) {
                dev->removeConnection(conn);
                if (conn.contains("Uuid"))
                    conn.remove("Uuid");
                dev->addConnection(conn);
            }
        }

        KInterface::getInstance()->rebuildCurrentWirelessList();
        return;
    }

    if (!m_deviceList.contains(deviceName))
        return;

    if(m_deviceList.value(deviceName)->containsConnectionName(ssid))
    {
        NetDevicePtr dev = m_deviceList.value(deviceName);
        if (status == ACTIVATED || status == ACTIVATING) {
            QMap<QString, QVariant> conn = dev->getConnectionByName(ssid);
            dev->removeConnection(conn);
            if (conn.contains("Uuid"))
                conn.remove("Uuid");
            conn.insert("Uuid", uuid);
            conn.remove("State");
            conn.insert("State", status);
            dev->addConnection(conn);

            KInterface::getInstance()->rebuildCurrentWirelessList();
            return;
        }

        if (status == DEACTIVATED) {
            QMap<QString, QVariant> conn = dev->getConnectionByName(ssid);
            dev->removeConnection(conn);
            if (conn.contains("Uuid"))
                conn.remove("Uuid");
            conn.remove("State");
            conn.insert("State", status);
            dev->addConnection(conn);
 
            KInterface::getInstance()->rebuildCurrentWirelessList();
            return;
        }
    }
    valueMap=m_deviceList.value(deviceName)->getConnections().at(0).toMap();
    if (uuid == valueMap.value("Uuid").toString()) {
        NetDevicePtr dev = m_deviceList.value(deviceName);
        QMap<QString, QVariant> conn = dev->getConnections().at(0).toMap();
        dev->removeConnection(conn);
        conn.remove("State");
        conn.insert("State", status);
        if (conn.contains("Uuid"))
            conn.remove("Uuid");
        if (status != DEACTIVATED)
            conn.insert("Uuid", uuid);
        dev->addConnection(conn);
        KInterface::getInstance()->rebuildCurrentWirelessList();
    }
}

void KnmWlanDataKeeper::onNetworkAdd(QString deviceName, QStringList wlanInfo)
{
    qDebug() << Q_FUNC_INFO << __LINE__ << deviceName << wlanInfo;
    if (!m_switchState || deviceName.isEmpty())
        return;

    if (!m_deviceList.contains(deviceName))
        return;

    qDebug() << deviceName << wlanInfo << wlanInfo.count();
    NetDevicePtr dev = m_deviceList.value(deviceName);
    /*根据后端链表每个项添加的参数来的，改了后端需要改这里，只有6项表示未连接，7项表示已连接；后面需要优化可一样的项数通过赋特定空值区分，不然维护,容易改漏且容易歧义*/
    if (wlanInfo.count() == 6)
        dev->addConnection(makeConnectionMap(DEACTIVATED, wlanInfo));
    if (wlanInfo.count() == 7)
        dev->addConnection(makeConnectionMap(ACTIVATED, wlanInfo));
    KInterface::getInstance()->getWirelessDevConnList();
}

void KnmWlanDataKeeper::onNetworkRemove(QString deviceName, QString wlanName)
{
    if (deviceName.isEmpty())
        return;

    if (!m_deviceList.contains(deviceName))
        return;

    NetDevicePtr dev = m_deviceList.value(deviceName);
    dev->removeConnectionByName(wlanName);
    KInterface::getInstance()->getWirelessDevConnList();
}

void KnmWlanDataKeeper::onDeviceNameChanged(QString oldName, QString newName, int type)
{
    if (type != WIRELESS_DEVICE)
        return;

    if (!m_deviceList.contains(oldName))
        return;

    if (m_deviceList.keys().contains(newName))
        return;

    emit onDeviceStatusChanged();
}

void KnmWlanDataKeeper::onSwitchBtnChanged(bool status)
{
    m_switchState = status;
    KInterface::getInstance()->updateWirelessSwitch();
}

//对部分QML功能有影响，待完善
void KnmWlanDataKeeper::updateList()
{
    if (!m_switchState)
        return;

//    emit onDeviceStatusChanged();
}
