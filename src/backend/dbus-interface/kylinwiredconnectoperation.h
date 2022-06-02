#ifndef KYLINWIREDCONNECTOPERATION_H
#define KYLINWIREDCONNECTOPERATION_H

#include <QSettings>
#include <QDir>

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"
#include "kylinconnectoperation.h"

const QString WIRED_NETWORK_STATE_CONF_FILE = QDir::homePath() + "/.config/ukui/kylin-nm-wired.ini";

class KyWiredConnectOperation : public KyConnectOperation
{
    Q_OBJECT
public:
    explicit KyWiredConnectOperation(QObject *parent = nullptr);
    ~KyWiredConnectOperation();

public:
    void  createWiredConnect(KyConnectSetting &connectSettingsInfo);
    void  updateWiredConnect(const QString &connectUuid, const KyConnectSetting &connectSettingsInfo);
    void  deleteWiredConnect(const QString &connectUuid);
    void  activateWiredConnection(const QString connectUuid, const QString devName);
    void  activateVpnConnection(const QString connectUuid);
    void  deactivateWiredConnection(const QString activeConnectName, const QString &activeConnectUuid);

    int closeWiredNetworkWithDevice(QString deviceName);
    int openWiredNetworkWithDevice(QString deviceName);

private:
    void getActiveConnection(QString &deviceName, QString &connectUuid);
    void saveActiveConnection(QString &deviceName, QString &connectUuid);
};

#endif // KYLINWIREDCONNECTOPERATION_H
