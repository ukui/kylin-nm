/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
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

#ifndef ENTSECURITYWIDGET_H
#define ENTSECURITYWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>

//#include "coninfo.h"
#include "kwidget.h"
#include "kpasswordedit.h"
#include "kdialog.h"
#include "kballontip.h"
#include "depend/kyenterpricesettinginfo.h"

using namespace kdk;

// ------------------------"coninfo.h"------------------------------
enum PeapInnerType
{
    MSCHAPV2_PEAP = 0,
    MD5_PEAP,
    GTC_PEAP,
};

enum TtlsInnerType
{
    PAP = 0,
    MSCHAP,
    MSCHAPV2_EAP,
    MSCHAPV2,
    CHAP,
    MD5_EAP,
    GTC_EAP
};

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}
    ~LineEdit() {}

protected:
    void contextMenuEvent(QContextMenuEvent *event) {
        QMenu *menu = createStandardContextMenu();//默认的标准右键菜单，如果不需要刻意完全自己实现
        menu->setPalette(this->palette());
        menu->exec(event->globalPos());
        delete menu;
    }
};

// ------------------------------------------------------

class LogHintDialog : public KDialog
{
    Q_OBJECT

public:
    LogHintDialog(QWidget *parent = nullptr);

private:
    QLabel * m_iconLabel = nullptr;
    QLabel * m_contentLabel = nullptr;
    QPushButton *m_confirmBtn;
    QVBoxLayout *m_dialogLayout;

Q_SIGNALS:
    void LogHintDialogClosed();
};

class EntSecurityWidget : public QWidget
{
    Q_OBJECT

public:
    EntSecurityWidget(bool isLockScreen, QWidget *parent = nullptr);
    ~EntSecurityWidget();

//    void setTlsInfo(KyEapMethodTlsInfo &info);
//    void setPeapInfo(KyEapMethodPeapInfo &info);
//    void setTtlsInfo(KyEapMethodTtlsInfo &info);
    KyEapMethodPeapInfo assemblePeapInfo();
    KyEapMethodTtlsInfo assembleTtlsInfo();
    void getEnterpriseType(KyEapMethodType &enterpriseType);

private:
    void initUI();
    void initConnect();

    void showTls();
    void showPeapOrTtls();

public:
    bool checkConnectBtnIsEnabled();

private:
    //企业wifi共有
    QLabel *m_eapTypeLabel;
    QComboBox *m_eapTypeCombox;

    //TLS
    QLabel *m_identityLable;
    QLabel *m_domainLable;
    QLabel *m_caCertPathLabel;
    QLabel *m_caNeedFlagLabel;
    QLabel *m_clientCertPathLabel;
    QLabel *m_clientPrivateKeyLabel;
    QLabel *m_clientPrivateKeyPwdLabel;
    QLabel *m_pwdOptionLabel;

    //PEAP TTLS共有
    QLabel *m_eapMethodLabel;
    QLabel *m_userNameLabel;
    QLabel *m_userPwdLabel;

    //TLS
    LineEdit *m_identityEdit;
    LineEdit *m_domainEdit;
    QComboBox *m_caCertPathCombox;
    QCheckBox *m_caNeedBox;
    QComboBox *m_clientCertPathCombox;
    QComboBox *m_clientPrivateKeyCombox;
    KPasswordEdit *m_clientPrivateKeyPwdEdit = nullptr;
    QComboBox *m_pwdOptionCombox;

    QWidget *m_tlsWidget;
//    LogHintDialog *m_loginHintDialog = nullptr;
    bool m_isLockScreen;

    //PEAP && TTLS
    QComboBox *m_eapMethodCombox;
    LineEdit *m_userNameEdit;
    KPasswordEdit *m_userPwdEdit = nullptr;

private Q_SLOTS:
    void onEapTypeComboxIndexChanged();
    void setEnableOfSaveBtn();

    void onCaNeedBoxClicked();

    void onCaCertPathComboxIndexChanged(QString str);
    void onClientCertPathComboxIndexChanged(QString str);
    void onClientPrivateKeyComboxIndexChanged(QString str);

    void setComboxIndex();

Q_SIGNALS:
    void setSecuPageState(bool);
    void eapTypeChanged(const KyEapMethodType &type);

};

#endif // ENTSECURITYWIDGET_H
