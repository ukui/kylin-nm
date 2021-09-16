#include "ipv4page.h"
#include "netdetail.h"

Ipv4Page::Ipv4Page(QWidget *parent):QFrame(parent)
{
    initUI();
    initComponent();
}

void Ipv4Page::initUI() {
    ipv4ConfigCombox = new QComboBox(this);
    ipv4addressEdit = new QLineEdit(this);
    netMaskEdit = new QLineEdit(this);
    gateWayEdit = new QLineEdit(this);
    firstDnsEdit = new QLineEdit(this);
    secondDnsEdit = new QLineEdit(this);

    m_configLabel = new QLabel(this);
    m_addressLabel = new QLabel(this);
    m_maskLabel = new QLabel(this);
    m_gateWayLabel = new QLabel(this);
    m_dnsLabel = new QLabel(this);
    m_secDnsLabel = new QLabel(this);

    m_configLabel->setText(tr("Ipv4Config"));
    m_addressLabel->setText(tr("Address"));
    m_maskLabel->setText(tr("Netmask"));
    m_gateWayLabel->setText(tr("Default Gateway"));
    m_dnsLabel->setText(tr("Prefs DNS"));
    m_secDnsLabel->setText(tr("Alternative DNS"));

    m_detailLayout = new QFormLayout(this);
    m_detailLayout->addRow(m_configLabel,ipv4ConfigCombox);
    m_detailLayout->addRow(m_addressLabel,ipv4addressEdit);
    m_detailLayout->addRow(m_maskLabel,netMaskEdit);
    m_detailLayout->addRow(m_gateWayLabel,gateWayEdit);
    m_detailLayout->addRow(m_dnsLabel,firstDnsEdit);
    m_detailLayout->addRow(m_secDnsLabel,secondDnsEdit);

    ipv4ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv4ConfigCombox->addItem(tr("Manual")); //"手动"

//    netMaskCombox->addItem("");
//    netMaskCombox->addItem("255.255.255.0"); //24
//    netMaskCombox->addItem("255.255.254.0"); //23
//    netMaskCombox->addItem("255.255.252.0"); //22
//    netMaskCombox->addItem("255.255.0.0"); //16
//    netMaskCombox->addItem("255.0.0.0"); //8


    // IP的正则格式限制
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    ipv4addressEdit->setValidator(new QRegExpValidator(rx, this));
    gateWayEdit->setValidator(new QRegExpValidator(rx, this));
    netMaskEdit->setValidator(new QRegExpValidator(rx, this));
    firstDnsEdit->setValidator(new QRegExpValidator(rx, this));
    secondDnsEdit->setValidator(new QRegExpValidator(rx, this));
}

