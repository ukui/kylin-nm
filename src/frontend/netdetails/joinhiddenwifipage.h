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
#include <QTimer>

#include "coninfo.h"
#include "securitypage.h"
#include "divider.h"
#include "kwidget.h"
#include "kdialog.h"
#include "kborderlessbutton.h"
#include "../../backend/dbus-interface/kywirelessconnectoperation.h"

using namespace kdk;

class JoinHiddenWiFiPage : public KDialog
{
    Q_OBJECT

public:
    JoinHiddenWiFiPage(QString devName, KDialog *parent = nullptr);
    ~JoinHiddenWiFiPage();

    void setEnterpriseInfo(const QVariantMap &info);

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
    QTimer *m_timeoutTimer = nullptr;
    bool m_bReConnectNet = false;
    /* 企业wifi密码重连：已向 SecretAgent onSubmitPassword 提交后为 true，避免 close 时再 onCancelPassword */
    bool m_reConnectNetSecretSubmitted = false;

private Q_SLOTS:
    void onBtnJoinClicked();
    void onBtnShowListClicked();
    void onSecuTypeChanged(const KySecuType &type);
    void onEapTypeChanged(const KyEapMethodType &type);
    void resetTimeout();

Q_SIGNALS:
    void hiddenWiFiPageClose(QString);
    /* 仅在企业重连场景下，用户关闭对话框且未点「连接」提交凭据时发出（用于 onCancelPassword） */
    void cancelReConnectEnterprise(QString);
    void showWlanList(int type);

    void reConnectTlsConnect(KyEapMethodTlsInfo info, KyWirelessConnectSetting connSettingInfo);
    void reConnectPeapConnect(KyEapMethodPeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void reConnectTtlsConnect(KyEapMethodTtlsInfo info, KyWirelessConnectSetting connSettingInfo);
    void reConnectLeapConnect(KyEapMethodLeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void reConnectPwdConnect(KyEapMethodPwdInfo info, KyWirelessConnectSetting connSettingInfo);
    void reConnectFastConnect(KyEapMethodFastInfo info, KyWirelessConnectSetting connSettingInfo);
};

#endif // JOINHIDDENWIFIPAGE_H
