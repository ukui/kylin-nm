/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#ifndef DLGCONNHIDWIFISECTLS_H
#define DLGCONNHIDWIFISECTLS_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class DlgConnHidWifiSecTls;
}

class DlgConnHidWifiSecTls : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiSecTls(int type, QWidget *parent = 0);
    ~DlgConnHidWifiSecTls();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialogSecu();
    void changeDialogAuth();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_checkBoxCA_stateChanged(int arg1);

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_checkBoxPwdSec_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_leIdentity_textEdited(const QString &arg1);

    void on_leDomain_textEdited(const QString &arg1);

    void on_leCaPwd_textEdited(const QString &arg1);

    void on_leUserCertifyPwd_textEdited(const QString &arg1);

    void on_leUserKeyPwd_textEdited(const QString &arg1);

    void on_cbxCA_currentIndexChanged(const QString &arg1);

    void on_cbxUserCertify_currentIndexChanged(const QString &arg1);

    void on_cbxUserPriKey_currentIndexChanged(const QString &arg1);

private:
    Ui::DlgConnHidWifiSecTls *ui;
    int WepOrWpa = 0;//0 WEP;1WPA

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

//    QString labelQss, cbxQss, leQss, btnConnQss, btnCancelQss, lineQss, checkBoxQss, checkBoxCAQss;

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFISECTLS_H
