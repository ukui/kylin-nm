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
#ifndef IPV4PAGE_H
#define IPV4PAGE_H

#include <QWidget>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QScrollArea>
#include <QSpacerItem>
#include <QComboBox>
#include <QLineEdit>
#include <QDebug>

//#include "kylinconnectsetting.h"
#include "coninfo.h"

class Ipv4Page : public QFrame
{
    Q_OBJECT
public:
    Ipv4Page(QWidget *parent = nullptr);
    void setIpv4Config(KyIpConfigType ipv4Config);
    void setIpv4(const QString &ipv4);
    void setNetMask(const QString &netMask);
    void setIpv4FirDns(const QString &ipv4FirDns);
    void setIpv4SecDns(const QString &ipv4SecDns);
    void setGateWay(const QString &gateWay);

    bool checkIsChanged(const ConInfo info, KyConnectSetting &setting);

private:
    QComboBox *ipv4ConfigCombox;
    LineEdit *ipv4addressEdit;
    LineEdit *netMaskEdit;
    LineEdit *gateWayEdit;
    LineEdit *firstDnsEdit;
    LineEdit *secondDnsEdit;

private:
    QFormLayout *m_detailLayout;
    QVBoxLayout *mvBoxLayout;
    QLabel *m_configLabel;
    QLabel *m_addressLabel;
    QLabel *m_maskLabel;
    QLabel *m_gateWayLabel;
    QLabel *m_dnsLabel;
    QLabel *m_secDnsLabel;

    QLabel *m_configEmptyLabel;
    QLabel *m_addressHintLabel;
    QLabel *m_maskHintLabel;
    QLabel *m_gateWayEmptyLabel;
    QLabel *m_firstDnsEmptyLabel;
private:
    void initUI();
    void initComponent();
    void setLineEnabled(bool check);
    void configSave();
    bool getTextEditState(QString text);
    bool netMaskIsValide(QString text);
    QString getNetMaskText(QString text);
    bool checkConnectBtnIsEnabled();


private slots:
    void setEnableOfSaveBtn();
    void configChanged(int index);
    void onAddressTextChanged();
    void onNetMaskTextChanged();

Q_SIGNALS:
    void setIpv4PageState(bool);

};

#endif // IPV4PAGE_H
