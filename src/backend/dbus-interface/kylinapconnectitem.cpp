
#include "kylinapconnectitem.h"

KyApConnectItem::KyApConnectItem(QObject *parent) : QObject(parent)
{
    m_connectName = "";
    m_connectUuid = "";
    m_connectSsid = "";
    m_ifaceName = "";
    m_password = "";
    m_band = "";
    m_isActivated = false;
}


KyApConnectItem::~KyApConnectItem()
{

}
