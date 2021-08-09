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

#ifndef DLGCONNHIDWIFISECPEAP_H
#define DLGCONNHIDWIFISECPEAP_H

#include <QDialog>
#include <QMouseEvent>

class MainWindow;

namespace Ui {
class DlgHideWifiEapPeap;
}

class DlgHideWifiEapPeap : public QDialog
{
    Q_OBJECT

public:
    //type: 0是动态 WEP， 1是企业wpa, beUsed:是否是之前已经连接过多网络
    explicit DlgHideWifiEapPeap(int type, int beUsed, MainWindow *mw = 0, QWidget *parent = 0);
    ~DlgHideWifiEapPeap();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialogSecu();
    void changeDialogAuth();
    void changeWindow();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_cbxCA_currentIndexChanged(const QString &arg1);

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_checkBoxCA_stateChanged(int arg1);

    void on_checkBoxPwdSec_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_leAnonyId_textEdited(const QString &arg1);

    void on_leDomain_textEdited(const QString &arg1);

    void on_leCaPwd_textEdited(const QString &arg1);

    void on_leUserName_textEdited(const QString &arg1);

    void on_lePassword_textEdited(const QString &arg1);

private:
    Ui::DlgHideWifiEapPeap *ui;
    int WepOrWpa = 0;//0 WEP;1 WPA
    int isUsed;//=0 current wifi not used before; >=1 used
    MainWindow *mw;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

//    QString labelQss, cbxQss, leQss, btnConnQss, btnCancelQss, lineQss, checkBoxQss, checkBoxCAQss;

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFISECPEAP_H
