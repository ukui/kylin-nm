#include "creatnetpage.h"



CreatNetPage::CreatNetPage(QWidget *parent):QFrame(parent)
{
    initUI();
    initComponent();
}

void CreatNetPage::initUI()
{
    connNameEdit = new QLineEdit(this);
    ipv4ConfigCombox = new QComboBox(this);
    ipv4addressEdit = new QLineEdit(this);
    netMaskEdit = new QLineEdit(this);
    gateWayEdit = new QLineEdit(this);
    firstDnsEdit = new QLineEdit(this);
    secondDnsEdit = new QLineEdit(this);

    m_connNameLabel = new QLabel(this);
    m_configLabel = new QLabel(this);
    m_addressLabel = new QLabel(this);
    m_maskLabel = new QLabel(this);
    m_gateWayLabel = new QLabel(this);
    m_dnsLabel = new QLabel(this);
    m_secDnsLabel = new QLabel(this);

    m_connNameLabel->setText(tr("Connection Name"));
    m_configLabel->setText(tr("Ipv4Config"));
    m_addressLabel->setText(tr("Address"));
    m_maskLabel->setText(tr("Netmask"));
    m_gateWayLabel->setText(tr("Default Gateway"));
    m_dnsLabel->setText(tr("Prefs DNS"));
    m_secDnsLabel->setText(tr("Alternative DNS"));

    m_detailLayout = new QFormLayout(this);
    m_detailLayout->addRow(m_connNameLabel,connNameEdit);
    m_detailLayout->addRow(m_configLabel,ipv4ConfigCombox);
    m_detailLayout->addRow(m_addressLabel,ipv4addressEdit);
    m_detailLayout->addRow(m_maskLabel,netMaskEdit);
    m_detailLayout->addRow(m_gateWayLabel,gateWayEdit);
    m_detailLayout->addRow(m_dnsLabel,firstDnsEdit);
    m_detailLayout->addRow(m_secDnsLabel,secondDnsEdit);

    ipv4ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv4ConfigCombox->addItem(tr("Manual")); //"手动"


    // IP的正则格式限制
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    ipv4addressEdit->setValidator(new QRegExpValidator(rx, this));
    gateWayEdit->setValidator(new QRegExpValidator(rx, this));
    netMaskEdit->setValidator(new QRegExpValidator(rx, this));
    firstDnsEdit->setValidator(new QRegExpValidator(rx, this));
    secondDnsEdit->setValidator(new QRegExpValidator(rx, this));
}

void CreatNetPage::initComponent() {
    if (ipv4ConfigCombox->currentIndex() == AUTO_CONFIG) {
        setLineEnabled(false);
    } else if (ipv4ConfigCombox->currentIndex() == MANUAL_CONFIG) {
        setLineEnabled(true);
    }
    connect(ipv4ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));

    connect(connNameEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(ipv4ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(netMaskEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(gateWayEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(firstDnsEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(secondDnsEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
}

bool CreatNetPage::checkConnectBtnIsEnabled()
{
    if (connNameEdit->text().isEmpty()) {
        qDebug() << "create connName empty or invalid";
        return false;
    }
    qDebug() << "checkConnectBtnIsEnabled currentIndex" << ipv4ConfigCombox->currentIndex();
    if (ipv4ConfigCombox->currentIndex() == AUTO_CONFIG) {
        return true;
    } else {
        if (ipv4addressEdit->text().isEmpty() || !getTextEditState(ipv4addressEdit->text())) {
            qDebug() << "create ipv4address empty or invalid";
            return false;
        }

        if (netMaskEdit->text().isEmpty() || !getTextEditState(netMaskEdit->text())) {
            qDebug() << "create ipv4 netMask empty or invalid";
            return false;
        }

        if (gateWayEdit->text().isEmpty() || !getTextEditState(gateWayEdit->text())) {
            qDebug() << "create ipv4 gateway empty or invalid";
            return false;
        }

        if (firstDnsEdit->text().isEmpty() && !secondDnsEdit->text().isEmpty()) {
            qDebug() << "create ipv4 dns sort invalid";
            return false;
        }

        if (!getTextEditState(firstDnsEdit->text())) {
            qDebug() << "create ipv4 first dns invalid";
            return false;
        }

        if (!getTextEditState(secondDnsEdit->text())) {
            qDebug() << "create ipv4 second dns invalid";
            return false;
        }
    }
    return true;
}

void CreatNetPage::configChanged(int index) {
    if (index == AUTO_CONFIG) {
        setLineEnabled(false);
    }
    if (index == MANUAL_CONFIG) {
        setLineEnabled(true);
    }
}

void CreatNetPage::setLineEnabled(bool check) {

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

void CreatNetPage::setEnableOfSaveBtn() {
    emit setCreatePageState(checkConnectBtnIsEnabled());
}

bool CreatNetPage::getTextEditState(QString text)
{
    if (text.isEmpty()) {
        return true;
    }
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    bool match = false;
    match = rx.exactMatch(text);

    return match;
}

void CreatNetPage::constructIpv4Info(KyConnectSetting &setting)
{
    setting.m_connectName = connNameEdit->text();
    QString ipv4address =ipv4addressEdit->text();
    QString netMask = netMaskEdit->text();
    QString gateWay = gateWayEdit->text();
    qDebug() << ipv4address << netMask << gateWay;

    QStringList dnsList;
    dnsList.empty();
    if (!firstDnsEdit->text().isEmpty()) {
        dnsList << firstDnsEdit->text();
        if (!secondDnsEdit->text().isEmpty()) {
            dnsList << secondDnsEdit->text();
        }
    }
    if (ipv4ConfigCombox->currentData() == AUTO_CONFIG) {
        setting.setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
    } else {
        setting.setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
        setting.ipv4AddressConstruct(ipv4address, netMask, gateWay, dnsList);
    }

}