void Ipv4Page::initComponent() {
    if (ipv4ConfigCombox->currentIndex() == AUTO_CONFIG) {
        setLineEnabled(false);
    } else if (ipv4ConfigCombox->currentIndex() == MANUAL_CONFIG) {
        setLineEnabled(true);
    }
    connect(ipv4ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));

    connect(ipv4ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(netMaskEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(gateWayEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(firstDnsEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(secondDnsEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
}

void Ipv4Page::setIpv4Config(KyIpConfigType ipv4Config)
{
    if (ipv4Config ==  CONFIG_IP_MANUAL) {
        ipv4ConfigCombox->setCurrentIndex(MANUAL_CONFIG);
    } else {
        ipv4ConfigCombox->setCurrentIndex(AUTO_CONFIG);
    }
}

void Ipv4Page::setIpv4(const QString &ipv4)
{
    ipv4addressEdit->setText(ipv4);
}

void Ipv4Page::setNetMask(const QString &netMask)
{
    netMaskEdit->setText(netMask);
}

void Ipv4Page::setIpv4FirDns(const QString &ipv4FirDns)
{
    firstDnsEdit->setText(ipv4FirDns);
}

void Ipv4Page::setIpv4SecDns(const QString &ipv4SecDns)
{
    secondDnsEdit->setText(ipv4SecDns);
}

void Ipv4Page::setGateWay(const QString &gateWay)
{
    gateWayEdit->setText(gateWay);
}

bool Ipv4Page::checkIsChanged(const ConInfo info, KyConnectSetting &setting)
{
    bool isChanged = false;
    if (ipv4ConfigCombox->currentIndex() == AUTO_CONFIG) {
        if (info.ipv4ConfigType != CONFIG_IP_DHCP) {
            qDebug() << "ipv4ConfigType change to Auto";
            setting.setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
            isChanged = true;
        }
    } else {
        if (info.ipv4ConfigType != CONFIG_IP_MANUAL) {
            qDebug() << "ipv4ConfigType change to Manual";
            setting.setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
            isChanged =  true;
        }
        if(info.strIPV4Address != ipv4addressEdit->text()
                || info.strIPV4NetMask != netMaskEdit->text()
                || info.strIPV4GateWay != gateWayEdit->text()
                || info.strIPV4FirDns  != firstDnsEdit->text()
                || info.strIPV4SecDns  != secondDnsEdit->text()) {

            qDebug() << "ipv4 info changed";
            QStringList dnsList;
            dnsList.empty();
            if (!firstDnsEdit->text().isEmpty()) {
                dnsList << firstDnsEdit->text();
                if (!secondDnsEdit->text().isEmpty()) {
                    dnsList << secondDnsEdit->text();
                }
            }

            QString ipv4address =ipv4addressEdit->text();
            QString netMask = netMaskEdit->text();
            QString gateWay = gateWayEdit->text();
            qDebug() << ipv4address << netMask << gateWay;
            setting.ipv4AddressConstruct(ipv4address, netMask, gateWay, dnsList);
            setting.dumpInfo();
            isChanged =  true;
        }
    }
    return isChanged;
}

bool Ipv4Page::checkConnectBtnIsEnabled()
{
    qDebug() << "checkConnectBtnIsEnabled currentIndex" << ipv4ConfigCombox->currentIndex();
    if (ipv4ConfigCombox->currentIndex() == AUTO_CONFIG) {
        return true;
    } else {
        if (ipv4addressEdit->text().isEmpty() || !getTextEditState(ipv4addressEdit->text())) {
            qDebug() << "ipv4address empty or invalid";
            return false;
        }

        if (netMaskEdit->text().isEmpty() || !getTextEditState(netMaskEdit->text())) {
            qDebug() << "ipv4 netMask empty or invalid";
            return false;
        }

        if (gateWayEdit->text().isEmpty() || !getTextEditState(gateWayEdit->text())) {
            qDebug() << "ipv4 gateway empty or invalid";
            return false;
        }

        if (firstDnsEdit->text().isEmpty() && !secondDnsEdit->text().isEmpty()) {
            qDebug() << "ipv4 dns sort invalid";
            return false;
        }

        if (!getTextEditState(firstDnsEdit->text())) {
            qDebug() << "ipv4 first dns invalid";
            return false;
        }

        if (!getTextEditState(secondDnsEdit->text())) {
            qDebug() << "ipv4 second dns invalid";
            return false;
        }
    }
    return true;
}

void Ipv4Page::configChanged(int index) {
    if (index == AUTO_CONFIG) {
        setLineEnabled(false);
    }
    if (index == MANUAL_CONFIG) {
        setLineEnabled(true);
    }
}

void Ipv4Page::setLineEnabled(bool check) {

    ipv4addressEdit->setEnabled(check);
    netMaskEdit->setEnabled(check);
    gateWayEdit->setEnabled(check);
    firstDnsEdit->setEnabled(check);
    secondDnsEdit->setEnabled(check);

    if (!check) {
        ipv4addressEdit->clear();
        netMaskEdit->clear();
        gateWayEdit->clear();
        firstDnsEdit->clear();
        secondDnsEdit->clear();
    }
}

void Ipv4Page::setEnableOfSaveBtn() {
    emit setIpv4PageState(checkConnectBtnIsEnabled());
}

bool Ipv4Page::getTextEditState(QString text)
{
    if (text.isEmpty()) {
        return true;
    }
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    bool match = false;
    match = rx.exactMatch(text);

    return match;
}

