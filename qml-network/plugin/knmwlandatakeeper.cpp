#include "knmwlandatakeeper.h"
#include "knminterface.h"
#include "knmdbuscaller.h"

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

const QString KWifiSymbolic     = "network-wireless-signal-excellent-symbolic";
const QString KWifiLockSymbolic = "network-wireless-signal-excellent-secure-symbolic";
const QString KWifiGood         = "network-wireless-signal-good-symbolic";
const QString KWifiLockGood     = "network-wireless-signal-good-secure-symbolic";
const QString KWifiOK           = "network-wireless-signal-ok-symbolic";
const QString KWifiLockOK       = "network-wireless-signal-ok-secure-symbolic";
const QString KWifiLow          = "network-wireless-signal-weak-symbolic";
const QString KWifiLockLow      = "network-wireless-signal-weak-secure-symbolic";
const QString KWifiNone         = "network-wireless-signal-none-symbolic";
const QString KWifiLockNone     = "network-wireless-signal-none-secure-symbolic";


const QString KWifi6Symbolic        = "ukui-wifi6-full-symbolic";
const QString KWifi6PlusSymbolic    = "ukui-wifi6+-full-symbolic";
const QString KWifi7Symbolic        = "ukui-wifi7-full-symbolic";

const QString KWifi6LockSymbolic    = "ukui-wifi6-full-pwd-symbolic";
const QString KWifi6PlusLockSymbolic= "ukui-wifi6+-full-pwd-symbolic";
const QString KWifi7LockSymbolic    = "ukui-wifi7-full-pwd-symbolic";

const QString KWifi6Good            = "ukui-wifi6-high-symbolic";
const QString KWifi6PlusGood        = "ukui-wifi6+-high-symbolic";
const QString KWifi7Good            = "ukui-wifi7-high-symbolic";

const QString KWifi6LockGood        = "ukui-wifi6-high-pwd-symbolic";
const QString KWifi6PlusLockGood    = "ukui-wifi6+-high-pwd-symbolic";
const QString KWifi7LockGood        = "ukui-wifi7-high-pwd-symbolic";

const QString KWifi6OK              = "ukui-wifi6-medium-symbolic";
const QString KWifi6PlusOK          = "ukui-wifi6+-high-medium-symbolic";
const QString KWifi7OK              = "ukui-wifi7-medium-symbolic";

const QString KWifi6LockOK          = "ukui-wifi6-medium-pwd-symbolic";
const QString KWifi6PlusLockOK      = "ukui-wifi6+-medium-pwd-symbolic";
const QString KWifi7LockOK          = "ukui-wifi7-medium-pwd-symbolic";

const QString KWifi6Low             = "ukui-wifi6-low-symbolic";
const QString KWifi6PlusLow         = "ukui-wifi6+-low-symbolic";
const QString KWifi7Low             = "ukui-wifi7-low-symbolic";

const QString KWifi6LockLow         = "ukui-wifi6-low-pwd-symbolic";
const QString KWifi6PlusLockLow     = "ukui-wifi6+-low-pwd-symbolic";
const QString KWifi7LockLow         = "ukui-wifi7-low-pwd-symbolic";

const QString KWifi6None            = "ukui-wifi6-none-symbolic";
const QString KWifi6PlusNone        = "ukui-wifi6+-none-symbolic";
const QString KWifi7None            = "ukui-wifi7-none-symbolic";

