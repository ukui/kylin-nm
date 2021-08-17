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
    QString  m_connectUuid;
    QString  m_ifaceName;
    QString  m_password;
};

#endif // KYLINAPCONNECTITEM_H
