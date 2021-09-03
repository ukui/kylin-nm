#include "ipv6page.h"

Ipv6Page::Ipv6Page(bool isWlan)
    :isWlan(isWlan)
{
    initUI();
    initComponent();
}

void Ipv6Page::setIpv6Config(const QString &ipv6Config)
{
    if (ipv6Config ==  "0") {
        ipv6ConfigCombox->setCurrentIndex(1);
    } else {
        ipv6ConfigCombox->setCurrentIndex(0);
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
    ipv6ConfigCombox = new QComboBox;
    ipv6AddressEdit = new QLineEdit;
    lengthEdit = new QLineEdit;
    gateWayEdit = new QLineEdit;
    firstDnsEdit = new QLineEdit;
    secondDnsEdit = new QLineEdit;

    mConfigLabel = new QLabel;
    mAddressLabel = new QLabel;
    mSubnetLabel = new QLabel;
    mGateWayLabel = new QLabel;
    mDnsLabel = new QLabel;
    mSecDnsLabel = new QLabel;


    mConfigLabel->setText(tr("Ipv6Config"));
    mAddressLabel->setText(tr("Address"));
    mSubnetLabel->setText(tr("Subnet prefix Length"));
    mGateWayLabel->setText(tr("Default Gateway"));
    mDnsLabel->setText(tr("Prefs DNS"));
    mSecDnsLabel->setText(tr("Alternative DNS"));


    mDetailLayout = new QFormLayout(this);
    mDetailLayout->addRow(mConfigLabel,ipv6ConfigCombox);
    mDetailLayout->addRow(mAddressLabel,ipv6AddressEdit);
    mDetailLayout->addRow(mSubnetLabel,lengthEdit);
    mDetailLayout->addRow(mGateWayLabel,gateWayEdit);
    mDetailLayout->addRow(mDnsLabel,firstDnsEdit);
    mDetailLayout->addRow(mSecDnsLabel,secondDnsEdit);

    ipv6ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv6ConfigCombox->addItem(tr("Manual")); //"手动"

    QRegExp ipv6_rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");
    ipv6AddressEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
}

void Ipv6Page::initComponent() {
    if (ipv6ConfigCombox->currentIndex() == 0) {
        setControlEnabled(false);
    } else if (ipv6ConfigCombox->currentIndex() == 1) {
        setControlEnabled(true);
    }
    connect(ipv6ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));
}

void Ipv6Page::configChanged(int index) {
    if (index == 0) {
        setControlEnabled(false);
    }
    if (index == 1) {
        setControlEnabled(true);
    }
}

void Ipv6Page::setControlEnabled(bool check) {
    mAddressLabel->setEnabled(check);
    mSubnetLabel->setEnabled(check);
    lengthEdit->setEnabled(check);
    mGateWayLabel->setEnabled(check);
    mDnsLabel->setEnabled(check);
    mSecDnsLabel->setEnabled(check);

    ipv6AddressEdit->setEnabled(check);
    mSubnetLabel->setEnabled(check);
    gateWayEdit->setEnabled(check);
    firstDnsEdit->setEnabled(check);
    secondDnsEdit->setEnabled(check);
}
