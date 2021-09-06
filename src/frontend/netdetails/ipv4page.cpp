#include "ipv4page.h"
#include "netdetail.h"
Ipv4Page::Ipv4Page(bool isWlan, QWidget *parent)
    : isWlan(isWlan), QFrame(parent)
{
    initUI();
    initComponent();
}

void Ipv4Page::initUI() {
    ipv4ConfigCombox = new QComboBox(this);
    ipv4addressEdit = new QLineEdit(this);
    netMaskCombox = new QComboBox(this);
    gateWayEdit = new QLineEdit(this);
    firstDnsEidt = new QLineEdit(this);
    secondDnsEidt = new QLineEdit(this);

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
    m_detailLayout->addRow(m_maskLabel,netMaskCombox);
    m_detailLayout->addRow(m_gateWayLabel,gateWayEdit);
    m_detailLayout->addRow(m_dnsLabel,firstDnsEidt);
    m_detailLayout->addRow(m_secDnsLabel,secondDnsEidt);

    ipv4ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv4ConfigCombox->addItem(tr("Manual")); //"手动"

    netMaskCombox->addItem("255.255.255.0"); //24
    netMaskCombox->addItem("255.255.254.0"); //23
    netMaskCombox->addItem("255.255.252.0"); //22
    netMaskCombox->addItem("255.255.0.0"); //16
    netMaskCombox->addItem("255.0.0.0"); //8

    // IP的正则格式限制
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ipv4addressEdit->setValidator(new QRegExpValidator(rx, this));
    gateWayEdit->setValidator(new QRegExpValidator(rx, this));
    firstDnsEidt->setValidator(new QRegExpValidator(rx, this));
    secondDnsEidt->setValidator(new QRegExpValidator(rx, this));
    setEnableOfSaveBtn();
}

void Ipv4Page::initComponent() {
    if (ipv4ConfigCombox->currentIndex() == AUTO_CONFIG) {
        setLineEnabled(false);
    } else if (ipv4ConfigCombox->currentIndex() == MANUAL_CONFIG) {
        setLineEnabled(true);
    }
    connect(ipv4ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));
//    connect(mNetMask, SIGNAL(currentIndexChanged(int)), this, SLOT(cbMaskChanged(int)));
}

void Ipv4Page::setIpv4Config(const QString &ipv4Config)
{
    if (ipv4Config.toInt() ==  AUTO_CONFIG) {
        ipv4ConfigCombox->setCurrentIndex(MANUAL_CONFIG);
    } else {
        ipv4ConfigCombox->setCurrentIndex(AUTO_CONFIG);
    }
}

void Ipv4Page::setIpv4(const QString &ipv4)
{
    ipv4addressEdit->setText(ipv4);
}

void Ipv4Page::setIpv4FirDns(const QString &ipv4FirDns)
{
    firstDnsEidt->setText(ipv4FirDns);
}

void Ipv4Page::setIpv4SecDns(const QString &ipv4SecDns)
{
    secondDnsEidt->setText(ipv4SecDns);
}

void Ipv4Page::setGateWay(const QString &gateWay)
{
    gateWayEdit->setText(gateWay);
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
    m_addressLabel->setEnabled(check);
    m_maskLabel->setEnabled(check);
    m_gateWayLabel->setEnabled(check);
    m_dnsLabel->setEnabled(check);
    m_secDnsLabel->setEnabled(check);

    ipv4addressEdit->setEnabled(check);
    netMaskCombox->setEnabled(check);
    gateWayEdit->setEnabled(check);
    firstDnsEidt->setEnabled(check);
    secondDnsEidt->setEnabled(check);
}

void Ipv4Page::setEnableOfSaveBtn() {
    if (ipv4ConfigCombox->currentIndex() == 1) {
//        if (mIpv4address->text().isEmpty()) {
//            //当ipv4和ipv6地址均未设置时，禁止保存
//            emit setBtnEnableFalse();
//            return;
//        }

//        if (!ui->leAddr->text().isEmpty() && !this->getTextEditState(ui->leAddr->text()) ) {
//            emit setBtnEnableFalse();
//            return;
//        }

//        if (!ui->leGateway->text().isEmpty() && !this->getTextEditState(ui->leGateway->text()) ) {
//            emit setBtnEnableFalse();
//            return;
//        }

//        if (!ui->leDns->text().isEmpty() && !this->getTextEditState(ui->leDns->text()) ) {
//            emit setBtnEnableFalse();
//            return;
//        }

//        if (!ui->leAddr_ipv6->text().isEmpty() && ! this->getIpv6EditState(ui->leAddr_ipv6->text())) {
//            emit setBtnEnableFalse();
//            return;
//        }
//        if(ui->leDns2->text().isEmpty()){

//        }else{
//            if(!this->getTextEditState(ui->leDns2->text())){
//                emit setBtnEnableFalse();
//                return ;
//            }
//        }
    }
}

