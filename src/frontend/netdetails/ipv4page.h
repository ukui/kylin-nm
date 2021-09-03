#ifndef IPV4PAGE_H
#define IPV4PAGE_H

#include <QWidget>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QScrollArea>
#include <QSpacerItem>
#include <QComboBox>
#include <QLineEdit>
#include <QDebug>

struct ConnProperties
{
    QString uuidName; //uuid
    QString v4method; //
    QString v4addr;   //ipv4地址
    QString mask;     //
    QString gateway;  //网关
    QString dns;      //DNS
    bool isActConf;   //
    QString type;     //网络类型
};

class Ipv4Page : public QFrame
{
    Q_OBJECT
public:
    Ipv4Page(bool isWlan);
    void setIpv4Config(const QString &ipv4Config);
    void setIpv4(const QString &ipv4);
    void setIpv4FirDns(const QString &ipv4FirDns);
    void setIpv4SecDns(const QString &ipv4SecDns);
    void setGateWay(const QString &gateWay);
public:
    QComboBox *ipv4ConfigCombox;
    QLineEdit *ipv4addressEdit;
    QComboBox *netMaskCombox;
    QLineEdit *gateWayEdit;
    QLineEdit *firstDnsEidt;
    QLineEdit *secondDnsEidt;

    QLabel *m_configLabel;
    QLabel *m_addressLabel;
    QLabel *m_maskLabel;
    QLabel *m_gateWayLabel;
    QLabel *m_dnsLabel;
    QLabel *m_secDnsLabel;

private:
    QFormLayout *m_detailLayout;
    QVBoxLayout *mvBoxLayout;
    bool isWlan;
private:
    void initUI();
    void initComponent();
    void setEnableOfSaveBtn();
    void setLineEnabled(bool check);
    void configSave();


public slots:
    void configChanged(int index);
Q_SIGNALS:
//    void setBtnEnableFalse();

};

#endif // IPV4PAGE_H
