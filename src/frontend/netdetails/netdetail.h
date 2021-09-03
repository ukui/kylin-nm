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

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>

#include "detailpage.h"
#include "ipv4page.h"
#include "ipv6page.h"
#include "securitypage.h"
#include "creatnetpage.h"
#include "kywirelessnetitem.h"
#include "kylinconnectresource.h"
#include "kylinactiveconnectresource.h"
#include "kywirelessnetresource.h"
#include "tab-pages/tabpage.h"

typedef struct ConInfo_s {
    QString strConName;
    QString strConUUID;
    QString strConType;
    QString strSecType;
    QString strChan;
    QString strMac;
    QString strHz;
    QString strBandWidth;

    QString strIPV4ConfigType;
    QString strIPV4Address;
    QString strIPV4Prefix;
    QString strIPV4FirDns;
    QString strIPV4SecDns;
    QString strIPV4GateWay;

    QString strIPV6ConfigType;
    QString strIPV6Address;
    QString strIPV6FirDns;
    QString strIPV6SecDns;
    QString strIPV6GateWay;
    QString strIPV6Prefix;
}ConInfo;


class NetDetail : public QDialog
{
    Q_OBJECT

public:
    NetDetail(QString name, QString uuid, bool isActive, bool isWlan, bool isCreateNet, QWidget *parent = nullptr);
    ~NetDetail();
protected:
    void paintEvent(QPaintEvent *event);

private:
    void initUI();
    void initWifiDevice();//初始化无线默认设备
    void initLanDevice();//初始化有线默认设备
    void centerToScreen();
    void initComponent();
    void getConInfo(QList<ConInfo>& qlConInfo);
    bool checkConfig();
    void loadPage();
    void pagePadding(QString netName, bool isWlan);

private:
    KyNetworkDeviceResourse *m_netDeviceResource = nullptr;

    QStackedWidget * stackWidget;

    DetailPage     * detailPage;
    Ipv4Page       * ipv4Page;
    Ipv6Page       * ipv6Page;
    SecurityPage   * securityWidget;
    CreatNetPage   * createNetPage;
//    AddLanWidget   * addLanWidget;

    QWidget      * titleWidget;
    QWidget      * centerWidget;
    QWidget      * bottomWidget;

    QLabel       * titleLabel;
    QLabel       * iconLabel;

    QPushButton  * closeBtn;
    QPushButton  * cancelBtn;
    QPushButton  * forgetBtn;
    QPushButton  * confimBtn;

    QPushButton  * detailBtn;
    QPushButton  * Ipv4Btn;
    QPushButton  * Ipv6Btn;
    QPushButton  * SecurityBtn;
    QFrame       * pageFrame;

    QString      name;
    QString      uuid;
    QString      ssid;
    QStringList  devList;
    QString      m_deviceName;

    bool         isWlan;
    bool         isCreateNet;
    bool         isActive;

    QList<ConInfo> mInfo;

private slots:
    void on_btnConfirm_clicked();

};
#endif // NETDETAIL_H
