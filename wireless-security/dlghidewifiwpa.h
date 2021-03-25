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

#ifndef DLGCONNHIDWIFIWPA_H
#define DLGCONNHIDWIFIWPA_H

#include <QDialog>
#include <QMouseEvent>
#include <QThread>
#include <QProcess>

class MainWindow;

namespace Ui {
class DlgHideWifiWpa;
}

class DlgHideWifiWpa : public QDialog
{
    Q_OBJECT

public:
    explicit DlgHideWifiWpa(int type, MainWindow *mw = 0, QWidget *parent = 0);
    ~DlgHideWifiWpa();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialog();
    void changeWindow();
    void emitSignal();
    void on_execSecConn();
    void slotStartConnectHiddenWifi();
    void slotStartConnectRememberedHiddenWifi();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_lePassword_textEdited(const QString &arg1);

//    void finishedProcess(int res);

signals:
    void reSetWifiList();
    void stopSignal();
    void stopSignalAgain();
    void sigConnHiddenWifi(QString wifiName, QString wifiPasswd);
    void sigConnRememberedHiddenWifi(QString wifiName);

private:
    Ui::DlgHideWifiWpa *ui;
    int isUsed;//=0 current wifi not used before; >=1 used
    MainWindow *mw;
    QString strWifiname;
    QString strWifiPassword;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

//    QString labelQss, cbxQss, leQss, btnConnQss, btnCancelQss, lineQss, checkBoxQss;

    bool isPress;
    QPoint winPos;
    QPoint dragPos;

    QString shellOutput = "";
    QStringList wlist;
    QProcess * shellProcess;
};

#endif // DLGCONNHIDWIFIWPA_H
