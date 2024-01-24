/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
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

#ifndef WLANITEM_H
#define WLANITEM_H
#include <QObject>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QMenu>
#include <QCheckBox>

#include "../component/InfoButton/infobutton.h"
#include "../component/RadioItemButton/radioitembutton.h"
#include "../component/FixLabel/fixlabel.h"
#include "kpasswordedit.h"
#include "hiddenwifi/enterprisewlanpage.h"
using namespace kdk;

class WlanItem : public QFrame
{
    Q_OBJECT
public:
    WlanItem(bool isSimple, QWidget *parent = nullptr);
public:
    void setName(QString name) {
        titileLabel->setLabelText(name);
    }

    QString getName() {
        return titileLabel->getText();
    }

    void setItemIcon(const QIcon &icon) {
        radioBtn->setButtonIcon(icon);
    }

    void setStatus(bool isAcitve){
        m_isAcitve = isAcitve;
        radioBtn->setActive(isAcitve);
    }

    bool getStatus(){
        return m_isAcitve;
    }

    bool getIsLoading() {
        return m_loading;
    }

    QString getUuid(){
        return m_uuid;
    }

    void setUuid(QString uuid){
        m_uuid = uuid;
    }

    QString getPath(){
        return m_dbusPath;
    }

    void setPath(QString dbusPath){
        m_dbusPath = dbusPath;
    }

    QString getSecuType(){
        return m_secutype;
    }

    void setSecuType(QString &secuType){
        m_secutype = secuType;
    }

    QString getInterface(){
        return m_interface;
    }

    void setInterface(QString &interface){
        m_interface = interface;
    }

    void setPasswordError(QString pwd) {
        m_pwdLineEdit->setState(LoginFailed);
        m_pwdLineEdit->setText(pwd);
    }

    void showEnterPricePage(QString devName, QWidget *widget);

    void startLoading();
    void stopLoading();

    void setExpend(bool);
    bool getExpend();

Q_SIGNALS:
    void itemClick();
    void infoButtonClick();
    void connectButtonClick(QString , bool);
    void passwordFocusIn();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QTimer *waitTimer = nullptr;
    QList<QIcon> loadIcons;
    int currentIconIndex=0;

    QMenu *m_menu = nullptr;
    RadioItemButton* radioBtn = nullptr;
    InfoButton * infoLabel = nullptr;
    FixLabel * titileLabel = nullptr;

    bool isSimple;
    bool m_isAcitve = false;
    bool m_loading = false;
    bool m_isExpand = false;

    QString m_uuid = "";
    QString m_dbusPath ="";
    //ap类型
    QString m_secutype = "";
    QString m_interface = "";

    QFrame* m_nameFrame = nullptr;
    QFrame* m_expendFrame = nullptr;
    QFrame* m_pwdFrame = nullptr;
    QFrame* m_autoConnectFrame = nullptr;

    KPasswordEdit* m_pwdLineEdit = nullptr;
    QPushButton* m_connectButton = nullptr;
    QCheckBox* m_autoConnectCheckBox = nullptr;
    QLabel* m_autoConnectLabel = nullptr;

    QHBoxLayout* m_autoConnectFrameLyt = nullptr;
    QHBoxLayout* m_pwdFrameLyt = nullptr;

    EnterpriseWlanPage* m_enterPirsePage = nullptr;

    bool m_isIn = false;

private Q_SLOTS:
    void updateIcon();
//    void onMenuTriggered(QAction *action);
    void onPwdEditorTextChanged();
    void onConnectButtonClicked();

Q_SIGNALS:
    void connectPeapConnect(KyEapMethodPeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectTtlsConnect(KyEapMethodTtlsInfo info, KyWirelessConnectSetting connSettingInfo);
};

#endif // WLANITEM_H
