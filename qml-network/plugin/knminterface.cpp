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
}

KnmInterface::~KnmInterface()
{
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
    for(auto iter : dev){
        list.append(iter->devName());
    }
    return list;
}

QVariantList KnmInterface::wirelessDeviceList()
{
    QVariantList list;

    auto dev = KNMDC::getInstance()->wirelessDeviceList();
    for(auto iter : dev){
        list.append(iter->devName());
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

void KnmInterface::openNetworkSetting()
{
    if(m_pProcess) {
         m_pProcess->deleteLater();
    }

    m_pProcess = new QProcess();
    QString cmd = "ukui-control-center";
    QStringList arg;
    arg.clear();
    arg << "-m";
    arg << "netconnect";
    m_pProcess->startDetached(cmd,arg);
}

void KnmInterface::getWiredDevConnList(QString devName)
{
    QVariantList list;

    m_currentWiredDevice = devName;
    auto conList = KNMDC::getInstance()->wiredDeviceConnList(devName);

    for (int i = 0; i < conList.count(); i++) {
        list << conList.at(i);
    }
    m_wiredDevConnList = list;
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

void KnmInterface::getWirelessDevConnList(QString devName)
{
    QVariantList list;

    m_currentWirelessDevice = devName;
    auto conList = KNMDC::getInstance()->wirelessDeviceConnList(devName);

    for (int i = 0; i < conList.count(); i++) {
        list << conList.at(i);
    }
    m_wirelessDevConnList = list;
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

void KnmInterface::rescanWirelessConn()
{
    KNMDC::getInstance()->rescanWirelessConn();
}

void KnmInterface::passwordConnect(QString devName, QString ssid, QString type, QString psk, bool autoConnect)
{
    KNMDC::getInstance()->passwordConnect(devName, ssid, type, psk, autoConnect);
}



