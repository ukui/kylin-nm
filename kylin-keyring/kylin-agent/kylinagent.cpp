#include "kylinagent.h"

KylinAgent::KylinAgent(QObject *parent) : QObject(parent)
{

}

KylinAgent::~KylinAgent()
{

}

void KylinAgent::startKylinAgent()
{
    m_secretAgnet = new KylinSecretAgent("kylinagent", pWidget, this);
}

//void KylinAgent::stopKylinAgent()
//{
//    delete m_kylinAgnetObject;
//    m_kylinAgnetObject = nullptr;
//    return;
//}
