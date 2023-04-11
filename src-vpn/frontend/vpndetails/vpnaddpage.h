/*
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 */
#ifndef VPNADDPAGE_H
#define VPNADDPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QFormLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>

#include "kyvpnconnectoperation.h"

class vpnAddPage : public QWidget
{
    Q_OBJECT
public:
    explicit vpnAddPage(QWidget *parent = nullptr);
    void centerToScreen();
    ~vpnAddPage();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initWindow();
    void initUI();
    void initVpnTypeFrame();
    void initVpnNameFrame();
    void initVpnServerFrame();
    void initButtonFrame();

    void initConnection();

    bool checkConfimBtnIsEnabled();
    bool createVpnConnect();

    QFrame *m_vpnTypeFrame = nullptr;
    QFrame *m_vpnNameFrame = nullptr;
    QFrame *m_vpnServerFrame = nullptr;
    QFrame *m_buttonFrame = nullptr;

    QComboBox *m_vpnTypeComboBox = nullptr;
    QLineEdit *m_vpnNameLineEdit = nullptr;
    QLineEdit *m_vpnServerLineEdit = nullptr;

    QPushButton *m_confimBtn = nullptr;
    QPushButton *m_cancelBtn = nullptr;
    QCheckBox *m_autoConnectBox = nullptr;

    KyVpnConnectOperation *m_vpnConnOperation = nullptr;

private Q_SLOTS:
    void onConfimBtnClicked();
    void onSetConfimBtnEnable();

Q_SIGNALS:
    void closed();
};

#endif // VPNADDPAGE_H
