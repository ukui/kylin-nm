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

bool Ipv6Page::checkIsChanged(const ConInfo info, KyConnectSetting &setting)
{
    bool isChanged = false;
    if (ipv6ConfigCombox->currentIndex() == AUTO_CONFIG) {
        if (info.ipv6ConfigType != CONFIG_IP_DHCP) {
            qDebug() << "ipv6ConfigType change to Auto";
            setting.setIpConfigType(IPADDRESS_V6, CONFIG_IP_DHCP);
            QString ipv6address("");
            QString prefix("");
            QString gateWay("");
            QStringList dnsList;
            dnsList.empty();
            setting.ipv6AddressConstruct(ipv6address, prefix, gateWay, dnsList);
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
                || info.strIPV6GateWay != gateWayEdit->text()
                || info.strIPV6FirDns  != firstDnsEdit->text()
                || info.strIPV6SecDns  != secondDnsEdit->text()) {

            qDebug() << "ipv6 info changed";
            QStringList dnsList;
            dnsList.empty();
            if (!firstDnsEdit->text().isEmpty()) {
                dnsList << firstDnsEdit->text();
                if (!secondDnsEdit->text().isEmpty()) {
                    dnsList << secondDnsEdit->text();
                }
            }

            QString ipv6address =ipv6AddressEdit->text();
            QString prefix = lengthEdit->text();
            QString gateWay = gateWayEdit->text();
            setting.ipv6AddressConstruct(ipv6address, prefix, gateWay, dnsList);
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
    firstDnsEdit = new LineEdit(this);
    secondDnsEdit = new LineEdit(this);

    m_configLabel = new QLabel(this);
    m_addressLabel = new QLabel(this);
    m_subnetLabel = new QLabel(this);
    m_gateWayLabel = new QLabel(this);
    m_dnsLabel = new QLabel(this);
    m_secDnsLabel = new QLabel(this);

    m_configEmptyLabel = new QLabel(this);
    m_configEmptyLabel->setFixedHeight(LABEL_HEIGHT);

    m_addressHintLabel = new QLabel(this);
    m_addressHintLabel->setFixedHeight(LABEL_HEIGHT);
    m_addressHintLabel->setContentsMargins(HINT_TEXT_MARGINS);
    initConflictHintLable();

    m_gateWayHintLabel = new QLabel(this);
    m_gateWayHintLabel->setFixedHeight(LABEL_HEIGHT);
    m_gateWayHintLabel->setContentsMargins(HINT_TEXT_MARGINS);

    m_subnetEmptyLabel = new QLabel(this);
    m_subnetEmptyLabel->setFixedHeight(LABEL_HEIGHT);

    m_firstDnsEmptyLabel = new QLabel(this);
    m_firstDnsEmptyLabel->setFixedHeight(LABEL_HEIGHT);


    m_configLabel->setText(tr("Ipv6Config"));
    m_addressLabel->setText(tr("Address"));
    m_subnetLabel->setText(tr("Subnet prefix Length"));
    m_gateWayLabel->setText(tr("Default Gateway"));
    m_dnsLabel->setText(tr("Prefs DNS"));
    m_secDnsLabel->setText(tr("Alternative DNS"));

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QHBoxLayout *pPwdLayout = new QHBoxLayout(ipv6AddressEdit);
    pPwdLayout->addStretch();
    pPwdLayout->addWidget(m_statusLabel);

    QPalette hintTextColor;
    hintTextColor.setColor(QPalette::WindowText, Qt::red);
    m_addressHintLabel->setPalette(hintTextColor);
    m_gateWayHintLabel->setPalette(hintTextColor);

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

    m_detailLayout = new QFormLayout(this);
    m_detailLayout->setContentsMargins(0, 0, 0, 0);
    m_detailLayout->setVerticalSpacing(0);
    m_detailLayout->addRow(m_configLabel,ipv6ConfigCombox);
    m_detailLayout->addRow(m_configEmptyLabel);
    m_detailLayout->addRow(m_addressLabel,addressWidget);
    m_detailLayout->addRow(m_subnetLabel,lengthEdit);
    m_detailLayout->addRow(m_subnetEmptyLabel);
    m_detailLayout->addRow(m_gateWayLabel,gateWayWidget);
    m_detailLayout->addRow(m_dnsLabel,firstDnsEdit);
    m_detailLayout->addRow(m_firstDnsEmptyLabel);
    m_detailLayout->addRow(m_secDnsLabel,secondDnsEdit);

    ipv6ConfigCombox->addItem(tr("Auto(DHCP)")); //"自动(DHCP)"
    ipv6ConfigCombox->addItem(tr("Manual")); //"手动"

    QRegExp ipv6_rx("^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$");
    ipv6AddressEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
    gateWayEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
    firstDnsEdit->setValidator(new QRegExpValidator(ipv6_rx, this));
    secondDnsEdit->setValidator(new QRegExpValidator(ipv6_rx, this));

    QRegExp prefix_rx("\\b(?:(?:12[0-8]|1[0-1][0-9]|^[1-9][0-9]?$)\\.){3}(?:12[0-8]|1[0-1][0-9]|^[1-9][0-9]?$)\\b");
    lengthEdit->setValidator(new QRegExpValidator(prefix_rx,this));

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
    connect(firstDnsEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
    connect(secondDnsEdit, SIGNAL(textChanged(QString)), this, SLOT(setEnableOfSaveBtn()));
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
        firstDnsEdit->clear();
        secondDnsEdit->clear();

        ipv6AddressEdit->setPlaceholderText(" ");
        lengthEdit->setPlaceholderText(" ");
        gateWayEdit->setPlaceholderText(" ");
    } else {
        ipv6AddressEdit->setPlaceholderText(tr("Required")); //必填
        lengthEdit->setPlaceholderText(tr("Required")); //必填
        gateWayEdit->setPlaceholderText(tr("Required")); //必填
    }

    ipv6AddressEdit->setEnabled(check);
    lengthEdit->setEnabled(check);
    gateWayEdit->setEnabled(check);
    firstDnsEdit->setEnabled(check);
    secondDnsEdit->setEnabled(check);
}

void Ipv6Page::setEnableOfSaveBtn()
{
    emit setIpv6PageState(checkConnectBtnIsEnabled());
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
    }
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

