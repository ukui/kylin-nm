#ifndef KYWIRELESSNETITEM_H
#define KYWIRELESSNETITEM_H

#include <QString>
#include "kywirelessconnectoperation.h"

class KyWirelessNetItem
{
public:
    QString m_NetSsid = "";
    QString m_connectUuid = "";
    QString m_dbusPath = "";
    KySecuType m_connectSecuType = KySecuType::NONE;
    bool m_isApConnection = false;
    QString m_secuType = "";
    int m_signalStrength = 0;
    int m_category = 0;
};

#endif // KYWIRELESSNETITEM_H
