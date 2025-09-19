#include "knmlandatakeeper.h"
#include "knminterface.h"

KnmLanDataKeeper::KnmLanDataKeeper(QObject *parent) : KnmDataKeeper(parent)
{
    m_pInterface = new QDBusInterface("com.kylin.network",
                                     "/com/kylin/network",
                                     "com.kylin.network",
                                     QDBusConnection::sessionBus());
    if(!m_pInterface->isValid()) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

    qDBusRegisterMetaType<QVector<QStringList>>();

    // 有线网络断开或连接时刷新可用网络列表
    connect(m_pInterface, SIGNAL(lanActiveConnectionStateChanged(QString, QString, int)), this, SLOT(onActiveConnectionChanged(QString, QString, int)), Qt::QueuedConnection);
    //有线网络新增时添加网络
    connect(m_pInterface, SIGNAL(lanAdd(QString, QStringList)), this, SLOT(onLanAdd(QString, QStringList)), Qt::QueuedConnection);
    //删除有线网络
    connect(m_pInterface, SIGNAL(lanRemove(QString)), this, SLOT(onLanRemove(QString)), Qt::QueuedConnection);
    //更新有线网络
    connect(m_pInterface, SIGNAL(lanUpdate(QString, QStringList)), this, SLOT(updateLanInfo(QString, QStringList)),Qt::QueuedConnection);
    //网卡插拔处理,此信号处理需要优化
    connect(m_pInterface, SIGNAL(deviceStatusChanged()), this, SIGNAL(onDeviceStatusChanged()),Qt::QueuedConnection);
    //网卡name处理
    connect(m_pInterface, SIGNAL(deviceNameChanged(QString, QString, int)), this, SLOT(onDeviceNameChanged(QString, QString, int)),Qt::QueuedConnection);

    connect(m_pInterface, SIGNAL(wiredMainSwitchBtnChanged(bool)), this, SLOT(onSwitchBtnChanged(bool)), Qt::QueuedConnection);

    netSpeedInit();
}


KnmLanDataKeeper::~KnmLanDataKeeper()
{
    if (nullptr != m_pInterface) {
        delete m_pInterface;
        m_pInterface = nullptr;
    }
}

QMap<QString, QVariant> KnmLanDataKeeper::makeConnectionMap(int status, QStringList conPath)
{
    QMap<QString, QVariant> connectionMap;
    connectionMap.insert("Name", conPath.at(0));
    connectionMap.insert("Uuid", conPath.at(1));
    connectionMap.insert("Path", conPath.at(2));
    connectionMap.insert("State", status);
    connectionMap.insert("Loading", false);
    
    // Add DSL detection logic
    bool isDSL = false;
   if (isDslConnection(conPath.at(1))) {
        isDSL = true;
    }
    connectionMap.insert("IsDSL", isDSL);
    // 默认设置为完全连通
    connectionMap.insert("Connectivity", 4);

    return connectionMap;
}

void KnmLanDataKeeper::addDevConnection(QString devName, QList<QStringList> conPath)
{
    if (conPath.isEmpty())
        return;

    if (!m_deviceList.contains(devName))
        return;

    NetDevicePtr dev =m_deviceList.take(devName);

    //列表首个连接为空或为已激活的连接,单独处理
    if (!conPath.at(0).contains("--")) {
        dev->addConnection(makeConnectionMap(ACTIVATED, conPath.at(0)));
        m_connected = true;
    }

    //单独处理后抛出
    conPath.takeAt(0);

    //剩下的为已保存的未激活连接
    for (int i = 0; i < conPath.count(); i++)
    {
        dev->addConnection(makeConnectionMap(DEACTIVATED, conPath.at(i)));
    }

    m_deviceList.insert(devName, dev);
}

void KnmLanDataKeeper::onActiveConnectionChanged(QString deviceName, QString uuid, int status)
{
    if (uuid.isEmpty())
        return;
    if (deviceName.isEmpty() && status != DEACTIVATED)
        return;

    // 获取设备连通性状态
    int connectivity = 4; // 默认为完全连通
    if (status == ACTIVATED) {
        QDBusReply<int> reply = m_pInterface->call(QStringLiteral("getDeviceConnectivity"), deviceName);
        if (reply.isValid()) {
            connectivity = reply.value();
            qDebug() << "Device connectivity:" << deviceName << connectivity;
        }
    }

    // 断开时处理
    if (deviceName.isEmpty()) {
        QMap<QString, QVariant> conn;
        QStringList devList = m_deviceList.keys();

        for (int i = 0; i < devList.count(); i++) {
            NetDevicePtr dev = m_deviceList.value(devList.at(i));
            QMap<QString, QVariant> conn = dev->updateConnection(uuid, status);
            dev->updateConnectivity(uuid, status, connectivity);
            if (!conn.isEmpty()) {
                m_deviceList.remove(devList.at(i));
                m_deviceList.insert(devList.at(i), dev);
                break;
            }
        }

        for (int i = 0; i < devList.count(); i++) {
            NetDevicePtr dev = m_deviceList.take(devList.at(i));
            dev->addConnection(conn);
            m_deviceList.insert(devList.at(i), dev);
        }
        KInterface::getInstance()->getWiredDevConnList();
        return;
    }

    //若已指定设备且存在该连接时
    if (m_deviceList.contains(deviceName)) {
        if (!m_deviceList.value(deviceName)->containsConnection(uuid))
            return;
        NetDevicePtr dev = m_deviceList.value(deviceName);
        QMap<QString, QVariant> conn = dev->updateConnection(uuid, status);
        dev->updateConnectivity(uuid, status, connectivity);
        if (!conn.isEmpty()) {
            m_deviceList.remove(deviceName);
            m_deviceList.insert(deviceName, dev);
            KInterface::getInstance()->getWiredDevConnList();
        }
        return;
    }

    //若非以上情况,则为虚拟网卡
    if (status != ACTIVATED && status != DEACTIVATED)
        return;

    if (!m_deviceList.value(deviceName)->containsConnection(uuid)) {
        return;
    }

    NetDevicePtr dev = m_deviceList.take(deviceName);
    dev->removeConnection(uuid);
    m_deviceList.insert(deviceName, dev);
    KInterface::getInstance()->getWiredDevConnList();
}

