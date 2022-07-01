#include "kylinagent.h"

KylinAgent* KylinAgent::m_pInstance = nullptr;

KylinAgent* KylinAgent::getInstance()
{
    if (m_pInstance == NULL) {
        m_pInstance = new KylinAgent();
    }

    return m_pInstance;
}

void KylinAgent::Release()
{
    if (m_pInstance != NULL) {
        delete m_pInstance;
        m_pInstance = NULL;
    }

    return;
}

KylinAgent::KylinAgent(QObject *parent) : QObject(parent)
{
    m_agentThread =new QThread(this);
}

KylinAgent::~KylinAgent()
{
    if (nullptr != m_kylinAgnetObject) {
        delete m_kylinAgnetObject;
        m_kylinAgnetObject = nullptr;
    }
}

void KylinAgent::startKylinAgent()
{
    m_kylinAgnetObject = new KylinAgentObject();
    m_kylinAgnetObject->moveToThread(m_agentThread);
    QObject::connect(m_agentThread, SIGNAL(started()), m_kylinAgnetObject, SLOT(onInitAgent()));
    m_agentThread->start();

    return;
}

void KylinAgent::stopKylinAgent()
{
    m_kylinAgnetObject->disconnect();

    m_agentThread->quit();
    m_agentThread->wait();

    delete m_kylinAgnetObject;
    m_kylinAgnetObject = nullptr;

    return;
}
