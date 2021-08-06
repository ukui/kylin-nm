#ifndef KYLINWIREDCONNECTOPERATION_H
#define KYLINWIREDCONNECTOPERATION_H

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"
#include "kylinconnectoperation.h"

class KyWiredConnectOperation : public KyConnectOperation
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
    void  activateVpnConnection(const QString connectUuid);
    void  deactivateWiredConnection(const QString activeConnectName, const QString &activeConnectUuid);
};

#endif // KYLINWIREDCONNECTOPERATION_H
