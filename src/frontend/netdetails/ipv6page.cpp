/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "ipv6page.h"
#include "netdetail.h"

#define  LAYOUT_MARGINS 0,0,0,0
#define  LAYOUT_SPACING 0
#define  HINT_TEXT_MARGINS 8, 1, 0, 3
#define  LABEL_HEIGHT 24
#define FRAME_SPEED 150
#define ICON_SIZE 16,16

Ipv6Page::Ipv6Page(QWidget *parent):QFrame(parent)
{
    initUI();
    initComponent();
    initNetCtrl();
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

void Ipv6Page::setGateWay(const QString &gateWay)
{
    gateWayEdit->setText(gateWay);
}

bool Ipv6Page::checkIsChanged(const ConInfo info, KyConnectSetting &setting)
{
    bool isChanged = false;
    KyIpConfigType type;
    if (ipv6ConfigCombox->currentIndex() == AUTO_CONFIG) {
        type = CONFIG_IP_DHCP;
        if (info.ipv6ConfigType != CONFIG_IP_DHCP) {
            qDebug() << "ipv6ConfigType change to Auto";
            isChanged = true;
        }
    } else {
        type = CONFIG_IP_MANUAL;
        if (info.ipv6ConfigType != CONFIG_IP_MANUAL) {
            qDebug() << "ipv6ConfigType change to Manual";
            isChanged =  true;
        }

        if(info.strIPV6Address != ipv6AddressEdit->text()
                || info.iIPV6Prefix != lengthEdit->text().toInt()
                || info.strIPV6GateWay != gateWayEdit->text()) {

            isChanged =  true;
        }
    }
    QList<QHostAddress> ipv6dnsList;
    ipv6dnsList.clear();
    ipv6dnsList = m_dnsWidget->getDns();
    if (info.ipv6DnsList != ipv6dnsList) {
        isChanged = true;
    }

    if (isChanged) {
        setting.setIpConfigType(IPADDRESS_V6, type);
        QString ipv6address =ipv6AddressEdit->text();
        QString prefix = lengthEdit->text();
        QString gateWay = gateWayEdit->text();
        setting.ipv6AddressConstruct(ipv6address, prefix, gateWay);
        setting.ipv6DnsConstruct(ipv6dnsList);
        setting.dumpInfo();
    }
    return isChanged;
}

void Ipv6Page::initUI() {
    ipv6ConfigCombox = new QComboBox(this);
    ipv6AddressEdit = new LineEdit(this);
    lengthEdit = new LineEdit(this);
    gateWayEdit = new LineEdit(this);

    m_configLabel = new QLabel(this);
    m_addressLabel = new QLabel(this);
    m_subnetLabel = new QLabel(this);
    m_gateWayLabel = new QLabel(this);

    m_configEmptyLabel = new QLabel(this);
    m_configEmptyLabel->setFixedHeight(LABEL_HEIGHT);

    m_addressHintLabel = new KLabel(this);
    m_addressHintLabel->setFixedHeight(LABEL_HEIGHT);
    m_addressHintLabel->setContentsMargins(HINT_TEXT_MARGINS);
    initConflictHintLable();

    m_gateWayHintLabel = new KLabel(this);
    m_gateWayHintLabel->setFixedHeight(LABEL_HEIGHT);
    m_gateWayHintLabel->setContentsMargins(HINT_TEXT_MARGINS);

    m_subnetEmptyLabel = new QLabel(this);
    m_subnetEmptyLabel->setFixedHeight(LABEL_HEIGHT);

    m_configLabel->setText(tr("IPv6Config"));
    m_addressLabel->setText(tr("Address"));
    m_subnetLabel->setText(tr("Subnet prefix Length"));
    m_gateWayLabel->setText(tr("Default Gateway"));

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QHBoxLayout *pPwdLayout = new QHBoxLayout(ipv6AddressEdit);
    pPwdLayout->addStretch();
    pPwdLayout->addWidget(m_statusLabel);

    //V11 qlabel 使用调色板颜色显示有问题
    m_addressHintLabel->setFontColorRole(QPalette::WindowText);
    m_addressHintLabel->setFontColor(Qt::red);
    m_gateWayHintLabel->setFontColorRole(QPalette::WindowText);
    m_gateWayHintLabel->setFontColor(Qt::red);

    QWidget *addressWidget = new QWidget(this);
    QVBoxLayout *addressLayout = new QVBoxLayout(addressWidget);
    addressLayout->setContentsMargins(LAYOUT_MARGINS);
    addressLayout->setSpacing(LAYOUT_SPACING);
    addressLayout->addWidget(ipv6AddressEdit);
    addressLayout->addWidget(m_addressHintLabel);

    QWidget *gateWayWidget = new QWidget(this);
    QVBoxLayout *gateWayLayout = new QVBoxLayout(gateWayWidget);
    gateWayLayout->setContentsMargins(LAYOUT_MARGINS);
    gateWayLayout->setSpacing(LAYOUT_SPACING);
    gateWayLayout->addWidget(gateWayEdit);
    gateWayLayout->addWidget(m_gateWayHintLabel);

    QRegularExpression ipv6_rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");
    m_dnsWidget = new MultipleDnsWidget(ipv6_rx, false, this);

    m_detailLayout = new QFormLayout(this);
    m_detailLayout->setContentsMargins(0, 0, 0, 0);
    m_detailLayout->setVerticalSpacing(0);
    m_detailLayout->addRow(m_configLabel,ipv6ConfigCombox);
    m_detailLayout->addRow(m_configEmptyLabel);
    m_detailLayout->addRow(m_addressLabel,addressWidget);
    m_detailLayout->addRow(m_subnetLabel,lengthEdit);
    m_detailLayout->addRow(m_subnetEmptyLabel);
    m_detailLayout->addRow(m_gateWayLabel,gateWayWidget);
    m_detailLayout->addRow(m_dnsWidget);
    m_addressLabel->setContentsMargins(0, 0, 0, LABEL_HEIGHT);  //解决布局错位问题
    m_gateWayLabel->setContentsMargins(0, 0, 0, LABEL_HEIGHT);

    ipv6ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv6ConfigCombox->addItem(tr("Manual")); //"手动"

    ipv6AddressEdit->setValidator(new QRegularExpressionValidator(ipv6_rx, this));
    gateWayEdit->setValidator(new QRegularExpressionValidator(ipv6_rx, this));

    QRegularExpression prefix_rx("\\b(?:(?:12[0-8]|1[0-1][0-9]|^[1-9][0-9]?$)\\.){3}(?:12[0-8]|1[0-1][0-9]|^[1-9][0-9]?$)\\b");
    lengthEdit->setValidator(new QRegularExpressionValidator(prefix_rx,this));

    initLoadingIcon();
}

void Ipv6Page::initComponent() {
    if (ipv6ConfigCombox->currentIndex() == AUTO_CONFIG) {
        setControlEnabled(false);
    } else if (ipv6ConfigCombox->currentIndex() == MANUAL_CONFIG) {
        setControlEnabled(true);
    }
    connect(ipv6ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(configChanged(int)));

    connect(ipv6AddressEdit, SIGNAL(textChanged(QString)), this, SLOT(onAddressTextChanged()));
    connect(ipv6AddressEdit, SIGNAL(editingFinished()), this, SLOT(onAddressEidtFinished()));
    connect(gateWayEdit, SIGNAL(textChanged(QString)), this, SLOT(onGatewayTextChanged()));

    connect(ipv6ConfigCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnableOfSaveBtn()));
    connect(ipv6AddressEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(lengthEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(gateWayEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));

    connect(m_dnsWidget, &MultipleDnsWidget::scrollToBottom, this, &Ipv6Page::scrollToBottom);
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
    if (!check) {
        ipv6AddressEdit->clear();
        lengthEdit->clear();
        gateWayEdit->clear();

        ipv6AddressEdit->setPlaceholderText(" ");
        lengthEdit->setPlaceholderText(" ");
        gateWayEdit->setPlaceholderText(" ");
    } else {
        ipv6AddressEdit->setPlaceholderText(tr("Required")); //必填
        lengthEdit->setPlaceholderText(tr("Required")); //必填
    }

    updateUi();
}

void Ipv6Page::setEnableOfSaveBtn()
{
    Q_EMIT setIpv6PageState(checkConnectBtnIsEnabled());
}

void Ipv6Page::onAddressTextChanged()
{
    m_iconLabel->hide();
    m_textLabel->hide();

    if (!getIpv6EditState(ipv6AddressEdit->text())) {
        m_addressHintLabel->setText(tr("Invalid address"));
    } else {
        m_addressHintLabel->clear();
    }
}

void Ipv6Page::onGatewayTextChanged()
{
    if (!getIpv6EditState(gateWayEdit->text())) {
        m_gateWayHintLabel->setText(tr("Invalid gateway"));
    } else {
        m_gateWayHintLabel->clear();
    }
}

void Ipv6Page::onAddressEidtFinished()
{
    if (ipv6AddressEdit->isModified()) {
        if (!ipv6AddressEdit->text().isEmpty() && getIpv6EditState(ipv6AddressEdit->text())) {
            Q_EMIT ipv6EditFinished(ipv6AddressEdit->text());
        }
    }
}

bool Ipv6Page::checkConnectBtnIsEnabled()
{
    if (ipv6ConfigCombox->currentIndex() != AUTO_CONFIG){
        if (ipv6AddressEdit->text().isEmpty() || !getIpv6EditState(ipv6AddressEdit->text())) {
            qDebug() << "ipv6address empty or invalid";
            m_errorMessage = tr("IPv6 address is empty or invalid");
            return false;
        }

        if (lengthEdit->text().isEmpty()) {
            qDebug() << "ipv6 prefix length empty";
            m_errorMessage = tr("IPv6 prefix length is empty or invalid");
            return false;
        }

//        if (gateWayEdit->text().isEmpty() || !getIpv6EditState(gateWayEdit->text())) {
//            qDebug() << "ipv6 gateway empty or invalid";
//            return false;
//        }
    }
    m_errorMessage.clear();
    return true;
}

void Ipv6Page::initConflictHintLable()
{
    QIcon icon = QIcon::fromTheme("dialog-warning");
    m_iconLabel = new QLabel(m_addressHintLabel);
    m_iconLabel->setPixmap(icon.pixmap(ICON_SIZE));
    m_textLabel = new QLabel(m_addressHintLabel);
    m_textLabel->setText(tr("Address conflict"));
    QHBoxLayout *conflictHintLayout = new QHBoxLayout(m_addressHintLabel);
    conflictHintLayout->setContentsMargins(0, 0, 0, 0);
    conflictHintLayout->addWidget(m_iconLabel);
    conflictHintLayout->addWidget(m_textLabel);
    conflictHintLayout->addStretch();
    m_addressHintLabel->setLayout(conflictHintLayout);
    m_iconLabel->hide();
    m_textLabel->hide();
}

bool Ipv6Page::getIpv6EditState(QString text)
{
    if (text.isEmpty()) {
        return true;
    }
    QRegularExpression rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");

    // 进行匹配并返回结果
    QRegularExpressionMatch match = rx.match(text);
    return match.hasMatch();
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

void Ipv6Page::initLoadingIcon()
{
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    m_iconTimer = new QTimer(this);
    connect(m_iconTimer, &QTimer::timeout, this, &Ipv6Page::updateIcon);
}

void Ipv6Page::updateIcon()
{
    if (m_currentIconIndex > 6) {
        m_currentIconIndex = 0;
    }
    m_statusLabel->setPixmap(m_loadIcons.at(m_currentIconIndex).pixmap(ICON_SIZE));
    m_currentIconIndex ++;
}

void Ipv6Page::startLoading()
{
    m_iconTimer->start(FRAME_SPEED);
}

void Ipv6Page::stopLoading()
{
    m_iconTimer->stop();
    m_statusLabel->clear();
}

void Ipv6Page::showIpv6AddressConflict(bool isConflict)
{
    if (isConflict) {
        m_iconLabel->show();
        m_textLabel->show();
    } else {
        m_iconLabel->hide();
        m_textLabel->hide();
    }
}

QString Ipv6Page::getErrorMessage()
{
    checkConnectBtnIsEnabled();
    return m_errorMessage;
}


void Ipv6Page::updateNetCtrl(QString modName,QVariantMap value)
{
    if(modName!="IPV6" && modName!="DNS") return;

    qInfo()<<modName<<value;
    for (auto it = value.cbegin(); it != value.cend(); ++it)
    {
        QString key = it.key();
        QVariant value = it.value();
        if(key==QString("netIPV6ModifyCtrol"))
        {
            m_ipaddressCtrl=value.toBool();
        }
        if(key==QString("netIPV6DNSModifyCtrol"))//管控第二次需求修改需要ipv4 ipv6的dns管控分开
        {
            m_dnsWayCtrl=value.toBool();
        }
        if(key==QString("netMaskModifyCtrol"))
        {
            m_netMaskCtrl=value.toBool();
        }
        if(key==QString("netGwModifyCtrol"))
        {
            m_gateWayCtrl=value.toBool();
        }
    }

    updateUi();
    //setLineEnabled(false);
    return;
}

void Ipv6Page::updateUi()
{
    int currentIndex=ipv6ConfigCombox->currentIndex();

    if (m_ipaddressCtrl || currentIndex==AUTO_CONFIG)
    {
        ipv6AddressEdit->setEnabled(false);
    }
    else
    {
        ipv6AddressEdit->setEnabled(true);
    }

    if (m_netMaskCtrl || currentIndex==AUTO_CONFIG)
    {
        lengthEdit->setEnabled(false);
    }
    else
    {
        lengthEdit->setEnabled(true);
    }

    if (m_gateWayCtrl || currentIndex==AUTO_CONFIG)
    {
        gateWayEdit->setEnabled(false);
    }
    else
    {
        gateWayEdit->setEnabled(true);
    }

    if(m_dnsWayCtrl)
        m_dnsWidget->setEnabled(false);
    else
        m_dnsWidget->setEnabled(true);
}

void Ipv6Page::initNetCtrl()
{
    QVariantMap map;
    int errCode=0;
    QString netCtrlIPV6Name="IPV6";
    QString netCtrlDNSName="DNS";
    QDBusInterface dbusInterface("com.kylin.networkCtrol",
                                 "/com/kylin/networkCtrol",
                                 "com.kylin.networkCtrol",
                              QDBusConnection::systemBus());
    if (!dbusInterface.isValid()) {
        qWarning()<<Q_FUNC_INFO<<__LINE__<<"dbusInterface error!";
    }
    dbusInterface.setTimeout(2000);
    QDBusMessage result = dbusInterface.call("getNetContrlRule",netCtrlIPV6Name);
    if(result.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "[ipv6]getNetContrlRule error:" << result.errorMessage();
    }

   if( result.arguments().size()>=2)
   {
       const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
       dbusArg1st >> map;
       errCode = result.arguments().at( 1 ).toInt();
       qInfo()<<"ipv6"<<map<<errCode;
       if(errCode==0) updateNetCtrl(netCtrlIPV6Name,map);
       map.clear();
   }


   result = dbusInterface.call("getNetContrlRule",netCtrlDNSName);
   if(result.type() == QDBusMessage::ErrorMessage)
   {
       qWarning() << "[ipv6]getNetContrlRule error:" << result.errorMessage();
   }

  if( result.arguments().size()>=2)
  {
      const QDBusArgument &dbusArg1st = result.arguments().at( 0 ).value<QDBusArgument>();
      dbusArg1st >> map;
      errCode = result.arguments().at( 1 ).toInt();
      qInfo()<<"ipv6"<<map<<errCode;
      if(errCode==0) updateNetCtrl(netCtrlDNSName,map);
      map.clear();
  }

   // connect(m_interface,SIGNAL(sigNetContrlRuleChanged(QString ,QVariantMap )),this,SLOT(updateNetCtrl(QString ,QVariantMap)),Qt::QueuedConnection);

    QDBusConnection::systemBus().connect("com.kylin.networkCtrol",
                                         "/com/kylin/networkCtrol",
                                         "com.kylin.networkCtrol",
                                         "sigNetContrlRuleChanged",
                                         this,
                                         SLOT(updateNetCtrl(QString ,QVariantMap)));

    qInfo()<<"initNetCtrl success";


    return;
}
