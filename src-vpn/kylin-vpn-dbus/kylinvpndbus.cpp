#include "kylinvpndbus.h"
#include <QDBusReply>
#include <QProcess>

#define LOG_FLAG "[KylinVpnDbus]"

KylinVpnDbus::KylinVpnDbus(QObject *parent) : QObject(parent)
{
    qDBusRegisterMetaType<QVector<QStringList> >();

    m_thread = new QThread;
    m_manager = new KyNetworkManager();
    m_manager->moveToThread(m_thread);
    connect(m_thread, &QThread::started, m_manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(m_thread, &QThread::finished, m_manager, &KyNetworkManager::deleteLater);
    m_thread->start();

    while (!m_manager->isInitFinished()) {
        ::usleep(1000);
    }

    initDbusConnect();
}

void KylinVpnDbus::initDbusConnect()
{
    connect(m_manager, &KyNetworkManager::vpnConnectionAdd, this, &KylinVpnDbus::sendVpnAddSignal);
    connect(m_manager, &KyNetworkManager::vpnConnectionUpdate, this, &KylinVpnDbus::sendVpnUpdateSignal);
    connect(m_manager, &KyNetworkManager::vpnConnectionRemove, this, &KylinVpnDbus::vpnRemove);
    connect(m_manager, &KyNetworkManager::vpnStateChange, this, &KylinVpnDbus::vpnActiveConnectionStateChanged);
}

QVector<QStringList> KylinVpnDbus::getVirtualList()
{
    QVector<QStringList> vector;
    QList<KyVirtualConnectItem *> netConnectList;

     m_manager->getVpnAndVirtualConnections(netConnectList);
     if (!netConnectList.isEmpty()) {
         for (const auto vpnConnectItem : netConnectList) {
             vector.append(QStringList() << vpnConnectItem->m_connectName
                           << vpnConnectItem->m_connectUuid
                           << vpnConnectItem->m_connectPath
                           << QString::number(vpnConnectItem->m_connectState));
         }
     }

     return vector;
}

void KylinVpnDbus::deleteVpn(const QString &connUuid)
{
    qDebug()<< LOG_FLAG << "delete vpn connect uuid " << connUuid;
    m_manager->onDeleteConnect(connUuid);
}

void KylinVpnDbus::activateVpn(const QString &connUuid)
{
    qDebug()<< LOG_FLAG << "activate vpn connect uuid " << connUuid;
    m_manager->activateVpnConnection(connUuid);
}

void KylinVpnDbus::deactivateVpn(const QString &connUuid)
{
    qDebug() << LOG_FLAG << "deactivate vpn connect uuid " << connUuid;
    m_manager->onDeactivateConnection(connUuid);
}

void KylinVpnDbus::showKylinVpn()
{
    parent()->onShowMainWindow();
}

void KylinVpnDbus::showVpnAddWidget()
{
#ifdef VPNDETAIL
    //todo 打开添加VPN界面(自研)
#else
    runExternalApp();
    return;
#endif
}

void KylinVpnDbus::showDetailPage(const QString &connUuid)
{
    qDebug() << LOG_FLAG << "show vpn detail page uuid " << connUuid;
#ifdef VPNDETAIL
    //todo 打开VPN详情界面(自研)
#else
    runExternalApp();
    return;
#endif
}

void KylinVpnDbus::sendVpnAddSignal(QString connectUuid, QString connectName, QString connectPath)
{
    QStringList info;
    QList<KyVirtualConnectItem *> vpnConnectList;
    m_manager->getVpnAndVirtualConnections(vpnConnectList);
    KyConnectState state = CONNECT_STATE_UNKONW;
    if (!vpnConnectList.isEmpty()) {
        for (const auto vpnConnectItem : vpnConnectList) {
            if (connectUuid == vpnConnectItem->m_connectUuid) {
                state = vpnConnectItem->m_connectState;
                break;
            }
        }
    }

    info << connectName
         << connectUuid
         << connectPath
         << QString::number(state);

    Q_EMIT vpnAdd(info);
}

void KylinVpnDbus::sendVpnUpdateSignal(QString connectUuid, QString connectName, QString connectPath)
{
    QStringList info;
    info << connectName
         << connectUuid
         << connectPath;
    Q_EMIT vpnUpdate(info);
}


#define KYLIN_APP_MANAGER_NAME "com.kylin.AppManager"
#define KYLIN_APP_MANAGER_PATH "/com/kylin/AppManager"
#define KYLIN_APP_MANAGER_INTERFACE "com.kylin.AppManager"
bool KylinVpnDbus::launchApp(QString desktopFile)
{
    QDBusInterface appManagerDbusInterface(KYLIN_APP_MANAGER_NAME,
                                             KYLIN_APP_MANAGER_PATH,
                                             KYLIN_APP_MANAGER_INTERFACE,
                                             QDBusConnection::sessionBus());

    if (!appManagerDbusInterface.isValid()) {
        qWarning() << LOG_FLAG << "appManagerDbusInterface init error";
        return false;
    } else {
        QDBusReply<bool> reply = appManagerDbusInterface.call("LaunchApp", desktopFile);
        return reply;
    }
}

void KylinVpnDbus::runExternalApp()
{
    if (!launchApp("nm-connection-editor.desktop")){
        QString cmd = "nm-connection-editor";
        QProcess process(this);
        process.startDetached(cmd);
    }
}
