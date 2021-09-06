#include "ipv6page.h"
#include "netdetail.h"
Ipv6Page::Ipv6Page(bool isWlan, QWidget *parent)
    :isWlan(isWlan), QFrame(parent)
{
    initUI();
    initComponent();
}

void Ipv6Page::setIpv6Config(const QString &ipv6Config)
{
    if (ipv6Config.toInt() ==  AUTO_CONFIG) {
        ipv6ConfigCombox->setCurrentIndex(MANUAL_CONFIG);
    } else {
        ipv6ConfigCombox->setCurrentIndex(AUTO_CONFIG);
    }
}

void Ipv6Page::setIpv6(const QString &ipv4)
{
    ipv6AddressEdit->setText(ipv4);
}

void Ipv6Page::setIpv6FirDns(const QString &ipv6FirDns)
{
    firstDnsEdit->setText(ipv6FirDns);
}

void Ipv6Page::setIpv6SecDns(const QString &ipv6SecDns)
{
    secondDnsEdit->setText(ipv6SecDns);
}

void Ipv6Page::setGateWay(const QString &gateWay)
{
    gateWayEdit->setText(gateWay);
}

void Ipv6Page::initUI() {
    ipv6ConfigCombox = new QComboBox(this);
    ipv6AddressEdit = new QLineEdit(this);
    lengthEdit = new QLineEdit(this);
    gateWayEdit = new QLineEdit(this);
    firstDnsEdit = new QLineEdit(this);
    secondDnsEdit = new QLineEdit(this);

    m_configLabel = new QLabel(this);
    m_addressLabel = new QLabel(this);
    m_subnetLabel = new QLabel(this);
    m_gateWayLabel = new QLabel(this);
    m_dnsLabel = new QLabel(this);
    m_secDnsLabel = new QLabel(this);


    m_configLabel->setText(tr("Ipv6Config"));
    m_addressLabel->setText(tr("Address"));
    m_subnetLabel->setText(tr("Subnet prefix Length"));
    m_gateWayLabel->setText(tr("Default Gateway"));
    m_dnsLabel->setText(tr("Prefs DNS"));
    m_secDnsLabel->setText(tr("Alternative DNS"));


    m_detailLayout = new QFormLayout(this);
    m_detailLayout->addRow(m_configLabel,ipv6ConfigCombox);
    m_detailLayout->addRow(m_addressLabel,ipv6AddressEdit);
    m_detailLayout->addRow(m_subnetLabel,lengthEdit);
    m_detailLayout->addRow(m_gateWayLabel,gateWayEdit);
    m_detailLayout->addRow(m_dnsLabel,firstDnsEdit);
    m_detailLayout->addRow(m_secDnsLabel,secondDnsEdit);

    ipv6ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv6ConfigCombox->addItem(tr("Manual")); //"手动"

    QRegExp ipv6_rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");
    ipv6AddressEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
}

void Ipv6Page::initComponent() {
    if (ipv6ConfigCombox->currentIndex() == MANUAL_CONFIG) {
        setControlEnabled(false);
    } else if (ipv6ConfigCombox->currentIndex() == MANUAL_CONFIG) {
        setControlEnabled(true);
    }
    connect(ipv6ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));
}

void Ipv6Page::configChanged(int index) {
    if (index == AUTO_CONFIG) {
        setControlEnabled(false);
    }
    if (index == MANUAL_CONFIG) {
        setControlEnabled(true);
    }
}

void Ipv6Page::setControlEnabled(bool check) {
    m_addressLabel->setEnabled(check);
    m_subnetLabel->setEnabled(check);
    lengthEdit->setEnabled(check);
    m_gateWayLabel->setEnabled(check);
    m_dnsLabel->setEnabled(check);
    m_secDnsLabel->setEnabled(check);

    ipv6AddressEdit->setEnabled(check);
    m_subnetLabel->setEnabled(check);
    gateWayEdit->setEnabled(check);
    firstDnsEdit->setEnabled(check);
    secondDnsEdit->setEnabled(check);
}
