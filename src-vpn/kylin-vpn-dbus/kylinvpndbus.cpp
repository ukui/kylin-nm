#include "kylinvpndbus.h"

KylinVpnDbus::KylinVpnDbus(QObject *parent) : QObject(parent)
{
    qDBusRegisterMetaType<QVector<QStringList> >();
}

void KylinVpnDbus::getVirtualList(QVector<QStringList> &vector)
{

}

void KylinVpnDbus::deleteVpn(const QString &connUuid)
{

}

void KylinVpnDbus::activateVpn(const QString &connUuid)
{

}

void KylinVpnDbus::deactivateVpn(const QString &connUuid)
{

}

void KylinVpnDbus::showKylinVpn()
{

}

void KylinVpnDbus::showVpnAddWidget()
{

}

void KylinVpnDbus::showDetailPage(const QString &connUuid)
{

}
