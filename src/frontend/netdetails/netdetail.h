#ifndef NETDETAIL_H
#define NETDETAIL_H

#include <QDialog>
#include <QPainter>
#include <QObject>
#include <QStackedWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QVariantMap>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QButtonGroup>

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>
#include <QCloseEvent>

#include "detailpage.h"
#include "ipv4page.h"
#include "ipv6page.h"
#include "securitypage.h"
#include "creatnetpage.h"
#include "coninfo.h"
#include "tab-pages/tabpage.h"

class NetDetail : public QWidget
{
    Q_OBJECT

public:
    NetDetail(QString interface, QString name, QString uuid, bool isActive, bool isWlan, bool isCreateNet, QWidget *parent = nullptr);
    ~NetDetail();
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *w, QEvent *event);

private:
    void initUI();
    void centerToScreen();
    void initComponent();
    void getConInfo(ConInfo &conInfo);
    void loadPage();
    void pagePadding(QString netName, bool isWlan);
    void initSecuData();

    void initTlsInfo(ConInfo &conInfo);
    void initPeapInfo(ConInfo &conInfo);
    void initTtlsInfo(ConInfo &conInfo);

    void updateWirelessPersonalConnect();
    void updateWirelessEnterPriseConnect(KyEapMethodType enterpriseType);

    //详情ssid 带宽 物理地址 无线额外(安全性 频带 通道)
    void getBaseInfo(ConInfo &conInfo);
    //详情ipv4 ipv6 ipv4Dns
    void getDynamicIpInfo(ConInfo &conInfo, bool bActived);
    //ipv4+ipv6页面
    void getStaticIpInfo(ConInfo &conInfo, bool bActived);

    void setConfirmEnable();

    bool checkIpv4Conflict(QString ipv4Address);
    bool checkIpv6Conflict(QString ipv6Address);

    bool createWiredConnect();
    bool createWirelessConnect();
    bool updateConnect();

    bool checkWirelessSecurity(KySecuType secuType);

    void showDesktopNotify(const QString &message);

    void setNetdetailSomeEnable(bool on);

    QPalette lightPalette() const;
private:
    KyNetworkDeviceResourse *m_netDeviceResource = nullptr;
    KyConnectOperation* m_connectOperation = nullptr;
    KyWirelessConnectOperation *m_wirelessConnOpration = nullptr;
    KyWiredConnectOperation *m_wiredConnOperation = nullptr;
    KyWirelessNetResource *m_resource = nullptr;

    QStackedWidget * stackWidget;

    DetailPage     * detailPage;
    Ipv4Page       * ipv4Page;
    Ipv6Page       * ipv6Page;
    SecurityPage   * securityPage;
    CreatNetPage   * createNetPage;

    QWidget      * centerWidget;
    QWidget      * bottomWidget;

    QPushButton  * cancelBtn;
    QPushButton  * forgetBtn;
    QPushButton  * confimBtn;

    QPushButton  * detailBtn;
    QPushButton  * ipv4Btn;
    QPushButton  * ipv6Btn;
    QPushButton  * securityBtn;
    QFrame       * pageFrame;

    QString      m_name;
    QString      m_uuid;
    QString      m_deviceName;

    bool         isWlan;
    bool         m_isCreateNet;
    bool         isActive;
    bool         isHideWlan;

    bool         isCreateOk;
    bool         isDetailOk;
    bool         isIpv4Ok;
    bool         isIpv6Ok;
    bool         isSecuOk;
    bool         isConfirmBtnEnable;

    ConInfo      m_info;

    QButtonGroup *m_group;

private slots:
    void on_btnConfirm_clicked();
    void on_btnForget_clicked();
    void onPaletteChanged();

signals:
    void detailPageClose(bool on);
    void createPageClose(QString);
};



#endif // NETDETAIL_H
