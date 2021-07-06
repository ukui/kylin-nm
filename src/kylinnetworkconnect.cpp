#include "kylinnetworkconnect.h"

KyNetworkConnect::KyNetworkConnect()
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
}

KyNetworkConnect::~KyNetworkConnect()
{
    m_networkResourceInstance = nullptr;
}

int KyNetworkConnect::activateConnection(const QString connectUuid)
{
    QString conn_uni;
    QString dev_uni;
    QString conn_name;
    QString dev_name;
    QString spec_object;

    qDebug()<<"it will activate connect"<<connectUuid;
    NetworkManager::Connection::Ptr connectPtr = m_networkResourceInstance->getConnect(connectUuid);
    conn_uni = connectPtr->path();
    conn_name = connectPtr->name();
    if (NetworkManager::ConnectionSettings::Vpn == connectPtr->settings()->connectionType()) {
        spec_object = dev_uni = QStringLiteral("/");
        /*
        // find first non-vpn active connection
        const auto act_i = std::find_if(d->mActiveConns.cbegin(), d->mActiveConns.cend(), [] (NetworkManager::ActiveConnection::Ptr const & conn) -> bool
        {
        return nullptr != dynamic_cast<NetworkManager::VpnConnection const *>(conn.data());
        });
        if (act_i != d->mActiveConns.cend() && !(*act_i)->devices().empty())
        {
        dev_uni = (*act_i)->devices().front();
        spec_object = (*act_i)->path();
        }
        */

    } else {
        dev_name = connectPtr->settings()->interfaceName();
        for (auto const & dev : m_networkResourceInstance->m_devices) {
            for (auto const & dev_conn : dev->availableConnections()) {
                if (dev_conn == connectPtr) {
                    dev_uni = dev->uni();
                    dev_name = dev->interfaceName();
                    break;
                }
            }
        }

        if (dev_uni.isEmpty() && !dev_name.isEmpty()) {
            auto dev = m_networkResourceInstance->findDeviceInterface(dev_name);
            if (!dev.isNull())
                dev_uni = dev->uni();
        }
    }

    if (dev_uni.isEmpty()) {
        //TODO: in what form should we output the warning messages
        qWarning() << QStringLiteral("can't find device '%1' to activate connection '%2' on").arg(dev_name).arg(connectPtr->name());
        return -EINVAL;
    }

    qDebug() << __FUNCTION__ << conn_uni << dev_uni << conn_name << dev_name << spec_object;

    //TODO: check vpn type etc..
    QDBusPendingCallWatcher * watcher;
    watcher = new QDBusPendingCallWatcher{NetworkManager::activateConnection(conn_uni, dev_uni, spec_object), this};
    connect(watcher, &QDBusPendingCallWatcher::finished, [conn_name, dev_name] (QDBusPendingCallWatcher * watcher) {
        if (watcher->isError() || !watcher->isValid()) {
            //TODO: in what form should we output the warning messages
            qWarning() << QStringLiteral("activation of connection '%1' on interface '%2' failed: %3").arg(conn_name)
                    .arg(dev_name).arg(watcher->error().message());
         }
         watcher->deleteLater();
    });

    return 0;
}

int KyNetworkConnect::deactivateConnection(const QString connectName, const QString &uuid)
{
    int index = 0;
    NetworkManager::ActiveConnection::Ptr activateConnectPtr = nullptr;

    qDebug()<<"deactivetate connect name"<<connectName<<"uuid"<<uuid;
    for (index = 0; index < m_networkResourceInstance->m_activeConns.size(); ++index) {
        activateConnectPtr = m_networkResourceInstance->m_activeConns.at(index);
        if (activateConnectPtr->uuid() == uuid) {
            break;
        }
    }

    if (index >= m_networkResourceInstance->m_activeConns.size()) {
        qWarning()<<"it can not find the activate connect"<<connectName << "uuid"<<uuid;
        return -EINVAL;
    }

    qDebug() << __FUNCTION__ <<"dead active connection path:"<< activateConnectPtr->path();

    QDBusPendingReply<> reply = NetworkManager::deactivateConnection(activateConnectPtr->path());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, [activateConnectPtr] (QDBusPendingCallWatcher * watcher) {
        //TODO::it may should send signal deactivateConnectionFinished
        if (watcher->isError() || !watcher->isValid()) {
            //TODO: in what form should we output the warning messages
            qWarning() << QStringLiteral("deactivation of connection '%1' failed: %3").arg(activateConnectPtr->connection()->name())
                    .arg(watcher->error().message());
        } else {
            qWarning()<<"deactive connect finished"<<activateConnectPtr->connection()->name();
        }
         watcher->deleteLater();
    });

    return 0;
}

void KyNetworkConnect::requestScan(const QString ifaceName)
{
    auto const & dev = m_networkResourceInstance->getNetworkDevice(ifaceName);
    auto spec_dev = dev->as<NetworkManager::WirelessDevice>();
    if (nullptr == spec_dev) {
        //TODO: in what form should we output the warning messages
        qWarning() << "dropping request for scan on non wireles device " << dev->interfaceName();
        return;
    }

    m_networkResourceInstance->requestScan(spec_dev);
}

void KyNetworkConnect::requestAllWifiScan()
{
    return;
}
