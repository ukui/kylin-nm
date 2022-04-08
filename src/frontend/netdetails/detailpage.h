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

class DetailPage : public QFrame
{
    Q_OBJECT
public:
    DetailPage(bool isWlan, bool isCreate = false, QWidget *parent = nullptr);

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

public:
    QListWidget * m_listWidget = nullptr;
    FirstDetailWidget * m_ssidWidget = nullptr;

    DetailWidget * m_protocolWidget = nullptr;
    DetailWidget * m_secTypeWidget = nullptr;
    DetailWidget * m_hzWidget = nullptr;
    DetailWidget * m_chanelWidget = nullptr;
    DetailWidget * m_bandwidthWidget = nullptr;
    DetailWidget * m_ipv4Widget = nullptr;
    DetailWidget * m_ipv4DnsWidget = nullptr;
    DetailWidget * m_ipv6Widget = nullptr;
    DetailWidget * m_macWidget = nullptr;

    CopyButton  * m_netCopyButton;
    LineEdit *mSSIDEdit;
//    FixLabel *mSSIDLabel;
    QLabel *mSSIDLabel;
    QLabel *mProtocol;
    QLabel *mSecType;
    QLabel *mHz;
    QLabel *mChan;
    QLabel *mBandWidth;
    QLabel *mIPV4;
    QLabel *mIPV4Dns;
    FixLabel *mIPV6;
    QLabel *mMac;
    QLabel *autoConnect;

//    QWidget *autoFrame;
private:
    QVBoxLayout *layout;
    QVBoxLayout *mDetailLayout;
    QHBoxLayout *mAutoLayout;
    QCheckBox *forgetNetBox;
    bool mIsWlan;
    bool isCreate;


    QClipboard   * m_clipboard ;
    QStringList  m_netDetailList;
    QString      m_formerSsid;
    QString      m_formerIPV6;

    QString      m_ssidCopy = tr("SSID:");
    QString      m_protocolCopy = tr("Protocol:");
    QString      m_securityCopy = tr("Security Type:");
    QString      m_hzCopy= tr ("Hz:");
    QString      m_chanCopy= tr ("Chan:");
    QString      m_bandwithCopy = tr("BandWidth:");
    QString      m_ipv4Copy = tr("IPV4:");
    QString      m_ipv4dnsCopy = tr("IPV4 Dns:");
    QString      m_ipv6Copy = tr("IPV6:");
    QString      m_macCopy = tr("Mac:");
    QString      m_netDetailCopyText;

    bool         isCopyOk = false;

private slots:
    void setEnableOfSaveBtn();
    void on_btnCopyNetDetail_clicked();

signals:
     void setDetailPageState(bool);

};

#endif // DETAILPAGE_H
