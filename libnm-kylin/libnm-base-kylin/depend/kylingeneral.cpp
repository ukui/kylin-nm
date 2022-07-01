#include "kylingeneral.h"
#include "kylinnetworkdeviceresource.h"
#include "kylinactiveconnectresource.h"

const QByteArray GSETTINGS_SCHEMA = "org.ukui.kylin-nm.switch";
const QString    KEY_WIRED_SWITCH             = "wiredswitch";

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

    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        gsettings = new QGSettings(GSETTINGS_SCHEMA);
        if (gsettings->keys().contains(KEY_WIRED_SWITCH)) {
            connect(gsettings, &QGSettings::changed, [&](QString key){
                if (key == KEY_WIRED_SWITCH) {
                    setWiredEnabled(gsettings->get(key).toBool());
                }
            });
        }
    }
}

KylinGeneralOpration::~KylinGeneralOpration()
{
    m_networkResourceInstance = nullptr;
    delete gsettings;
    gsettings = nullptr;
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
    KyNetworkDeviceResourse devResource;
    QStringList devList;
    devResource.getNetworkDeviceList(DEVICE_TYPE_ETHERNET, devList);

    if (!enabled) {
        for (int i = 0; i < devList.size(); ++i) {
            devResource.closeWiredNetworkWithDevice(devList.at(i));
        }
    } else {
        QMap<QString, bool> stateMap;
        devResource.getWiredDeviceEnableState(stateMap);
        for (int i = 0; i < devList.size(); ++i) {
            if (stateMap.contains(devList.at(i))
                    && !stateMap[devList.at(i)]) {
                //do nothing
            } else {
                qDebug() << "openWiredNetworkWithDevice" << devList.at(i);
                devResource.openWiredNetworkWithDevice(devList.at(i));
            }
        }
    }
    updateGsetting(enabled);
}

void KylinGeneralOpration::updateGsetting(bool enable)
{
    if (QGSettings::isSchemaInstalled(GSETTINGS_SCHEMA)) {
        QGSettings *gsettings = new QGSettings(GSETTINGS_SCHEMA);
        if (gsettings->keys().contains(KEY_WIRED_SWITCH)) {
            if (enable != gsettings->get(KEY_WIRED_SWITCH).toBool()) {
                qDebug() << "updateGsetting set " << enable;
                gsettings->set(KEY_WIRED_SWITCH, enable);
            }
        }
    }
}
