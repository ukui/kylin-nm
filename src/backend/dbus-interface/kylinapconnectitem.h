#ifndef KYLINAPCONNECTITEM_H
#define KYLINAPCONNECTITEM_H

#include <QObject>

class KyApConnectItem : public QObject
{
    Q_OBJECT

public:
    explicit KyApConnectItem(QObject *parent = nullptr);
    ~KyApConnectItem();

public:
    QString  m_connectName;
    QString  m_connectSsid;
    QString  m_connectUuid;
    QString  m_ifaceName;
    QString  m_password;
    QString m_band;
    bool  m_isActivated;

};

#endif // KYLINAPCONNECTITEM_H
