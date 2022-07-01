#include "kylinagent.h"
#include <QApplication>
#include <QTranslator>

KylinAgent::KylinAgent(QObject *parent) : QObject(parent)
{
    QString locale = QLocale::system().name();
    QTranslator* translator = new QTranslator(this);
    if (translator->load(":/translations/kylin-keyring_"+ locale + ".qm")) {
        QApplication::installTranslator(translator);
    } else {
//        qWarning() << "Translations load fail";
    }
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
