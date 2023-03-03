#include "kylinnmuidbus.h"

#define LOG_FLAG "[KylinNmUiDbus]"

KylinNmUiDbus::KylinNmUiDbus(QObject *parent)
    :QObject(parent)
{

}

void KylinNmUiDbus::showPropertyWidget(const QString devName, const QString ssid)
{
    Q_EMIT signalShowPropertyWidget(devName, ssid);
}

void KylinNmUiDbus::showCreateWiredConnectWidget(const QString devName)
{
    Q_EMIT signalShowCreateWiredConnectWidget(devName);
}

void KylinNmUiDbus::showAddOtherWlanWidget(const QString devName)
{
    Q_EMIT signalShowAddOtherWlanWidget(devName);
}

void KylinNmUiDbus::activateConnect(int type, QString devName, QString ssid)
{
    Q_UNUSED(type)
    Q_EMIT signalActivateWireless(devName, ssid);
}
