#include "kyapnetresource.h"
#include "kylinnetworkdeviceresource.h"
#include "kywirelessconnectoperation.h"
#include "kylinnetworkmanagerutil.h"

const QString str2GBand = "2.4Ghz";
const QString str5GBand = "5Ghz";

KyApNetResource::KyApNetResource(QObject *parent) : QObject(parent)
{
    m_networkResourceInstance = KyNetworkResourceManager::getInstance();
    connect(m_networkResourceInstance, &KyNetworkResourceManager::wirelessApConnectStateChange,
                                            this, &KyApNetResource::wirelessApConnectStateChange);
}

KyApNetResource::~KyApNetResource()
{
    m_networkResourceInstance = nullptr;
}

KyApConnectItem KyApNetResource::getApConnectItem(NetworkManager::Connection::Ptr connectPtr)
{
    if (nullptr == connectPtr) {
        qWarning()<<"[KyConnectResourse]"<<"get bluetooth connection item failed, the connect is empty";
        return KyApConnectItem();
    }

    NetworkManager::ConnectionSettings::Ptr settingPtr = connectPtr->settings();
    NetworkManager::WirelessSetting::Ptr wirelessSetting
        = settingPtr->setting(NetworkManager::Setting::Wireless).dynamicCast<NetworkManager::WirelessSetting>();
    if (NetworkManager::WirelessSetting::NetworkMode::Ap
                                    != wirelessSetting->mode()) {
        qWarning() << "[KyConnectResourse]" <<"get ap item failed, the active connect mode is not ap.";
        return KyApConnectItem();
    }

    KyNetworkDeviceResourse deviceResource;
    if (!deviceResource.wirelessDeviceIsExist(settingPtr->interfaceName())) {
        qWarning() << "[KyConnectResourse]" <<"get ap item failed, the ap device is not exist yet";
        return KyApConnectItem();
    }

    QByteArray rawSsid = wirelessSetting->ssid();

    KyApConnectItem apConnectItem;
    apConnectItem.m_connectName = connectPtr->name();
    apConnectItem.m_connectSsid = getSsidFromByteArray(rawSsid);
    apConnectItem.m_connectUuid = connectPtr->uuid();
    if (wirelessSetting->band() == NetworkManager::WirelessSetting::FrequencyBand::A) {
        apConnectItem.m_band = str2GBand;
    } else if (wirelessSetting->band() == NetworkManager::WirelessSetting::FrequencyBand::Bg) {
        apConnectItem.m_band = str5GBand;
    }
    apConnectItem.m_ifaceName = settingPtr->interfaceName();
    apConnectItem.m_isActivated = KyNetworkResourceManager::getInstance()->isActiveConnection(connectPtr->uuid());

    //NetworkManager::WirelessSecuritySetting::Ptr wirelessSecuritySetting
    //    = settingPtr->setting(NetworkManager::Setting::WirelessSecurity).dynamicCast<NetworkManager::WirelessSecuritySetting>();
    KyWirelessConnectOperation wirelessOperation;
    apConnectItem.m_password = wirelessOperation.getPsk(apConnectItem.m_connectUuid);// wirelessSecuritySetting->psk();

    return apConnectItem;
}

KyApConnectItem KyApNetResource::getApConnectionByUuid(QString connectUuid)
{
    NetworkManager::Connection::Ptr connectPtr = nullptr;

    connectPtr = KyNetworkResourceManager::getInstance()->findConnectByUuid(connectUuid);
    if (nullptr == connectPtr) {
        return KyApConnectItem();
    }

    if (NetworkManager::ConnectionSettings::ConnectionType::Wireless
            != connectPtr->settings()->connectionType()) {
        return KyApConnectItem();
    }

    KyApConnectItem connectItem = getApConnectItem(connectPtr);

    return connectItem;
}

void KyApNetResource::getApConnections(QList<KyApConnectItem> &apConnectItemList)
{
    QList<KyApConnectItem> apActiveConnectItemList;
    QList<KyApConnectItem> apDeactiveConnectItemList;
    NetworkManager::Connection::List connectList;

    connectList.clear();
    connectList = KyNetworkResourceManager::getInstance()->getConnectList();

    if (connectList.empty()) {
        qWarning()<<"[KyConnectResourse]"<<"get ap connections failed, the connect list is empty";
        return;
    }

    NetworkManager::Connection::Ptr connectPtr = nullptr;
    for (int index = 0; index < connectList.size(); index++) {
        connectPtr = connectList.at(index);
        if (NetworkManager::ConnectionSettings::ConnectionType::Wireless
                != connectPtr->settings()->connectionType()) {
            continue;
        }

        KyApConnectItem connectItem = getApConnectItem(connectPtr);
        if (connectItem.m_isActivated) {
            apActiveConnectItemList << connectItem;
        } else {
            apDeactiveConnectItemList << connectItem;
        }
    }

    apConnectItemList << apActiveConnectItemList << apDeactiveConnectItemList;

    return;
}
