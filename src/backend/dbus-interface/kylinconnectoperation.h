#ifndef KYLINCONNECTOPERATION_H
#define KYLINCONNECTOPERATION_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"

class KyConnectOperation : public QObject
{
    Q_OBJECT
public:
    explicit KyConnectOperation(QObject *parent = nullptr);
    ~KyConnectOperation();

public:
    void  createConnect(KyConnectSetting &connectSettingsInfo);
    void  updateConnect(NetworkManager::ConnectionSettings::Ptr connectionSettings, const KyConnectSetting &connectSettingsInfo);
    void  deleteConnect(const QString &connectUuid);
    void  activateConnection(const QString connectUuid, const QString deviceName);
    void  deactivateConnection(const QString activeConnectName, const QString &activeConnectUuid);

public:
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

#endif // KYLINCONNECTOPERATION_H
