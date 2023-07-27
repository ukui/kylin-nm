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
#ifndef JOINHIDDENWIFIPAGE_H
#define JOINHIDDENWIFIPAGE_H

#include <QScrollArea>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "coninfo.h"
#include "kywirelessconnectoperation.h"
#include "securitypage.h"
#include "divider.h"
#include "kwidget.h"
#include "kdialog.h"
#include "kborderlessbutton.h"

using namespace kdk;

class JoinHiddenWiFiPage : public KDialog
{
    Q_OBJECT

public:
    JoinHiddenWiFiPage(QString devName, KDialog *parent = nullptr);
    ~JoinHiddenWiFiPage();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void initUI();
    void initComponent();

    void setJoinBtnEnable();

private:
    KyWirelessConnectOperation *m_wirelessConnOpration = nullptr;
    QString m_devName;

    QWidget *m_topWidget;
    QWidget *m_centerWidget;
    QWidget *m_bottomWidget;
    SecurityPage *m_secuWidget;

    FixLabel *m_descriptionLabel;
    FixLabel *m_nameLabel;
    LineEdit *m_nameEdit;

    Divider *m_bottomDivider = nullptr;
    KBorderlessButton *m_showListBtn;
    QPushButton *m_cancelBtn;
    QPushButton *m_joinBtn;

    QScrollArea *m_hiddenWifiScrollArea;
    QVBoxLayout *m_pageLayout;
    QHBoxLayout *m_topLayout;
    QVBoxLayout *m_centerVBoxLayout;
    QHBoxLayout *m_bottomLayout;

    bool m_isJoinBtnEnable = false;
    bool m_isSecuOk = false;
    ConInfo      m_info;

private Q_SLOTS:
    void onBtnJoinClicked();
    void onBtnShowListClicked();
    void onSecuTypeChanged(const KySecuType &type);
    void onEapTypeChanged(const KyEapMethodType &type);
    //void onPaletteChanged();

Q_SIGNALS:
    void hiddenWiFiPageClose(QString);
    void showWlanList(int type);
};

#endif // JOINHIDDENWIFIPAGE_H
