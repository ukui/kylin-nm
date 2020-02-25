/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
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


#include "dlgconnhidwifiwpa.h"
#include "ui_dlgconnhidwifiwpa.h"
#include "kylinheadfile.h"
#include "backthread.h"
#include "mainwindow.h"
#include <unistd.h>
#include <stdlib.h>

DlgConnHidWifiWpa::DlgConnHidWifiWpa(int type, MainWindow *mainWindow, QWidget *parent) :
    isUsed(type),
    QDialog(parent),
    ui(new Ui::DlgConnHidWifiWpa)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);");

    ui->lbBoder->setStyleSheet("QLabel{border-radius:6px;background-color:rgba(19,19,20,0.95);border:1px solid rgba(255, 255, 255, 0.05);}");
    ui->lbBoder->hide();
    ui->lbLeftupTitle->setStyleSheet("QLabel{border:0px;font-size:20px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbConn->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbNetName->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbSecurity->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");
    ui->lbPassword->setStyleSheet("QLabel{border:0px;color:rgba(255,255,255,0.97);background-color:transparent;}");

    ui->cbxConn->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;color:rgba(255,255,255,1);border:1px solid rgba(255, 255, 255, 0.05);background:rgba(255,255,255,0.08);}"
                              "QComboBox::drop-down{border:0;width:30px;}"
                              "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}");
    ui->leNetName->setStyleSheet("QLineEdit{color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}");
    ui->lePassword->setStyleSheet("QLineEdit{color:rgba(255,255,255,0.97);background:rgba(255,255,255,0.08);}");
    ui->cbxSecurity->setStyleSheet("QComboBox{padding-left:20px;font-size:13px;color:rgba(255,255,255,1);border:1px solid rgba(255, 255, 255, 0.05);background:rgba(255,255,255,0.08);}"
                              "QComboBox::drop-down{border:0;width:30px;}"
                              "QComboBox::down-arrow{image:url(:/res/g/down_arrow.png);}");
    ui->checkBoxPwd->setStyleSheet("QCheckBox::indicator {width: 18px; height: 9px;}"
                                   "QCheckBox::indicator:checked {image: url(:/res/h/show-pwd.png);}"
                                   "QCheckBox::indicator:unchecked {image: url(:/res/h/hide-pwd.png);}");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(255,255,255,0.12);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(255,255,255,0.2);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(255,255,255,0.08);}");
    ui->btnConnect->setStyleSheet("QPushButton{border:0px;border-radius:4px;background-color:rgba(61,107,229,1);color:white;font-size:14px;}"
                               "QPushButton:Hover{border:0px solid rgba(255,255,255,0.2);border-radius:4px;background-color:rgba(107,142,235,1);}"
                               "QPushButton:Pressed{border-radius:4px;background-color:rgba(50,87,202,1);}");
    ui->lineUp->setStyleSheet("background:rgba(255,255,255,0.08);");
    ui->lineDown->setStyleSheet("background:rgba(255,255,255,0.08);");
    ui->btnCancel->setFocusPolicy(Qt::NoFocus);


    ui->lbLeftupTitle->setText(tr("Add hidden Wi-Fi")); //加入隐藏Wi-Fi
    ui->lbConn->setText(tr("Connection")); //连接设置:
    ui->lbNetName->setText(tr("Network name")); //网络名称:
    ui->lbSecurity->setText(tr("Wi-Fi security")); //Wi-Fi 安全性:
    ui->lbPassword->setText(tr("Password")); //密码:
    ui->btnCancel->setText(tr("Cancel")); //取消
    ui->btnConnect->setText(tr("Connect")); //连接

    ui->cbxConn->addItem(tr("C_reate…")); //新建...
    system("nmcli connection show>/tmp/kylin-nm-connshow");
    QFile file("/tmp/kylin-nm-connshow");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"Can't open the file!";
    }
    QString txt = file.readAll();
    QStringList txtLine = txt.split("\n");
    file.close();
    foreach (QString line, txtLine) {
        if(line.indexOf("wifi") != -1){
            QStringList subLine = line.split(" ");
            ui->cbxConn->addItem(subLine[0]);
        }
    }
    ui->cbxConn->setCurrentIndex(0);
    connect(ui->cbxConn,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeWindow()));

    ui->cbxSecurity->addItem(tr("None")); //无
    ui->cbxSecurity->addItem(tr("WPA & WPA2 Personal")); //WPA 及 WPA2 个人
    ui->cbxSecurity->addItem(tr("WEP 40/128-bit Key (Hex or ASCII)")); //WEP 40/128 位密钥(十六进制或ASCII)
    ui->cbxSecurity->addItem(tr("WEP 128-bit Passphrase")); //WEP 128 位密码句
    ui->cbxSecurity->addItem("LEAP");
    ui->cbxSecurity->addItem(tr("Dynamic WEP (802.1X)")); //动态 WEP (802.1x)
    ui->cbxSecurity->addItem(tr("WPA & WPA2 Enterprise")); //WPA 及 WPA2 企业
    ui->cbxSecurity->setCurrentIndex(1);
    connect(ui->cbxSecurity,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeDialog()));

    if (isUsed == 0){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->cbxConn->setCurrentIndex(isUsed);
        ui->leNetName->setText(ui->cbxConn->currentText());
        ui->leNetName->setEnabled(false);
        ui->lbNetName->setEnabled(false);
        ui->lbSecurity->setEnabled(false);
        ui->cbxSecurity->setEnabled(false);
        ui->lbPassword->setEnabled(false);
        ui->lePassword->setText("<Hidden>");
        ui->lePassword->setEnabled(false);
        ui->btnConnect->setEnabled(true);
        ui->checkBoxPwd->setEnabled(false);
    }

    this->setFixedSize(432,412);

    this->mw = mainWindow;
}

