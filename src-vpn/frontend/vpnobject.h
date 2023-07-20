#ifndef VPNOBJECT_H
#define VPNOBJECT_H

#include <QObject>
#include <QWidget>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QGSettings>
#include <QDBusInterface>
#include <QTimer>

#define VISIBLE "visible"
#define GSETTINGS_VPNICON_VISIBLE "org.ukui.kylin-nm.vpnicon"
//const QByteArray GSETTINGS_VPNICON_VISIBLE = "org.ukui.kylin-nm.vpnicon";


class VpnObject : public QMainWindow
{
    Q_OBJECT
public:
    explicit VpnObject(QMainWindow *parent = nullptr);
    ~VpnObject();

    void showMainWindow();
    void hideMainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool m_loadFinished = false; //是否二级启动已执行完
    QTimer * m_secondaryStartTimer = nullptr; //执行二级启动的倒计时
    QDBusInterface * m_positionInterface = nullptr;

    //一级启动
    void firstlyStart();
    //二级启动
    void secondaryStart();
    void initPlatform();
    void initWindowProperties();
    void setWindowProperties();
    //获取和重置窗口位置
    void resetWindowPosition();

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
