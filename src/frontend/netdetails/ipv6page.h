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

class Ipv6Page : public QFrame
{
    Q_OBJECT
public:
    Ipv6Page(bool isWlan, QWidget *parent = nullptr);
    void setIpv6Config(const QString &ipv6Config);
    void setIpv6(const QString &ipv4);
    void setIpv6FirDns(const QString &ipv6FirDns);
    void setIpv6SecDns(const QString &ipv6SecDns);
    void setGateWay(const QString &gateWay);

public:
    QComboBox *ipv6ConfigCombox;
    QLineEdit *ipv6AddressEdit;
    QLineEdit *lengthEdit;
    QLineEdit *gateWayEdit;
    QLineEdit *firstDnsEdit;
    QLineEdit *secondDnsEdit;
private:
    QFormLayout *m_detailLayout;
    QLabel *m_configLabel;
    QLabel *m_addressLabel;
    QLabel *m_subnetLabel;
    QLabel *m_gateWayLabel;
    QLabel *m_dnsLabel;
    QLabel *m_secDnsLabel;
private:
    bool isWlan;
private:
    void initUI();
    void initComponent();
    void setControlEnabled(bool check);

public slots:
    void configChanged(int index);
};

#endif // IPV6PAGE_H
