#include "kywirelessnetitem.h"

KyWirelessNetItem::KyWirelessNetItem()
{
    m_NetSsid = "";
    m_connectUuid = "";
    m_connectSecuType = KySecuType::NONE;
    m_secuType = "";
    m_signalStrength = 0;
}
