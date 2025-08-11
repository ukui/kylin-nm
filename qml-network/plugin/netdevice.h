#ifndef NETDEVICE_H
#define NETDEVICE_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QDebug>

#define ACTIVATING   1
#define ACTIVATED    2
#define DEACTIVATING 3
#define DEACTIVATED  4

typedef enum {
    WIRED_DEVICE,
    WIRELESS_DEVICE
} DeviceType;

//网卡对象
class NetDevice
{
public:
    NetDevice(QString devName, DeviceType type);
    ~NetDevice();

    bool isAvailable();

    QString devName();

    DeviceType getDevType();

    void setAvailable(bool available);

    bool containsConnection(QString uuidOrPath);

    bool containsConnectionName(QString connName);

    void addConnection(QMap<QString, QVariant> connections);

    void removeConnection(QMap<QString, QVariant> connections);

    void removeConnection(QString path);

    void removeConnectionByName(QString devName);

    QMap<QString, QVariant> updateConnection(QString uuid, int status);

    QMap<QString, QVariant> getConnectionByName(QString connName);

    QVariantList getConnections();

    void clearConnections();

private:
    QString     m_Name;
    bool        m_Available;
    DeviceType  m_DevType;
    QVariantList m_connectionsList;
};

#endif // NETDEVICE_H
