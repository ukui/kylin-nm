#ifndef KYLINAPCONNECTITEM_H
#define KYLINAPCONNECTITEM_H

#include <QString>

class KyApConnectItem
{
public:
    QString  m_connectName = "";
    QString  m_connectSsid = "";
    QString  m_connectUuid = "";
    QString  m_ifaceName = "";
    QString  m_password = "";
    QString  m_band = "";
    bool  m_isActivated = false;

};

#endif // KYLINAPCONNECTITEM_H
