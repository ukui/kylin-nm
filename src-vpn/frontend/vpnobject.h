#ifndef VPNOBJECT_H
#define VPNOBJECT_H

#include <QObject>
#include <QWidget>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QGSettings>
#include <QDBusInterface>

#define VISIBLE "visible"
#define GSETTINGS_VPNICON_VISIBLE "org.ukui.kylin-nm.vpnicon"
//const QByteArray GSETTINGS_VPNICON_VISIBLE = "org.ukui.kylin-nm.vpnicon";


class vpnObject : public QMainWindow
{
    Q_OBJECT
public:
    explicit vpnObject(QMainWindow *parent = nullptr);
    ~vpnObject();

private:
    void initUI();
    void initTrayIcon();
    void initVpnIconVisible();
    void initDbusConnnect();


private:
    QWidget * vpnWidget = nullptr;

    QSystemTrayIcon * m_vpnTrayIcon = nullptr;
    QGSettings * m_vpnGsettings;   //VPN配置文件
    QGSettings  * StyleSettings = nullptr;
    double tran =1;

    QDBusInterface * m_positionInterface = nullptr;
    bool m_isShowInCenter = false;

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
