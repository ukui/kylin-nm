#include "kylinagentobject.h"

KylinAgentObject::KylinAgentObject(QObject *parent) : QObject(parent)
{

}

KylinAgentObject::~KylinAgentObject()
{
    if (nullptr != m_secretAgnet) {
        delete m_secretAgnet;
        m_secretAgnet = nullptr;
    }
}

void KylinAgentObject::onIntKylinAgent()
{
    m_secretAgnet = new KylinSecretAgent("kylinagent");

    return;
}
