#ifndef VPNOBJECT_H
#define VPNOBJECT_H

#include <QObject>
#include <QWidget>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QMainWindow>

#include "vpnpage.h"
#include "vpnaddpage.h"
class VpnPage;

#define VISIBLE "visible"
#define GSETTINGS_VPNICON_VISIBLE "org.ukui.kylin-nm.vpnicon"
//const QByteArray GSETTINGS_VPNICON_VISIBLE = "org.ukui.kylin-nm.vpnicon";


class vpnObject : public QMainWindow
{
    Q_OBJECT
public:
    explicit vpnObject(QMainWindow *parent = nullptr);
    ~vpnObject();

    void getVirtualList(QVector<QStringList> &vector);
    //Vpn连接删除
    void deleteVpn(const QString &connUuid);
    //有线连接断开
    void activateVpn(const QString& connUuid);
    void deactivateVpn(const QString& connUuid);

    void showDetailPage(const QString& connUuid);

    void showVpnAddWidget();

private:
    void initUI();
    void initTrayIcon();
    void initVpnIconVisible();
    void initDbusConnnect();


private:
    VpnPage *m_vpnPage = nullptr;
    QSystemTrayIcon * m_vpnTrayIcon = nullptr;
    QGSettings * m_vpnGsettings;   //VPN配置文件
    double tran =1;
    QGSettings       *StyleSettings = nullptr;
    QWidget * vpnWidget = nullptr;

    QDBusInterface * m_positionInterface = nullptr;
    bool m_isShowInCenter = false;

    vpnAddPage *m_vpnAddPage = nullptr;

public Q_SLOTS:
    void onShowMainWindow();

private Q_SLOTS:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onTabletModeChanged(bool mode);


Q_SIGNALS:
    void vpnAdd(QStringList info);
    void vpnRemove(QString dbusPath);
    void vpnUpdate(QStringList info);
    void vpnActiveConnectionStateChanged(QString uuid, int status);
    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);
    void mainWindowVisibleChanged(const bool &visible);
};

#endif // VPNOBJECT_H
