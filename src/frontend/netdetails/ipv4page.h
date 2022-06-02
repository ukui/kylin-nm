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

//#include "kylinconnectsetting.h"
#include "coninfo.h"

class Ipv4Page : public QFrame
{
    Q_OBJECT
public:
    Ipv4Page(QWidget *parent = nullptr);
    void setIpv4Config(KyIpConfigType ipv4Config);
    void setIpv4(const QString &ipv4);
    void setNetMask(const QString &netMask);
    void setIpv4FirDns(const QString &ipv4FirDns);
    void setIpv4SecDns(const QString &ipv4SecDns);
    void setGateWay(const QString &gateWay);

    bool checkIsChanged(const ConInfo info, KyConnectSetting &setting);
private:
    QComboBox *ipv4ConfigCombox;
    LineEdit *ipv4addressEdit;
    LineEdit *netMaskEdit;
    LineEdit *gateWayEdit;
    LineEdit *firstDnsEdit;
    LineEdit *secondDnsEdit;

private:
    QFormLayout *m_detailLayout;
    QVBoxLayout *mvBoxLayout;
    QLabel *m_configLabel;
    QLabel *m_addressLabel;
    QLabel *m_maskLabel;
    QLabel *m_gateWayLabel;
    QLabel *m_dnsLabel;
    QLabel *m_secDnsLabel;
private:
    void initUI();
    void initComponent();
    void setLineEnabled(bool check);
    void configSave();
    bool getTextEditState(QString text);
    bool netMaskIsValide(QString text);
    QString getNetMaskText(QString text);
    bool checkConnectBtnIsEnabled();


private slots:
    void setEnableOfSaveBtn();
    void configChanged(int index);
Q_SIGNALS:
    void setIpv4PageState(bool);

};

#endif // IPV4PAGE_H