const QString KWifi6LockNone        = "ukui-wifi6-none-pwd-symbolic";
const QString KWifi6PlusLockNone    = "ukui-wifi6+-none-pwd-symbolic";
const QString KWifi7LockNone        = "ukui-wifi7-none-pwd-symbolic";

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

    //network属性更新
    connect(m_pInterface, SIGNAL(networkProChanged(QString, QString, int)), this, SLOT(onNetworkProChanged(QString, QString, int)), Qt::QueuedConnection);

    //无线开关处理
    connect(m_pInterface, SIGNAL(wirelessSwitchBtnChanged(bool)), this, SLOT(onSwitchBtnChanged(bool)), Qt::QueuedConnection);

    //输入接管请求 使用object connect对于QVariantMap可能不识别，需使用QDBusConnection connect
    //connect(m_pInterface, SIGNAL(sigRequestInputPasswdAgent(QString ,QVariantMap )), this, SLOT(onRequestInputPasswdAgent(QString ,QVariantMap )), Qt::QueuedConnection);
    QDBusConnection::sessionBus().connect("com.kylin.network",
                                         "/com/kylin/network",
                                         "com.kylin.network",
                                         "sigRequestInputPasswdAgent",
                                         this,
                                         SLOT(onRequestInputPasswdAgent(QString ,QVariantMap)));

    QDBusConnection::sessionBus().connect("com.kylin.network",
                                         "/com/kylin/network",
                                         "com.kylin.network",
                                         "sigNetworkPropChanged",
                                         this,
                                         SLOT(onNetworkPropChanged(QVariantMap)));

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

    //qWarning() << Q_FUNC_INFO << __LINE__ << "status:" << status << "conPath:" << conPath;
    int index = 0;

    QMap<QString, QVariant> connectionMap;
    connectionMap.insert("Name", conPath.at(0));
    connectionMap.insert("Signal", conPath.at(1));
    connectionMap.insert("Security", conPath.at(2));

    /*这个10根据后端链表每个项添加的参数来的，改了后端需要改这里，这里补充判断是否大>=10起保护作用*/
    if (status == ACTIVATED || conPath.count()>=10) {
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
    connectionMap.insert("autoConnect", conPath.at(8 + index).toInt());

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
    } else if (category == 3) {//显示wifi7图标
        switch (strength)
        {
        case SIGNAL_EXCELLENT:
            return isLock ? KWifi7LockSymbolic : KWifi7Symbolic;
        case SIGNAL_GOOD:
            return isLock ? KWifi7LockGood : KWifi7Good;
        case SIGNAL_OK:
            return isLock ? KWifi7LockOK : KWifi7OK;
        case SIGNAL_LOW:
            return isLock ? KWifi7LockLow : KWifi7Low;
        case SIGNAL_NONE:
            return isLock ? KWifi7LockNone : KWifi7None;
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

    if (!m_deviceList.contains(deviceName)){
        qDebug() << Q_FUNC_INFO << __LINE__<< "not such wireless dev" << deviceName;
        return;
    }
    if (status == ACTIVATED || status == ACTIVATING) {

        if(m_deviceList.value(deviceName)->containsConnectionName(ssid))
        {
            NetDevicePtr dev = m_deviceList.value(deviceName);
            QMap<QString, QVariant> conn = dev->getConnectionByName(ssid);
            dev->removeConnection(conn);
            if (conn.contains("Uuid"))
                conn.remove("Uuid");
            conn.insert("Uuid", uuid);
            conn.remove("State");
            conn.insert("State", status);

            if(status == ACTIVATED) {
                conn["Configured"]=1;
            }
            dev->addConnection(conn);
        }
        KInterface::getInstance()->rebuildCurrentWirelessList();
        return;
    }

    QStringList devList = m_deviceList.keys();
    for (int i = 0; i < devList.count(); i++) {
        NetDevicePtr dev = m_deviceList.value(devList.at(i));
        QMap<QString, QVariant> conn = dev->updateConnection(uuid, status);
        if(conn.isEmpty()){
            continue;
        }
        qDebug() << Q_FUNC_INFO << __LINE__
                 << "deviceName:" << dev->devName()
                 << "ssid:" << conn["Name"]
                 << "uuid:" << conn["Uuid"]
                 << "status:" << status;
        if (status == DEACTIVATED) {
            /*临时规避方案这里可能存在dbus调用阻塞，待正向把老kylin-nm数据同步问题解决后解*/
            KNMDC::getInstance()->updateWirelessConListSync(dev->devName());
        }
    }

    KInterface::getInstance()->rebuildCurrentWirelessList();
    return;

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
    /*根据后端链表每个项添加的参数来的，改了后端需要改这里，只有10项表示未连接，11项表示已连接；后面需要优化可一样的项数通过赋特定空值区分，不然维护,容易改漏且容易歧义*/
    if (wlanInfo.count() == 10)
        dev->addConnection(makeConnectionMap(DEACTIVATED, wlanInfo));
    if (wlanInfo.count() == 11)
        dev->addConnection(makeConnectionMap(DEACTIVATED, wlanInfo));
    KInterface::getInstance()->getWirelessDevConnList();
}

void KnmWlanDataKeeper::onNetworkRemove(QString deviceName, QString wlanName)
{
    if (deviceName.isEmpty())
        return;

    if (!m_deviceList.contains(deviceName))
        return;

    NetDevicePtr dev = m_deviceList.value(deviceName);
    if(dev.isNull()) {
        qWarning() << Q_FUNC_INFO << __LINE__ << deviceName << "no device";
        return;
    }
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

void KnmWlanDataKeeper::onRequestInputPasswdAgent(QString agentName,QVariantMap parm)
{
    qDebug() << Q_FUNC_INFO <<__LINE__ << agentName<<parm;
    KInterface::getInstance()->onRequestInputPasswdAgent(agentName,parm);

    return ;
}

void KnmWlanDataKeeper::onNetworkPropChanged(QVariantMap parm)
{
    qDebug() << Q_FUNC_INFO <<__LINE__ <<parm;
    QString deviceName=parm.value("DeviceName").toString();
    if (!m_deviceList.contains(deviceName))
        return;

    NetDevicePtr dev = m_deviceList.value(deviceName);
    dev->updateConnectionProp(parm);
    KInterface::getInstance()->wirelessDevConnListPropUpdate(deviceName,parm.value("Name").toString());

    return ;
}

