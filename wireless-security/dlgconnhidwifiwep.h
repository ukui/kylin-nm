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

#ifndef DLGCONNHIDWIFIWEP_H
#define DLGCONNHIDWIFIWEP_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class DlgConnHidWifiWep;
}

class DlgConnHidWifiWep : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiWep(int type, QWidget *parent = 0);
    ~DlgConnHidWifiWep();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialog();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_leKey_textEdited(const QString &arg1);

    void on_leNetName_textEdited(const QString &arg1);

private:
    Ui::DlgConnHidWifiWep *ui;
    int WepPwdOrCode = 0; //0 WEP password;1 WEP Code Sentence

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

//    QString labelQss, cbxQss, leQss, btnConnQss, btnCancelQss, lineQss, checkBoxQss;

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFIWEP_H