void KnmLanDataKeeper::onLanAdd(QString deviceName, QStringList lanInfo)
{
    if (!deviceName.isEmpty() && !m_deviceList.contains(deviceName))
        return;

    //设备名为空时,表示该连接不从属任何设备,所有设备需要添加该连接
    if (deviceName.isEmpty()) {
        QStringList devList = m_deviceList.keys();
        for (int i = 0; i < devList.count(); i++)
        {
            NetDevicePtr dev = m_deviceList.take(devList.at(i));
            dev->addConnection(makeConnectionMap(DEACTIVATED, lanInfo));
            m_deviceList.insert(devList.at(i), dev);
        }
        KInterface::getInstance()->getWiredDevConnList();
        return;
    }

    //设备名不为空,即该连接从属于某设备,更新该设备连接的属性即可
    NetDevicePtr dev = m_deviceList.take(deviceName);
    dev->addConnection(makeConnectionMap(DEACTIVATED, lanInfo));
    m_deviceList.insert(deviceName, dev);
    KInterface::getInstance()->getWiredDevConnList();
}

void KnmLanDataKeeper::onLanRemove(QString path)
{
    QStringList devList = m_deviceList.keys();
    for (int i = 0; i < devList.count(); i++)
    {
        //所有网卡设备均删除该连接
        if (m_deviceList.value(devList.at(i))->containsConnection(path)) {
            NetDevicePtr dev = m_deviceList.take(devList.at(i));
            dev->removeConnection(path);
            m_deviceList.insert(devList.at(i), dev);
        }
    }
    KInterface::getInstance()->getWiredDevConnList();
}

void KnmLanDataKeeper::updateLanInfo(QString deviceName, QStringList lanInfo)
{
    qDebug() << "dddname: " << deviceName << "   info:" << lanInfo;
    QStringList devList = m_deviceList.keys();
    if (deviceName.isEmpty()) {
        for (int i = 0; i < devList.count(); i++) {
            //连接不属于任何设备,任何设备均需要添加该连接
            if (!m_deviceList.value(devList.at(i))->containsConnection(lanInfo.at(1))) {
                NetDevicePtr dev = m_deviceList.take(devList.at(i));
                dev->addConnection(makeConnectionMap(DEACTIVATED, lanInfo));
                m_deviceList.insert(devList.at(i), dev);
                continue;
            }

            //连接不属于任何设备,且修改属性
            NetDevicePtr dev = m_deviceList.take(devList.at(i));
            int status = DEACTIVATED;
            QMap<QString, QVariant>valueMap=dev->getConnections().at(0).toMap();
            if(valueMap.value("State").toInt()
                    && valueMap.value("Uuid").toString() == lanInfo.at(1)) {
                status =  lanInfo.at(4).toInt();
            }
            dev->removeConnection(lanInfo.at(2));
            dev->addConnection(makeConnectionMap(status, lanInfo));
            m_deviceList.insert(devList.at(i), dev);
        }
        KInterface::getInstance()->getWiredDevConnList();
        return;
    }

    for (int i = 0; i < devList.count(); i++)
    {
        //连接被修改为不属于列表第i个设备,故而删除
        if (devList.at(i) != deviceName) {
            NetDevicePtr dev = m_deviceList.take(devList.at(i));
            dev->removeConnection(lanInfo.at(2));
            m_deviceList.insert(devList.at(i), dev);
            continue;
        }

        //连接被修改为属于列表第i个设备,故而新增
        if(!m_deviceList.value(devList.at(i))->containsConnection(lanInfo.at(1))){
            NetDevicePtr dev = m_deviceList.take(devList.at(i));
            dev->addConnection(makeConnectionMap(DEACTIVATED, lanInfo));
            m_deviceList.insert(devList.at(i), dev);
            continue;
        }

        //连接被修改了名称
        NetDevicePtr dev = m_deviceList.take(devList.at(i));
        int status = DEACTIVATED;
        QMap<QString, QVariant>valueMap=dev->getConnections().at(0).toMap();

        if(valueMap.value("State").toInt()
                && valueMap.value("Uuid").toString() == lanInfo.at(1)) {
            status = lanInfo.at(4).toInt();
        }
        dev->removeConnection(lanInfo.at(2));
        dev->addConnection(makeConnectionMap(status, lanInfo));
        m_deviceList.insert(devList.at(i), dev);
    }
    KInterface::getInstance()->getWiredDevConnList();
}

void KnmLanDataKeeper::onDeviceNameChanged(QString oldName, QString newName, int type)
{
    if (type != WIRED_DEVICE)
        return;

    if (!m_deviceList.contains(oldName))
        return;

    if (m_deviceList.keys().contains(newName))
        return;

    emit onDeviceStatusChanged();
}

void KnmLanDataKeeper::onSwitchBtnChanged(bool status)
{
    m_wiredMainSwitchState = status;
    m_switchState = status;
    KInterface::getInstance()->updateWiredMainSwitch();
}

bool KnmLanDataKeeper::isDslConnection(const QString &uuid)
{
    QProcess process;
    process.start("nmcli", {"-g", "connection.type", "con", "show", uuid});
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput().trimmed();
    return (output == "pppoe");  // 如果是pppoe类型则返回true
}
