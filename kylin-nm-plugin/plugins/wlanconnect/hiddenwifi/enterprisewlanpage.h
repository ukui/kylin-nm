/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

#ifndef ENTERPRISEWLANPAGE_H
#define ENTERPRISEWLANPAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>

#include <kylinnetworkmanager.h>
#include "entsecuritywidget.h"

class EnterpriseWlanPage : public QWidget
{
    Q_OBJECT

public:
    EnterpriseWlanPage(QString ssid, QString device, bool isLockScreen, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initUI();
    void initConnections();
    void centerToScreen();

private:
    QString m_deviceName;
    QString m_ssid;

    QLabel *m_ssidTitleLabel;
    QLabel *m_ssidLabel;
    EntSecurityWidget *m_entSecurityWidget = nullptr;
    QPushButton *m_cancelBtn;
    QPushButton *m_connectBtn;
    QWidget *m_ssidWidget;
    QWidget *m_btnWidget;

    QVBoxLayout  *m_mainLayout;

private Q_SLOTS:
    void onBtnConnectClicked();
    void onEapTypeChanged(const KyEapMethodType &type);

Q_SIGNALS:
    void connectPeapConnect(KyEapMethodPeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectTtlsConnect(KyEapMethodTtlsInfo info, KyWirelessConnectSetting connSettingInfo);

};

#endif // ENTERPRISEWLANPAGE_H
