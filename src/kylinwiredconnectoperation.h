#ifndef KYLINWIREDCONNECTOPERATION_H
#define KYLINWIREDCONNECTOPERATION_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"

class KyWiredConnectOperation : public QObject
{
    Q_OBJECT
public:
    KyWiredConnectOperation();
    ~KyWiredConnectOperation();

public:
    void  createWiredConnect(KyConnectSetting &connectSettingsInfo);
    void  updateWiredConnect(const QString &connectUuid, const KyConnectSetting &connectSettingsInfo);
    void  deleteWiredConnect(const QString &connectUuid);
    void  activateWiredConnection(const QString connectUuid);
    void  deactivateWiredConnection(const QString activeConnectName, const QString &activeConnectUuid);

private:
    void connectSettingSet(
                        NetworkManager::ConnectionSettings::Ptr connectionSettings,
                        const KyConnectSetting &connectSettingsInfo);
    void ipv4SettingSet(NetworkManager::Ipv4Setting::Ptr &ipv4Setting,
                         const KyConnectSetting &connectSettingsInfo);
    void ipv6SettingSet(NetworkManager::Ipv6Setting::Ptr &ipv6Setting,
                         const KyConnectSetting &connectSettingsInfo);
    inline void errorProcess(QString errorMessage);

signals:
    void createConnectionError(QString errorMessage);
    void updateConnectionError(QString errorMessage);
    void deleteConnectionError(QString errorMessage);
    void activateConnectionError(QString errorMessage);
    void deactivateConnectionError(QString errorMessage);

private:
    KyNetworkResourceManager *m_networkResourceInstance = nullptr;
};

#endif // KYLINWIREDCONNECTOPERATION_H
