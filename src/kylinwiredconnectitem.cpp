
#include "kylinwiredconnectitem.h"

KyWiredConnectItem::KyWiredConnectItem(QObject *parent):QObject(parent)
{
    m_connectName = "";
    m_connectUuid = "";
    m_ifaceName = "";

    m_ipv4 = "";
    m_ipv6 = "";
    m_hardAddress = "";

    m_bandWith = "";
    m_upSpeed = 0;
    m_downSpeed = 0;

    m_state = NetworkManager::ActiveConnection::State::Unknown;       //deactive、activing and actived
    m_itemType = "";  //activeconnect or connect
}

KyWiredConnectItem::~KyWiredConnectItem()
{

}

void KyWiredConnectItem::dumpInfo()
{
    qDebug()<<"wired connection item info:";
    qDebug()<<"connect name:"<<m_connectName;
    qDebug()<<"connect uuid:"<<m_connectUuid;
    qDebug()<<"iface name:"<<m_ifaceName;

    qDebug()<<"ipv4 address:"<<m_ipv4;
    qDebug()<<"ipv6 address:"<<m_ipv6;
    qDebug()<<"hard address:"<<m_hardAddress;

    qDebug()<<"band width:"<< m_bandWith;
    qDebug()<<"up speed:"<<m_upSpeed;
    qDebug()<<"down speed:"<<m_downSpeed;

    qDebug()<<"state:"<<m_state;
}
