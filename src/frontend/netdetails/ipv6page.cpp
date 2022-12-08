#include "ipv6page.h"
#include "netdetail.h"

Ipv6Page::Ipv6Page(QWidget *parent):QFrame(parent)
{
    initUI();
    initComponent();
}

void Ipv6Page::setIpv6Config(KyIpConfigType ipv6Config)
{
    if (ipv6Config ==  CONFIG_IP_MANUAL) {
        ipv6ConfigCombox->setCurrentIndex(MANUAL_CONFIG);
    } else {
        ipv6ConfigCombox->setCurrentIndex(AUTO_CONFIG);
    }
}

void Ipv6Page::setIpv6(const QString &ipv6)
{
    ipv6AddressEdit->setText(ipv6);
}

void Ipv6Page::setIpv6Perfix(const int &ipv6Perfix)
{
    lengthEdit->setText(QString::number(ipv6Perfix));
}

void Ipv6Page::setMulDns(const QList<QHostAddress> &dns)
{
    m_dnsWidget->setDnsListText(dns);
}
#if 0
void Ipv6Page::setIpv6FirDns(const QString &ipv6FirDns)
{
    firstDnsEdit->setText(ipv6FirDns);
}

void Ipv6Page::setIpv6SecDns(const QString &ipv6SecDns)
{
    secondDnsEdit->setText(ipv6SecDns);
}
#endif
void Ipv6Page::setGateWay(const QString &gateWay)
{
    gateWayEdit->setText(gateWay);
}

bool Ipv6Page::checkIsChanged(const ConInfo info, KyConnectSetting &setting)
{
    bool isChanged = false;
    QList<QHostAddress> ipv6DnsList;
    ipv6DnsList.clear();
    ipv6DnsList = m_dnsWidget->getDns();
    if (info.ipv6DnsList != ipv6DnsList) {
        qDebug() << "ipv6 dns changed";
        setting.ipv6DnsConstruct(ipv6DnsList);
        isChanged =  true;
    }

    if (ipv6ConfigCombox->currentIndex() == AUTO_CONFIG) {
        if (info.ipv6ConfigType != CONFIG_IP_DHCP) {
            qDebug() << "ipv6ConfigType change to Auto";
            setting.setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);
            QString ipv6address("");
            QString prefix("");
            QString gateWay("");
            setting.ipv6AddressConstruct(ipv6address, prefix, gateWay);
            isChanged = true;
        }
    } else {
        if (info.ipv6ConfigType != CONFIG_IP_MANUAL) {
            qDebug() << "ipv6ConfigType change to Manual";
            setting.setIpConfigType(IPADDRESS_V6, CONFIG_IP_MANUAL);
            isChanged =  true;
        }

        if(info.strIPV6Address != ipv6AddressEdit->text()
                || info.iIPV6Prefix != lengthEdit->text().toInt()
                || info.strIPV6GateWay != gateWayEdit->text()) {

            qDebug() << "ipv6 info changed";

            QString ipv6address =ipv6AddressEdit->text();
            QString prefix = lengthEdit->text();
            QString gateWay = gateWayEdit->text();
            setting.ipv6AddressConstruct(ipv6address, prefix, gateWay);
            setting.dumpInfo();
            isChanged =  true;
        }
    }
    return isChanged;
}

