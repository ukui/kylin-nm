#include "kylingeneral.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinactiveconnectresource.h"
#include "kylinnetworkmanagerutil.h"

KylinGeneralOpration::KylinGeneralOpration(QObject *parent) : QObject(parent)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();

    connect(m_networkResourceInstance, &KyNetworkResourceManager::wifiEnabledChanged,
                                        this, &KylinGeneralOpration::wifiEnabledChanged);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::networkingEnabledChanged,
                                        this, &KylinGeneralOpration::networkingEnabledChanged);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::primaryConnectionTypeChanged,
                                        this, &KylinGeneralOpration::updateConnectStatus);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::connectivityChanged,
                                        this, &KylinGeneralOpration::updateConnectStatus);
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wiredEnabledChanged,
                                            this, &KylinGeneralOpration::wiredEnabledChanged);
}

KylinGeneralOpration::~KylinGeneralOpration()
{
    m_networkResourceInstance = nullptr;
}

void KylinGeneralOpration::getConnectStatus(KyConnectStatus &status)
{
    updateConnectStatus();
    status = m_status;
}

void KylinGeneralOpration::updateConnectStatus()
{
    KyConnectStatus status;

    KyConnectionType connectType;
    KyActiveConnectResourse activeResource;
    getPrimaryConnectionType(connectType);
    if (activeResource.wiredConnectIsActived()) {
        status = LAN_CONNECTED;
    } else if (activeResource.wirelessConnectIsActived()) {
        status = WLAN_CONNECTED;
    } else {
        status = NOT_CONNECTED;
    }

    KyConnectivity connecttivity;
    getConnectivity(connecttivity);
    if (connecttivity != CONNECTIVITY_FULL) {
        if (status == LAN_CONNECTED) {
            status = LAN_CONNECTED_LIMITED;
        } else if (status == WLAN_CONNECTED) {
            status = WLAN_CONNECTED_LIMITED;
        }
    }

    if (status != m_status) {
        m_status = status;
        Q_EMIT connectStatusChanged(m_status);
    }
}

void KylinGeneralOpration::getConnectivity(KyConnectivity &connectivity)
{
    connectivity = (KyConnectivity)NetworkManager::connectivity();
}

void KylinGeneralOpration::getPrimaryConnectionType(KyConnectionType &connectType)
{
    connectType = (KyConnectionType)NetworkManager::primaryConnectionType();
}

bool KylinGeneralOpration::getWirelessEnabled()
{
    return NetworkManager::isWirelessEnabled();
}

void KylinGeneralOpration::setWirelessNetworkEnabled(bool enabled)
{
    NetworkManager::setWirelessEnabled(enabled);
}

bool KylinGeneralOpration::getNetworkingEnabled()
{
    return NetworkManager::isNetworkingEnabled();
}

void KylinGeneralOpration::setNetworkingEnabled(bool enabled)
{
    NetworkManager::setNetworkingEnabled(enabled);
}

void KylinGeneralOpration::setWiredEnabled(bool enabled)
{
    setWiredEnabledByGDbus(enabled);
}

bool KylinGeneralOpration::getWiredEnabled()
{
    return getWiredEnabledByGDbus();
}
