#ifndef KYWIRELESSNETITEM_H
#define KYWIRELESSNETITEM_H

#include <QString>

#include "kylinnetworkresourcemanager.h"
#include "kywirelessconnectoperation.h"

QString enumToQstring(NetworkManager::AccessPoint::Capabilities, NetworkManager::AccessPoint::WpaFlags, NetworkManager::AccessPoint::WpaFlags);

class KyWirelessNetItem
{
public:
    KyWirelessNetItem(NetworkManager::WirelessNetwork::Ptr net);
    KyWirelessNetItem() {;}
    ~KyWirelessNetItem();

private:
    void init(NetworkManager::WirelessNetwork::Ptr net);
    void initInfoBySsid();

public:
    QString                                 m_NetSsid;
    QString                                 m_connectUuid;
    QString                                 m_bssid;
    int                                     m_signalStrength;
    uint                                    m_frequency;
    QString                                 m_secuType;
    KySecuType                              m_kySecuType;
    QString                                 m_uni;

    //only for m_isConfiged = true
    bool                                    m_isConfigured;
    QString                                 m_connName;
    QString                                 m_connDbusPath;
    uint                                    m_channel;

    int getCategory(QString uni);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
    QString                                 m_device;

};

#endif // KYWIRELESSNETITEM_H
