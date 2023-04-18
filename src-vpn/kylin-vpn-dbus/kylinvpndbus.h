#ifndef KYLINVPNDBUS_H
#define KYLINVPNDBUS_H

#include <QObject>
#include <QDBusMetaType>
#include <QMetaObject>
#include <QMetaType>
#include <QDBusConnection>

#include <kylin-nm/kylinnetworkmanager.h>

class KylinVpnDbus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylin.kylinvpntest")
public:
    explicit KylinVpnDbus(QObject *parent = nullptr);
    ~KylinVpnDbus() = default;

public Q_SLOTS:
    Q_NOREPLY void getVirtualList(QVector<QStringList> &vector);
    //删除
    Q_NOREPLY void deleteVpn(const QString &connUuid);
    //连接
    Q_NOREPLY void activateVpn(const QString& connUuid);
    //断开
    Q_NOREPLY void deactivateVpn(const QString& connUuid);

    void showKylinVpn();

    Q_NOREPLY void showVpnAddWidget();
    Q_NOREPLY void showDetailPage(const QString& connUuid);

Q_SIGNALS:
    void vpnAdd(QStringList info);
    void vpnRemove(QString dbusPath);
    void vpnUpdate(QStringList info);
    void vpnActiveConnectionStateChanged(QString uuid, int status);

    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
};

#endif // KYLINVPNDBUS_H
