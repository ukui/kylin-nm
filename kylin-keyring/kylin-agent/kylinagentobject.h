#ifndef KYLINAGENTOBJECT_H
#define KYLINAGENTOBJECT_H

#include "kylinsecretagent.h"

#include <QObject>

class KylinAgentObject : public QObject
{
    Q_OBJECT
public:
    explicit KylinAgentObject(QObject *parent = nullptr);
    ~KylinAgentObject();

public Q_SLOTS:
    void onInitAgent();

private:
    KylinSecretAgent *m_secretAgnet;

};

#endif // KYLINAGENTOBJECT_H