DlgConnHidWifiWpa::~DlgConnHidWifiWpa()
{
    delete ui;
}

void DlgConnHidWifiWpa::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}
void DlgConnHidWifiWpa::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
    this->setWindowOpacity(1);
}
void DlgConnHidWifiWpa::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        this->setWindowOpacity(0.9);
        event->accept();
    }
}

//切换到其他Wi-Fi安全类型
void DlgConnHidWifiWpa::changeDialog()
{
    if(ui->cbxSecurity->currentIndex()==0){
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifi *connHidWifi = new DlgConnHidWifi(0,mw);
        connHidWifi->show();
        connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    } else if(ui->cbxSecurity->currentIndex()==1) {
        qDebug()<<"it's not need to change dialog";
    } else if(ui->cbxSecurity->currentIndex()==2) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiWep *connHidWifiWep = new DlgConnHidWifiWep(0);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==3) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiWep *connHidWifiWep = new DlgConnHidWifiWep(1);
        connHidWifiWep->show();
    } else if(ui->cbxSecurity->currentIndex()==4) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiLeap *connHidWifiLeap = new DlgConnHidWifiLeap();
        connHidWifiLeap->show();
    } else if(ui->cbxSecurity->currentIndex()==5) {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiSecTls *connHidWifiSecTls = new DlgConnHidWifiSecTls(0);
        connHidWifiSecTls->show();
    } else {
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifiSecTls *connHidWifiSecTls = new DlgConnHidWifiSecTls(1);
        connHidWifiSecTls->show();
    }
}

