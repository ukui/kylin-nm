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
#include "creatnetpage.h"
#include "math.h"

#define MAX_NAME_LENGTH 32

CreatNetPage::CreatNetPage(QWidget *parent):QFrame(parent)
{
    initUI();
    initComponent();
}

void CreatNetPage::initUI()
{
    connNameEdit = new LineEdit(this);
    connNameEdit->setMaxLength(MAX_NAME_LENGTH);
    ipv4ConfigCombox = new QComboBox(this);
    ipv4addressEdit = new LineEdit(this);
    netMaskEdit = new LineEdit(this);
    gateWayEdit = new LineEdit(this);

    m_connNameLabel = new QLabel(this);
    m_configLabel = new QLabel(this);
    m_addressLabel = new QLabel(this);
    m_maskLabel = new QLabel(this);
    m_gateWayLabel = new QLabel(this);

    // IP的正则格式限制
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    m_dnsWidget = new MultipleDnsWidget(rx, this);

    m_connNameLabel->setText(tr("Connection Name"));
    m_configLabel->setText(tr("IPv4Config"));
    m_addressLabel->setText(tr("Address"));
    m_maskLabel->setText(tr("Netmask"));
    m_gateWayLabel->setText(tr("Default Gateway"));

    m_detailLayout = new QFormLayout(this);
    m_detailLayout->setContentsMargins(0, 0, 0, 0);
    m_detailLayout->setSpacing(24);
    m_detailLayout->addRow(m_connNameLabel,connNameEdit);
    m_detailLayout->addRow(m_configLabel,ipv4ConfigCombox);
    m_detailLayout->addRow(m_addressLabel,ipv4addressEdit);
    m_detailLayout->addRow(m_maskLabel,netMaskEdit);
    m_detailLayout->addRow(m_gateWayLabel,gateWayEdit);
    m_detailLayout->addRow(m_dnsWidget);

    ipv4ConfigCombox->addItem(tr("Auto(DHCP)"), AUTO_CONFIG); //"自动(DHCP)"
    ipv4ConfigCombox->addItem(tr("Manual"), MANUAL_CONFIG); //"手动"

    ipv4addressEdit->setValidator(new QRegExpValidator(rx, this));
    gateWayEdit->setValidator(new QRegExpValidator(rx, this));
    netMaskEdit->setValidator(new QRegExpValidator(rx, this));
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

        if (netMaskEdit->text().isEmpty() || !netMaskIsValide(netMaskEdit->text())) {
            qDebug() << "create ipv4 netMask empty or invalid";
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
    m_dnsWidget->setEditEnabled(check);

    if (!check) {
        ipv4addressEdit->clear();
        netMaskEdit->clear();
        gateWayEdit->clear();

        ipv4addressEdit->setPlaceholderText(" ");
        netMaskEdit->setPlaceholderText(" ");
    } else {
        ipv4addressEdit->setPlaceholderText(tr("Required")); //必填
        netMaskEdit->setPlaceholderText(tr("Required")); //必填
    }
}

void CreatNetPage::setEnableOfSaveBtn() {
    Q_EMIT setCreatePageState(checkConnectBtnIsEnabled());
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
    QString netMask = getNetMaskText(netMaskEdit->text());
    QString gateWay = gateWayEdit->text();
    qDebug() << "constructIpv4Info: " << "ipv4address " << ipv4address
             << " netMask " << netMask
             << " gateWay " << gateWay;

    QStringList dnsList;
    dnsList.clear();

    QList<QHostAddress> ipv4dnsList;
    ipv4dnsList.clear();
    ipv4dnsList = m_dnsWidget->getDns();
    for (QHostAddress str: ipv4dnsList) {
        if (!dnsList.contains(str.toString())) {
            dnsList << str.toString();
        }
    }

    if (ipv4ConfigCombox->currentData() == AUTO_CONFIG) {
        setting.setIpConfigType(IPADDRESS_V4, CONFIG_IP_DHCP);
    } else {
        setting.setIpConfigType(IPADDRESS_V4, CONFIG_IP_MANUAL);
        setting.ipv4AddressConstruct(ipv4address, netMask, gateWay, dnsList);
    }

}

bool CreatNetPage::netMaskIsValide(QString text)
{
    if (getTextEditState(text)) {
        return true;
    } else {
        if (text.length() > 0 && text.length() < 3) {
            int num = text.toInt();
            if (num > 0 && num < 33) {
                return true;
            }
        }
    }
    return false;
}

QString CreatNetPage::getNetMaskText(QString text)
{
    if (text.length() > 2) {
        return text;
    }

    int num = text.toInt();
    QStringList list;
    list << "0" << "0" << "0" << "0";
    int count = 0;
    while (num - 8 >= 0) {
        list[count] = "255";
        num = num - 8;
        count ++;
    }
    if (num > 0) {
        int size = pow(2, 8) - pow(2,(8-num));
        list[count] = QString::number(size);
    }
    return QString("%1.%2.%3.%4").arg(list[0],list[1],list[2],list[3]);
}

