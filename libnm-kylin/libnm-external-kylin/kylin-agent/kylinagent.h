#ifndef KYLINAGENT_H
#define KYLINAGENT_H

#include "kylinagentobject.h"

#include <QObject>
#include <QThread>

#include "libnm-external-kylin_global.h"

/*
* 该类为单例模式
*/
class LIBNMEXTERNALKYLIN_EXPORT KylinAgent : public QObject
{
    Q_OBJECT
public:
    static KylinAgent* getInstance();
    static void Release();

public:
    explicit KylinAgent(QObject *parent = nullptr);
    ~KylinAgent();

public:
    void startKylinAgent();
    void stopKylinAgent();

public:
    static KylinAgent* m_pInstance;

private:
    KylinAgentObject *m_kylinAgnetObject;
    QThread *m_agentThread;
};


#endif // KYLINAGENT_H
