#ifndef KYLINVPNDBUS_H
#define KYLINVPNDBUS_H

#include <QObject>
#include <QThread>
#include <QDBusMetaType>
#include <QMetaObject>
#include <QMetaType>
#include <QDBusConnection>

#include <kylin-nm/kylinnetworkmanager.h>
#include "vpnobject.h"

class KylinVpnDbus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.kylinvpn")
public:
    explicit KylinVpnDbus(QObject *parent = nullptr);
    ~KylinVpnDbus() = default;
    inline VpnObject *parent() const
    { return static_cast<VpnObject *>(QObject::parent()); }

private:
    KyNetworkManager* m_manager;
    QThread*        m_thread;
    void initDbusConnect();

    bool launchApp(QString desktopFile);
    void runExternalApp();

public Q_SLOTS:
    QVector<QStringList>  getVirtualList();
    //删除
    Q_NOREPLY void deleteVpn(const QString &connUuid);
    //连接
    Q_NOREPLY void activateVpn(const QString& connUuid);
    //断开
    Q_NOREPLY void deactivateVpn(const QString& connUuid);

    void showKylinVpn();

    Q_NOREPLY void showVpnAddWidget();
    Q_NOREPLY void showDetailPage(const QString& connUuid);

private Q_SLOTS:
    void sendVpnAddSignal(QString connectUuid, QString connectName, QString connectPath);
    void sendVpnUpdateSignal(QString connectUuid, QString connectName, QString connectPath);

Q_SIGNALS:
    void vpnAdd(QStringList info);
    void vpnRemove(QString dbusPath);
    void vpnUpdate(QStringList info);
    void vpnActiveConnectionStateChanged(QString uuid, int status);
};

#endif // KYLINVPNDBUS_H
