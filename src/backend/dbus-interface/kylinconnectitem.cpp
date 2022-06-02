
#include "kylinconnectitem.h"

KyConnectItem::KyConnectItem(QObject *parent) : QObject(parent)
{
    m_connectName = "";
    m_connectUuid = "";
    m_connectPath = "";

    m_ifaceName = "";

    m_connectState = NetworkManager::ActiveConnection::State::Unknown;       //deactive、activing and actived
    m_itemType = NetworkManager::ConnectionSettings::ConnectionType::Unknown;
}

KyConnectItem::~KyConnectItem()
{

}

void KyConnectItem::setConnectUuid(QString uuid)
{
    m_connectUuid = uuid;
}

void KyConnectItem::dumpInfo()
{
    qDebug()<<"wired connection item info:";
    qDebug()<<"connect name:"<<m_connectName;
    qDebug()<<"connect uuid:"<<m_connectUuid;
    qDebug()<<"iface name:"<<m_ifaceName;
    qDebug()<<"connect path"<<m_connectPath;

    qDebug()<<"state:"<<m_connectState;
}
