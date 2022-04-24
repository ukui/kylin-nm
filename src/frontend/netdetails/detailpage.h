#ifndef DETAILPAGE_H
#define DETAILPAGE_H
#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QPainter>
#include <QCheckBox>
#include <QSpacerItem>
#include <QDebug>
#include <QListWidget>
#include <QApplication>
#include <QPushButton>
#include <QClipboard>

#include "detailwidget.h"
#include "coninfo.h"
#include "kwidget.h"
#include "ktabbar.h"
#include "kballontip.h"
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

using namespace kdk;

class DetailPage : public QFrame
{
    Q_OBJECT
public:
    DetailPage(bool isWlan, bool isCreate = false, QWidget *parent = nullptr);
    ~DetailPage();
    void setSSID(const QString &ssid);
    void setProtocol(const QString &protocol);
    void setSecType(const QString &secType);
    void setHz(const QString &hz);
    void setChan(const QString &chan);
    void setBandWidth(const QString &brandWidth);
    void setIpv4(const QString &ipv4);
    void setIpv4Dns(const QString &ipv4Dns);
    void setIpv6(const QString &ipv6);
    void setMac(const QString &mac);
    void setAutoConnect(bool flag);

    bool checkIsChanged(const ConInfo info);

    void getSsid(QString &ssid);

private:
    void initUI();
    void addDetailItem(QListWidget *listWidget, QWidget *detailWidget);
    void newCopiedTip();
    QPalette getTheme();

public:
    QListWidget  *m_listWidget = nullptr;
    DetailWidget *m_ssidWidget = nullptr;
    DetailWidget *m_protocolWidget = nullptr;
    DetailWidget *m_secTypeWidget = nullptr;
    DetailWidget *m_hzWidget = nullptr;
    DetailWidget *m_chanelWidget = nullptr;
    DetailWidget *m_bandwidthWidget = nullptr;
    DetailWidget *m_ipv4Widget = nullptr;
    DetailWidget *m_ipv4DnsWidget = nullptr;
    DetailWidget *m_ipv6Widget = nullptr;
    DetailWidget *m_macWidget = nullptr;

    QPushButton  *m_netCopyButton;
    LineEdit     *m_SSIDEdit;
    QLabel       *m_SSIDLabel;
    QLabel       *m_Protocol;
    QLabel       *m_SecType;
    QLabel       *m_Hz;
    QLabel       *m_Chan;
    QLabel       *m_BandWidth;
    QLabel       *m_IPV4;
    QLabel       *m_IPV4Dns;
    FixLabel     *m_IPV6;
    QLabel       *m_Mac;
    QLabel       *m_autoConnect;
    KBallonTip   *m_copiedTip;

//    QWidget *autoFrame;
private:
    QVBoxLayout  *m_layout;
    QVBoxLayout  *m_DetailLayout;
    QHBoxLayout  *m_AutoLayout;
    QCheckBox    *m_forgetNetBox;
    bool         m_IsWlan;
    bool         m_IsCreate;

    QString      m_formerSSID;
    QString      m_formerIPV6;

private slots:
    void setEnableOfSaveBtn();
    void on_btnCopyNetDetail_clicked();

signals:
     void setDetailPageState(bool);

};

#endif // DETAILPAGE_H
