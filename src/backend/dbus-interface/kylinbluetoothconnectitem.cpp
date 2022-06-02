#include "kylinbluetoothconnectitem.h"


KyBluetoothConnectItem::KyBluetoothConnectItem(QObject *parent) : QObject(parent)
{
    m_connectName = "";
    m_connectUuid = "";

    m_deviceAddress = "";

    m_ipv4Address = "";
    m_ipv6Address = "";

    m_state = NetworkManager::ActiveConnection::State::Deactivated;
}

KyBluetoothConnectItem::~KyBluetoothConnectItem()
{
    m_connectName = "";
    m_connectUuid = "";

    m_deviceAddress = "";

    m_ipv4Address = "";
    m_ipv6Address = "";

    m_state = NetworkManager::ActiveConnection::State::Deactivated;
}

