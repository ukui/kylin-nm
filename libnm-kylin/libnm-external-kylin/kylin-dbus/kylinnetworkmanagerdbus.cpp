#include "kylinnetworkmanagerdbus.h"

KyNetworkManagerDbus::KyNetworkManagerDbus(QObject *parent) : QObject(parent)
{

}

KyNetworkManagerDbus::~KyNetworkManagerDbus()
{

}

void KyNetworkManagerDbus::registerNetworkManagerDbus()
{
    KylinDbusInterface dbusInterface;
    KylinDbusAdaptor dbusAdapter(&dbusInterface);
    Q_UNUSED(dbusAdapter);

    auto connection = QDBusConnection::sessionBus();
    if (!connection.registerService("com.kylin.network") || !connection.registerObject("/com/kylin/network", &dbusInterface)) {
        qCritical() << "QDbus register service failed reason:" << connection.lastError();
    }

    return;
}