//同一 Wi-Fi安全类型的窗口变换
void DlgConnHidWifiWpa::changeWindow(){
    if (ui->cbxConn->currentIndex() == 0){
        QApplication::setQuitOnLastWindowClosed(false);
        this->hide();
        DlgConnHidWifi *connHidWifi = new DlgConnHidWifi(0, mw);
        connHidWifi->show();
        connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
    }else if (ui->cbxConn->currentIndex() >= 1){
        QString currStr = "nmcli connection show " + ui->cbxConn->currentText() + " >/tmp/kylin-nm-connshow";
        system(currStr.toUtf8().data());
        QFile file("/tmp/kylin-nm-connshow");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"Can't open the file!";
        }
        QString txt = file.readAll();
        file.close();
        if (txt.indexOf("802-11-wireless-security.key-mgmt:") != -1){
            isUsed = ui->cbxConn->currentIndex();
            ui->lbNetName->setEnabled(false);
            ui->leNetName->setText(ui->cbxConn->currentText());
            ui->leNetName->setEnabled(false);
            ui->lbSecurity->setEnabled(false);
            ui->cbxSecurity->setEnabled(false);
            ui->lePassword->setText("<Hidden>");
            ui->lbPassword->setEnabled(false);
            ui->lePassword->setEnabled(false);
            ui->btnConnect->setEnabled(true);
        }else {
            QApplication::setQuitOnLastWindowClosed(false);
            this->hide();
            DlgConnHidWifi *connHidWifi = new DlgConnHidWifi(ui->cbxConn->currentIndex(), mw);
            connHidWifi->show();
            connect(connHidWifi, SIGNAL(reSetWifiList() ), mw, SLOT(on_btnWifiList_clicked()) );
        }
    }
}

void DlgConnHidWifiWpa::on_btnCancel_clicked()
{
    this->close();
}

void DlgConnHidWifiWpa::on_btnConnect_clicked()
{
    QThread *t = new QThread();
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotStartLoading()));
    connect(this, SIGNAL(stopSignal()), t, SLOT(quit()));
    t->start();

    QString wifiName = ui->leNetName->text();
    QString wifiPassword = ui->lePassword->text();
    BackThread *bt = new BackThread();
    strWifiname = wifiName;
    strWifiPassword = wifiPassword;
    if (isUsed == 0){
        int x = 0;
        do{
            sleep(1);
            QString cmd = "nmcli device wifi connect " + wifiName + " password " + wifiPassword + " hidden yes >/tmp/kylin-nm-btoutput";
            system(cmd.toUtf8().data());

            QFile file("/tmp/kylin-nm-btoutput");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug()<<"Can't open the file!"<<endl;
            }
            QString text = file.readAll();
            file.close();
            if(text.indexOf("Scanning not allowed") != -1){x = 1;} else { x = 0;}
        } while(x == 1);

        connect(this, SIGNAL(execSecConn()), this,SLOT(on_execSecConn() ));
        QTimer::singleShot(5*1000, this, SLOT(on_execSecConn() ));
    } else {
        bt->execConnWifi(wifiName);
        connect(this, SIGNAL(sendMessage()), this,SLOT(emitSignal() ));
        QTimer::singleShot(4*1000, this, SLOT(emitSignal() ));
    }
    this->close();
}

void DlgConnHidWifiWpa::on_checkBoxPwd_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->lePassword ->setEchoMode(QLineEdit::Password);
    } else {
        ui->lePassword->setEchoMode(QLineEdit::Normal);
    }
}

void DlgConnHidWifiWpa::on_leNetName_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "" || ui->lePassword->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgConnHidWifiWpa::on_lePassword_textEdited(const QString &arg1)
{
    if (ui->leNetName->text() == "" || ui->lePassword->text() == ""){
        ui->btnConnect->setEnabled(false);
    } else {
        ui->btnConnect->setEnabled(true);
    }
}

void DlgConnHidWifiWpa::slotStartLoading()
{
    mw->startLoading();
}

void DlgConnHidWifiWpa::on_execSecConn()
{
    QString str = "nmcli device wifi connect " + strWifiname + " password " + strWifiPassword;
    system(str.toUtf8().data());
    connect(this, SIGNAL(sendMessage()), this,SLOT(emitSignal() ));
    QTimer::singleShot(3*1000, this, SLOT(emitSignal() ));
}

void DlgConnHidWifiWpa::emitSignal()
{
    emit reSetWifiList();
    mw->stopLoading();
    emit this->stopSignal();
}
