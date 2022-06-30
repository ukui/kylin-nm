#ifndef IPV6PAGE_H
#define IPV6PAGE_H

#include <QWidget>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QCheckBox>
#include <QScrollArea>
#include <QSpacerItem>
#include <QComboBox>
#include <QLineEdit>

//#include "kylinconnectsetting.h"
#include "coninfo.h"

class Ipv6Page : public QFrame
{
    Q_OBJECT
public:
    Ipv6Page(QWidget *parent = nullptr);
    void setIpv6Config(KyIpConfigType ipv6Config);
    void setIpv6(const QString &ipv4);
    void setIpv6Perfix(const int &ipv6Perfix);
    void setIpv6FirDns(const QString &ipv6FirDns);
    void setIpv6SecDns(const QString &ipv6SecDns);
    void setGateWay(const QString &gateWay);

    bool checkIsChanged(const ConInfo info, KyConnectSetting &setting);

    int getPerfixLength(QString text);

public:
    QComboBox *ipv6ConfigCombox;
    LineEdit *ipv6AddressEdit;
    LineEdit *lengthEdit;
    LineEdit *gateWayEdit;
    LineEdit *firstDnsEdit;
    LineEdit *secondDnsEdit;
private:
    QFormLayout *m_detailLayout;
    QLabel *m_configLabel;
    QLabel *m_addressLabel;
    QLabel *m_subnetLabel;
    QLabel *m_gateWayLabel;
    QLabel *m_dnsLabel;
    QLabel *m_secDnsLabel;

    QLabel *m_configEmptyLabel;
    QLabel *m_addressHintLabel;
    QLabel *m_subnetEmptyLabel;
    QLabel *m_gateWayHintLabel;
    QLabel *m_firstDnsEmptyLabel;
private:
    void initUI();
    void initComponent();
    void setControlEnabled(bool check);

    bool getIpv6EditState(QString text);

    bool checkConnectBtnIsEnabled();

private slots:
    void configChanged(int index);
    void setEnableOfSaveBtn();
    void onAddressTextChanged();
    void onGatewayTextChanged();

signals:
    void setIpv6PageState(bool);
};

#endif // IPV6PAGE_H
