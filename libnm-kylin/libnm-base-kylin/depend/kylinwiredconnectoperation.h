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

    void createWiredConnect(KyConnectSetting &connectSettingsInfo);
};

#endif // KYLINWIREDCONNECTOPERATION_H
