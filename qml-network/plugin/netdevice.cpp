#include "netdevice.h"

NetDevice::NetDevice(QString devName, DeviceType type)
{
    this->m_Name = devName;
    this->m_DevType = type;
}

NetDevice::~NetDevice()
{
}

bool NetDevice::isAvailable()
{
    return this->m_Available;
}

QString NetDevice::devName()
{
    return this->m_Name;
}

void NetDevice::setAvailable(bool available)
{
    this->m_Available = available;
}

DeviceType NetDevice::getDevType()
{
    return this->m_DevType;
}

void NetDevice::addConnection(QMap<QString, QVariant> connections)
{
    if (m_connectionsList.contains(connections))
        return;
    if (connections.value("State").toInt() == ACTIVATED
            || connections.value("State").toInt() == ACTIVATING)
        m_connectionsList.push_front(connections);
    else
        m_connectionsList.append(connections);
}

void NetDevice::removeConnection(QMap<QString, QVariant> connections)
{
    if (!m_connectionsList.contains(connections))
        return;
    m_connectionsList.removeOne(connections);
}

void NetDevice::removeConnection(QString path)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Path").toString() == path) {
            m_connectionsList.removeAt(i);
        }
    }
}

void NetDevice::removeConnectionByName(QString devName)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Name").toString() == devName) {
            m_connectionsList.removeAt(i);
        }
    }
}

QVariantList NetDevice::getConnections()
{
    QVariantList list;
    if(m_connectionsList.isEmpty()) {
        return list;
    }
    return m_connectionsList;
}

QMap<QString, QVariant> NetDevice::updateConnection(QString uuid, int status)
{
    int i = 0;
    QMap<QString, QVariant> conn;
    while (i < m_connectionsList.count()) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (!valueMap.contains("Uuid"))
            continue;
        if (valueMap.value("Uuid").toString() == uuid) {
            conn = m_connectionsList.takeAt(i).toMap();
            conn.remove("State");
            conn.insert("State", status);
            if (status == ACTIVATED) {
                m_connectionsList.push_front(conn);
            } else
                m_connectionsList.insert(i, conn);
            break;
        }
        i++;
    }
    return conn;
}

bool NetDevice::containsConnection(QString uuidOrPath)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Uuid").toString() == uuidOrPath) {
            return true;
        }
        if (m_DevType == WIRELESS_DEVICE)
            continue;
        if (valueMap.value("Path").toString() == uuidOrPath) {
            return true;
        }
    }
    return false;
}

bool NetDevice::containsConnectionName(QString connName)
{
    for (int i = 0; i < m_connectionsList.count(); i++) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Name").toString() == connName) {
            return true;
        }
    }
    return false;
}

void NetDevice::clearConnections()
{
    m_connectionsList.clear();
}

QMap<QString, QVariant> NetDevice::getConnectionByName(QString connName)
{
    QMap<QString, QVariant> ret;
    for (int i = 0; i < m_connectionsList.count(); i++) {
         QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
        if (valueMap.value("Name").toString() == connName) {
            return m_connectionsList.at(i).toMap();
        }
    }
    return ret;
}

QMap<QString, QVariant> NetDevice::updateConnectivity(QString uuid, int status, int connectivity)
{
    int i = 0;
    QMap<QString, QVariant> conn;
    while (i < m_connectionsList.count()) {
        QMap<QString, QVariant> valueMap = m_connectionsList.at(i).toMap();
        if (valueMap.value("Uuid").toString() == uuid) {
            conn = m_connectionsList.takeAt(i).toMap();
            conn.remove("State");
            conn.insert("State", status);
            conn.remove("Connectivity");
            conn.insert("Connectivity", connectivity);
            
            if (status == ACTIVATED) {
                m_connectionsList.push_front(conn);
            } else {
                m_connectionsList.insert(i, conn);
            }
            break;
        }
        i++;
    }
    return conn;
}

void NetDevice::updateConnectionProp(QVariantMap parm)
{
    int i = 0;

    QMap<QString, QVariant> conn;
    while (i < m_connectionsList.count()) {
        QMap<QString, QVariant>valueMap=m_connectionsList.at(i).toMap();
         if (valueMap.value("Name").toString() == parm.value("Name").toString()) {
            conn = m_connectionsList.takeAt(i).toMap();
            if(parm.contains("Signal")){
                conn.remove("Signal");
                conn.insert("Signal", parm.value("Signal"));
            }
            if(parm.contains("Security")){
                conn.remove("Security");
                conn.insert("Security", parm.value("Security"));
            }
            if(parm.contains("Uuid")){
                conn.remove("Uuid");
                conn.insert("Uuid", parm.value("Uuid"));
            }
            if(parm.contains("frequency")){
                conn.remove("frequency");
                conn.insert("frequency", parm.value("frequency"));
            }
            if(parm.contains("Configured")){
                conn.remove("Configured");
                conn.insert("Configured", parm.value("Configured"));
            }
            if(parm.contains("isMix")){
                conn.remove("isMix");
                conn.insert("isMix", parm.value("isMix"));
            }
            if(parm.contains("State")){
                conn.remove("State");
                conn.insert("State", parm.value("State"));
            }

            m_connectionsList.insert(i, conn);
            break;
        }
        i++;
    }
    return ;
}