void Ipv6Page::initUI() {
    ipv6ConfigCombox = new QComboBox(this);
    ipv6AddressEdit = new LineEdit(this);
    lengthEdit = new LineEdit(this);
    gateWayEdit = new LineEdit(this);
//    firstDnsEdit = new LineEdit(this);
//    secondDnsEdit = new LineEdit(this);

    m_configLabel = new QLabel(this);
    m_addressLabel = new QLabel(this);
    m_subnetLabel = new QLabel(this);
    m_gateWayLabel = new QLabel(this);
//    m_dnsLabel = new QLabel(this);
//    m_secDnsLabel = new QLabel(this);


    m_configLabel->setText(tr("Ipv6Config"));
    m_addressLabel->setText(tr("Address"));
    m_subnetLabel->setText(tr("Subnet prefix Length"));
    m_gateWayLabel->setText(tr("Default Gateway"));
//    m_dnsLabel->setText(tr("Prefs DNS"));
//    m_secDnsLabel->setText(tr("Alternative DNS"));

    QRegExp ipv6_rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");
    m_dnsWidget = new MultipleDnsWidget(ipv6_rx, this);

    m_detailLayout = new QFormLayout(this);
    m_detailLayout->addRow(m_configLabel,ipv6ConfigCombox);
    m_detailLayout->addRow(m_addressLabel,ipv6AddressEdit);
    m_detailLayout->addRow(m_subnetLabel,lengthEdit);
    m_detailLayout->addRow(m_gateWayLabel,gateWayEdit);
//    m_detailLayout->addRow(m_dnsLabel,firstDnsEdit);
//    m_detailLayout->addRow(m_secDnsLabel,secondDnsEdit);
    m_detailLayout->addRow(m_dnsWidget);

    ipv6ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv6ConfigCombox->addItem(tr("Manual")); //"手动"

    ipv6AddressEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
    gateWayEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
//    firstDnsEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
//    secondDnsEdit->setValidator(new QRegExpValidator(ipv6_rx, this));

    QRegExp prefix_rx("\\b(?:(?:12[0-8]|1[0-1][0-9]|^[1-9][0-9]?$)\\.){3}(?:12[0-8]|1[0-1][0-9]|^[1-9][0-9]?$)\\b");
    lengthEdit->setValidator(new QRegExpValidator(prefix_rx,this));
}

void Ipv6Page::initComponent() {
    if (ipv6ConfigCombox->currentIndex() == AUTO_CONFIG) {
        setControlEnabled(false);
    } else if (ipv6ConfigCombox->currentIndex() == MANUAL_CONFIG) {
        setControlEnabled(true);
    }
    connect(ipv6ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));

    connect(ipv6ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(ipv6AddressEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(lengthEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(gateWayEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
}

void Ipv6Page::configChanged(int index) {
    if (index == AUTO_CONFIG) {
        setControlEnabled(false);
    }
    if (index == MANUAL_CONFIG) {
        setControlEnabled(true);
    }
}

void Ipv6Page::setControlEnabled(bool check)
{
    ipv6AddressEdit->setEnabled(check);
    lengthEdit->setEnabled(check);
    gateWayEdit->setEnabled(check);


    if (!check) {
        ipv6AddressEdit->clear();
        lengthEdit->clear();
        gateWayEdit->clear();
    }
}

void Ipv6Page::setEnableOfSaveBtn()
{
    emit setIpv6PageState(checkConnectBtnIsEnabled());
}

bool Ipv6Page::checkConnectBtnIsEnabled()
{
    if (ipv6ConfigCombox->currentIndex() == AUTO_CONFIG) {
        return true;
    } else {
        if (ipv6AddressEdit->text().isEmpty() || !getIpv6EditState(ipv6AddressEdit->text())) {
            qDebug() << "ipv6address empty or invalid";
            return false;
        }

        if (lengthEdit->text().isEmpty()) {
            qDebug() << "ipv6 prefix length empty";
            return false;
        }

        if (gateWayEdit->text().isEmpty() || !getIpv6EditState(gateWayEdit->text())) {
            qDebug() << "ipv6 gateway empty or invalid";
            return false;
        }
#if 0
        if (firstDnsEdit->text().isEmpty() && !secondDnsEdit->text().isEmpty()) {
            qDebug() << "ipv6 dns sort invalid";
            return false;
        }

        if (!getIpv6EditState(firstDnsEdit->text())) {
            qDebug() << "ipv6 first dns invalid";
            return false;
        }

        if (!getIpv6EditState(secondDnsEdit->text())) {
            qDebug() << "ipv6 second dns invalid";
            return false;
        }
#endif
    }
    return true;
}

bool Ipv6Page::getIpv6EditState(QString text)
{
    if (text.isEmpty()) {
        return true;
    }
    QRegExp rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");

    bool match = false;
    match = rx.exactMatch(text);

    return match;
}

int Ipv6Page::getPerfixLength(QString text)
{
    qDebug() << "getPerfixLength" << text;
    int length = 0;
    QStringList list= text.split(":");
    for (int i = 0; i < list.size(); ++i) {
        QString temp = list.at(i);
        if (temp.isEmpty()) {
            continue;
        }
        bool ok;
        unsigned int val = temp.toUInt(&ok, 16);
        temp = temp.setNum(val,2);
        for(int j = 0; j < temp.length(); ++j) {
            if (temp.at(j) == "1") {
                length++;
            }
        }
    }
    qDebug() << "getPerfixLength" << length;
    return length;
}

